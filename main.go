package main

import (
	"crypto/md5"
	"encoding/hex"
	"fmt"
	"io"
	"os"
	"path"
)

func getFileHash(filePath string) string {
	file, err := os.Open(filePath)
	if err != nil {
		panic(err)
	}
	defer file.Close()
	hash := md5.New()
	_, err = io.Copy(hash, file)
	if err != nil {
		panic(err)
	}
	return hex.EncodeToString(hash.Sum(nil))
}

func isDirectory(path string) (bool, error) {
	fileInfo, err := os.Stat(path)
	if err != nil {
		return false, err
	}

	return fileInfo.IsDir(), err
}

func readDir(dirPath string) []string {
	arr := make([]string, 0)
	contents, err := os.ReadDir(dirPath)

	if err != nil {
		panic(err)
	}

	for _, file := range contents {
		fullPath := path.Join(dirPath, file.Name())
		if file.IsDir() {
			arr = append(arr, readDir(fullPath)...)
		} else {
			arr = append(arr, fullPath)
		}
	}

	return arr
}

func main() {
	if len(os.Args) != 2 {
		fmt.Println("Please provide a directory path to scan")
		return
	}

	dirPath := os.Args[1]
	isDir, err := isDirectory(dirPath)
	if err != nil {
		panic(err)
	}

	if isDir {
		dict := make(map[string][]string)

		for _, filePath := range readDir(dirPath) {
			fileHash := getFileHash(filePath)
			if arr, ok := dict[fileHash]; ok {
				arr = append(arr, filePath)
				dict[fileHash] = arr
			} else {
				dict[fileHash] = []string{filePath}
			}
		}

		for _, v := range dict {
			if len(v) > 1 {
				fmt.Printf("%q\n", v)
			}
		}
	} else {
		fmt.Println("Provided path is not a directory")
	}
}
