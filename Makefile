# @author Alex Smith (alsmi14@ilstu.edu)
# @date 1/30/22

CC=gcc
RM=rm -f
LIBS=-lm
BIN=Bin/
OUT=Out/
FLAGS=-g -Wall

numerical:numerical_methods.c
	$(CC) $(FLAGS) -o $(BIN)numerical_methods numerical_methods.c $(LIBS)

clean:cleanNumerical cleanRunApproximation

cleanNumerical:
	$(RM) $(BIN)numerical_methods

cleanRunApproximation:cleanNumerical
	$(RM) $(OUT)numerical_out