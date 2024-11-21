#!/bin/bash

# Codes de couleurs ANSI pour la colorisation
RED='\033[31m'
GREEN='\033[32m'
BLUE='\033[34m'
YELLOW='\033[33m'
CYAN='\033[36m'
NC='\033[0m' # No Color

# Dossiers des fichiers
CONFIG_DIR="Requests_Tester/config_files"
REQUESTS_DIR="Requests_Tester/requests"
EXPECTED_ANSWERS_DIR="Requests_Tester/expected_answers"
ANSWERS_DIR="Requests_Tester/answers"
DIFF_DIR="Requests_Tester/diff"

# Fichier de vérification global
CHECK_FILE="Requests_Tester/check_file.ans"
> "$CHECK_FILE" # On réinitialise le fichier à chaque exécution

# Création des répertoires de résultats et de différences s'ils n'existent pas
mkdir -p "$ANSWERS_DIR"
mkdir -p "$DIFF_DIR"

# Boucle sur chaque fichier de configuration dans le répertoire config_files
for config_file in "$CONFIG_DIR"/*.ini; do
	# Extraction du numéro de configuration (par exemple, config_1.ini -> 1)
	config_number=$(basename "$config_file" .ini | sed 's/config_//')

	# Vérification et colorisation des résultats (conf file)
	echo -e "${BLUE}=== Configuration : ${config_file}${NC}" >> "$CHECK_FILE"
	echo >> "$CHECK_FILE"
	echo -e "${YELLOW}$(cat "$config_file")${NC}" >> "$CHECK_FILE"
	echo >> "$CHECK_FILE"

	# Lancement de Webserv avec le fichier de configuration
	./webserv "$config_file" > /dev/null 2>&1 &  # Lancement en arrière-plan de Webserv avec le fichier de configuration
	webserv_pid=$! # Récupère le PID du processus Webserv
	
	#TODO&&&&&&&&&&&&&&& DELAY TO BE ADJUSTED? &&&&&&&&&&&&&&&&&&&&
	sleep 0.2 # Laisser un peu de temps pour que Webserv démarre
	#TODO&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

	# Boucle sur les fichiers de requêtes pour ce fichier de configuration
	request_file="$REQUESTS_DIR/config_${config_number}_requests.txt"

	test_counter=1
	# Lire les requêtes dans le fichier et envoyer chaque requête individuellement
	request_counter=1
	while IFS= read -r request; do
		# Si on rencontre un délimiteur de fin de requête (ligne vide ou "--END-REQUEST--"), on passe à la requête suivante
		if [[ -z "$request" || "$request" == "--END-REQUEST--" ]]; then
			continue
		fi

		# Envoi de la requête à Webserv et enregistrement de la réponse
		response_file="$ANSWERS_DIR/config_${config_number}_test_${test_counter}_actual.txt"
		http_response=$($request)

		# Sauvegarde de la réponse
		echo "$http_response" > "$response_file"
		# Comparaison des réponses avec les résultats attendus
		expected_answer_file="$EXPECTED_ANSWERS_DIR/config_${config_number}_test_${test_counter}_expected.txt"
		
		
		#TODO && UNCOMMENT TO FILL EXPECTED_ANSWER_FILES &&&&&&&&&&
		echo "$http_response" > "$expected_answer_file"
		#TODO &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

		#TODO UNCOMMENT TO HAVE HTTP REQUEST
		# curl -v "localhost:3${config_number}" -d @"$request_file" 2>&1 | grep "^>"

		# Vérification et colorisation des résultats
		echo -e "${RED}--- Requête : $request${NC}" >> "$CHECK_FILE"
		echo >> "$CHECK_FILE"
		echo -e "${BLUE}--- Expected Answer :${NC}" >> "$CHECK_FILE"
		echo -e "${GREEN}$(cat "$expected_answer_file")${NC}" >> "$CHECK_FILE"
		echo >> "$CHECK_FILE"
		echo -e "${BLUE}--- Actual Answer :${NC}" >> "$CHECK_FILE"
		echo >> "$CHECK_FILE"
		echo -e "${CYAN}$(cat "$response_file")${NC}" >> "$CHECK_FILE"
		echo >> "$CHECK_FILE"
		echo >> "$CHECK_FILE"

		diff_file="$DIFF_DIR/config_${config_number}_test_${test_counter}.diff"
		
		if diff -q "$response_file" "$expected_answer_file" > /dev/null; then
			echo -e "${GREEN}config_$config_number, test $test_counter : $request ✅ Success${NC}"
		else
			echo -e "${GREEN}config_$config_number, test $test_counter : $request ❌ Failure${NC}"
			# Enregistrer le diff dans un fichier
			diff "$response_file" "$expected_answer_file" > "$diff_file"
			cat "$diff_file"
		fi
		test_counter=$((test_counter + 1))
	done < "$request_file"

	# Arrêter Webserv une fois tous les tests effectués
	kill "$webserv_pid"
	wait "$webserv_pid" 2>/dev/null || {
		kill -9 "$webserv_pid" 2>/dev/null
	}

done
