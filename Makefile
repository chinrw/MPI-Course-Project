all: compile

COMPILER = mpicc

compile: mpi_primes.c
	$(COMPILER) mpi_primes.c -o hw4.out -Wall
