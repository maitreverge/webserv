#! /bin/sh

# This scripts aims to finally delete the ressources created by the script creation_files_script.sh

chmod 666 no_read_file.txt

rm -rf no_read_file.txt

# Delete the whole folder of tests
rm -rf Requests_Tester/delete_folder