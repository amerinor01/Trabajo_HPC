CC = gcc	
C_OPTIMIZED_FLAGS = -O3 -march=native
CFLAGS= -Iinclude/ -fopenmp
all: mpi_gemm

mpi_gemm: 
	$(CC) -c src/memoryfun.c -o lib/memoryfun.o  $(CFLAGS) $(C_OPTIMIZED_FLAGS)
	$(CC) -c src/esqueleto.c -o lib/esqueleto.o  $(CFLAGS) $(C_OPTIMIZED_FLAGS)
	$(CC) -o opt/esqueleto lib/esqueleto.o lib/memoryfun.o -lcblas -fopenmp $(C_OPTIMIZED_FLAGS)


