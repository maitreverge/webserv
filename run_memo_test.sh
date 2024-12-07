#!/bin/bash

# Fonction de nettoyage
cleanup() {
    echo "Nettoyage en cours..."
    pkill -f "./webserv"  # Arrêter le serveur
    echo "Serveur arrêté."
}

# Installer le piège sur EXIT, INT et TERM pour appeler la fonction cleanup
trap cleanup EXIT INT TERM


# Initialiser le fichier de sortie
echo "" > memoTestOutput.txt

# Lancer Valgrind et rediriger la sortie immédiatement
valgrind --trace-children=yes --leak-check=full --show-leak-kinds=all --track-fds=yes ./webserv -s > memoTestOutput.txt 2>&1 &

# Récupérer le PID du serveur
server_pid=$!

# Attendre que le serveur soit prêt (par exemple, vérifier si le port est ouvert)
echo "Attente que le serveur écoute sur le port 1512..."
while ! ss -tuln | grep -q ":1512"; do
    sleep 1
done
echo "Serveur prêt."

# Exécuter les tests Siege
./run_siege_tests.sh