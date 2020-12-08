all: main.c

		gcc main.c -o main

all-GDB: main.c
		
		gcc -g main.c -o main