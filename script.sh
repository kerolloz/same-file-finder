#!/bin/bash

# Function to calculate the MD5 hash of a file
get_file_hash() {
    md5sum "$1" | awk '{ print $1 }'
}

# Function to recursively read directories and find files
read_dir() {
    find "$1" -type f
}

# Check if directory argument is provided and valid
if [ "$#" -ne 1 ] || [ ! -d "$1" ]; then
    echo "Please provide a valid directory path"
    exit 1
fi

dir="$1"
declare -A hash_storage

# Populate hash storage with file hashes
while IFS= read -r file; do
    hash=$(get_file_hash "$file")
    if [ -n "${hash_storage[$hash]}" ]; then
        hash_storage["$hash"]+="$file "
    else
        hash_storage["$hash"]="$file "
    fi
done < <(read_dir "$dir")

# Find and print files with similar hashes
for key in "${!hash_storage[@]}"; do
    files=(${hash_storage[$key]})
    if [ ${#files[@]} -gt 1 ]; then
        echo "${files[@]}"
    fi
done

exit 0
