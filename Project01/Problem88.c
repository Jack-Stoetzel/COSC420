#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"

//**************************************************************
// k = 2        4
// k = 3        10
// k = 4        18
// k = 5        18
// k = 6        30
// k = 12       61
// k = 12000    7587457
//**************************************************************

int check(int prod, int sum, int k) {
    if(sum < k){
        return 0;
    }
    if(prod == 1){
        if(sum == k){
            return 1;
        }
        else{
            return 0;
        }
    }
    if(k == 1){
        if(sum == prod){
            return 1;
        }
        else{
            return 0;
        }
    }
    int i;
    for(i = 2; i <= prod && sum - i >= k - 1; i++) {
        if(prod % i == 0) {
            if(check(prod / i, sum - i, k - 1)){
                return 1;
            }
        }
    }
    return 0;
}



int main(int argc, char *argv[]){
    MPI_Init(NULL, NULL);

    //----------------------------------------------------------
    int world;

    MPI_Comm comm = MPI_COMM_WORLD;

    MPI_Comm_size(comm, &world);

    int rank;

    MPI_Comm_rank(comm, &rank);
    //----------------------------------------------------------


    int k = atoi(argv[1]);
    int *finalArr = malloc((k - 1) * sizeof(int));
    int i, n, count, found, result = 0, total = 0;
    for(i = 0; i < (k-1); i++){
        finalArr[i] = 0;
    }

    int lower, upper;



    int block = k / world;
    int overflow = k % world + block - 1;

    int sendCounts[world];
    int displs[world];


    for(i = 0; i < world; i++){
        displs[i] = i * (block);
        if(i == world)
            sendCounts[i] = overflow;
        else
            sendCounts[i] = block;
    }

    MPI_Bcast(&k, 1, MPI_INT, 0, comm);

    //*****************Dividing work to nodes*******************

    if(rank == 0)
    {
        lower = 2;
    }
    else
    {
        lower = rank * (k / world) + 1;
    }
    if(rank == (world - 1))
    {
        upper = k;
    }
    else
    {
        upper = (rank + 1) * (k / world);
    }

    int answers[upper - lower + 1];

    for(i = 0; i < sendCounts[rank]; i++)
        answers[i] = 0;

    int m = 0;
    /*
    for(i = lower; i <= upper; i++)
    {
        found = 0;
        for(n = i; found == 0; n++) {
            //printf("Checking %d and %d \n", n, i);
            if(check(n, n, i)){
                answers[m] = n;
                printf("k = %d: %d \n", i, n);
                found = 1;
            }
        }
    }
    */

    for(m = 0; m < (upper-lower)+1; ){
        for(i = lower; i <= upper; i++){
            n = 1;
            while(check(n,n,i) == 0){
                //printf("k = %d: %d \n", i, n);
                n++;
            }
            answers[m] = n;
            m++;
        }
    }


    MPI_Gatherv(answers, ((upper-lower)+1), MPI_INT, finalArr, sendCounts, displs, MPI_INT, 0, comm);

    for(i = 0; i < k - 1; i++){
        for(n = 0; n < k - 1; n++){
            if(finalArr[i] == finalArr[n] && i != n){
                //printf("%d == %d \n", finalArr[i], finalArr[n]);
                finalArr[n] = 0;
            }
        }
        total += finalArr[i];
    }

    if(rank == 0)
        printf("The minimum product-sum of %d is %d \n", k, total);

    MPI_Finalize();
    return 0;
}
