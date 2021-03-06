#ifndef MATRIXMATH_H
#define MATRIXMATH_H

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

#define INDEX(n, m, i, j) m*i+j
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

//DONE
void addMatrixS(struct matrix *A, struct matrix *B, struct matrix *F){
	int i, k;
	for(i = 0; i < A -> rows; i++)
		for(k = 0; k < A -> cols; k++)
			ACCESS(F, i, k) = ACCESS(A, i, k) + ACCESS(B, i, k);
}

//DONE
void addMatrixP(struct matrix *A, struct matrix *B, struct matrix *F,
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

	MPI_Gatherv(finalArr, sendCounts[rank], MPI_INT, F -> arr, sendCounts,
				displs, MPI_INT, 0, comm);

}

//DONE
void subMatrixS(struct matrix *A, struct matrix *B, struct matrix *F){
	int i, k;
	for(i = 0; i < A -> rows; i++)
		for(k = 0; k < A -> cols; k++)
			ACCESS(F, i, k) = ACCESS(A, i, k) - ACCESS(B, i, k);
}

//DONE
void subMatrixP(struct matrix *A, struct matrix *B, struct matrix *F,
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

	MPI_Gatherv(finalArr, sendCounts[rank], MPI_INT, F -> arr, sendCounts,
				displs, MPI_INT, 0, comm);

}

//DONE
void multiMatrixS(struct matrix *A, struct matrix *B, struct matrix *F){
	int i, k, n;

		for(i = 0; i < A -> cols; i++)
			for(k = 0; k < B -> cols; k++)
				for(n = 0; n < A -> cols; n++)
					ACCESS(F, i, k) += (ACCESS(A, i, n) * ACCESS(B, n, k));
}

//NOT DONE
void multiMatrixP(struct matrix *A, struct matrix *B, struct matrix *F,
				  MPI_Comm comm, int world, int rank){
	struct matrix T;
	initMatrix(&T, B -> rows, B -> cols, 0);
	transposeMatixS(B, &T);

	int i, k, n, prod;
	int *arrA = malloc(A -> cols * sizeof(int));
	int *arrB = malloc(B -> cols * sizeof(int));

	for(i = 0; i < F -> rows; i++){
		for(k = 0; k < F -> cols; k++){
			for(n = 0; n < A -> rows; n++){
				arrA[n] = A -> arr[i * A -> rows + n];
				arrB[n] = T.arr[k * B -> rows + n];
			}
			prod = innerProd(arrA, arrB, A -> cols, comm, world, rank);
			ACCESS(F, i, k) = prod;
		}
	}
}

//DONE
void transposeMatixS(struct matrix *A, struct matrix *F){
	int i, k;
	for(i = 0; i < A -> rows; i++)
		for(k = 0; k < A -> cols; k++)
			ACCESS(F, k, i) = ACCESS(A, i, k);
}

int innerProd(int *arr1, int *arr2, int arrSize, MPI_Comm comm, int world, int rank){
	int divide = arrSize/world;
    int prod = 0, i = 0;

    if(rank == 0){
        for(; i < arrSize; i++){
            arr1[i] = rand() % 5 + 1;
            arr2[i] = rand() % 5 + 1;
        }
    }

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
    int arrB[sendCounts[rank]];
    int arrA[sendCounts[rank]];

    for(i = 0; i < sendCounts[rank]; i++)
        arrA[i] = 0;

    for(i = 0; i < sendCounts[rank]; i++)
        arrB[i] = 0;

    MPI_Scatterv(arr1, sendCounts, displs, MPI_INT, arrA, sendCounts[rank],
                 MPI_INT, 0, comm);

    MPI_Scatterv(arr2, sendCounts, displs, MPI_INT, arrB, sendCounts[rank],
                 MPI_INT, 0, comm);

    for( i = 0; i < sendCounts[rank]; i++)
        prod += arrA[i] * arrB[i];

    MPI_Reduce(&prod, &final, 1, MPI_INT, MPI_SUM, 0, comm);

	if(rank == 0)
		return final;
}

#endif
