#!/bin/sh

echo "CREATING RESSOURCES TO BE DELETED"
echo "THIS IS SOME CONTENT FOR FILE1.JSON" > file1.json
echo "Je voudrai pas creer de debats inutiles en temps de correction, mais on dit chocolatine" > file2.txt
touch impossible_to_delete.txt
chmod 000 impossible_to_delete.txt
cp ../1_get_ok/surprise/cat_weird.jpg .
echo "Ressources created"