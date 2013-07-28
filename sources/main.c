#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include "config.h"

#define FILE_PID "/var/run/thinkpad-fan.pid"
#define FILE_TEMP "/proc/acpi/ibm/thermal"
#define FILE_FAN "/proc/acpi/ibm/fan"
#define INITIAL_LEVEL 1

int running = 1;

void catch_termination_signal() {
	fprintf(stdout, "termination signal received.\n");
	running = 0;
}

void write_to_fanfile(char *message) {
	FILE *fanfile = fopen(FILE_FAN, "w");
	if (fanfile == NULL) {
		fprintf(stderr, "failed to open \"%s\" for setting fan level.\n", FILE_FAN);
		return;
	}

	fprintf(fanfile, "%s\n", message);
	fclose(fanfile);
}

int get_cpu_temp() {
	FILE *tempfile = fopen(FILE_TEMP, "r");
	if (tempfile == NULL) {
		fprintf(stderr, "failed to open \"%s\" for reading temps.\n", FILE_TEMP);
		return;
	}

	int cpu_temp;
	fscanf(tempfile, "temperatures:%d", &cpu_temp);
	fclose(tempfile);

	return cpu_temp;
}

typedef struct _config_item {
	int min;
	int max;
	int sleep;
} config_item;

config_item config[] = {
	{0,  55, 4},
	{49, 59, 3},
	{55, 63, 3},
	{59, 67, 2},
	{63, 71, 2},
	{67, 75, 2},
	{71, 79, 2},
	{75, 999, 6}
};

int main() {
#ifdef VERBOSE
	fprintf(stdout, "running a VERBOSE build.\n");
#endif

	FILE *pidfile = fopen(FILE_PID, "r");
	if (pidfile != NULL) {
		fprintf(stderr, "file already exists \"%s\". already running?\n", FILE_PID);
		exit(EXIT_FAILURE);
	}

	pidfile = fopen(FILE_PID, "w");
	if (pidfile == NULL) {
		fprintf(stderr, "failed to open for writing \"%s\". got root?\n", FILE_PID);
		exit(EXIT_FAILURE);
	}

	fprintf(pidfile, "%d\n", getpid());
	fclose(pidfile);

	if (signal(SIGINT, catch_termination_signal) == SIG_ERR) {
		fprintf(stderr, "failed to setup signal handler for SIGINT.\n");
	}

	if (signal(SIGTERM, catch_termination_signal) == SIG_ERR) {
		fprintf(stderr, "failed to setup signal handler for SIGTERM.\n");
	}

	write_to_fanfile("watchdog 30");

	int cpu_temp;
	int current_level = INITIAL_LEVEL;
	char message[8] = {'\0'};

	while (running) {
		cpu_temp = get_cpu_temp();

		int min = config[current_level].min;
		int max = config[current_level].max;

#ifdef VERBOSE
		int old_level = current_level;
#endif

		if (current_level < 7 && cpu_temp >= max) {
			current_level++;
		} else if (current_level > 0 && cpu_temp <= min) {
			current_level--;
		}

#ifdef VERBOSE
		if (current_level != old_level) {
			fprintf(stdout, "switching from level %d to level %d (cpu_temp=%d).\n", old_level, current_level, cpu_temp);
		}
#endif

		sprintf(message, "level %d", current_level);
		write_to_fanfile(message);

		int sec = config[current_level].sleep;
		sleep(sec);
	}

	// cleanup
	write_to_fanfile("level auto");
	unlink(FILE_PID);

	fprintf(stdout, "done.");
	return 0;
}
