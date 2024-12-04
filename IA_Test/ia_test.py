#!/usr/bin/env python3
# import openai
from openai import OpenAI
import sys
import os
import logging
import requests
logging.basicConfig(level=logging.ERROR) 

url = (
    f"https://webservcgi-default-rtdb.europe-west1.firebasedatabase.app/users"
    f"/user1.json"
)

response = requests.get(url)
data = None
if response.status_code == 200:   
    data = response.json()
else:
    print("Erreur lors de la requête :", response.status_code)
client = OpenAI()

path_info = os.getenv("PATH_INFO", "").replace("%20", " ").strip("/")
mood = path_info if path_info else "neutre"

user_input = sys.stdin.read().strip()

pre_prompt = f"""Réponds selon l'humeur suivante : {mood}.
	Sois court et pertinent. Utilise les informations suivantes {data}
    pour les integrers d'une facon eloquente dans la réponse."""

try:
    response = client.chat.completions.create(
        model="gpt-4o-mini",
        messages=[
            {"role": "system", "content": pre_prompt},
            {"role": "user", "content": user_input}
        ],
        max_tokens=500
    )

    chatbot_response = response.choices[0].message.content
    if "userInput" in user_input:
        user_input = user_input.split('=', 1)[1]

    html_output = f"""
	<!DOCTYPE html>
	<html lang="fr">
	<head>
		<meta charset="UTF-8">
		<meta name="viewport" content="width=device-width, initial-scale=1.0">
		<title>Response</title>
	</head>
	<body>
		<h1>Voici votre réponse :</h1>
		<p>{user_input}:</p>
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
