CC = gcc
CFLAGS = -lglfw -lGL -lm -ldl -I./include
SRCP = ./src

test: test.o 
	$(CC) -o test $(SRCP)/glad.c  test.o $(CFLAGS)

.PHONY: clean #‘make clean’ will run the recipe regardless of whether there is a file named clean.

clean: 
	rm *.o
