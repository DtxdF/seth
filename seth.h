#ifndef _SAMPLE_H
#define _SAMPLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#include "wifi_scan.h"

// Default values

#define DB_NAME "scan.db"
#define SLEEPING 1
#define BSS_LIMIT 30

// arguments of the command line

struct params {
	char * interface;
	char * db_name;
	int sleeping;
	int bss_limit;

};

void get_params(int argc, char * argv[], struct params * args) {
	int opt;
	char * ptr;

	args->interface = NULL;
	args->db_name = DB_NAME;
	args->sleeping = SLEEPING;
	args->bss_limit = BSS_LIMIT;

	while ((opt = getopt(argc, argv, "i:d:s:b:h")) != -1) {
		switch(opt) {
			case 'i':
				args->interface = optarg;
				break;

			case 'd':
				args->db_name = optarg;
				break;

			case 's':
				args->sleeping = (int)strtol(optarg, &ptr, 10);
				break;

			case 'b':
				args->bss_limit = (int)strtol(optarg, &ptr, 10);
				break;

			case 'h':
				printf("Syntax: %s [OPTIONS] -i <Interface name>\n", argv[0]);
				printf("\n");
				printf("-i <Interface:str>     -     The interface name to listen the wifi networks.\n");
				printf("-d <Database:str>      -     The database name to save the wifi network information. Default: %s\n",
					   DB_NAME);
				printf("-s <Interval:int>      -     The interval of times to listen the wifi networks. Default: %d\n",
					   SLEEPING);
				printf("-b <BSS limit:int>     -     The BSS (Basic service set) limits. Default: %d\n",
					   BSS_LIMIT);
				printf("-h                     -     Show this text\n");
				exit(1);

			case ':':
				fprintf(stderr, "%s: option '%c' requires an argument!\n",
								argv[0], optopt);
				exit(1);

			case '?':
			default:
				fprintf(stderr, "%s: option '%c' is invalid and ignored.\n",
								argv[0], optopt);

		}

	}

	if ((args->sleeping == 0) || (args->bss_limit == 0)) {
		fprintf(stderr, "%s: The value of a parameter requires an integer and greater than 0.\n", argv[0]);
		exit(1);
	
	}

	if (!(args->interface)) {
		fprintf(stderr, "%s: The interface name needs a value!\n", argv[0]);
		exit(1);

	}

}

// Convert a bssid numeric to bssid string

const char * bssid_to_string(uint8_t bssid[BSSID_LENGTH], char bssid_string[BSSID_STRING_LENGTH]) {
	snprintf(bssid_string, BSSID_STRING_LENGTH, "%02x:%02x:%02x:%02x:%02x:%02x",
		 bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5]);

	return bssid_string;

}

// Get the network status

char * get_status(int status) {
	switch (status) {
		case -1:
			return "None";

		case 0:
			return "Authenticated";
	
		case 1:
			return "Associated";

		case 2:
			return "IBSS JOINED";

		default:
			return "Error";

	}

}

// Clear stdout screen

void clrscr(void) {
	printf("\033[1f\033[2J");

}

#endif
