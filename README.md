### About:

Set_x_root_name is a tiny utility to set the root window name to the last line read from stdin.

### Building:

#### Dependencies:

* libxcb

#### Build:

	$ ./build.sh

### Usage:

#### Synopsis:

	set_x_root_name

#### Conky status bar for DWM:

	$ conky | set_x_root_name &

### Bugs:

* In order in avoid `malloc()`, the line is stored in a static buffer. As a result, lines longer than `NAME_LENGTH` (defined in `set_x_root_name.c`; default value is `1024`) will be truncated.
