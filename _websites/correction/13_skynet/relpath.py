#!/usr/bin/env python3

import sys

try:

	sys.stderr = open('/dev/null', 'w')
	filename = 'relpath.txt'  
	with open(filename, 'r') as fichier:
		data = fichier.read() 

	content_length = len(data.encode('utf-8'))

	print("HTTP/1.1 200 OK\r")
	print("Content-Type: text/html; charset=utf-8\r")
	print(f"Content-Length: {content_length}\r")
	print("\r")
	print(data)
		
except Exception as e:
	sys.exit(1)