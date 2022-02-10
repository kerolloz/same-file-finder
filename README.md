# Same file finder

## Problem Statement

> You have a directory with nested files (or directories). You need to find similar files according to their `md5` hash.

You need to recursively traverse a given directory and hash every file you find. While hashing the files you need to find files with matching hashes. You will also need to save the full path for the hashed files. Finally, you should output the full paths of the files that have the same hash value, if any.

## Solutions

> Test data  
> A directory with ~400 PDF files (3.6 GB)

- [JavaScript](./index.js) (took 1m 40s)