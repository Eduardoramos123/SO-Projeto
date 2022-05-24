#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define LINE_SIZE 256



int isempty(const char *s){
  while (*s) {
    if (!isspace(*s))
      return 0;
    s++;
  }
  return 1;
}


int main(int argc, char* argv[]) {
	FILE* file;
	char line[LINE_SIZE];
	char copy[LINE_SIZE];
	
	if (argc == 1) {
		printf("usage: phrases [-l] file\n");
		return 1;
	}
	else if (argc == 2) {
		file = fopen(argv[1], "r");
		int num = 0;
		
		if (!file) {
        	perror(argv[1]);
        	return 1;
    	}
    		
    	int flg = 1;
    		
    	while (fgets(line, LINE_SIZE, file)) {
    		flg = 0;
    		for (int i = 0; i < strlen(line); i++) {
    			if (line[i] == '.' || line[i] == '!' || line[i] == '?') {
    				num++;
    			}
    		}
    		strcpy(copy, line);
    	}
    		
    		
    	if (flg == 0) {
    		if (copy[strlen(copy) - 2] != '.' && copy[strlen(copy) - 2] != '!' && copy[strlen(copy) - 2] != '?') {
	    		num++;
	    	}
    	}
    	printf("%d\n", num);
    	fclose(file);
    }
    else if (argc == 3) {
    	if (strcmp(argv[1], "-l") != 0) {
    		printf("usage: phrases [-l] file\n");
			return 1;
		}
		
		file = fopen(argv[2], "r");
		
		if (!file) {
        	perror(argv[2]);
        	return 1;
    	}
    		
    	char text[4096];
    	int flg = 1;
    	int index = 0;
    	int num = 1;
    	char sig[256];
    	int sig_index = 0;
    		
    	while (fgets(line, LINE_SIZE, file)) {
    		flg = 0;
    		for (int i = 0; i < strlen(line); i++) {
				if (line[i] == '.' || line[i] == '!' || line[i] == '?') {
					text[index] = line[i];
					sig[sig_index] = line[i];
					sig_index++;			
				}
    			else if (line[i] == '\n') {
    				text[index] = ' ';
    			}
    			else {
    				text[index] = line[i];
    			}
    			index++;
    		}
    	}
    		    		
    	char *tokens = strtok(text,".!?");
    	int sig_num = 0;

    	if (flg == 0) {
    		while(tokens != NULL) {
    			if (isempty(tokens) == 1) {
    				tokens = strtok(NULL,".!?");
    				continue;
    			}
    			if (num == 1) {
    				printf("[%d]  %s%c\n", num, tokens, sig[sig_num]);
    			}
    			else {
    				printf("[%d] %s%c\n", num, tokens, sig[sig_num]);
    			}
    			tokens = strtok(NULL,".!?");
    			num++;
    			sig_num++;
    		}
    	}
    }
    else {
    	printf("usage: phrases [-l] file\n");
		return 1;
	}
	
    return 0;
}

    			