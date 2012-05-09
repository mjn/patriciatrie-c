
# Compiler
CC=gcc
CFLAGS=-g -Wall -march=x86-64

all : patricia_test

patricia_test :
	$(CC) $(CFLAGS) $(LDFLAGS) -o bin/patricia_test src/patricia_test.c src/patricia.c

clean :
	rm -f *.o bin/patricia_test
	rm -rf bin/patricia_test.dSYM
