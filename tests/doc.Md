# Framework de Tests pour Webserv

## 1. run_unit_tests.sh : Tests Unitaires avec Google Test

**Prérequis**
Télécharger la dépendance Google Test en tant que sous-module :
```bash
git submodule update --init googletest
```
installer Google Test:
```bash
cd googletest
mkdir build
cd build
cmake ..
make
cd ../..
```
**Utilisation**
Sans argument : Tous les tests unitaires seront exécutés.
```bash
./run_unit_tests.sh
```
un nom de classe en argument : seuls les tests associés à cette classe seront exécutés.
```bash
./run_unit_tests.sh <NomClasse>
```

## 2. run_siege_tests.sh : Tests de Requêtes HTTP avec Siege

**Prérequis**
Adresse IP : dans le fichier **tests/config/my_ip.txt**.
trouve ton adresse avec la commande
```bash
ip a
```
Webserv en cours d'exécution :
Webserv doit être lancé et accessible sur le port 1510.

**Utilisation**
```bash
./run_siege_tests.sh
```
Le script se base sur siege_urls.txt pour tester différentes requêtes.

## 3. run_all_tests.sh : Exécution de tous les tests
```bash
./run_all_tests.sh
```

----

# Prompts ChatGPT

Je developpe une application en C++98.

Je teste cette application a l'aide du framework de test Google Test, compile en C++14.

voici la definition d'une classe que je veux tester:

INSERER_DECLARATION_DE_CLASSE_ICI

Donne moi la definition de la classe de test correspondante, en y ajoutant la redirection de l'output de sortie standard (std::freopen("/dev/null", "w", stdout); et std::freopen("/dev/tty", "w", stdout);).

JE NE VEUX QUE LA DEFINITION POUR L'INSTANT. Ensuite, je te donnerai des methodes a tester une a une pour que tu me produise le code de test. Confirme que tu as compris ce que j'attends de toi.