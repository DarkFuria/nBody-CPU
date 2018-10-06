CC=gcc
SRC=main.c src/helpers.o src/cpuModel.o
FLAGS=-lm -O3

all: src/helpers.o src/cpuModel.o
	$(CC) $(SRC) -o nbody -g $(FLAGS)

src/helpers.o: src/helpers.c
	$(CC) -c src/helpers.c
	mv helpers.o src/helpers.o
	
src/cpuModel.o: src/cpuModel.c
	$(CC) -c src/cpuModel.c $(FLAGS)
	mv cpuModel.o src/cpuModel.o

clean:
	rm nbody
