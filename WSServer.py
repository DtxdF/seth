import sqlite3
from tornado.web import (Application,
                         RequestHandler)
from tornado.websocket import WebSocketHandler
from tornado.httpserver import HTTPServer
from tornado.ioloop import IOLoop
from json import loads

LPORT = 8080
db = sqlite3.connect('scan.db')
cursor = db.cursor()

cursor.execute('PRAGMA journal_mode = OFF;')

def save_location(result):
    data = tuple(result.values())

    try:
        cursor.execute('CREATE TABLE current_location(id INTEGER PRIMARY KEY AUTOINCREMENT, latitude INTEGER NOT NULL, longitude INTEGER NOT NULL, accuracy INTEGER, altitude INTEGER, altitudeAccuracy INTEGER, speed INTEGER, heading INTEGER, registred TEXT NOT NULL);')
        cursor.executemany('INSERT INTO current_location(latitude, longitude, accuracy, altitude, altitudeAccuracy, speed, heading, registred) VALUES (?, ?, ?, ?, ?, ?, ?, datetime());', (data,))
    
    except sqlite3.OperationalError:
        cursor.executemany('UPDATE current_location SET latitude = ?, longitude = ?, accuracy = ?, altitude = ?, altitudeAccuracy = ?, speed = ?, heading = ?, registred = datetime();', (data,))

    try:
        db.commit()

    except Exception as Except:
        pass

class WSHandler(WebSocketHandler):
    def check_origin(self, origin):
        return True

    def open(self):
        print("New connection is available!")

    def on_message(self, message):
        data = loads(message)
        
        save_location(data);

    def on_close(self):
        print("Closed!")

app = Application([(r'/', WSHandler), ])

try:
    wsd = HTTPServer(app, ssl_options={
                                'keyfile'   : 'keyfile',
                                'certfile'  : 'certfile'
        
                            })

except Exception as Except:
    print('Certificate or password does not exist, you aren\'t using it!. Exception:', Except)
    wsd = HTTPServer(app)

wsd.listen(LPORT)

try:
    print('Listening on', LPORT, '...')

    IOLoop().current().start()

except KeyboardInterrupt:
    db.close()
