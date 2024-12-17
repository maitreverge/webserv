#!/bin/bash

# Check if two arguments are provided
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <directory1> <directory2>"
    exit 1
fi

DIR1="$1"
DIR2="$2"

# Verify that both arguments are directories
if [ ! -d "$DIR1" ]; then
    echo "Error: $DIR1 is not a directory."
    exit 1
fi

if [ ! -d "$DIR2" ]; then
    echo "Error: $DIR2 is not a directory."
    exit 1
fi

# Find all files in DIR1, ignoring .git directories
echo "Comparing files in $DIR1 and $DIR2..."

find "$DIR1" -type d -name ".git" -prune -o -type f -print | while read -r FILE1; do
    # Get the relative path of the file in DIR1
    RELATIVE_PATH="${FILE1#$DIR1/}"
    FILE2="$DIR2/$RELATIVE_PATH"

    # Check if the corresponding file exists in DIR2
    if [ -f "$FILE2" ]; then
        # Compare the files using diff
        diff -q "$FILE1" "$FILE2" > /dev/null
        if [ $? -ne 0 ]; then
            echo "Difference found: $RELATIVE_PATH"
            diff "$FILE1" "$FILE2"
        fi
    else
        echo "File missing in $DIR2: $RELATIVE_PATH"
    fi
done

# Check for files that are in DIR2 but not in DIR1, ignoring .git directories
find "$DIR2" -type d -name ".git" -prune -o -type f -print | while read -r FILE2; do
    RELATIVE_PATH="${FILE2#$DIR2/}"
    FILE1="$DIR1/$RELATIVE_PATH"

    if [ ! -f "$FILE1" ]; then
        echo "File missing in $DIR1: $RELATIVE_PATH"
    fi
done

