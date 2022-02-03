# @author Alex Smith (alsmi14@ilstu.edu)
# @date 1/30/22

CC=gcc
RM=rm -f
LIBS=-lm
BIN=Bin/
FLAGS=-g -Wall

numerical: numerical_methods.c
	$(CC) $(FLAGS) -o $(BIN)numerical_methods numerical_methods.c $(LIBS)

clean:
	$(RM) $(BIN)numerical_methods