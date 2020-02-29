### Lauren Golian and Jack Stoetzel

# Project 2: The Arxiv Search

## How to Run This Program

To run this program, you will first need to run the following commands to load mpich:

````
module purge
module load mpi/mpich-x86_64
````
We wanted to use lseek() to traverse our input file, but encountered multiple errors when using MPICC.
So, we decided to run the program with MPICH instead to resolve these issues.

To compile the program, a makefile has been included. The make file makes 2 executables
  
   * search1 - a version of the program written in parallel that does not work
  ````
  mpiexec -n <N> ./search1
  ````
  Where <N> is the number of nodes to run the program on.
  
  * search2 - a version of the program in sequential (without MPI). To compile:
  ````
  ./search2 <A>
  ````
  Where \<A\> is the number of articles to read into the program. 
  This makes the program testable since it would take too long to read in all the data.
  

## What Does This Program Do?

The goal of this program was to apply our knowledge of parallel programming to make a backwards index database of articles 
and be able to search them and display results using a Page Rank or HITS algorithm to provide the most relevant results 
based on the search term(s).

Our program reads in the file called 'arxiv-metadata.txt' and forms a Red-Black Tree based on all the articles ID's
and a Binary Search Tree of words that are listed in each article abstract. 
Once all data has been read in, the user will be prompted to search for a word. 
The word will be used to search through the BST and go through a linked list of ID's 
that will be used to search the RB tree and retrieve all other information on the article (title and author(s)).

## Questions

### How long does it take to load your database into memory?

In sequential, this takes A VERY LONG TIME to do. 
After testing over the weekend, it never finished but ran for just under 30 hours. 

In our parallel attempt, it should theoretically divide the time by the number of nodes available to the program.

### How long does it take to return results to a user?

Because the program uses Red-Black Trees and Binary Search trees, the program should take O(n * log(n)) time.
O(log(n)) to search the word tree, O(n) to walk the list of article ID, and O(log(n)) to search for ID's in the ID tree.

So, O(log(n)) + O(n) * O(log(n)) == O(n * log(n)).

### Where is there room for improvement?

EVERYWHERE!!!

* Currently, the program does not work in parallel. 
  There were many attempts to read the file in parallel but none successful. 
    * We tried using MPI_File_read but were having problems with segmentation faults and not reading in meaningful data. 
    * Then we tried working with lseek() and having each node start at a different point in the file. 
    This being out last attempt also failed. 
    If we had more time, we would have tried to fix this so that each node reads a part of the data and makes its own tree. 
    Then have all of the trees merge together to one tree in the master node. 
    The idea could work if when we combined trees, we made sure we always inserted to spot that would result in a black node. 

* The searching only works with one word. 
In a crunch for time, we figured being able to search for one word would be better than no searching. 
But, if two words are entered, they will result in individual list of articles that contain each word.

* There is no algorithm to calculate the Page Rank or HITS.
Since we focused so much on reading in the meta-data file and making sure that was working, 
the citation file and all of its data was not used. 

* Author's data could be saved to the program.
As discussed in your office hours, there is a segmentation fault when trying to insert author's information to the tree.
ID and title work fine, but there still seems to be a problem with authors that I couldn't solve.

### Would your program make a decent “Google for research papers”?

If I had 1 more month and no other responsibilities, this could be made to a very good "Google for Research Papers". 
But in its current form, this is nothing close to Google.

## Output

### search2
````
./search2 100
Reading in 'arxiv-metadata.txt'...
Enter a word to search for (enter EXIT to quit): calculation

Articles with word 'calculation' include (but are not limited to): 

0704.0001 
Calculation of prompt diphoton production cross sections at Tevatron and  LHC energies 

0704.0025 
Spectroscopic Properties of Polarons in Strongly Correlated Systems by  Exact Diagrammatic Monte Carlo Method 

0704.0065 
Littlewood-Richardson polynomials

Enter a word to search for (enter EXIT to quit): EXIT
````
