#! /bin/sh

# This scripts aims to create ressources for the DELETE method testing
# some with authorizations, some without

mkdir -p delete_folder
cd delete_folder
mkdir -p folder_1

touch file_{1..2}.txt
chmod 644 *

# Write some content in the file for getting them back
echo "This is file_1.txt content" > file_1.txt
echo "This is file_2.txt content" > file_2.txt

# Remove write access to file_1.txt only
chmod -w file_1.txt

cd folder_1
touch hello.txt
chmod 644 *
echo "This is hello.txt content" > file_2.txt

# Back to root
cd ../../