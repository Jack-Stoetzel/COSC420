# Lab 04

## Questions

### What is the theoretical time complexity of your algorithms (best and worst case), in terms of the input size?

The current version only operates with 3 numbers on either side of the passwords.
So every word is calculated 2002 times over.
* Once for reading in the word
* Once for calulating without numbers
* And the 2000 for numbers being attached to both sides (1000 numbers on the front, and 1000 numbers on the back).
Therefore, the time complexity of the algorithm in best case and worst case respectively is: O(n*2002), O((n * size of dicitonary * 2002)/# of processors).

### According to the data, does adding more nodes perfectly divide the time taken by the program?

No, adding more nodes does not perfectly divide the time of the program. 
Currently the program divides works in the following way:

* The number of nodes is subtracted by two if there is an odd number, or subtracted by two if it is an even number. 
  * These one or two nodes will be used to compute words without any number attached to them. Ex) password
* The remaining nodes are then divided in half. 
  * Even nodes will be used to compute words with numbers attached to the end. Ex) password123
  * Odd nodes will be used to compute words with numbers attached to the begining. Ex) 123password
  
So with large sets of nodes, the program runs very inefficiently. 

### Consider the problem of brute-forcing passwords under only maximum string length. How much time would it take to complete this hack, extrapolating from your measurements?



### What are some real-world software examples that would need the above routines? Why? Would they benefit greatly from using your distributed code?

Off the top of my head, the only two practical uses of this software are:
  * Hackers who want to use this code to brute force peoples passwords to access other poeple's accounts.
  * A company might want to use this to test if any of their employees have weak passwords on their accounts.

This code is very computation heavy. 
For anyone to use this and get results in a reasonable amount of time, the work would need to be distibuted amongst MANY processes. 

### How could the code be improved in terms of usability, efficiency, and robustness?

As stated in the second question, there is a lot of inefficency with how the work is divided.
To improve this, more nodes could be allocated to computing words with no numbers attached. 
Though that would take away from the computations on words with numbers attached, there could be a formula to determine an optimal amount to allocate for each operation so that all three parts finish in relativley the same amount of time.

This could also be improved in with more effcient file reading. 
Currently, it takes 13s to read from the words.txt file in serial. 
After many attempts to read the words file in parallel with MPI, we decided to read in serial. 
If this was done in parallel, that would significantly increase the overall speed of the program.

Another change to increase usablitity would be to read from a shadow file. 
For the sake of time, we hard coded the salt and the passwords in the shadow file.
If this was read from an actual shadow file, the usability could universal for any shadow file.
