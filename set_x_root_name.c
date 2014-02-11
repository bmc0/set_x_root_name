/* Sets the root window name every time a line is read from stdin to the
   contents of the line read.
*/

#include <xcb/xcb.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#define NAME_LENGTH 1024  /* maximum number of characters read per line */

static xcb_connection_t *xc;
static xcb_screen_t *screen;
static char name[NAME_LENGTH];

void sigh(int sig)
{
	fprintf(stderr, "Signal %d, exiting...\n", sig);
	xcb_disconnect(xc);
	exit(1);
}

int main(int argc, char *argv[])
{
	int i, c = '\0';

	xc = xcb_connect(NULL, NULL);
	if (xcb_get_setup(xc) == NULL) {
		fprintf(stderr, "%s: unable to open display\n", argv[0]);
		xcb_disconnect(xc);
		exit(2);
	}
	screen = xcb_setup_roots_iterator(xcb_get_setup(xc)).data;

	signal(SIGINT, sigh);
	signal(SIGTERM, sigh);

	while (c != EOF) {
		for (i = 0; i < NAME_LENGTH; ++i) {
			c = getchar();
			if (c == '\n' || c == EOF)
				break;
			name[i] = c;
		}
		if (c != EOF) {
			xcb_change_property(xc, XCB_PROP_MODE_REPLACE, screen->root, XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8, i, name);
			xcb_flush(xc);
		}
	}
	xcb_disconnect(xc);

	return 0;
}
