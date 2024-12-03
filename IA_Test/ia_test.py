#!/usr/bin/env python3

import openai
import sys
import os
import logging

logging.basicConfig(level=logging.ERROR) 

openai.api_key = ("sk-proj-Lr-uJ-sX316xnR7-Owv09X8GERyKZCrdeJviLGUWQFV_2JNAVphF"
	"vMXGOjG03SaPJ6KpdwWcoiT3BlbkFJtUbHEhwMu__LraTcV5qqCeOKWgjMKi2_VuwwG6WtQaX"
	"LYDuvVcUk59h-BfThffRsmJsbaFEPAA")

path_info = os.getenv("PATH_INFO", "").replace("%20", " ").strip("/")
humeur = path_info if path_info else "neutre"

user_input = sys.stdin.read().strip()

pre_prompt = f"Réponds selon l'humeur suivante : {humeur}. \
	Sois court et pertinent."

try:
    response = openai.ChatCompletion.acreate(
        model="gpt-3.5-turbo",
        messages=[
            {"role": "system", "content": pre_prompt},
            {"role": "user", "content": user_input}
        ],
        max_tokens=50
    )

    chatbot_response = response["choices"][0]["message"]["content"]

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
		<p>{chatbot_response}</p>
	</body>
	</html>
	"""

    content_length = len(html_output.encode('utf-8'))

    print("HTTP/1.1 200 OK")
    print("Content-Type: text/html; charset=utf-8")
    print(f"Content-Length: {content_length}")
    print() 

    print(html_output)

except Exception as e:
    print("Content-Type: text/plain\n")
    print(f"Erreur : {str(e)}")
