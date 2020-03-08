import folium
import sqlite3
from tornado.web import (Application,
                         RequestHandler)
from tornado.httpserver import HTTPServer
from tornado.ioloop import IOLoop
from random import randint

LPORT = 8081
db = sqlite3.connect('scan.db')
cursor = db.cursor()
lat_or_lon = 0

def get_status(code):
    code = int(code)

    if (code == -1):
        return "None"

    elif (code == 0):
        return "Authenticate"

    elif (code == 1):
        return "Associated"

    elif (code == 2):
        return "IBSS JOINED"

    else:
        return "Error!"

def get_location(location_string, signal):
    global lat_or_lon

    (latitude, longitude) = location_string.split(',')
    latitude = float(latitude)
    longitude = float(longitude)

    signal = (-1*signal) - 20

    latitude += (signal / 100000)
    longitude += (signal / 100000)

    return(latitude, longitude)

def get_vendor(mac):
    mac = mac.replace(':', '').upper()
    db = sqlite3.connect('mac-vendor.db')
    cursor = db.cursor()

    cursor.execute('SELECT vendor FROM vendors WHERE mac = {} LIMIT 1;'.format(repr(mac)))

    result = cursor.fetchone()

    db.close()

    if (result == None):
        return "Unknown"

    return result[0]

class Handler(RequestHandler):
    def get(self):
        self.render('index.html')

class THandler(RequestHandler):
    def get(self):
        Map = folium.Map()
        cursor.execute('SELECT * FROM router;')
        result = cursor.fetchall()
        marks = {}

        for _ in result:
            vendor = get_vendor(_[1][:8])

            html = ['<h4>Information of {} ({})</h4>'.format(_[1], vendor),
                    '<strong>ID:</strong> {}'.format(_[0]),
                    '<strong>BSSID:</strong> {}'.format(_[1]),
                    '<strong>SSID:</strong> {}'.format(_[2]),
                    '<strong>Frequency:</strong> {}'.format(_[3]),
                    '<strong>Signal:</strong> {}'.format(_[4]),
                    '<strong>Seen ms ago:</strong> {}'.format(_[5]),
                    '<strong>Status:</strong> {}'.format(_[6]),
                    '<strong>Registred:</strong> {}'.format(_[7]),
                    '<strong>Location:</strong> {}'.format(_[8])
            ]

            iframe = folium.IFrame('<br>'.join(html), width=300, height=200)
            mark = folium.Marker(location=get_location(_[8], _[4]),
                                 popup=folium.Popup(iframe, max_width=300, sticky=True),
                                 icon=folium.Icon(color='red', icon_color='white', icon='map-marker', prefix='fa'),
                                 draggable=True)
            if (marks.get(vendor) == None):
                marks[vendor] = []
            
            marks[vendor].append(mark)

        for i in marks:
            group_name = folium.FeatureGroup(name=i)

            for j in marks[i]:
                j.add_to(group_name)

            group_name.add_to(Map)

        folium.LayerControl().add_to(Map)

        self.write(Map._repr_html_())

app = Application([(r'/', Handler), 
                   (r'/tracked', THandler)])

try:
    httpd = HTTPServer(app, ssl_options={
                                'keyfile'   : 'keyfile',
                                'certfile'  : 'certfile'

                            })

except Exception as Except:
    print('Certificate or password does not exist, you aren\'t using it!. Exception:', Except)
    httpd = HTTPServer(app)

httpd.listen(LPORT)

print("Listening on", LPORT, '...')

try:
    IOLoop().current().start()

except KeyboardInterrupt:
    db.close()
