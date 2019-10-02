/*
 * Lauren Golian & Jack Stoetzel
 * 2019/09/04
 * Dr. Anderson
 * COSC 420-001
 * 
 */

#include <stdio.h>
#include <mpi.h>
 
int main(int argc, char ***argv){
    
    MPI_Init(NULL, NULL);  //Initializes MPI functions
    //---------------------------------------------------------
    //               Print Out Processor Name
    char name[MPI_MAX_PROCESSOR_NAME];
    
    int length;
    
    MPI_Get_processor_name(name, &length);
    
    printf("Hello, World! My name is %s.\n", name);    
    //---------------------------------------------------------
    //		   Print Out Rank and World Size
    MPI_Comm communicator = MPI_COMM_WORLD;	// Initializes
						// MPI comm
    int rank;
    
    MPI_Comm_rank(communicator, &rank);		// Gets rank of
						// current node
    int world;
    
    MPI_Comm_size(communicator, &world);		// Gets size of
						// of the world
    printf("I am rank %d out of %d.\n\n", rank, world);
    
    MPI_Finalize();	//Ends MPI Functions
    
    return 0;
}