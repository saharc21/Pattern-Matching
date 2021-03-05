all: main.c

		gcc main.c -o main -lpthread

all-GDB: main.c
		
		gcc -g main.c -o main -lpthread