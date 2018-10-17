#!/bin/sh

fail() {
	echo ">> Failed"
	exit $1
}

[ -z "$CC" ] && CC="gcc"
CFLAGS="-I/usr/X11R6/include -Os -Wall $CFLAGS"
LDFLAGS="-L/usr/X11R6/lib -lxcb $LDFLAGS"
EXEC_NAME="set_x_root_name"

$CC -o $EXEC_NAME set_x_root_name.c $CFLAGS $LDFLAGS $CPPFLAGS || fail $?

echo ">> Build successful"
