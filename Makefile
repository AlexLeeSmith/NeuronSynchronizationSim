# @author Alex Smith (alsmi14@ilstu.edu)
# @date 1/30/22

CC=gcc
RM=rm -f
LIBS=-lm
BIN=Bin/
OUT=Out/
FLAGS=-g -Wall

allclean:all cleanObject

all:numerical spike main
	$(CC) $(FLAGS) numerical_methods.o spike_calculations.o main.o -o $(BIN)main $(LIBS)

numerical:numerical_methods.c
	$(CC) -c numerical_methods.c 

spike:spike_calculations.c
	$(CC) -c spike_calculations.c 

main:main.c
	$(CC) -c main.c

clean:cleanObject cleanMain cleanRunApproximation

cleanObject:
	$(RM) numerical_methods.o spike_calculations.o main.o

cleanMain:
	$(RM) $(BIN)main

cleanRunApproximation:
	$(RM) $(OUT)numerical_out