#!/bin/sh

echo "Creating ressources to delete"
touch file1.json
touch file2.txt
touch impossible_to_delete.txt
chmod 000 impossible_to_delete.txt
cp ../../ressources_to_upload/cat_weird.jpg .
echo "Ressources deleted"