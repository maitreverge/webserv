#!/usr/bin/env python3

import sys
import os
from gradio_client import Client
import logging

original_stdout = sys.stdout
original_stderr = sys.stderr
# Rediriger stdout et stderr pour éviter les messages non désirés
sys.stdout = open(os.devnull, 'w')
sys.stderr = open(os.devnull, 'w')

logging.basicConfig(level=logging.ERROR)  # Changer le niveau à ERROR pour éviter les infos

# Lire l'input depuis l'entrée standard (sys.stdin)
user_input = sys.stdin.read().strip()

client = Client("dsylvain/myNewAPI")

try:
	result = client.predict(user_input=user_input)

	# Rétablir stdout et stderr
	sys.stdout = original_stdout
	sys.stderr = original_stderr
	html_output = f"""
	<!DOCTYPE html>
	<html lang="fr">
	<head>
		<meta charset="UTF-8">
		<meta name="viewport" content="width=device-width, initial-scale=1.0">
		<title>Résultat de l'API</title>
	</head>
	<body>
		<h1>Voici votre réponse :</h1>
		<p>in response to {user_input}:</p>
		<p>{result}</p>
	</body>
	</html>
	"""

	content_length = len(html_output.encode('utf-8'))

	# Imprimer les en-têtes HTTP
	print("HTTP/1.1 200 OK")
	print("Content-Type: text/html; charset=utf-8")
	print(f"Content-Length: {content_length}")
	print()  # Ligne vide pour séparer les en-têtes du corps

	print(html_output)
except Exception as e:
	logging.error(f"An error occurred: {e}")
	sys.stdout = original_stdout
	sys.stderr = original_stderr