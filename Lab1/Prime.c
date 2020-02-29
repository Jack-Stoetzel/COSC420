/*
 * Lauren Golian & Jack Stoetzel
 * 2019/09/04
 * Dr. Anderson
 * COSC 420-001
 * 
Write a first “hello world” program to:
(a) Output the “rank” of the node, and
   the size of the current environment
(b) Output the processor name of the node

Write a second program to perform a “primality” test of a
given number, N, an unsigned long integer:
(a) Use brute force, but you need only test up to √N. 
    Note you may need to use the compiler option -lm to
    link against the math libraries.
(b) If the number is found to be composite
    report one way to factor it.
(c) Split the work across the available number of nodes,
    using the mpi interfaces.
(d) Include some diagnostic messages to display which 
    nodes are testing which ranges of factors, and other 
    relevant information you find helpful to be sure 
    the program is working appropriately
 */

#include <stdio.h>
#include <mpi.h>
#include <math.h>



int main(int argc, char*argv[]){

    MPI_Init(NULL, NULL);
    //---------------------------------------------------------
    char name[MPI_MAX_PROCESSOR_NAME];
    
    int length;

    MPI_Get_processor_name(name, &length);	// Gets name

    MPI_Comm communicator = MPI_COMM_WORLD;	// Initializes
						// MPI comm
    int rank;
    
    MPI_Comm_rank(communicator, &rank);		// Gets rank of
						// current node
    int world;
    
    MPI_Comm_size(communicator, &world);	// Gets size of
						// of the world
    //---------------------------------------------------------
    int i = rank + 2;
    unsigned long int num = atoi(argv[1]);   
    
    while(num > 3 && i <= sqrt(num)){
	//printf("Node %d (%s) is testing %d.\n", rank, name, i);
       if(num % i == 0){
           printf("%d is divisible by %d.\n", num, i);
           MPI_Abort(communicator, 1);
       }
       i += world;
   }
   MPI_Finalize();
   return 0;
}
