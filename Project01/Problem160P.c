//**************************************************************
// N >= 1,000,000 is incorrect. Numbers are close but not exact.
//
// Reconsider
//      - Modulus -> 0 reduction, or vice versa
//      - Other reduction computations
//      - Problem with moudulus or 0 reductions??? (less likely)
//**************************************************************

//**************************************************************
//   10                  =   36288 DONE
//   100                 =   16864 DONE
//   1,000               =   53472 DONE
//   10,000              =   79008 DONE
//   100,000             =   62496 DONE
//   1,000,000           =   12544 DONE
//   10,000,000          =   94688 DONE
//   100,000,000         =   54176
//   1,000,000,000       =   38144
//   10,000,000,000      =   46112
//   100,000,000,000     =   67808
//   1,000,000,000,000   =   16576
//**************************************************************


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "mpi.h"

int main(int argc, char *argv[])
{
    long long fact;
    long long lower,upper;
    long long i;
    long long result = 1;
    long long total = 1;


    MPI_Init(NULL, NULL);

    //----------------------------------------------------------
    int world;

    MPI_Comm comm = MPI_COMM_WORLD;

    MPI_Comm_size(comm, &world);

    int rank;

    MPI_Comm_rank(comm, &rank);
    //----------------------------------------------------------


    if(rank==0)
    {
        fact = atoi(argv[1]);
    }

    long long* finalArr = malloc(world * sizeof(long long));

    MPI_Bcast(&fact, 1, MPI_INT, 0, MPI_COMM_WORLD);

    //*****************Dividing work to nodes*******************

    if(rank==0)
    {
        lower = 1;
    }
    else
    {
        lower = rank * (fact / world) + 1;
    }
    if(rank == (world - 1))
    {
        upper = fact;
    }
    else
    {
        upper = (rank + 1) * (fact / world);
    }

    //******************Solving factorials**********************

    for(i = lower; i <= upper; i++)
    {

        long long temp = i;

        result *= temp;


        // Chops off trailing zeros
        while(result % 10 == 0)
        {
            //puts("Loop 1");
            result /= 10;
        }
        // Reduces result to 5 digits
        while(result / fact > 1)
        {
            //puts("Loop 2");
            result = result % fact;
        }
    }
    printf ("Rank %ld   %ld - %ld = %ld\n", rank, lower, upper, result);


    MPI_Gather(&result, 1, MPI_LONG_LONG, finalArr, 1, MPI_LONG_LONG, 0, comm);

    //***************Reducing each nodes results****************



    for(i = 0; i < world; i++)
    {
        if(rank == 0)
            printf("arr = %ld \n",  finalArr[i]);

        total *= finalArr[i];

        while(total % 10 == 0)
        {
            total /= 10;
        }
        while(total / 100000 >= 1)
        {
            total = total % 100000;
        }
    }

    if(rank==0)
    {
        int fd1 = open("Out.txt", O_WRONLY || O_CREAT || O_EXCL, 0777);
        dup2(fd1, 1);
        printf("The factorials 5 trailing digits are %1.0ld. \n",total);
        MPI_Finalize();
        close(fd1);
        return 0;
    }
    return 0;
}
