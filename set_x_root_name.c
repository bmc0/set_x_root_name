#include <stdio.h>
#include <signal.h>
#include <xcb/xcb.h>

#define NAME_LENGTH 1024  /* maximum number of characters to read per line */

static volatile sig_atomic_t term_sig = 0;

void sig_handler_term(int sig)
{
	term_sig = sig;
}

int main(int argc, char *argv[])
{
	xcb_connection_t *xc;
	xcb_screen_t *screen;
	struct sigaction sa;
	static char name[NAME_LENGTH];
	int i, ret = 0, c = '\0';

	sa.sa_handler = sig_handler_term;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);

	xc = xcb_connect(NULL, NULL);
	if (xcb_connection_has_error(xc)) {
		fprintf(stderr, "%s: xcb_connect() failed\n", argv[0]);
		ret = 2;
		goto done;
	}
	screen = xcb_setup_roots_iterator(xcb_get_setup(xc)).data;

	while (c != EOF) {
		for (i = 0;; ++i) {
			c = getchar();
			if (c == '\n' || c == EOF) break;
			if (i < NAME_LENGTH) name[i] = c;
		}
		if (term_sig) goto got_term_sig;
		if (xcb_connection_has_error(xc)) {
			fprintf(stderr, "%s: fatal connection error\n", argv[0]);
			ret = 1;
			goto done;
		}
		if (xcb_request_check(xc, xcb_change_property(xc, XCB_PROP_MODE_REPLACE, screen->root, XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8, i, name)) != NULL)
			fprintf(stderr, "%s: xcb_change_property() failed\n", argv[0]);
	}
	done:
	xcb_disconnect(xc);
	return ret;

	got_term_sig:
	fprintf(stderr, "%s: signal %d: terminating...\n", argv[0], term_sig);
	goto done;
}
