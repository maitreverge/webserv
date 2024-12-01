#!/bin/bash

# variables de compteurs de tests réussis qui vont bien
total_tests=1
successful_tests=1

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

# launch creation_files_script.sh
./Requests_Tester/creation_files_script.sh

# Création des répertoires de résultats et de différences s'ils n'existent pas
mkdir -p "$ANSWERS_DIR"
mkdir -p "$DIFF_DIR"

# Vérifie si un fichier de configuration est passé en argument
if [ -n "$1" ]; then
	CONFIG_FILE="$1" # Si un fichier est spécifié, on l'utilise
	show_diffs=true
else
	CONFIG_FILE="" # Sinon, on traite tous les fichiers
	show_diffs=false
fi

# Si un fichier spécifique est donné, on le traite
if [ -n "$CONFIG_FILE" ]; then
	# Traitement d'un seul fichier de configuration
	config_file="$CONFIG_FILE"
	# Ton code pour traiter ce fichier unique
	config_number=$(basename "$config_file" .ini | sed 's/config_//')

	echo -e "${BLUE}=== Configuration : ${config_file}${NC}" >> "$CHECK_FILE"
	echo >> "$CHECK_FILE"
	echo -e "${YELLOW}$(cat "$config_file")${NC}" >> "$CHECK_FILE"
	echo >> "$CHECK_FILE"

	# Lancer Webserv
	./webserv "$config_file" > /dev/null 2>&1 &  # Lancement en arrière-plan de Webserv
	webserv_pid=$!  # Récupère le PID du processus Webserv
	
	# Attente pour permettre à Webserv de démarrer
	#TODO&&&&&&&&&&&&&&& DELAY TO BE ADJUSTED? &&&&&&&&&&&&&&&&&&&&
	sleep 1
	#TODO&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

	# Boucle pour lire les requêtes et envoyer à Webserv
	request_file="$REQUESTS_DIR/config_${config_number}_requests.txt"
	test_counter=1
	while IFS= read -r request; do
		# Traitement des requêtes, comme dans ton code précédent
		if [[ -z "$request" || "$request" == "--END-REQUEST--" ]]; then
			continue
		fi

		response_file="$ANSWERS_DIR/config_${config_number}_test_${test_counter}_actual.txt"
		http_response=$(eval $request)
		echo "$http_response" > "$response_file"


		#TODO && UNCOMMENT TO FILL EXPECTED_ANSWER_FILES &&&&&&&&&&
		# echo "$http_response" > "$response_file"
		#TODO &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

		# Comparaison et enregistrement des réponses attendues
		expected_answer_file="$EXPECTED_ANSWERS_DIR/config_${config_number}_test_${test_counter}_expected.txt"
		
		# Vérification des réponses
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
		
		if diff -Z -q "$response_file" "$expected_answer_file" > /dev/null; then
			echo -e "${GREEN}config_$config_number, test $test_counter ✅ Success${NC}"
			successful_tests=$((successful_tests + 1))
		else
			echo -e "${RED}config_$config_number, test $test_counter : $request ❌ Failure${NC}"
			diff -Z "$response_file" "$expected_answer_file" > "$diff_file"
			cat "$diff_file"
		fi
		test_counter=$((test_counter + 1))
		total_tests=$((total_tests + 1))
	done < "$request_file"

	# Arrêter Webserv une fois tous les tests effectués
	kill "$webserv_pid"
	wait "$webserv_pid" 2>/dev/null || {
		kill -9 "$webserv_pid" 2>/dev/null
	}

else
	# Sinon, on parcourt tous les fichiers de configuration
	for config_file in "$CONFIG_DIR"/*.ini; do
		# Traitement de chaque fichier de configuration comme précédemment
		config_number=$(basename "$config_file" .ini | sed 's/config_//')

		echo -e "${BLUE}=== Configuration : ${config_file}${NC}" >> "$CHECK_FILE"
		echo >> "$CHECK_FILE"
		echo -e "${YELLOW}$(cat "$config_file")${NC}" >> "$CHECK_FILE"
		echo >> "$CHECK_FILE"

		./webserv "$config_file" > /dev/null 2>&1 &  # Lancer Webserv en arrière-plan
		webserv_pid=$!

		# Attente pour permettre à Webserv de démarrer
		#TODO&&&&&&&&&&&&&&& DELAY TO BE ADJUSTED? &&&&&&&&&&&&&&&&&&&&
		sleep 1
		#TODO&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

		request_file="$REQUESTS_DIR/config_${config_number}_requests.txt"
		test_counter=1
		while IFS= read -r request; do
			if [[ -z "$request" || "$request" == "--END-REQUEST--" ]]; then
				continue
			fi

			response_file="$ANSWERS_DIR/config_${config_number}_test_${test_counter}_actual.txt"
			http_response=$(eval $request)
			echo "$http_response" > "$response_file"
			
			expected_answer_file="$EXPECTED_ANSWERS_DIR/config_${config_number}_test_${test_counter}_expected.txt"
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
			
			if diff -Z -q "$response_file" "$expected_answer_file" > /dev/null; then
				echo -e "${GREEN}$total_tests => config_$config_number, test $test_counter ✅ Success${NC}"
				successful_tests=$((successful_tests + 1))
			else
				echo -e "${RED}$total_tests => config_$config_number, test $test_counter : $request ❌ Failure${NC}"
				if [ "$show_diffs" = true ]; then
					diff -Z "$response_file" "$expected_answer_file" > "$diff_file"
					cat "$diff_file"
				fi
			fi
			total_tests=$((total_tests + 1))
			test_counter=$((test_counter + 1))
		done < "$request_file"

		kill "$webserv_pid"
		wait "$webserv_pid" 2>/dev/null || {
			kill -9 "$webserv_pid" 2>/dev/null
		}

	done
fi

echo ""
echo -e "${CYAN}Résumé global :${NC}"
echo -e "${GREEN}Tests réussis : $successful_tests/$total_tests${NC}"

if [ "$successful_tests" -ne "$total_tests" ]; then
    echo -e "${RED}Certains tests ont échoué. Veuillez vérifier les résultats.${NC}"
else
    echo -e "${GREEN}Tous les tests ont été réussis ! 🎉${NC}"
fi

# Delete Florian ressources (including the no_read_file)
./Requests_Tester/delete_file_script.sh