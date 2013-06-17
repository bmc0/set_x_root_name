#!/bin/dash

fail() {
	echo ">> Failed"
	exit $1
}

[ -z "$CC" ] && CC="gcc"
CFLAGS="-ansi -pedantic -Os -Wall $CFLAGS"
LDFLAGS="-lxcb $LDFLAGS"
EXEC_NAME="set_x_root_name"

$CC -o $EXEC_NAME set_x_root_name.c $CFLAGS $LDFLAGS || fail $?
strip -s $EXEC_NAME || echo "strip failed..."

echo ">> Build successful"
