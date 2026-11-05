#!/usr/bin/env python3
from http.server import BaseHTTPRequestHandler, HTTPServer
import json
from datetime import datetime, timezone
class Handler(BaseHTTPRequestHandler):
    def do_GET(self):
        if self.path == "/healthz":
            body=json.dumps({"status":"ok","time":datetime.now(timezone.utc).isoformat()}).encode()
            self.send_response(200); self.send_header("Content-Type","application/json")
            self.send_header("Content-Length",str(len(body))); self.end_headers(); self.wfile.write(body)
        elif self.path == "/":
            body=b"<!doctype html><title>University Result Portal</title><h1>University Result Portal</h1><p>Sample service is running.</p><a href='/healthz'>Health check</a>"
            self.send_response(200); self.send_header("Content-Type","text/html; charset=utf-8")
            self.send_header("Content-Length",str(len(body))); self.end_headers(); self.wfile.write(body)
        else: self.send_error(404)
    def log_message(self, fmt, *args): print("%s - %s"%(self.client_address[0],fmt%args),flush=True)
if __name__ == "__main__": HTTPServer(("127.0.0.1",8000),Handler).serve_forever()
