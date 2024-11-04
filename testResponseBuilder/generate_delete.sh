#/bin/sh

# Little script for generating both a folder and a file to delete wherever you are in your $PWD
if [ "$USER" = "flverge" ]; then
	cd /home/flverge/Documents/42_pedago/webserv/testResponseBuilder # 42 PATH
	export BACK=$PWD
else # Home path
	cd $HOME
fi

echo "Creating folder_to_delete"
mkdir -p "folder_to_delete"
cd folder_to_delete

echo "Creating file_to_delete"
touch file_to_delete
echo "lorem ipsum" > file_to_delete

cd $BACK
unset BACK