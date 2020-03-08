SHELL = /bin/sh
CC = gcc
CFLAGS = -Wall -O2 -pipe --verbose
LDLIBS = -lpthread -lmnl
OBJS = wifi_scan.o sqlite3.o
TARGET = seth

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c $(OBJS) $(LDLIBS)

wifi_scan.o: wifi_scan.c wifi_scan.h
	$(CC) $(CFLAGS) -c wifi_scan.c

sqlite3.o: sqlite3.c sqlite3.h
	$(CC) $(CFLAGS) -c sqlite3.c -DSQLITE_OMIT_LOAD_EXTENSION

clean:
	rm -f *.o
