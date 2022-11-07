#!/usr/bin/python

# Gere o seu próprio certificado:
# openssl req -x509 -newkey rsa:2048 -keyout key.pem -out cert.pem -days 365

import http.server
from http.server import HTTPServer, BaseHTTPRequestHandler
from io import BytesIO
import json
import time
import threading
import sys
import os
import ssl

"""
HTTP Request Handler class
"""

class SimpleHTTPRequestHandler(BaseHTTPRequestHandler):
    # --- GET
    def do_GET(self):
        reply = ""
        try : 
           if self.path == '/':
               res = "./web/index.html"
           else:
               res = '.' + self.path
           file = open(res, "rb")
           self.send_response(200)
           self.send_header('Content-type', 'text/html')
           if res.find(".png") > 0: 
              self.send_header('Content-type', 'image/png')
           elif res.find(".jpg") > 0: 
              self.send_header('Content-type', 'image/jpeg')
           elif res.find(".wav") > 0: 
              self.send_header('Content-type', 'audio/wav')
           self.send_header('Content-length', str(os.path.getsize(res)))
           self.end_headers()
           CHUNK_SIZE = 1024 * 100  # 100kB chunks
           while True:
              chunk = file.read(CHUNK_SIZE)
              if not chunk:
                      break
              self.wfile.write(chunk)
        except:
           self.send_response(404)
           self.end_headers()
        return

    # --- DELETE is not implemented
    def do_DELETE(self):
        self.send_response(405)
        self.end_headers()

    # --- POST
    def do_POST(self):
           self.send_response(405)
           self.end_headers()

    # --- PUT
    def do_PUT(self):
         self.send_response(405)
         self.end_headers()

"""
HTTP Server Thread
"""


if __name__ == "__main__":
    httpd = http.server.HTTPServer(('192.168.0.104', 4443),
            SimpleHTTPRequestHandler)

    httpd.socket = ssl.wrap_socket (httpd.socket,
            server_side=True,
            keyfile="./key.pem",
            certfile="./cert.pem",
            ssl_version=ssl.PROTOCOL_TLS)

    httpd.serve_forever()

