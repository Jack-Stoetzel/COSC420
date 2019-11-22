#include <stdio.h>
#include <stdlib.h>
//#include <mpi.h>
#include <time.h>
//#include "matrixMath.h"

#define INDEX(n, m, i, j) m * i + j
#define ACCESS(A, i, j) A ->arr [INDEX(A -> rows, A -> cols, i, j)]

typedef struct Matrix{
	int rows, cols;
	float *arr;
} matrix;

void initMatrix(matrix* A, int r, int c, int giveValue){
	A -> rows = r;
	A -> cols = c;
	A -> arr = malloc(r*c*sizeof(float));

	if(giveValue == 0){
		int i, k;
		for(i = 0; i < r; i++)
			for(k = 0; k < c; k++)
				ACCESS(A, i, k) = 0;
	}
	else if(giveValue == 1){
		int i, k;
		for(i = 0; i < r; i++){
			for(k = 0; k < c; k++){
				if(i == k)
					ACCESS(A, i, k) = 1;
				else
					ACCESS(A, i, k) = 0;
			}
		}
	}
	else if(giveValue == 2){
		int i, k;
		for(i = 0; i < r; i++)
			for(k = 0; k < c; k++)
				ACCESS(A, i, k) = rand() % 10 + 1;
	}
}

void printMatrix(matrix* A){
	int i, k;

	for(i = 0; i < A -> rows; i++){
		for(k = 0; k < A -> cols; k++)
			printf("%0.3f \t", ACCESS(A, i, k));
		printf("\n");
	}
}

void mulMatrixS(matrix *A, matrix *B, matrix *F){
	int i, k, n;

	for(i = 0; i < A -> cols; i++)
		for(k = 0; k < B -> cols; k++)
			for(n = 0; n < A -> cols; n++)
				ACCESS(F, i, k) += ACCESS(A, i, n) * ACCESS(B, n, k);
}

void transposeMatixS(matrix *A, matrix *F){
	int i, k;
	for(i = 0; i < A -> rows; i++)
		for(k = 0; k < A -> cols; k++)
			ACCESS(F, k, i) = ACCESS(A, i, k);
}

//TODO: Print out full matrix B
void printGJ(matrix *A, matrix *B){
	int i, k;

	for(i = 0; i < A -> rows; i++){
		for(k = 0; k < A -> cols; k++){
			printf("%8.3f \t", ACCESS(A, i, k));
		}
		printf("| %8.3f\n", ACCESS(B, i, 0));
	}
}

void GaussJordan(matrix *A, matrix *B){
	int k, i, r, c;
	float *l = malloc(A -> rows * sizeof(float));
	for(k = 0; k < A -> rows; k++){
		puts("Scalers");
		for(i = 0; i < A -> rows; i++){
			l[i] = ACCESS(A, i, k) / ACCESS(A, k, k);
			printf("%8.3f \n", l[i]);
		}
		puts("--------------------------");
		for(r = 0; r < A -> rows; r++){
			if( r != k){
				for(c = 0; c < A -> cols; c++)
					ACCESS(A, r, c) -= l[r] * ACCESS(A, k, c);
				for(c = 0; c < B -> cols; c++)
					ACCESS(B, r, c) -= l[r] * ACCESS(B, k, c);
			}
		}
		printGJ(A, B);
		puts("--------------------------");
	}
	puts("Diagonals");
	for(r = 0; r < A -> rows; r++){
		printf("%8.3f \n", ACCESS(A, r, r));
		ACCESS(B, r, 0) /= ACCESS(A, r, r);
		ACCESS(A, r, r) /= ACCESS(A, r, r);
	}
	puts("--------------------------");
	printGJ(A, B);
	puts("--------------------------");
}

int main(int argc, char** argv){

	matrix matA, solution;

	initMatrix(&matA, atoi(argv[1]), atoi(argv[2]), 2);

	printf("\033c");
	printf("Matrix A\n");

//--------------------------------------------------------------------
					// Gauss-Jordan(Matrix A))

	initMatrix(&solution, matA.cols, 1, 2);

	//printGJ(&matA, &solution);
	//printf("--------------------------\n");

	GaussJordan(&matA, &solution);

	puts("Gauss-Jordan Elimination solution for A ");
	printMatrix(&solution);

	free(solution.arr);
	free(matA.arr);

	return 0;
}
