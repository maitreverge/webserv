#!/usr/bin/env python3

import sys
from gradio_client import Client

# Configuration : remplace "dsylvain/myNewAPI" par l'identifiant de ton espace
API_SPACE = "dsylvain/myNewAPI"

def main():
    # Lecture de l'input utilisateur depuis la ligne de commande
    if len(sys.argv) < 2:
        print("Usage: app.py <input>")
        sys.exit(1)

    user_input = sys.argv[1]

    try:
        # Connexion au client Gradio
        client = Client(API_SPACE)
        print(f"Loaded as API: {API_SPACE} ✔")

        # Envoi de l'input utilisateur au modèle
        result = client.predict(user_input=user_input)
        print("Response from API:", result)

    except Exception as e:
        print(f"An error occurred: {e}")

if __name__ == "__main__":
    main()