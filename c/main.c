#include "main.h"

// This will compile the main.c and utils.c, and run the program with the example directory
// gcc -o c/output.out c/*.c -Wall -Wextra -pedantic -std=gnu17 -lssl -lcrypto && c/output.out example/

HashMap *createHashMapEntry(char *hash, char *filePath) {
    HashMap *entry = (HashMap *)malloc(sizeof(HashMap));
    entry->hash = hash;
    entry->fileList.files = (char **)malloc(sizeof(char *));
    entry->fileList.files[0] = filePath;
    entry->fileList.count = 1;
    entry->next = NULL;
    return entry;
}

void addToFileList(FileList *fileList, char *filePath) {
    fileList->files = (char **)realloc(fileList->files, (fileList->count + 1) * sizeof(char *));
    fileList->files[fileList->count] = filePath;
    fileList->count++;
}

void addToHashMap(HashMap **map, char *hash, char *filePath) {
    HashMap *current = *map;
    while (current != NULL) {
        if (strcmp(current->hash, hash) == 0) {
            addToFileList(&current->fileList, filePath);
            return;
        }
        current = current->next;
    }
    HashMap *newEntry = createHashMapEntry(hash, filePath);
    newEntry->next = *map;
    *map = newEntry;
}

void freeHashMap(HashMap *map) {
    while (map->next != NULL) {
        HashMap *temp = map;
        map = map->next;
        free(temp->hash);
        for (int i = 0; i < temp->fileList.count; ++i) {
            free(temp->fileList.files[i]);
        }
        free(temp->fileList.files);
        free(temp);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Provide a directory in argv\n");
        return EXIT_FAILURE;
    }

    const char *dirPath = argv[1];
    if (!isDirectory(dirPath)) {
        fprintf(stderr, "Provided path is not a directory\n");
        return EXIT_FAILURE;
    }

    int fileCount;
    char **fileList = readDir(dirPath, &fileCount);

    HashMap *hashMap = NULL;
    for (int i = 0; i < fileCount; ++i) {
        char *fileHash = getFileHash(fileList[i]);
        addToHashMap(&hashMap, fileHash, fileList[i]);
    }
    
    for (HashMap *entry = hashMap; entry != NULL; entry = entry->next) {
        if (entry->fileList.count > 1) {
            printf("[");
            for (int i = 0; i < entry->fileList.count; ++i) {
                printf("\"%s\"%s", entry->fileList.files[i], (i < entry->fileList.count - 1) ? ", " : "");
            }
            printf("]\n");
        }
    }

    freeFileList(fileList, fileCount);
    freeHashMap(hashMap);

    return EXIT_SUCCESS;
}
