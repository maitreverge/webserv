#!/usr/bin/env python3

import sys
import os
from gradio_client import Client
import logging

# Configurer les logs
logging.basicConfig(level=logging.DEBUG,  # DEBUG pour avoir tous les détails
                    format='%(asctime)s - %(levelname)s - %(message)s')

# Sauvegarder les flux originaux
original_stdout = sys.stdout
original_stderr = sys.stderr

try:
    # Rediriger stdout et stderr pour éviter les messages inutiles
    sys.stdout = open(os.devnull, 'w')
    sys.stderr = open(os.devnull, 'w')

    # Lire l'input depuis l'entrée standard (sys.stdin)
    logging.debug("Lecture des données depuis stdin...")
    user_input = sys.stdin.read().strip()
    logging.debug(f"User input: {user_input}")

    # Initialiser le client de l'API
    logging.debug("Initialisation du client Gradio...")
    client = Client("dsylvain/myNewAPI")

    # Appeler l'API avec les données de l'utilisateur
    logging.debug("Appel de l'API Gradio...")
    result = client.predict(user_input=user_input)
    logging.debug(f"Résultat de l'API : {result}")

    # Construire la réponse HTML
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
    logging.debug("HTML généré avec succès.")

    # Calculer la taille du contenu
    content_length = len(html_output.encode('utf-8'))

    # Rétablir stdout et stderr pour écrire la réponse HTTP
    sys.stdout = original_stdout
    sys.stderr = original_stderr

    # Imprimer les en-têtes HTTP
    print("HTTP/1.1 200 OK")
    print("Content-Type: text/html; charset=utf-8")
    print(f"Content-Length: {content_length}")
    print()  # Ligne vide pour séparer les en-têtes du corps

    # Imprimer le corps HTML
    print(html_output)
    logging.debug("Réponse HTTP envoyée avec succès.")

except Exception as e:
    # Récupérer les exceptions et les logguer
    logging.error(f"Une erreur est survenue : {e}")
    sys.stdout = original_stdout
    sys.stderr = original_stderr
    # Envoyer une réponse HTTP 500 en cas d'erreur
    error_message = "HTTP/1.1 500 Internal Server Error\r\n\r\nUne erreur est survenue."
    print(error_message)

finally:
    # Toujours restaurer stdout et stderr
    logging.debug("Restauration des flux stdout et stderr.")
    sys.stdout = original_stdout
    sys.stderr = original_stderr
