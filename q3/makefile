#This is a makefile

all: cypher

cyper: cypher.c transformations.c utils.c
	gcc -Wall -o cypher cypher.c utils.c transformations.c 

run:
	./cypher quote.txt quote_cyphered.txt

clean:
	rm -rf cypher
	rm -rf *.exe
	rm -rf *.o
