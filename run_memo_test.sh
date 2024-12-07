#!/bin/bash

# Initialiser le fichier de sortie
echo "" > memoTestOutput.txt

# Lancer Valgrind et rediriger la sortie immédiatement
valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes ./webserv -s > memoTestOutput.txt 2>&1 &

sleep 4

# Exécuter les tests Siege
./run_siege_tests.sh