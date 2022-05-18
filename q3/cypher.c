#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "utils.c"
#include "transformations.c"

#define READ_END 0
#define WRITE_END 1

#define TRANSFORMATIONS_SIZE 16

int main(int argc, char** argv){
    // Read transformations from the file
    struct Transformation transformations[TRANSFORMATIONS_SIZE];
    int numTransformations = readTransformations(transformations);
    if(numTransformations == 0) return 1;

    // Parent -> Child
    int fd_parent[2];
    // Child -> Parent
    int fd_child[2];

    // Proccess Buffer
    char buffer1[BUFFER_SIZE];
    char buffer2[BUFFER_SIZE];
    memset(buffer1, '\0', BUFFER_SIZE);
    memset(buffer2, '\0', BUFFER_SIZE);

    // Create pipes
    if(pipe(fd_parent) || pipe(fd_child)){
        perror("pipe error\n");
        return 1;
    }

    // Create child
    pid_t pid;
    if((pid = fork()) < 0){
        perror("fork error\n");
        return 1;
    } else if(pid > 0){
        // Parent
        close(fd_parent[WRITE_END]);
        close(fd_child[READ_END]);

        // Read input from stdin
        if(read(STDIN_FILENO, buffer1, BUFFER_SIZE) < 0){
            perror("read input error\n");   
            return 1;
        }

        // Send input to child
        write(fd_child[WRITE_END], buffer1, strlen(buffer1));
        close(fd_child[WRITE_END]);

        // Read child response
        if(read(fd_parent[READ_END], buffer1, BUFFER_SIZE) < 0){
            perror("read child message error\n");
            return 1;
        }

        close(fd_parent[READ_END]);

        if (waitpid(pid, NULL, 0) < 0) {
            perror("cannot wait for child\n");
        }

        // Write child response to stdout
        write(STDOUT_FILENO, buffer1, strlen(buffer1));
    } else{
        // Child
        close(fd_child[WRITE_END]);
        close(fd_parent[READ_END]);

        // Read input sent by parent
        if(read(fd_child[READ_END], buffer2, BUFFER_SIZE) < 0){
            perror("child failed reading\n");
            return 1;
        }

        close(fd_child[READ_END]);

        // Parse input
        int num_words;
        char** words = parseInput(buffer2, &num_words);

        char result[BUFFER_SIZE];
        memset(result, '\0', BUFFER_SIZE);
        
        // Apply the transformations
        int firstWordOfLine = 1; // Boolean
        for(int j = 0; j < num_words; j++){
            if(strcmp(words[j], "\n") == 0){
                firstWordOfLine = 1;
                strcat(result, "\n");
                continue;
            }

            if(!firstWordOfLine) strcat(result, " ");
            else firstWordOfLine = 0;

            // Boolean indicating if no transformation was found
            int rewrite = 1; // Boolean
            for(int i = 0; i < numTransformations; i++){
                char* ptr = strstr(words[j], transformations[i].old);
                if(ptr == NULL)
                    continue;

                char aux[BUFFER_SIZE];
                memset(aux, '\0', BUFFER_SIZE);

                strcpy(aux, transformations[i].new);

                // Check for punct. on last char of the word
                char last = words[j][strlen(words[j]) - 1];
                if(ispunct(last))
                    aux[strlen(aux)] = last;

                strcat(result, aux);
                rewrite = 0;
            }

            if(rewrite)
                strcat(result, words[j]);
        }
        
        // Sent result to parent
        write(fd_parent[WRITE_END], result, BUFFER_SIZE);
        close(fd_parent[WRITE_END]);
    }

    return 0;
}