/*
Lauren Golian & Jack Stoetzel
September 18, 2019
Dr. Anderson
COSC 420-001

Write a short program to practice using MPI_Scatter and MPI_Reduce where:
(a) A root node generates two random vectors in high dimension (thousands)
(b) “Blocks” of those vectors are then scattered to various nodes, each of
    which performs a partial inner product
(c) The partial inner products are then combined by the root and reported to
    the user
*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

int main(int argc, char** argv){

    MPI_Init(NULL, NULL);
    srand(time(NULL));
    //-------------------------------------------------------------
    int world, rank;

    MPI_Comm comm = MPI_COMM_WORLD;

    MPI_Comm_size(comm, &world);

    MPI_Comm_rank(comm, &rank);
    //-------------------------------------------------------------

    int *arr1, *arr2;
    int arrSize = atoi(argv[1]);
    int divide = arrSize/world;
    int prod = 0, i = 0;

    arr1 = malloc(arrSize * sizeof(int));
    arr2 = malloc(arrSize * sizeof(int));

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
        printf("Inner product of the two vectors is = %d \n", final);

    MPI_Finalize();
    return 0;
}
