#!/bin/bash

DB_NAME="scan.db"

set -e

if [ $EUID != 0 ];then
	echo "You aren't root!. Please, you needs execute the 'su' or 'sudo' command to continue." >&2
	exit 1

fi

# We Install dependencies

apt-get install build-essential libmnl0 libmnl-dev python3 curl sqlite3 python3-pip python3-setuptools

# We install dependencies of the libraries

python3 -m pip install -r requirements.txt

# Build the seth program

make

echo -n "Do you want erase the object files? - [y-n]: "
read -N 1 option

if [ $option = "y" ];then
	make clean

fi

sqlite3 $DB_NAME "PRAGMA journal_mode = WAL;"

clear

echo "Done."
