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
import json
import requests

class S(BaseHTTPRequestHandler):


    def _set_headers(self):
        self.send_response(200)
        self.send_header('Content-type', 'application/json')
        self.end_headers()

    def do_GET(self):
        leftIP = "http://192.168.10.10"
        rightIP = "http://192.168.10.11"

        if  "/leds" in self.path:
            print("Connection received")
            args = parse_qs(urlparse(self.path).query)
            side = args["side"][0]
            r = args["R"][0]
            g = args["G"][0]
            b = args["B"][0]
            t = args["t"][0]
            zone = args["z"][0]
            self._set_headers()
            ret = {}
            ret["Side"] = side
            ret["Zone"] = t
            ret["R"] = r
            ret["G"] = g
            ret["B"] = b
            ret["Fadetime"] = t

            if side == "left":
                requests.get(leftIP + self.path)
            elif side == "right":
                requests.get(rightIP + self.path)
            elif side == "both":
                requests.get(leftIP + self.path)
                requests.get(rightIP + self.path)


            lst = []
            lst.append(ret)
            retStr = json.dumps(lst)
            self.wfile.write(retStr.encode('utf-8'))

        if "/rightSide" in self.path:
            args = parse_qs(urlparse(self.path).query)
            pattern = args["pattern"][0]
            if pattern == "greenRed":
                requests.get(rightIP + "/leds?z=2&G=255&R=0&B=0&t=0")
                requests.get(rightIP + "/leds?z=1&R=255&G=0&B=0&t=0")
            if pattern == "redGreen":
                requests.get(rightIP + "/leds?z=1&G=255&R=0&B=0&t=0")
                requests.get(rightIP + "/leds?z=2&R=255&G=0&B=0&t=0")
        if "/leftSide" in self.path:
            args = parse_qs(urlparse(self.path).query)
            pattern = args["pattern"][0]
            if pattern == "greenRed":
                requests.get(leftIP + "/leds?z=2&G=255&R=0&B=0&t=0")
                requests.get(leftIP + "/leds?z=1&R=255&G=0&B=0&t=0")
            if pattern == "redGreen":
                requests.get(leftIP + "/leds?z=1&G=255&R=0&B=0&t=0")
                requests.get(leftIP + "/leds?z=2&R=255&G=0&B=0&t=0")
        

    def do_HEAD(self):
        self._set_headers()
        
    def do_POST(self):
        # Doesn't do anything with posted data
        self._set_headers()
        self.wfile.write("<html><body><h1>POST!</h1></body></html>")
        
def run(server_class=HTTPServer, handler_class=S, port=80):
    server_address = ('', port)
    httpd = server_class(server_address, handler_class)
    print('Starting httpd...')
    httpd.serve_forever()

if __name__ == "__main__":
    from sys import argv

    if len(argv) == 2:
        run(port=int(argv[1]))
    else:
        run(port=43021)
