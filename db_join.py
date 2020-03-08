import sqlite3
import sys

chgcount = 0
errcount = 0

program = sys.argv[0]
args = sys.argv[1:]

default_db_name = 'scan.db'

try:
    default_db = sqlite3.connect(default_db_name)
    default_cursor = default_db.cursor()

except Exception as Except:
    print("could not open database:", Except)
    sys.exit(1)

def check_mac(mac):
    default_cursor.execute('SELECT COUNT(bssid) FROM router WHERE bssid = {};'.format(repr(mac)))

    result = default_cursor.fetchone()

    if (result[0] >= 1):
        return True

    return False

if (args == []):
    print('Usage: {} <Database filename>'.format(program))
    sys.exit(1)

try:
    db_name = args[0]
    db = sqlite3.connect(db_name)

    cursor = db.cursor()
    cursor.execute('SELECT bssid, ssid, frequency, signal, seen_ms_ago, status, registred, location FROM router;')

    result = cursor.fetchall()

    for _ in result:
        if not (check_mac(_[0])):
            try:
                default_cursor.executemany('INSERT INTO router(bssid, ssid, frequency, signal, seen_ms_ago, status, registred, location) VALUES(?, ?, ?, ?, ?, ?, ?, ?);', ((_,)))
            except Exception as Except:
                errcount += 1

            else:
                chgcount += 1

    default_db.commit()

    default_db.close()
    db.close()

except Exception as Except:
    print("Error:", Except)
    sys.exit(1)

else:
    print('{} insertions with {} errors.'.format(chgcount, errcount))
