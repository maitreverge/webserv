import os
import re

ip_fichier = "tests/config/my_ip.txt"

fichier = "tests/siege/siege_urls.txt"

# Vérification et création des fichiers s'ils n'existent pas
os.makedirs(os.path.dirname(ip_fichier), exist_ok=True)
os.makedirs(os.path.dirname(fichier), exist_ok=True)

# Création de ip_fichier s'il n'existe pas
if not os.path.isfile(ip_fichier):
    with open(ip_fichier, 'w') as f:
        f.write('0.0.0.0')  # Fichier vide ou avec une IP par défaut

# Création de fichier s'il n'existe pas
if not os.path.isfile(fichier):
    with open(fichier, 'w') as f:
        f.write('http://0.0.0.0:1024\n')  # Ajouter une URL par défaut

try:
	# Lecture de la nouvelle adresse IP depuis le fichier
	with open(ip_fichier, 'r') as f:
		nouvelle_ip = f.readline().strip()  # Lire la première ligne et enlever les espaces

	# Lecture du fichier et remplacement de l'IP
	with open(fichier, 'r') as f:
		contenu = f.readlines()

	# Expression régulière pour remplacer l'adresse IP
	pattern = r'http://([0-9]+\.[0-9]+\.[0-9]+\.[0-9]+)'

	# Remplacement de l'ancienne IP par la nouvelle
	nouveau_contenu = [re.sub(pattern, f'http://{nouvelle_ip}', ligne) for ligne in contenu]

	# Écriture du nouveau contenu dans le fichier
	with open(fichier, 'w') as f:
		f.writelines(nouveau_contenu)

	print(f"L'adresse IP a été mise à jour dans {fichier} avec {nouvelle_ip}.")
except FileNotFoundError:
	print(f"Le fichier {fichier} ou {ip_fichier} n'a pas été trouvé.")
except Exception as e:
	print(f"Une erreur est survenue : {e}")
