#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include "matrixMath.h"

int main(int argc, char** argv){

	MPI_Init(NULL, NULL);
	srand(time(NULL));

	//--------------------------------------------------------------------------
	int world;

	MPI_Comm comm = MPI_COMM_WORLD;

	MPI_Comm_size(comm, &world);

	int rank;

	MPI_Comm_rank(comm, &rank);
	//--------------------------------------------------------------------------

	struct matrix matA, matB, solution, result;

	int doAdd, doMulti;

	initMatrix(&matA, atoi(argv[1]), atoi(argv[2]), 1);

	initMatrix(&matB, atoi(argv[3]), atoi(argv[4]), 1);

	if(rank == 0){

		printf("\033c");
		printf("Matrix A\n");
		printMatrix(&matA);
		printf("\nMatrix B\n");
		printMatrix(&matB);
	}
		//----------------------------------------------------------------------
		//								Matrix A + B

		initMatrix(&solution, matA.rows, matB.cols, 0);

		addMatrixS(&matA, &matB, &solution);

		if(rank == 0){
			puts("\nSolution of A + B");
			printMatrix(&solution);
		}

		initMatrix(&result, matA.rows, matB.cols, 0);

		addMatrixP(&matA, &matB, &result, comm, world, rank);

		if(rank == 0){
			puts("\nResult of A + B");
			printMatrix(&result);

			free(result.arr);
			free(solution.arr);
		}

		//----------------------------------------------------------------------
		//								Matrix A - B

		initMatrix(&solution, matA.rows, matB.cols, 0);

		subMatrixS(&matA, &matB, &solution);

		if(rank == 0){
			puts("\nSolution of A - B");
			printMatrix(&solution);
		}

		initMatrix(&result, matA.rows, matB.cols, 0);

		subMatrixP(&matA, &matB, &result, comm, world, rank);

		if(rank == 0){
			puts("\nResult of A - B");
			printMatrix(&result);

			free(result.arr);
			free(solution.arr);
		}

		//----------------------------------------------------------------------
		//								Matrix B - A

		initMatrix(&solution, matA.rows, matB.cols, 0);

		subMatrixS(&matB, &matA, &solution);

		if(rank == 0){
			puts("\nSolution of B - A");
			printMatrix(&solution);
		}

		initMatrix(&result, matA.rows, matB.cols, 0);

		subMatrixP(&matB, &matA, &result, comm, world, rank);

		if(rank == 0){
			puts("\nResult of B - A");
			printMatrix(&result);

			free(result.arr);
			free(solution.arr);
		}


		//----------------------------------------------------------------------
		//								Matrix A * B

		initMatrix(&solution, matA.rows, matB.cols, 0);

		multiMatrixS(&matA, &matB, &solution);

		if(rank == 0){
			puts("\nSolution of A * B");
			printMatrix(&solution);
		}

		initMatrix(&result, matA.rows, matB.cols, 0);

		multiMatrixP(&matA, &matB, &result, comm, world, rank);

		if(rank == 0){
			puts("\nResult of A * B");
			printMatrix(&result);

			free(result.arr);
			free(solution.arr);
		}

		//----------------------------------------------------------------------
		//								Matrix B * A

		initMatrix(&solution, matA.rows, matB.cols, 0);

		multiMatrixS(&matB, &matA, &solution);

		if(rank == 0){
			puts("\nSolution of B * A");
			printMatrix(&solution);
		}

		initMatrix(&result, matA.rows, matB.cols, 0);


		multiMatrixP(&matB, &matA, &result, comm, world, rank);

		if(rank == 0){
			puts("\nResult of B * A");
			printMatrix(&result);

			free(result.arr);
			free(solution.arr);
		}

	//--------------------------------------------------------------------------
								//Matrix A^(T)

	initMatrix(&result, matA.cols, matA.rows, 0);

	transposeMatixS(&matA, &result);

	if(rank == 0){
		puts("\nResult of A^(T)");
		printMatrix(&result);

		 free(result.arr);
	}

	//--------------------------------------------------------------------------
								//Matrix B^(T)

	initMatrix(&result, matB.cols, matB.rows, 0);

	transposeMatixS(&matB, &result);

	if(rank == 0){
		puts("\nResult of B^(T)");
		printMatrix(&result);

		 free(result.arr);
	}

	//--------------------------------------------------------------------------

	MPI_Finalize();

	free(matA.arr);
	free(matB.arr);

	return 0;

}
