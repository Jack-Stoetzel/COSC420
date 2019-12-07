#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include "matrixMath.h"

int main(int argc, char** argv){

	MPI_Init(NULL, NULL);
	srand(time(NULL));

	//----------------------------------------------------------------
	int world;

	MPI_Comm comm = MPI_COMM_WORLD;

	MPI_Comm_size(comm, &world);

	int rank;

	MPI_Comm_rank(comm, &rank);
	//----------------------------------------------------------------

	matrix matA, matX, solution;

    int matrixSize = atoi(argv[1]);

	initMatrix(&matA, matrixSize, matrixSize, 3);
	if(rank == 0){
		puts("Matrix A: ");
		//printMatrix(&matA);
		puts("");
	}
	initMatrix(&matX, matrixSize, 1, 0);
	initMatrix(&solution, matrixSize, 1, 1);

	int i;
	// I tried this tolerant function and it wasnt working how i wanted to so I scrapped it
	//while(notTolerant(&matX, &solution, .0001)){
	for(i = 0; i < 40; i++){
		// Computes an nx1 matrix of the sum of every row in matA
		mulMatrixP(&matA, &solution, &solution);

		// Normalizes the solution matrix
		normalize(&solution);

		copyMatrix(&solution, &matX);
	}

	if(rank == 0){
		puts("The Largest Eigenvector of Matrix A is: ");
		//printMatrix(&solution);
		puts("");
	}
	mulMatrixP(&matA, &solution, &matX);
	if(rank == 0){
		double normie = twoNormalize(&matX);
		printf("The Maximum Eigenvalue = %1.3f \n\n",normie);

		//puts("(A * Egienvector) ./ Eigenvector = The Maximum Eigenvalue ");
		//for(i = 0; i < solution.rows; i++){
		//	printf("%1.3f \n", matX.arr[i] / solution.arr[i]);
		//}
	}

	free(matA.arr);
	free(matX.arr);
	free(solution.arr);


	MPI_Finalize();
	return 0;
}
