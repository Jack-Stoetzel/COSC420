#ifndef MATRIXMATH_H
#define MATRIXMATH_H

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include <math.h>

#define INDEX(n, m, i, j) m*i+j
#define ACCESS(A, i, j) A ->arr [INDEX(A -> rows, A -> cols, i, j)]

typedef struct Matrix{
	int rows, cols;
	double *arr;
} matrix;

void initMatrix(matrix* A, int r, int c, int giveValue){
	A -> rows = r;
	A -> cols = c;
	A -> arr = malloc(r*c*sizeof(double));


	if(giveValue == 3){
		int i, k;
		for(i = 0; i < r; i++)
			for(k = 0; k < c; k++)
				ACCESS(A, i, k) = rand() % 10 + 1;
	}
	else if(giveValue == 2){
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
	else if(giveValue == 1){
		int i, k;
		for(i = 0; i < r; i++)
			for(k = 0; k < c; k++)
				ACCESS(A, i, k) = 1;
	}
	else if(giveValue == 0){
		int i, k;
		for(i = 0; i < r; i++)
			for(k = 0; k < c; k++)
				ACCESS(A, i, k) = 0;
	}
}

void initVector(matrix* V, int l){
	V -> rows = l;
	V -> cols = 1;
	V -> arr = malloc(V -> rows*V -> cols*sizeof(double));

	int i;
	for(i = 0; i < l; i++){
		ACCESS(V, i, 0) = rand() % 10 + 1;
	}
}

void copyMatrix(matrix *A, matrix *CP){
	CP -> rows = A -> rows;
	CP -> cols = A -> cols;
	free(CP -> arr);
	CP -> arr = malloc(CP -> rows * CP -> cols * sizeof(double));

	int i, k;
	for(i = 0; i < CP -> rows; i++){
		for(k = 0; k< CP -> cols; k++){
			ACCESS(CP, i, k) = ACCESS(A, i, k);
		}
	}
}

void printMatrix(matrix* A){
	int i, k;

	for(i = 0; i < A -> rows; i++){
		for(k = 0; k < A -> cols; k++)
			printf("%1.6f \t", ACCESS(A, i, k));
		printf("\n");
	}
}

void printGJ(matrix *A, matrix *B){
	int i, k;

	for(i = 0; i < A -> rows; i++){
		for(k = 0; k < A -> cols; k++){
			printf("%7.2f \t", ACCESS(A, i, k));
		}
		printf("|");
		for(k = 0; k < B -> cols; k++)
			printf("%7.2f \t", ACCESS(B, i, k));
		puts("");
	}
}

void transposeMatixS(matrix *A, matrix *F){
	int i, k;
	for(i = 0; i < A -> rows; i++)
		for(k = 0; k < A -> cols; k++)
			ACCESS(F, k, i) = ACCESS(A, i, k);
}

void addMatrixS(matrix *A, matrix *B, matrix *F){
	int i, k;
	for(i = 0; i < A -> rows; i++)
		for(k = 0; k < A -> cols; k++)
			ACCESS(F, i, k) = ACCESS(A, i, k) + ACCESS(B, i, k);
}

void addMatrixP(matrix *A, matrix *B, matrix *F){

	MPI_Comm comm = MPI_COMM_WORLD;

	int world;
	MPI_Comm_size(comm, &world);

	int rank;
	MPI_Comm_rank(comm, &rank);

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

void subMatrixP(matrix *A, matrix *B, matrix *F){

	MPI_Comm comm = MPI_COMM_WORLD;

	int world;
	MPI_Comm_size(comm, &world);

	int rank;
	MPI_Comm_rank(comm, &rank);

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
		for(k = 0; k < F -> cols; k++){
			for(n = 0; n < A -> cols; n++){
				ACCESS(F, i, k) += ACCESS(A, i, n) * ACCESS(B, n, k);
			}
		}
}

void mulMatrixP(matrix *A, matrix *B, matrix *F){

	MPI_Comm comm = MPI_COMM_WORLD;

	int world;
	MPI_Comm_size(comm, &world);

	int rank;
	MPI_Comm_rank(comm, &rank);

	matrix T;
	initMatrix(&T, B -> cols, B -> rows, 1);
	transposeMatixS(B, &T);

	int i, k, n;
	double *arrA = malloc(A -> cols * sizeof(double));
 	double *arrB = malloc(A -> cols * sizeof(double));

	for(i = 0; i < F -> rows; i++){
		for(k = 0; k < F -> cols; k++){
			for(n = 0; n < A -> cols; n++){
				//arrA[n] = ACCESS(A, i, n);
				arrA[n] = A -> arr[(i * A -> cols) + n];
				arrB[n] = T.arr[n * T.rows + k];
			}
			ACCESS(F, i, k) = dotProduct(arrA, arrB, A -> cols);
		}
	}
	free(arrA);
	free(arrB);
	free(T.arr);
}

int dotProduct(int *arr1, int *arr2, int arrSize){

	MPI_Comm comm = MPI_COMM_WORLD;

	int world;
	MPI_Comm_size(comm, &world);

	int rank;
	MPI_Comm_rank(comm, &rank);

	double prod = 0;
	int i = 0;
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

	double final = 0;
	double *arrA = malloc(sendCounts[rank] * sizeof(double));
    double *arrB = malloc(sendCounts[rank] * sizeof(double));

    for(i = 0; i < sendCounts[rank]; i++)
        arrA[i] = 0;

    for(i = 0; i < sendCounts[rank]; i++)
        arrB[i] = 0;

    MPI_Scatterv(arr1, sendCounts, displs, MPI_DOUBLE, arrA,
				 sendCounts[rank], MPI_DOUBLE, 0, comm);

    MPI_Scatterv(arr2, sendCounts, displs, MPI_DOUBLE, arrB,
				 sendCounts[rank], MPI_DOUBLE, 0, comm);

    for( i = 0; i < sendCounts[rank]; i++)
        prod += arrA[i] * arrB[i];

    MPI_Reduce(&prod, &final, 1, MPI_DOUBLE, MPI_SUM, 0, comm);

	free(arrA);
	free(arrB);

	if(rank == 0)
		return final;
}

void GaussJordanS(matrix *A, matrix *B){
	int k, i, r, c;
	double *l = malloc(A -> rows * sizeof(double));
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
		for(c = 0; c < B -> cols; c++){
			ACCESS(B, r, c) /= ACCESS(A, r, r);
		}
		ACCESS(A, r, r) /= ACCESS(A, r, r);
	}
}

void GaussJordanP(matrix *A, matrix *B){

	MPI_Comm comm = MPI_COMM_WORLD;

	int world;
	MPI_Comm_size(comm, &world);

	int rank;
	MPI_Comm_rank(comm, &rank);

	int k, i, r, c;
	double *l = malloc(A -> rows * sizeof(double));
	for(k = 0; k < A -> rows; k++){
		for(i = 0; i < A -> rows; i++){
			if(i != k)
				l[i] = ACCESS(A, i, k) / ACCESS(A, k, k);
			else
				l[i] = 0;
		}

		int slice = A -> rows  / world;
		int overflow = A -> rows % world + slice;
		int countsA[world], countsB[world], countsL[world];
		int displsA[world], displsB[world], displsL[world];

		for(i = 0; i < world; i++){
			displsA[i] = i * A -> cols * slice;
			if( i == world - 1)
				countsA[i] = overflow * A -> cols;
			else
				countsA[i] = slice * A -> cols;
		}
		for(i = 0; i < world; i++){
			displsB[i] = i * B -> cols * slice;
			if( i == world - 1)
				countsB[i] = overflow * B -> cols;
			else
				countsB[i] = slice * B -> cols;
		}
		for(i = 0; i < world; i++){
			displsL[i] = i * slice;
			if( i == world - 1)
				countsL[i] = overflow;
			else
				countsL[i] = slice;
		}

		double *arrA = malloc(countsA[rank] * sizeof(double));	// Number of rows * cols of A
		double *arrB = malloc(countsB[rank] * sizeof(double));	// Number of rows * cols of B
		double *arrL = malloc(countsL[rank] * sizeof(double));	// Number of rows scattered
		double *arrP = malloc((A -> cols + B -> cols) * sizeof(double));	// Size of one row of A

		for(i = 0; i < countsA[rank]; i++)
			arrA[i] = 0;
		for(i = 0; i < countsB[rank]; i++)
			arrB[i] = 0;
		for(i = 0; i < countsL[rank]; i++)
			arrL[i] = 0;

		for(i = 0; i < A -> cols; i++)
			arrP[i] = ACCESS(A, k, i);
		for(i = 0; i < B -> cols; i++)
			arrP[i + A -> cols] = ACCESS(B, k, i);

		MPI_Bcast(arrP, A -> cols + B -> cols, MPI_DOUBLE, 0, comm); 	// Broadcast pivot row
		MPI_Scatterv(l, countsL, displsL, MPI_DOUBLE, arrL, countsL[rank], MPI_DOUBLE, 0, comm); // Scatter L
		MPI_Scatterv(A -> arr, countsA, displsA, MPI_DOUBLE, arrA, countsA[rank], MPI_DOUBLE, 0, comm); // Scatter A rows
		MPI_Scatterv(B -> arr, countsB, displsB, MPI_DOUBLE, arrB, countsB[rank], MPI_DOUBLE, 0, comm); // Scatter B rows

		for(r = 0; r < countsL[rank]; r++){
			if((rank * slice + r) != k){
				for(c = 0; c < A -> cols; c++){
					//printf("%d - %d %1.2f = %1.2f - %1.2f * %1.2f \n", rank, c, (arrA[r * A -> cols + c] - (arrL[r] * arrP[c])), arrA[r * A -> cols + c], arrL[r], arrP[c]);
					arrA[r * A -> cols + c] -= (arrL[r] * arrP[c]);
				}
				for(c = 0; c < B -> cols; c++)
					arrB[r * B -> cols + c] -= (arrL[r] * arrP[A -> cols + c]);
			}
		}

		MPI_Gatherv(arrA, countsA[rank], MPI_DOUBLE, A -> arr, countsA, displsA, MPI_DOUBLE, 0, comm);
		MPI_Gatherv(arrB, countsB[rank], MPI_DOUBLE, B -> arr, countsB, displsB, MPI_DOUBLE, 0, comm);

//--------------------------------------------------------------
	}
	for(r = 0; r < A -> rows; r++){
		for(c = 0; c < B -> cols; c++){
			ACCESS(B, r, c) /= ACCESS(A, r, r);
		}
		ACCESS(A, r, r) /= ACCESS(A, r, r);
	}
}

double twoNormalize(matrix* A){
	int i;
	double x;
	double twoNorm = 0;
	for(i = 0; i < A -> rows; i++){
		x = abs(ACCESS(A, i, 0));
		//printf("[%d, %d] = %1.2f \n",i, 0, x);
		twoNorm += x * x;
	}
	return sqrt(twoNorm);
}

void normalize(matrix *A){
	int i;
	double x;
	double twoNorm = twoNormalize(A);
	for(i = 0; i < A -> rows; i++){
		x = ACCESS(A, i, 0);
		ACCESS(A, i, 0) = x / twoNorm;
	}
}

int notTolerant(matrix *A, matrix *B, double tol){
	int rank;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int i;
	double sol;
	for(i = 0; i < A -> rows; i++){
		if(rank == 0)
			printf("%1.4f, %1.4f\n",ACCESS(A, i, 0), ACCESS(B, i, 0) );
		sleep(3);
		sol = ACCESS(B, i, 0) - ACCESS(A, i, 0);
		if(sol > tol){
			return 1;
		}
	}
	return 0;
}


#endif
