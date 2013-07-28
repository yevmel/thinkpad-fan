thinkpad-fan
=====

DESCRIPTION
-----
simple daemon for controlling the fan on thinkpad notebooks. tested on E130.
by the way... there is a more established project called thinkfan. checkout your distributions repository if interested.

CONFIGURATION
-----
there is a array called config, which holds the MIN (first column) and MAX (second column) temps for every fan level (there are seven of them). When the cpu temperature reaches the MAX value of the current level, the level gets switched to a higher level. when it reaches the MIN value of the current level, the level gets switched to a lower level. the third column defines the amount of seconds to wait before checking the temperature again.

```c
config[] = {
	{0,  55, 4},
	{47, 59, 3},
	{55, 63, 3},
	{59, 67, 2},
	{63, 71, 2},
	{67, 75, 2},
	{71, 79, 2},
	{75, 999, 6}
};
```

BUILD
-----
cmake
make
sudo make install
