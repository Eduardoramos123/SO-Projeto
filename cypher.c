#include <sys/wait.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string.h>

#define READ_END 0
#define WRITE_END 1

#define LINESIZE 4096


int getIndex(char** words, char* word, int size) {
	int res = -1;
	for (int i = 0; i < size; i++) {
		if (strstr(word, words[i]) != NULL) {
			res = i;
		}
	}
	return res;
}

int main(int argc, char* argv[]) {
	if (argc == 3) {
		FILE *cypher, *file1, *file2;
		
		int nbytes, pipe1[2], pipe2[2];
		pid_t pid;
		char line[LINESIZE];
		
		if ((cypher = fopen("cypher.txt", "r")) == NULL) {
			perror("cannot open file");
			return 1;
		}
		
		int num_words = 0;
		char* st;
		size_t s = 0;
		char* words[250];
		char total[1024];
		strcpy(total, "");
		
		
		while ( getline(&st, &s, cypher) > 0) {
			strcat(total, st);
		}
		
		char* token1 = strtok(total," \n");
		while( token1 != NULL ) {
			words[num_words++] = token1;
			token1=strtok(NULL," \n");
		}
		
		if (pipe(pipe1) < 0) {
			perror("pipe error");
			exit(EXIT_FAILURE);
		}
		
		if (pipe(pipe2) < 0) {
			perror("pipe error");
			exit(EXIT_FAILURE);
		}
		
		if ((pid = fork()) < 0) {
			perror("fork error");
			exit(EXIT_FAILURE);
		}
		else if (pid > 0) {
			close(pipe1[READ_END]);
			close(pipe2[WRITE_END]);
			
			if ((file1 = fopen(argv[1], "r")) == NULL) {
				perror("cannot open file");
				return 1;
			}
			
			if ((file2 = fopen(argv[2], "w")) == NULL) {
				perror("cannot open file");
				return 1;
			}
			
			size_t size = 0;
			char* str;
			while ( getline(&str, &size, file1) > 0) {
				if ((nbytes = write(pipe1[WRITE_END], str, strlen(str))) < 0) {
					fprintf(stderr, "Unable to write to pipe: %s\n", strerror(errno));
				}
			}
			if ((nbytes = read(pipe2[READ_END], line, LINESIZE)) < 0 ) {
				fprintf(stderr, "Unable to read from pipe: %s\n", strerror(errno));
			}
			fprintf(file2, line);
			
			close(pipe1[WRITE_END]);
			close(pipe2[READ_END]);
			
			if ( waitpid(pid, NULL, 0) < 0) {
				fprintf(stderr, "Cannot wait for child: %s\n", strerror(errno));
			}
			exit(EXIT_SUCCESS);
		}
		else {
			close(pipe2[READ_END]);
			close(pipe1[WRITE_END]);
			
			int idk;
			if ((nbytes = read(pipe1[READ_END], line, LINESIZE)) < 0 ) {
				fprintf(stderr, "Unable to read from pipe: %s\n", strerror(errno));			
			}
			
			char final[1024];
			strcpy(final, "");
			char* token2 = strtok(line," ");
			while( token2 != NULL ) {
				idk = getIndex(words, token2, num_words);
				
				if (idk == -1) {
					strcat(final, token2);
					strcat(final, " ");
				}
				else if (idk % 2 == 0) {
					strcat(final, words[idk + 1]);
					if (strstr(token2, ".") != NULL) {
						strcat(final, ".");
					}
					if (strstr(token2, "!") != NULL) {
						strcat(final, "!");
					}
					if (strstr(token2, "?") != NULL) {
						strcat(final, "?");
					}
					strcat(final, " ");
				}
				else {
					strcat(final, words[idk - 1]);
					if (strstr(token2, ".") != NULL) {
						strcat(final, ".");
					}
					if (strstr(token2, "!") != NULL) {
						strcat(final, "!");
					}
					if (strstr(token2, "?") != NULL) {
						strcat(final, "?");
					}
					strcat(final, " ");
				}
					
				token2=strtok(NULL," ");
			}
			
			if ((nbytes = write(pipe2[WRITE_END], final, strlen(final))) < 0) {
					fprintf(stderr, "Unable to write to pipe: %s\n", strerror(errno));
			}
			
			close(pipe1[READ_END]);
			close(pipe2[WRITE_END]);
			exit(EXIT_SUCCESS);
		}
	}
	else if (argc == 2) {
		FILE *cypher, *file1;
		
		int nbytes, pipe1[2], pipe2[2];
		pid_t pid;
		char line[LINESIZE];
		
		if ((cypher = fopen("cypher.txt", "r")) == NULL) {
			perror("cannot open file");
			return 1;
		}
		
		int num_words = 0;
		char* st;
		size_t s = 0;
		char* words[250];
		char total[1024];
		strcpy(total, "");
		
		
		while ( getline(&st, &s, cypher) > 0) {
			strcat(total, st);
		}
		
		char* token1 = strtok(total," \n");
		while( token1 != NULL ) {
			words[num_words++] = token1;
			token1=strtok(NULL," \n");
		}
		
		if (pipe(pipe1) < 0) {
			perror("pipe error");
			exit(EXIT_FAILURE);
		}
		
		if (pipe(pipe2) < 0) {
			perror("pipe error");
			exit(EXIT_FAILURE);
		}
		
		if ((pid = fork()) < 0) {
			perror("fork error");
			exit(EXIT_FAILURE);
		}
		else if (pid > 0) {
			close(pipe1[READ_END]);
			close(pipe2[WRITE_END]);
			
			if ((file1 = fopen(argv[1], "r")) == NULL) {
				perror("cannot open file");
				return 1;
			}
			
			
			size_t size = 0;
			char* str;
			while ( getline(&str, &size, file1) > 0) {
				if ((nbytes = write(pipe1[WRITE_END], str, strlen(str))) < 0) {
					fprintf(stderr, "Unable to write to pipe: %s\n", strerror(errno));
				}
			}
			if ((nbytes = read(pipe2[READ_END], line, LINESIZE)) < 0 ) {
				fprintf(stderr, "Unable to read from pipe: %s\n", strerror(errno));
			}
			printf("%s\n", line);
			
			close(pipe1[WRITE_END]);
			close(pipe2[READ_END]);
			
			if ( waitpid(pid, NULL, 0) < 0) {
				fprintf(stderr, "Cannot wait for child: %s\n", strerror(errno));
			}
			exit(EXIT_SUCCESS);
		}
		else {
			close(pipe2[READ_END]);
			close(pipe1[WRITE_END]);
			
			int idk;
			if ((nbytes = read(pipe1[READ_END], line, LINESIZE)) < 0 ) {
				fprintf(stderr, "Unable to read from pipe: %s\n", strerror(errno));			
			}
			
			char final[1024];
			strcpy(final, "");
			char* token2 = strtok(line," ");
			while( token2 != NULL ) {
				idk = getIndex(words, token2, num_words);
				
				if (idk == -1) {
					strcat(final, token2);
					strcat(final, " ");
				}
				else if (idk % 2 == 0) {
					strcat(final, words[idk + 1]);
					if (strstr(token2, ".") != NULL) {
						strcat(final, ".");
					}
					if (strstr(token2, "!") != NULL) {
						strcat(final, "!");
					}
					if (strstr(token2, "?") != NULL) {
						strcat(final, "?");
					}
					strcat(final, " ");
				}
				else {
					strcat(final, words[idk - 1]);
					if (strstr(token2, ".") != NULL) {
						strcat(final, ".");
					}
					if (strstr(token2, "!") != NULL) {
						strcat(final, "!");
					}
					if (strstr(token2, "?") != NULL) {
						strcat(final, "?");
					}
					strcat(final, " ");
				}
					
				token2=strtok(NULL," ");
			}
			
			if ((nbytes = write(pipe2[WRITE_END], final, strlen(final))) < 0) {
					fprintf(stderr, "Unable to write to pipe: %s\n", strerror(errno));
			}
			
			close(pipe1[READ_END]);
			close(pipe2[WRITE_END]);
			exit(EXIT_SUCCESS);
		}	
	}
	else {
		printf("usage: \n");
	}
	return 0;
}
				
				
				
				  
			
			
			
			
			










		
		
