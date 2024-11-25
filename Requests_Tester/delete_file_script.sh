#! /bin/sh

# This scripts aims to finally delete the ressources created by the script creation_files_script.sh

cd Requests_Tester

chmod 666 no_read_file.txt

rm no_read_file.txt

# Delete the whole folder of tests
rm -rf delete_folder

cd ..