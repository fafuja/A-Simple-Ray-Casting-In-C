CC = gcc
CFLAGS = -lglfw -lGL -lm -ldl -Iinclude
SRCP = ./src

main: main.o shaders.o
	$(CC) -o main $(SRCP)/glad.c shaders.o  main.o $(CFLAGS)
	
.PHONY: clean #‘make clean’ will run the recipe regardless of whether there is a file named clean.

clean: 
	rm *.o
