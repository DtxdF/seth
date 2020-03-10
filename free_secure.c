#include <signal.h>
#include <stddef.h>

#include "wifi_scan.h"
#include "sqlite3.h"
#include "free_secure.h"

void free_secure(struct free_information * info) {
	static struct free_information * free_info;

	if (info != NULL) {
		free_info = info;
	
	} else {
		wifi_scan_close(free_info->wifi);
		sqlite3_close(free_info->DB);

		exit(0);
	
	}

}

void sig_secure(int signum) {
	signal(signum, SIG_IGN);

	free_secure(NULL);

	signal(signum, sig_secure);

}
