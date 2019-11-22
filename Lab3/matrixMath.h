#ifndef MATRIXMATH_H
#define MATRIXMATH_H

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

#define INDEX(n, m, i, j) m*i+j
#define ACCESS(A, i, j) A ->arr [INDEX(A -> rows, A -> cols, i, j)]

typedef struct Matrix{
	int rows, cols;
	float *arr;
} matrix;

void initMatrix(matrix* A, int r, int c, int giveValue){
	A -> rows = r;
	A -> cols = c;
	A -> arr = malloc(r*c*sizeof(float));

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

void printMatrix(matrix* A){
	int i, k;

	for(i = 0; i < A -> rows; i++){
		for(k = 0; k < A -> cols; k++)
			printf("%1.2f ", ACCESS(A, i, k));
		printf("\n");
	}
}

void addMatrixS(matrix *A, matrix *B, matrix *F){
	int i, k;
	for(i = 0; i < A -> rows; i++)
		for(k = 0; k < A -> cols; k++)
			ACCESS(F, i, k) = ACCESS(A, i, k) + ACCESS(B, i, k);
}

void addMatrixP(matrix *A, matrix *B, matrix *F,
			    MPI_Comm comm, int world, int rank){


	int arrSize= A -> rows * A -> cols;
	int block = arrSize / world;
	int overflow = arrSize % world + block;

	int sendCounts[world];
	int displs[world];

	int i;

	for(i = 0; i < world; i++){
		displs[i] = i * (block);
		if(i == world - 1)
			sendCounts[i] = overflow;
		else
			sendCounts[i] = block;
	}

	int arrA[sendCounts[rank]];
	int arrB[sendCounts[rank]];
	int finalArr[sendCounts[rank]];

	for(i = 0; i < sendCounts[rank]; i++)
		arrA[i] = 0;

	for(i = 0; i < sendCounts[rank]; i++)
		arrB[i] = 0;

	MPI_Scatterv(A -> arr, sendCounts, displs, MPI_INT,  arrA,
				 sendCounts[rank], MPI_INT, 0, comm);

	MPI_Scatterv(B -> arr, sendCounts, displs, MPI_INT, arrB,
				 sendCounts[rank], MPI_INT, 0, comm);

	for(i = 0; i < sendCounts[rank]; i++)
		finalArr[i] = arrA[i] + arrB[i];

	MPI_Gatherv(finalArr, sendCounts[rank], MPI_INT, F -> arr,
				sendCounts, displs, MPI_INT, 0, comm);

}

void subMatrixS(matrix *A, matrix *B, matrix *F){
	int i, k;
	for(i = 0; i < A -> rows; i++)
		for(k = 0; k < A -> cols; k++)
			ACCESS(F, i, k) = ACCESS(A, i, k) - ACCESS(B, i, k);
}

void subMatrixP(matrix *A, matrix *B, matrix *F,
			    MPI_Comm comm, int world, int rank){

	int arrSize= A -> rows * A -> cols;
	int block = arrSize / world;
	int overflow = arrSize % world + block;

	int sendCounts[world];
	int displs[world];

	int i;

	for(i = 0; i < world; i++){
		displs[i] = i * block;
		if(i == world - 1)
			sendCounts[i] = overflow;
		else
			sendCounts[i] = block;
	}

	int arrA[sendCounts[rank]];
	int arrB[sendCounts[rank]];
	int finalArr[sendCounts[rank]];

	for(i = 0; i < sendCounts[rank]; i++)
		arrA[i] = 0;

	for(i = 0; i < sendCounts[rank]; i++)
		arrB[i] = 0;

	MPI_Scatterv(A -> arr, sendCounts, displs, MPI_INT, arrA,
				 sendCounts[rank], MPI_INT, 0, comm);

	MPI_Scatterv(B -> arr, sendCounts, displs, MPI_INT, arrB,
				 sendCounts[rank], MPI_INT, 0, comm);

	for(i = 0; i < sendCounts[rank]; i++)
		finalArr[i] = arrA[i] - arrB[i];

	MPI_Gatherv(finalArr, sendCounts[rank], MPI_INT, F -> arr,
				sendCounts, displs, MPI_INT, 0, comm);

}

void mulMatrixS(matrix *A, matrix *B, matrix *F){
	int i, k, n;

	for(i = 0; i < F -> rows; i++)
		for(k = 0; k < F -> cols; k++)
			for(n = 0; n < A -> rows; n++)
				ACCESS(F, i, k) += ACCESS(A, i, n) * ACCESS(B, n, k);
}

void transposeMatixS(matrix *A, matrix *F){
	int i, k;
	for(i = 0; i < A -> rows; i++)
		for(k = 0; k < A -> cols; k++)
			ACCESS(F, k, i) = ACCESS(A, i, k);
}

void mulMatrixP(matrix *A, matrix *B, matrix *F,
				MPI_Comm comm, int world, int rank){
	matrix T;
	initMatrix(&T, B -> cols, B -> rows, 0);
	transposeMatixS(B, &T);

	int i, k, n;
	int *arrA = malloc(A -> cols * sizeof(int));
	int *arrB = malloc(B -> cols * sizeof(int));

	for(i = 0; i < F -> rows; i++){
		for(k = 0; k < F -> cols; k++){
			for(n = 0; n < A -> rows; n++){
				arrA[n] = ACCESS(A, i, n);
				arrB[n] = T.arr[k * B -> rows + n];
			}
			ACCESS(F, i, k) =
			dotProduct(arrA, arrB, A -> cols, comm, world, rank);
		}
	}
}

int dotProduct(int *arr1, int *arr2, int arrSize, MPI_Comm comm,
			   int world, int rank){

	int prod = 0, i = 0;
    int block = arrSize / world;
    int overflow = arrSize % world + block;
    int sendCounts[world];
    int displs[world];

    for(i = 0; i < world; i++){
        displs[i] = i * block;
        if( i == world - 1)
            sendCounts[i] = overflow;
        else
            sendCounts[i] = block;
    }

	int final = 0;
	int *arrA = malloc(sendCounts[rank] * sizeof(int));
    int *arrB = malloc(sendCounts[rank] * sizeof(int));

    for(i = 0; i < sendCounts[rank]; i++)
        arrA[i] = 0;

    for(i = 0; i < sendCounts[rank]; i++)
        arrB[i] = 0;

    MPI_Scatterv(arr1, sendCounts, displs, MPI_INT, arrA,
				 sendCounts[rank], MPI_INT, 0, comm);

    MPI_Scatterv(arr2, sendCounts, displs, MPI_INT, arrB,
				 sendCounts[rank], MPI_INT, 0, comm);

    for( i = 0; i < sendCounts[rank]; i++)
        prod += arrA[i] * arrB[i];

    MPI_Reduce(&prod, &final, 1, MPI_INT, MPI_SUM, 0, comm);

	if(rank == 0)
		return final;
}

void GaussJordanS(matrix *A, matrix *B){
	int k, i, r, c;
	float *l = malloc(A -> rows * sizeof(float));
	for(k = 0; k < A -> rows; k++){
		for(i = 0; i < A -> rows; i++){
			l[i] = ACCESS(A, i, k) / ACCESS(A, k, k);
		}
		for(r = 0; r < A -> rows; r++){
			if( r != k){
				for(c = 0; c < A -> cols; c++)
					ACCESS(A, r, c) -= l[r] * ACCESS(A, k, c);
				for(c = 0; c < B -> cols; c++)
					ACCESS(B, r, c) -= l[r] * ACCESS(B, k, c);
			}
		}
	}
	for(r = 0; r < A -> rows; r++){
		ACCESS(B, r, 0) /= ACCESS(A, r, r);
		ACCESS(A, r, r) /= ACCESS(A, r, r);
	}
}

void GaussJordanP(matrix *A, matrix *B){
	int k, i, r, c;
	float *l = malloc(A -> rows * sizeof(float));
	for(k = 0; k < A -> rows; k++){
		for(i = 0; i < A -> rows; i++){
			l[i] = ACCESS(A, i, k) / ACCESS(A, k, k);
		}
		for(r = 0; r < A -> rows; r++){
			if( r != k){
				for(c = 0; c < A -> cols; c++)
					ACCESS(A, r, c) -= l[r] * ACCESS(A, k, c);
				for(c = 0; c < B -> cols; c++)
					ACCESS(B, r, c) -= l[r] * ACCESS(B, k, c);
			}
		}
	}
	for(r = 0; r < A -> rows; r++){
		ACCESS(B, r, 0) /= ACCESS(A, r, r);
		ACCESS(A, r, r) /= ACCESS(A, r, r);
	}
}


#endif
