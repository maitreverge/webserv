#! /bin/sh

# This scripts aims to create ressources for the DELETE method testing
# some with authorizations, some without
echo $PWD

mkdir -p Requests_Tester/delete_folder
cd Requests_Tester/delete_folder
mkdir -p folder_1

touch file_{1..2}.txt
chmod 755 *

# Write some content in the file for getting them back
echo "This is file_1.txt content" > file_1.txt
echo "This is file_2.txt content" > file_2.txt

echo "I am here: $PWD"
echo "that's what I see:"
ls
# Remove write access to file_1.txt only
chmod -w file_1.txt

cd folder_1
touch hello.txt
chmod 755 *
echo "This is hello.txt content" > file_2.txt

# Back to root
cd ../../

# Create a file which can't be read
touch no_read_file.txt
echo "Content of no_read_file.txt" > no_read_file.txt
chmod 044 no_read_file.txt 