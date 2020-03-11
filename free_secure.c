#include <signal.h>
#include <stdlib.h>

#include "wifi_scan.h"

void free_secure(struct wifi_scan * wifi) {
	static struct wifi_scan * wifi2free = NULL;

	if (wifi != NULL) {
		wifi2free = wifi;
	
	} else {
		wifi_scan_close(wifi2free);

		exit(0);
	
	}

}

void sig_secure(int signum) {
	signal(signum, SIG_IGN);

	free_secure(NULL);

	signal(signum, sig_secure);

}
