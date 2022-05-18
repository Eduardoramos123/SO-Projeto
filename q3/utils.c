#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Separetes the buffer, into tokens by a given separator.
 * @param tokens Pointer to buffer where the tokens will be store
 * @param buffer Actual buffer to separate
 * @param separator Separator to be used
 * @param start Next position on tokens buffer
 * @return Number of tokens read
 */
int separate(char** tokens, char* buffer, char* separator, int start){
    char* token = strtok(buffer, separator);
    if(token == NULL) return 0;

    int i = start;
    int n = 0;
    do{
        tokens[i++] = token;
        n++;
        token = strtok(NULL, separator);
    } while(token != NULL);
    return n;
}

/**
 * Initializes a buffers with a given size
 * @param size Size of buffer
 * @return Pointer to the buffer
 */
char** initBuffer(int size){
    char** buffer = (char**)malloc(size);
    memset(buffer, '\0', size);
    return buffer;
}

/**
 * Parses a string into a array of words
 * @param buffer Buffer to parse
 * @param num Return parameter indicating the number of words read
 * @return Buffer with the words
 */
char** parseInput(char* buffer, int* num){
    char** lines = initBuffer(128);
    char** words = initBuffer(128);

    int num_lines = separate(lines, buffer, "\n", 0);
    int num_words = 0;

    for(int i = 0; i < num_lines; i++){
        int w = separate(words, lines[i], " ", num_words);
        num_words += w;
        words[num_words++] = "\n";
    }

    *num = num_words;
    free(lines);
    return words;
}
