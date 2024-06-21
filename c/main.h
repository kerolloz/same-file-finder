#ifndef MAIN_H
#define MAIN_H

#include <openssl/md5.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdint.h>

typedef struct FileList {
    char **files;
    int count;
} FileList;

typedef struct HashMap {
    char *hash;
    FileList fileList;
    struct HashMap *next;
} HashMap;

void freeFileList(char **fileList, int fileCount);

char *getFileHash(const char *filePath);
char **readDir(const char *dirPath, int *fileCount);

uint8_t isDirectory(const char *path);


#endif // MAIN_H
