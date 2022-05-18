#include <stdio.h>

#define BUFFER_SIZE 4096

struct Transformation{
    char old[32];
    char new[32];
};

/**
 * Reads the transformations from the cypher.txt file.
 * @param transformations Pointer containing the transformations read
 * @return Number of transformations found
 */
int readTransformations(struct Transformation* transformations){
    // Open file
    FILE* file = fopen("cypher.txt", "r");
    if(file == NULL){
        perror("file error\n");
        return 0;
    }

    // Read file
    char buffer[BUFFER_SIZE];
    int i = 0;
    int n = 0;
    while(fgets(buffer, BUFFER_SIZE, file)){
        // Old
        char* w1 = strtok(buffer, " ");
        // New
        char* w2 = strtok(NULL, " ");

        // Removes new line
        char* nl = strstr(w2, "\n");
        if(nl != NULL) strncpy(nl, "\0", 1);

        // Stores the transformation and its inverse
        struct Transformation t;
        strcpy(t.old, w1);
        strcpy(t.new, w2);
        transformations[i++] = t;
        strcpy(t.old, w2);
        strcpy(t.new, w1);
        transformations[i++] = t;

        n += 2;
    }

    // Close file
    fclose(file);
    return n;
}
