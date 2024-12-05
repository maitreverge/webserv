#!/usr/bin/env python3

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

if 'login' in data and data['login'] == 'user1':
    del data['login']  
 
pre_prompt = f"""Réponds selon l'humeur suivante : {mood}.
	Sois court et pertinent. Utilise les informations sur l'utilisateur
    suivantes {data} pour les integrers d'une facon eloquente dans la réponse.
    Surtout ne depasse jamais 400 characteres,
    n'hesite pas a mettre des emojies."""

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
		<p>{chatbot_response}</p>
	"""

    content_length = len(html_output.encode('utf-8'))

    print("HTTP/1.1 200 OK")
    print("Content-Type: text/plain; charset=utf-8")
    print(f"Content-Length: {content_length}")
    print() 

    print(html_output)

except Exception as e:
    print("Content-Type: text/plain\n")
    print(f"Erreur : {str(e)}")
