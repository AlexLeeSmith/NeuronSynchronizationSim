# @author Alex Smith (alsmi14@ilstu.edu)
# @date 2022-01-30

CC=gcc
RM=rm -f
LIBS=-lm
BIN=Bin/
SRC=Src/
OUT=Out/
FLAGS=-g -Wall

allclean:all clean

all:graphs differential numerical spike driver
	$(CC) $(FLAGS) *.o -o $(BIN)driver $(LIBS)

graphs:$(SRC)graph_manipulations.c
	$(CC) -c $(SRC)graph_manipulations.c 

differential:$(SRC)differential_equations.c
	$(CC) -c $(SRC)differential_equations.c 

numerical:$(SRC)numerical_methods.c
	$(CC) -c $(SRC)numerical_methods.c 

spike:$(SRC)spike_calculations.c
	$(CC) -c $(SRC)spike_calculations.c 

driver:$(SRC)simulation_driver.c
	$(CC) -c $(SRC)simulation_driver.c

clean:cleanObject cleanOut

cleanObject:
	$(RM) *.o

cleanOut:
	$(RM) $(OUT)approx* $(OUT)spikes* $(OUT)ISI* $(OUT)s_values $(OUT)avg_freqs