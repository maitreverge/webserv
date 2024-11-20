#!/bin/bash

# Dossiers des fichiers
CONFIG_DIR="Requests_Tester/config_files"
REQUESTS_DIR="Requests_Tester/requests"
EXPECTED_ANSWERS_DIR="Requests_Tester/expected_answers"
ANSWERS_DIR="Requests_Tester/answers"
DIFF_DIR="Requests_Tester/diff"

# Création des répertoires de résultats et de différences s'ils n'existent pas
mkdir -p "$ANSWERS_DIR"
mkdir -p "$DIFF_DIR"

# Boucle sur chaque fichier de configuration dans le répertoire config_files
for config_file in "$CONFIG_DIR"/*.ini; do
	# Extraction du numéro de configuration (par exemple, config_1.ini -> 1)
	config_number=$(basename "$config_file" .ini | sed 's/config_//')

	echo "Running tests for config_$config_number..."

	# Lancement de Webserv avec le fichier de configuration
	./webserv "$config_file" > /dev/null 2>&1 &  # Lancement en arrière-plan de Webserv avec le fichier de configuration
	webserv_pid=$! # Récupère le PID du processus Webserv
	sleep 1 # Laisser un peu de temps pour que Webserv démarre

	# Boucle sur les fichiers de requêtes pour ce fichier de configuration
	request_file="$REQUESTS_DIR/config_${config_number}_requests.txt"
	echo "Testing $request_file..."

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
		http_response=$(echo "$request" | http --ignore-stdin --timeout=2 "localhost:3${config_number}")

		# Sauvegarde de la réponse
		echo "$http_response" > "$response_file"
		# Comparaison des réponses avec les résultats attendus
		expected_answer_file="$EXPECTED_ANSWERS_DIR/config_${config_number}_test_${test_counter}_expected.txt"
		echo "$http_response" > "$expected_answer_file"
		diff_file="$DIFF_DIR/config_${config_number}_test_${test_counter}_diff.txt"

		if diff -q "$response_file" "$expected_answer_file" > /dev/null; then
			echo "Test $test_counter for config_$config_number: ✅ Success"
		else
			echo "Test $test_counter for config_$config_number: ❌ Failure"
			# Enregistrer le diff dans un fichier
			diff "$response_file" "$expected_answer_file" > "$diff_file"
		fi
		test_counter=$((test_counter + 1))
	done < "$request_file"

	# Arrêter Webserv une fois tous les tests effectués
	kill "$webserv_pid"
	wait "$webserv_pid" 2>/dev/null || {
		echo "Webserv did not terminate properly, forcing kill."
		kill -9 "$webserv_pid" 2>/dev/null
	}

	echo "Tests for config_$config_number completed."
done
