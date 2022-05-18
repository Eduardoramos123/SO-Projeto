#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/mman.h>

int main(int argc, char *argv[]) {
	if (argc != 3) {
		printf("usage: addmx file1 file2\n");
		return 1;
	}
	else {
		FILE *file1, *file2;
		
		if ((file1 = fopen(argv[1], "r")) == NULL) {
			perror("cannot open file");
			return 1;
		}
		
		if ((file2 = fopen(argv[2], "r")) == NULL) {
			perror("cannot open file");
			return 1;
		} 
		
		char* dim = NULL;
		size_t dimensions_size = 0;
		
		getline(&dim, &dimensions_size, file1);
		getline(&dim, &dimensions_size, file2);
		
		int dimensions[2];
		int size = 0;
		char* token1 = strtok(dim,"x");
		while( token1 != NULL ) {
			dimensions[size] = atoi(token1);
			size++;
			token1=strtok(NULL,"x");
		}
		
		int nprocs = dimensions[1];
		int matrix1[dimensions[0]][dimensions[1]];
		
		int i = 0, j = 0;
		while ( getline(&dim, &dimensions_size, file1) > 0) {
			char* token2 = strtok(dim," ");
			while( token2 != NULL ) {
				matrix1[i][j]=atoi(token2);
				token2=strtok(NULL," ");
				j++;
			}
			i++;
			j = 0;
		}
		int matrix2[dimensions[0]][dimensions[1]];
		i = 0;
		j = 0;
		while ( getline(&dim, &dimensions_size, file2) > 0) {
			char* token3 = strtok(dim," ");
			while( token3 != NULL ) {
				matrix2[i][j]=atoi(token3);
				token3=strtok(NULL," ");
				j++;
			}
			i++;
			j = 0;
		}
		
		fclose(file1);
		fclose(file2);
		
		int (*partials)[dimensions[1]] =  mmap(NULL, dimensions[0]*sizeof(int), PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS, 0, 0);
		
		if(partials == MAP_FAILED) {
			perror("mmap");
			return 1;
		}
		
		for (i = 0; i < dimensions[0]; i++) {
			for (j = 0; j < dimensions[1]; j++) {
				partials[i][j] = 0;
			}
		}
		
		for (i = 0; i < nprocs; i++) {
			pid_t pid;
			if ((pid = fork()) < 0) {
				perror("fork");
				exit(EXIT_FAILURE);
			}
			if (pid == 0) {
				for(int j = 0; j < dimensions[1]; j++) {
					if(j == i) {
						for(int k = 0; k < dimensions[0]; k++) {
							partials[k][j] = matrix1[k][j] + matrix2[k][j];
						}
						break;
					}
				}
				exit(EXIT_SUCCESS);
			}
		}
		
		for (i = 0; i < nprocs; i++) {
			if ( waitpid(-1, NULL, 0) < 0) {
				perror("waitpid");
				exit(EXIT_FAILURE);
			}
		}
		
		
		printf("%dx%d\n", dimensions[0], dimensions[1]);
		for (i = 0; i < dimensions[0]; i++) {
			for (j = 0; j < dimensions[1]; j++) {
				printf("%d ", partials[i][j]);
			}
			printf("\n");
		}
		
		if (munmap(partials, sizeof(partials)) < 0) {
			perror("munmap");
			exit(EXIT_FAILURE);
		}
		
	}
	return 0;
}
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
			
		
		 
