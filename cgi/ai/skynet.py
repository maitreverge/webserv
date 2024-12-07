#!/usr/bin/env python3

import sys

with open("skynet.html", 'r') as html:
    output = html.read()
# while True:
#     ...
#    exit(1)
content_length = len(output.encode('utf-8'))
print("HTTP/1.1 200 OK")
print("Content-Type: text/html; charset=utf-8")
print(f"Content-Length: {content_length}")
print()
print(output)
