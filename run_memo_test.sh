#!/bin/bash

# Initialiser le fichier de sortie
echo "" > memoTestOutput.txt

# Lancer Valgrind et rediriger la sortie immédiatement
valgrind ./webserv -s > memoTestOutput.txt 2>&1 &

# Récupérer le PID du serveur
server_pid=$!

# Attendre que le serveur soit prêt (par exemple, vérifier si le port est ouvert)
echo "Attente que le serveur écoute sur le port 1024..."
while ! ss -tuln | grep -q ":1024"; do
    sleep 1
done
echo "Serveur prêt."

# Exécuter les tests Siege
./run_siege_tests.sh