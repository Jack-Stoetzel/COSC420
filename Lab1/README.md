### Jack Stoetzel and Lauren Golian

# Lab 1: Intro to Parallel Programming

## What Do These Programs Do?

In the following lab, we apply Message Passing Interface (MPI) to practice running C programs in parallel.

1. HelloWorld.c should be running on multiple nodes and prints a hello world message and each node displays its rank number.
2. Prime.c finds numbers that divide into another number entered on the command line.

## Questions: 

### What is the theoretical time complexity of your sorting algorithms (best and worst case), in terms of the input size?

The theoretical time complexity our algorithm yields is O(nlog(n)) and O(log(n)) in the worst- and best-case scenarios, respectively. 
As our output suggests, performing the 'primality' test increases the time as nodes are added.

### According to the data, does adding more nodes perfectly divide the time taken by the program?

As mentioned in the previous question, when the work is divided across multiple nodes the time to determine a numbers primality increases slightly. (Check Excel File.)

### Justify the fact that one needs only checkup to (sqrt(N)) in the brute force primality test.

The square root of any number, N, returns two equivalent factors. 
Therefore, to determine if a number is divisible, a factor less than the square root and greater than the square root can be multiplied to equal the initial number, N. 
To test for primality, only numbers LESS THAN THE SQUARE ROOT need to be tested because no values greater than the square root can be factors of N.

### How could the code be improved in terms of usability, efficiency, and robustness?

Currently our test implements the 'scatter' model which tasks each node and returns the result with no communication across nodes. 
To increase efficiency, communication between nodes could eliminate factors that reduce into smaller factors. 
Example: numbers divisible by 6 are also divisible by 3 and 2 so in the best scenario our code would only test with prime numbers. 
