CC = gcc
flags = -std=c11 -Wall -ggdb


NOOBOBJ = main.c asker.c utility.c list.c tree.c

all: main

main: $(NOOBOBJ)
	$(CC) $(flags) -o main main.c 
