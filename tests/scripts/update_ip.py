import re

# Chemin vers le fichier contenant la nouvelle adresse IP
ip_fichier = "tests/config/my_ip.txt"

# Chemin vers le fichier siege_urls.txt
fichier = "tests/siege/siege_urls.txt"

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
