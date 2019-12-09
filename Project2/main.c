/*
arxiv-metadata.txt
Lines:          8,073,560
Words:          229,414,035
Chars:          1,652,562,265
Bytes:          1,652,562,299

Articles:       1,614,712
Middle Article: 1701.01948
Longest Line:   36,182

TESTING:
Sequential:
Start Time: 11:12:00    (Prints IDs)

NOTE TO SELF:
Ideas for parallel:
Take the total number of bytes in the file and divide by world size
Test each block to adjust its position to an ID Line
Broadcast all offets to each node so they know where to start reading
Each node will will add its article to a RB tree
BONUS IDEA: if they all have RB Trees, will adding the root of one tree to another tree still work???
- Root has to be black, so if we insert the node to a sopt where it black, does it keep its integrity?

Compute the adjacentcy matrix as we insert IDs into the the word list
*/
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "mpi.h"

#include "atree.h"
#include "wtree.h"

long long findOffset(int meta, long long displ, long long sendCount)
{

    char buf;
    int k, n;
    int found = 0;
    for(k = 0; found == 0; k++)
    {
        lseek(meta, (displ + sendCount - k), SEEK_SET);
        read(meta, &buf, 1);
        if(buf == '\n')
        {
            for(n = 1; n <= 6; n++)
            {
                lseek(meta, (displ + sendCount - k) - n, SEEK_SET);
                read(meta, &buf, 1);
                if(buf != '+')
                {
                    n = 6;
                    found = 0;
                }
                else
                {
                    found = 1;
                }
            }
        }
    }
    return (sendCount - (k -3   ));
}

int main()
{
    MPI_Init(NULL, NULL);

    MPI_Comm comm = MPI_COMM_WORLD;

    int world;
    MPI_Comm_size(comm, &world);

    int rank;
    MPI_Comm_rank(comm, &rank);

    Article arxiv;
    ArticleNode *aRoot = NULL;
    WordNode* wRoot;
    makeRoot(&wRoot);

    char buf;
    int meta;
    long long i, k;
    MPI_File file;
    MPI_Status status;
    char info[37000];
    memset(info, '\0', sizeof(info));

    int pos = 0;    // Will determine struct element 0. ID 1. Title 2. Author 3. Abstract
    int size[5];
    for(i = 0; i < 5; i++){
        size[i] = 0;
    }

    if(rank == 0)
    {
        meta = open("arxiv-metadata.txt", O_RDONLY);
        if(meta < 0)
        {
            puts("Input file error.");
            exit(1);
        }
    }
    //MPI_Bcast(meta, 1, MPI_INT, 0, comm);
    //-----------------------------------------------------------------------------------------------------------------
    long long biteSize = 1652562265;
    long long block = biteSize / world;
    long long overflow = biteSize % world + block;

    long long sendCounts[world];
    long long  displs[world];

    if(rank == 0)
    for(i = 0; i < world; i++){
        if(i == 0)
        {
            displs[i] = 0;
        }
        else
        {
            displs[i] = displs[i - 1] + sendCounts[i - 1];
        }
        if(i == world - 1)
        {
            sendCounts[i] = biteSize - displs[i];
        }
        else
        {
            sendCounts[i] = findOffset(meta, displs[i], block);
        }
        //printf("%lld - %lld (%lld)\n", displs[i], displs[i] + sendCounts[i], sendCounts[i]);
    }
    //-----------------------------------------------------------------------------------------------------------------
    if(rank != 0)
        sleep(5);
    lseek(meta, displs[i], SEEK_SET);
    for(i = 0; i < sendCounts[rank]; i++)
    {
        read(meta, &buf, 1);
        if(pos == 3)
        {
            info[size[pos]] = tolower(buf);
        }
        else
        {
            info[size[pos]] = buf;
        }
        size[pos]++;
        if((buf == ' ' || buf == '\n') && pos == 3)
        {
            //printf("%s \n", info);

            wordInsert(&wRoot, &arxiv, info, size[pos]);

            memset(info, '\0', sizeof(info));

            size[pos] = 0;
            if(buf == '\n')
            {
                pos++;
            }
        }
        else if(buf == '\n')
        {
            if(pos == 0)
            {
                // ID
                printf("%d - %s\n", rank, info);
                info[size[pos] - 1] = '\0';
                arxiv.IDSize = size[pos];
                size[pos] = 0;

                arxiv.ID = (char*) calloc(arxiv.IDSize, sizeof(char));
                strcpy(arxiv.ID, info);
            }
            else if(pos == 1)
            {
                // Title
                //printf("%d - %s\n", size[pos], info);
                info[size[pos] - 1] = '\0';
                arxiv.titleSize = size[pos];
                size[pos] = 0;

                arxiv.title = (char*) calloc(arxiv.titleSize, sizeof(char));
                strcpy(arxiv.title, info);
            }
            else if(pos == 2)
            {
                // Author
                //printf("%d - %s\n", size[pos], info);
                info[size[pos] - 1] = '\0';
                arxiv.authorSize = size[pos];
                size[pos] = 0;

                arxiv.author = (char*) calloc(arxiv.authorSize, sizeof(char));
                strcpy(arxiv.author, info);

                // Take off the indentation at the begining of the abstract
                read(meta, &buf, 1);
                read(meta, &buf, 1);
            }
            else if(pos == 4)
            {
                // Inserting the object into the RB Tree indexed by article ID's
                articleInsert(&aRoot, &arxiv);
                // Clearing the object so the space can be reused for the next set of input
                memset(arxiv.ID, '\0', sizeof(size[0]));
                memset(arxiv.title, '\0', sizeof(size[1]));
                memset(arxiv.author, '\0', sizeof(size[2]));

                size[pos] = 0;
                pos = -1;
            }
            pos++;
            memset(info, '\0', sizeof(info));
        }
    }
    if(rank == 0)
    {
        int running = 1;
        while(running){
            char str[128];
            printf("Enter a word to search for (enter EXIT to quit): ");
            scanf("%s",str);
            if(strcmp(str, "EXIT") == 0)
            {
                running = 0;
            }
            else
            {
                printf("\nArticles with word '%s' include (but are not limited to): \n\n", str);
                for(i = 0; i < strlen(str); i++)
                {
                    str[i] = tolower(str[i]);
                }
                wordSearch(&wRoot, str, &aRoot);
            }
        }
    }

    MPI_Finalize();
    return 0;
}
