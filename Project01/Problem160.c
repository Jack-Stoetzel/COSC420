#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    MPI_Init(NULL, NULL);
    //----------------------------------------------------------
    int world;
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Comm_size(comm, &world);
    int rank;
    MPI_Comm_rank(comm, &rank);
    //----------------------------------------------------------
    long long fact;
    long long lower,upper;
    long long i;
    long long result = 1;
    long long total = 1;

    if(rank==0)
    {
        fact = 1000000000000;
        //printf("%.3f\n",pow(a,b));
        if(fact == 1000000000000){
            double a = 5.0;
            double b = 7.0;
            fact = fact / pow(a,b);
        }

    }
    long long* finalArr = malloc(world * sizeof(long long));
    MPI_Bcast(&fact, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);
    //*****************Dividing work to nodes*******************
    if(rank==0)
        lower = 1;
    else
        lower = rank * (fact / world) + 1;
    if(rank == (world - 1))
        upper = fact;
    else
        upper = (rank + 1) * (fact / world);
    //******************Solving factorials**********************
    for(i = lower; i <= upper; i++){
        long long temp = i;
        result *= temp;

        // Chops off trailing zeros
        while(result % 10 == 0){
            //puts("Loop 1");
            result /= 10;
        }
        // Reduces result to 5 digits
        while(result / fact > 1){
            //puts("Loop 2");
            result = result % fact;
        }
    }
    //printf ("Rank %ld   %ld - %ld = %ld\n", rank, lower, upper, result);
    MPI_Gather(&result, 1, MPI_LONG_LONG, finalArr, 1, MPI_LONG_LONG, 0, comm);
    //***************Reducing each nodes results****************
    for(i = 0; i < world; i++){
        total *= finalArr[i];

        while(total % 10 == 0){
            total /= 10;
        }
        while(total / 100000 >= 1){
            total = total % 100000;
        }
    }

    if(rank==0)
    {
        printf("The factorials 5 trailing digits are %1.0ld. \n",total);
        free(finalArr);
    }
    MPI_Finalize();
    return 0;
}
