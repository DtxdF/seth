#ifndef _FREE_SECURE_H
#define _FREE_SECURE_H

#include "sqlite3.h"
#include "wifi_scan.h"

struct free_information {
	struct wifi_scan * wifi;
	struct sqlite3 * DB;

};

void free_secure(struct free_information * info);
void sig_secure(int signum);

#endif
