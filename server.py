#!/usr/bin/env python
"""
Very simple HTTP server in python.
Usage::
    ./dummy-web-server.py [<port>]
Send a GET request::
    curl http://localhost
Send a HEAD request::
    curl -I http://localhost
Send a POST request::
    curl -d "foo=bar&bin=baz" http://localhost

    example command: http://192.168.43.75/leds?side=both&z=0&R=0&G=0&B=0&t=0&submit=
"""
from http.server import BaseHTTPRequestHandler, HTTPServer
from urllib.parse import urlparse, parse_qs
import datetime
import json
import requests
import sched
import time

leftIP = "http://192.168.10.10"
rightIP = "http://192.168.10.11"
leftState = ""
rightState = ""

class S(BaseHTTPRequestHandler):


    def _set_headers(self):
        self.send_response(200)
        self.send_header('Content-type', 'application/json')
        self.end_headers()

    def do_GET(self):


        if  "/leds" in self.path:
            print("Connection received")
            args = parse_qs(urlparse(self.path).query)
            side = args["side"][0]
            picker = args["picker"][0]
            r = int(picker[1:3], 16)
            g = int(picker[3:5], 16)
            b = int(picker[5:7], 16)
            t = args["t"][0]
            zone = args["z"][0]
            self._set_headers()
            params = {}
            params["side"] = side
            params["z"] = zone
            params["R"] = r
            params["G"] = g
            params["B"] = b
            params["t"] = t

            r = ""
            try:
                if side == "left":
                    r = requests.get(leftIP + "/leds", params=params)
                    if r.status_code == 200:
                        leftState = params
                elif side == "right":
                    r = requests.get(rightIP + "/leds", params=params)
                    if r.status_code == 200:
                        rightState = params
                elif side == "both":
                    r = []
                    r.append(requests.get(leftIP + "/leds", params=params))
                    r.append(requests.get(rightIP + "/leds", params=params))
                    if r[0].status_code == 200 and r[1].status_code == 200:
                        leftState = params
                        rightState = params
            except requests.ConnectionError:
                print("Error connecting")

            lst = []
            lst.append(params)
            paramsStr = json.dumps(lst)
            self.wfile.write(paramsStr.encode('utf-8'))

        # Used for setting patterns
        if "pattern" in self.path:
            ip = ""
            if "leftSide" in self.path:
                ip = leftIP
            else:
                ip = rightIP
            args = parse_qs(urlparse(self.path).query)
            pattern = args["pattern"][0]
            r = []
            try:
                if pattern == "greenRed":
                    r.append(requests.get(ip + "/leds?z=2&G=255&R=0&B=0&t=0"))
                    r.append(requests.get(ip + "/leds?z=1&R=255&G=0&B=0&t=0"))
                if pattern == "redGreen":
                    r.append(requests.get(ip + "/leds?z=1&G=255&R=0&B=0&t=0"))
                    r.append(requests.get(ip + "/leds?z=2&R=255&G=0&B=0&t=0"))
            except requests.ConnectionError:
                print("Error connecting ")
            print(r)

    def do_HEAD(self):
        self._set_headers()
        
    def do_POST(self):
        # Doesn't do anything with posted data
        self._set_headers()
        self.wfile.write("<html><body><h1>POST!</h1></body></html>")

    def processStatus(self, r):
        if r is requests.models.Response:
         
        elif r is list:
            for i  in r:
                t = 1


def lightTimer():
    off = False
    on = True
    if off:
        requests.get(leftIP + "/leds?z=3&R=0&G=0&B=0&t=0")
        requests.get(rightIP + "/leds?z=3&R=0&G=0&B=0&t=0")
    elif on:
            requests.get(leftIP + "/leds", params=leftState)
            requests.get(rightIP + "/leds", params=rightState)     

def run(server_class=HTTPServer, handler_class=S, port=80):
    thread = threading.Thread(target=timer, args=())
    thread.daemon = True
    thread.start()
    server_address = ('', port)
    httpd = server_class(server_address, handler_class)
    print('Starting httpd...')
    httpd.serve_forever()
    sch = sched.scheduler(time.time, time.sleep)
    

    

if __name__ == "__main__":
    from sys import argv

    if len(argv) == 2:
        run(port=int(argv[1]))
    else:
        run(port=43021)
