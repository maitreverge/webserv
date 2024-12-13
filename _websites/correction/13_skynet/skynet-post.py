#!/usr/bin/env python3

from openai import OpenAI
import sys
import os
import requests

sys.stderr = open('/dev/null', 'w')

try:

	url = (
		f"https://webservcgi-default-rtdb.europe-west1.firebasedatabase.app/"
		f"users/user1.json"
	)
	response = requests.get(url)
	data = ""
	if response.status_code == 200:   
		data = response.json()
		if 'login' in data and data['login'] == 'user1':
			del data['login']  

	path_info = os.getenv("PATH_INFO", "").replace("%20", " ").strip("/")
	mood = path_info if path_info else "neutre"
	
	pre_prompt = f"""Réponds selon l'humeur suivante : {mood}.
		Sois court et pertinent. Surtout ne depasse jamais 300 characteres,
		n'hesite pas a mettre des emojies. Utilise les informations, sur
		l'utilisateur, suivantes: {data} pour les integrers d'une facon
		eloquente dans la réponse.
		"""

	user_input = sys.stdin.read().strip()

	client = OpenAI()
	response = client.chat.completions.create(
		model="gpt-4o-mini",
		messages=[
			{"role": "system", "content": pre_prompt},
			{"role": "user", "content": user_input}
		],
		max_tokens=400
	)

	output = response.choices[0].message.content
	content_length = len(output.encode('utf-8'))
	print("HTTP/1.1 200 OK\r")
	print("Content-Type: text/plain; charset=utf-8\r")
	print(f"Content-Length: {content_length}\r")
	print("\r")
	print(output)

except Exception as e:
    sys.exit(1)
