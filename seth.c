#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include "sqlite3.h"
#include "wifi_scan.h"
#include "seth.h"

char * get_current_location(sqlite3 * DB, char * buff, size_t limit) {
	sqlite3_stmt * stmt;

	float latitude;
	float longitude;
	
	int result;
	
	char * sql = "SELECT latitude, longitude FROM current_location ORDER BY id DESC LIMIT 1;";

	sqlite3_prepare(DB, sql, -1, &stmt, NULL);

	result = sqlite3_step(stmt);

	if (result == SQLITE_ROW) {
		latitude = sqlite3_column_double(stmt, 0);
		longitude = sqlite3_column_double(stmt, 1);

		snprintf(buff, limit, "%f,%f", latitude, longitude);

		return buff;

	} else {
		return "";
	
	}

}

int insert_intoDB(sqlite3 * DB,
				   unsigned char bssid[BSSID_LENGTH],
				   char * ssid,
				   uint32_t frequency,
				   int32_t signal_mbm,
				   int32_t seen_ms_ago,
				   int status) {
	char * errmsg = NULL;
	char mac[BSSID_STRING_LENGTH];
	char buff[60];

	const char * sql_query[2];
	
	sqlite3_stmt * query[2];

    sql_query[0] = "INSERT INTO router(bssid, ssid, frequency, signal, seen_ms_ago, status, registred, location) VALUES (?, ?, ?, ?, ?, ?, datetime(), ?);";
	sql_query[1] = "SELECT count(bssid) FROM router WHERE bssid = ?;";

	if (sqlite3_exec(DB, "CREATE TABLE IF NOT EXISTS router("
									"id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
									"bssid TEXT NOT NULL,"
									"ssid TEXT NOT NULl,"
									"frequency INTEGER NOT NULL,"
									"signal INTEGER NOT NULL,"
									"seen_ms_ago INTEGER NOT NULL,"
									"status INTEGER NOT NULL,"
									"registred TEXT NOT NULL,"
									"location TEXT);", NULL, NULL, &errmsg) != SQLITE_OK) {
		//printf("Error: %s\n", errmsg);
		return -1;
	
	}

	sqlite3_prepare(DB, sql_query[1], -1, &query[1], NULL);

	sqlite3_bind_text(query[1], 1, bssid_to_string(bssid, mac), -1, SQLITE_STATIC);

	sqlite3_step(query[1]);
	
	if (sqlite3_column_int(query[1], 0) >= 1) {
		return 1;
	
	}

	sqlite3_finalize(query[1]);

	//

	sqlite3_prepare(DB, sql_query[0], -1, &query[0], NULL);

	sqlite3_bind_text(query[0], 1, bssid_to_string(bssid, mac), -1, SQLITE_STATIC);
	sqlite3_bind_text(query[0], 2, ssid, -1, SQLITE_STATIC);
	sqlite3_bind_int(query[0], 3, (int)frequency);
	sqlite3_bind_int(query[0], 4, ((int)signal_mbm)/100);
	sqlite3_bind_int(query[0], 5, (int)seen_ms_ago);
	sqlite3_bind_int(query[0], 6, status);
	sqlite3_bind_text(query[0], 7, get_current_location(DB, buff, sizeof(buff)), -1, SQLITE_STATIC);

	sqlite3_step(query[0]);

	sqlite3_finalize(query[0]);
	
	return 1;

}

int main(int argc, char ** argv) {
	struct params args;

	get_params(argc, argv, &args);
	
	struct wifi_scan * wifi = wifi_scan_init(args.interface);
	struct bss_info bss[args.bss_limit];
	
	char mac[BSSID_STRING_LENGTH];

	int status;
	int sql_status;

	sqlite3 * DB;

	if (sqlite3_initialize() != SQLITE_OK) {
		fprintf(stderr, "Error: %s\n", sqlite3_errmsg(DB));

		return EXIT_FAILURE;
	
	}

	if ((sql_status = sqlite3_open(args.db_name, &DB) != SQLITE_OK)) {
		fprintf(stderr, "Error: %s\n", sqlite3_errmsg(DB));

		return EXIT_FAILURE;
	
	}

	while (true) {
		status = wifi_scan_all(wifi, bss, args.bss_limit);

		if (status < 0) {
			perror("Error");

			return EXIT_FAILURE;
		
		} else {
			clrscr();

			sleep(args.sleeping);

			for (register int i = 0; i < status && i < args.bss_limit; i++) {	
				printf("%d) - %s (%s); Signal: %d; Frequency: %u; Seen ms ago: %d; Status: %s\n",
					  (i+1), bss[i].ssid, bssid_to_string(bss[i].bssid, mac), bss[i].signal_mbm,
					  bss[i].frequency, bss[i].seen_ms_ago, get_status(bss[i].status));
				fflush(stdout);

				insert_intoDB(DB, bss[i].bssid, bss[i].ssid,
							  bss[i].frequency, bss[i].signal_mbm,
							  bss[i].seen_ms_ago, bss[i].status);

			}

		}

	}

	sqlite3_close(DB);
	wifi_scan_close(wifi);

	return EXIT_SUCCESS;

}
