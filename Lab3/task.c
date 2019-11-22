#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include "matrixMath.h"

int main(int argc, char** argv){

	MPI_Init(NULL, NULL);
	//srand(time(NULL));

	//----------------------------------------------------------------
	int world;

	MPI_Comm comm = MPI_COMM_WORLD;

	MPI_Comm_size(comm, &world);

	int rank;

	MPI_Comm_rank(comm, &rank);
	//----------------------------------------------------------------

	matrix matA, matB, solution, result;

	int doAdd, doMulti;

	initMatrix(&matA, atoi(argv[1]), atoi(argv[2]), 1);

	initMatrix(&matB, atoi(argv[3]), atoi(argv[4]), 1);

	initMatrix(&solution, matA.rows, matB.cols, 0);

	if(rank == 0){
		puts("Matrix A");
		printMatrix(&matA);

		puts("Matrix B");
		printMatrix(&matB);
	}

	//mulMatrixS(&matA, &matB, &solution);
	mulMatrixP(&matA, &matB, &solution, comm, world, rank);

	if(rank == 0){
		puts("Multiplication");
		printMatrix(&solution);
	}

	MPI_Finalize();

	free(matA.arr);
	free(matB.arr);

	return 0;
}
