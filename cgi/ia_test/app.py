#!/usr/bin/env python3

import sys
from gradio_client import Client

# Configuration : remplace "dsylvain/image_webserv" par l'identifiant de ton espace
API_SPACE = "dsylvain/image_webserv"

def main():
    try:
        # Lecture de l'input utilisateur depuis l'entrée standard
        user_input = sys.stdin.read().strip()

        if not user_input:
            print("Erreur : aucune entrée reçue depuis stdin.")
            sys.exit(1)

        # Connexion au client Gradio
        client = Client(API_SPACE)
        print(f"Loaded as API: {API_SPACE} ✔")

        # Paramètres à envoyer à l'API
        prompt = user_input  # Le prompt utilisateur pour la génération d'images
        negative_prompt = ""  # Laisser vide si tu n'as pas de prompt négatif
        seed = 42  # Un exemple de seed, tu peux le rendre aléatoire
        randomize_seed = False  # Modifier si tu veux randomiser le seed
        width = 124  # Largeur de l'image générée
        height = 124  # Hauteur de l'image générée
        guidance_scale = 7.5  # Exemple de guidance scale
        num_inference_steps = 50  # Exemple de nombre d'étapes d'inférence

        # Envoi de l'input utilisateur au modèle et récupération du résultat
        result = client.predict(
            prompt,
            negative_prompt,
            seed,
            randomize_seed,
            width,
            height,
            guidance_scale,
            num_inference_steps
        )
        print("Response from API:", result)

    except Exception as e:
        print(f"An error occurred: {e}")

if __name__ == "__main__":
    main()
