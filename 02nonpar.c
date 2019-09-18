#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>


#define INDEX(n, m, i, j) n*i+j
#define ACCESS(A, i, j) A ->arr [INDEX(A -> rows, A -> cols, i, j)]

struct matrix{
	int rows, cols;
	int *arr;
};

void initMatrix(struct matrix* A, int r, int c, int giveValue){
	A -> rows = r;
	A -> cols = c;
	A -> arr = malloc(r*c*sizeof(int));

	if(giveValue){
		int i, k;
		for(i = 0; i < r; i++)
			for(k = 0; k < c; k++)
				ACCESS(A, i, k) = rand() % 10 + 1;
	}
	else{
		int i, k;
		for(i = 0; i < r; i++)
			for(k = 0; k < c; k++)
				ACCESS(A, i, k) = 0;
	}
}

void printMatrix(struct matrix* A){
	int i, k;

	for(i = 0; i < A -> rows; i++){
		for(k = 0; k < A -> cols; k++)
			printf("%d ", ACCESS(A, i, k));
		printf("\n");
	}
}

void addMatrix(struct matrix *A, struct matrix *B, struct matrix *S){
	int i, k;
	for(i = 0; i < A -> rows; i++)
		for(k = 0; k < A -> cols; k++)
			ACCESS(S, i, k) = ACCESS(A, i, k) + ACCESS(B, i, k);
}

void subMatrix(struct matrix *A, struct matrix *B, struct matrix *S){
	int i, k;
	for(i = 0; i < A -> rows; i++)
		for(k = 0; k < A -> cols; k++)
			ACCESS(S, i, k) = ACCESS(A, i, k) - ACCESS(B, i, k);
}

void multiMatrix(struct matrix *A, struct matrix *B, struct matrix *S){
	int i, k, n;

		for(i = 0; i < A -> cols; i++)
			for(k = 0; k < B -> cols; k++)
				for(n = 0; n < A -> cols; n++)
					ACCESS(S, i, k) += (ACCESS(A, i, n) * ACCESS(B, n, k));
}

int main(int argc, char** argv){

	MPI_Init(NULL, NULL);
	srand(time(NULL));

	//-------------------------------------------------------------
	int size; //Holds the world size

	MPI_Comm comm = MPI_COMM_WORLD;

	MPI_Comm_size(comm, &size);

	int rank;

	MPI_Comm_rank(comm, &rank);
	//-------------------------------------------------------------

	struct matrix matA, matB, final;
	if(rank == 0){
		if(argc - 1 != 4)
			printf("Error!!! Did not enter correct number of parameters.\n");
		else{
			int rows, cols;

			rows = atoi(argv[1]);
			cols = atoi(argv[2]);

			initMatrix(&matA, rows, cols, 1);

			rows = atoi(argv[3]);
			cols = atoi(argv[4]);

			initMatrix(&matB, rows, cols, 1);

			printf("\033c");
			printf("Matrix A\n");
			printMatrix(&matA);
			printf("\nMatrix B\n");
			printMatrix(&matB);

			if(matA.rows == matB.rows && matA.cols == matB.cols){
				initMatrix(&final, matA.rows, matB.cols, 0);

				addMatrix(&matA, &matB, &final);

				puts("\nSum of A + B\n");
				printMatrix(&final);

				free(final.arr);
			}
			else
				puts("ERROR: Matricies must be of the same size.");

			if(matA.rows == matB.rows && matA.cols == matB.cols){
				initMatrix(&final, matA.rows, matA.cols, 0);

				subMatrix(&matA, &matB, &final);

				puts("\nDifference of A - B\n");
				printMatrix(&final);

				free(final.arr);
			}
			else
				puts("ERROR: Matricies must be of the same size.");

			if(matA.rows == matB.rows && matA.cols == matB.cols){
				initMatrix(&final, matA.rows, matA.cols, 0);

				subMatrix(&matB, &matA, &final);

				puts("\nDifference of B - A\n");
				printMatrix(&final);

				free(final.arr);
			}
			else
				puts("ERROR: Matricies must be of the same size.");

			if(matA.cols == matB.rows){
				initMatrix(&final, matA.cols, matB.rows, 0);

				multiMatrix(&matA, &matB, &final);

				puts("\nProduct of A * B\n");
				printMatrix(&final);

				free(final.arr);
			}
			else
				puts("ERROR: Mismatch of Matrix Dimensions.");

			if(matA.cols == matB.rows){
				initMatrix(&final, matB.cols, matA.rows, 0);

				multiMatrix(&matB, &matA, &final);

				puts("\nProduct of B * A\n");
				printMatrix(&final);

				free(final.arr);
			}
			else
				puts("ERROR: Mismatch of Matrix Dimensions.");

			free(matA.arr);
			free(matB.arr);
		}
	}
	MPI_Finalize();
	return 0;

}
