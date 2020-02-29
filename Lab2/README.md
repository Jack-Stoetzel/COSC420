### Lauren Golian and Jack Stoetzel

# Lab 2: Matrix Math

## What Do These Programs Do?

The objective of our program was to create a library that implements matrix multiplication, transposition, addition, and subtraction in parallel. To complete this, a user enters values for matrices (example: 4 2 -- representing a 4 x 2 matrix) and the matrix is populated with randomly generated values. This lab demonstrates newly found skills on MPI functions including; MPI_Scatter, MPI_Gather, and MPI_Reduce.

1. task1.c calculates the dot product of two randomly generated vectors.
2. task2.c implements all matrix operations from our matrixMath.h library. 
On two matrices A & B, we performed the following operations

	- A + B
	- A - B
	- B - A
	- A * B
	- B * A
	- A^(T) 
	- B^(T)

## Questions

### What is the theoretical time complexity of your algorithms (best and worst case), in terms of the input size?
	
The time complexity of our algorithm runs on the number of elements, n, divided by the number of processors that we're using. 
This is both best- and worst-case time for addition and subtraction of our algorithm. 
For multiplication, the time complexity is a bit more complicated, so I believe it is O ((n * m * n)/p).

### According to the data, does adding more nodes perfectly divide the time taken by the program?

Unfortunately, no. While this question should probably hold true, our code still has inefficiencies that we'd like to work out :(

### What are some real-world software examples that would need the above routines? Why? Would they benefit greatly from using your distributed code?

Although we haven't taken graphics at Salisbury, any 3D animation or video game	relies on linear algebra, namely matrices to convert between dimensional spaces. 
Therefore, this would be especially helpful in performing operations that required a lot of time.

### How could the code be improved in terms of usability, efficiency, and robustness?

Overall, our code could be improved in terms of usability by making our input and error checking more present. 
At this point, our program runs for most cases, but there may be some edge cases we could further test to ensure that our program works.

We attempted to check the rows and columns the user enters in the main function. 
After many tests this proved unsuccessful. 
Doing the checks in the main would prevent the parallel functions from executing. 
This error was too substantial to ignore, so we decided to remove the error checking all together. 
After submission, we discussed this problem with other groups and determined that a more effective approach would have been to do error checking inside out functions so that it would not prevent the process from occurring, and so further use of these library functions can be used in future labs.

Looking forward, this lab demonstrates intermediate knowledge of MPI_Scatter and MPI_Gather to which with more practice and understanding, we plan to implement the matrix method by scattering the row of matrix A and broadcasting matrix B to test performance rates.