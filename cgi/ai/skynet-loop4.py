#!/usr/bin/env python3

import sys

try:
	with open("skynet.html", 'r') as html:
		output = html.read()
		
	content_length = len(output.encode('utf-8'))
	print("HTTP/1.1 200 OK")

	while True:
		...
		
	print("Content-Type: text/html; charset=utf-8")
	
	print(f"Content-Length: {content_length}")
	print()
	print(output)

except Exception as e:
	sys.exit(1) 
