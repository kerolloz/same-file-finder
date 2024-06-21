#include "main.h"

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

/*
    * Ignored -Wdeprecated-declarations because:
         MD5_Init, 
         MD5_Update, 
         MD5_Final 
    * are deprecated
*/


char *getFileHash(const char *filePath) {
    FILE *file = fopen(filePath, "rb");
    if (!file) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    MD5_CTX md5Context;
    MD5_Init(&md5Context);

    unsigned char data[1024];
    size_t bytesRead;
    while ((bytesRead = fread(data, 1, sizeof(data), file)) != 0) {
        MD5_Update(&md5Context, data, bytesRead);
    }

    unsigned char hash[MD5_DIGEST_LENGTH];
    MD5_Final(hash, &md5Context);

    fclose(file);

    char *hashString = malloc(MD5_DIGEST_LENGTH * 2 + 1);
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
        sprintf(&hashString[i * 2], "%02x", hash[i]);
    }

    return hashString;
}

uint8_t isDirectory(const char *path) {
    struct stat pathStat;
    if (stat(path, &pathStat) != 0) {
        perror("stat");
        exit(EXIT_FAILURE);
    }
    return S_ISDIR(pathStat.st_mode);
}

char **readDir(const char *dirPath, int *fileCount) {
    *fileCount = 0;
    char **fileList = NULL;
    DIR *dir = opendir(dirPath);
    if (!dir) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                char subdirPath[1024];
                snprintf(subdirPath, sizeof(subdirPath), "%s/%s", dirPath, entry->d_name);
                int subdirFileCount;
                char **subdirFileList = readDir(subdirPath, &subdirFileCount);
                fileList = realloc(fileList, (*fileCount + subdirFileCount) * sizeof(char *));
                for (int i = 0; i < subdirFileCount; ++i) {
                    fileList[*fileCount + i] = subdirFileList[i];
                }
                *fileCount += subdirFileCount;
                free(subdirFileList);
            }
        } else {
            fileList = realloc(fileList, (*fileCount + 1) * sizeof(char *));
            fileList[*fileCount] = malloc(strlen(dirPath) + strlen(entry->d_name) + 2);
            sprintf(fileList[*fileCount], "%s/%s", dirPath, entry->d_name);
            (*fileCount)++;
        }
    }

    closedir(dir);
    return fileList;
}

void freeFileList(char **fileList, int fileCount) {
    for (int i = 0; i < fileCount; ++i) {
        free(fileList[i]);
    }
    free(fileList);
}
