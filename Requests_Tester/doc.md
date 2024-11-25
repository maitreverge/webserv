Commande pour générer automatiquement des fichiers de requete avec deux requetes de base:

for i in {001..014}; do echo -e "http GET localhost:$((3000 + ${i#0}))/index.html\n--END-REQUEST--\nhttp GET localhost:$((3000 + ${i#0}))/index.html\n--END-REQUEST--" > "Requests_Tester/requests/config_${i}_requests.txt"; done

