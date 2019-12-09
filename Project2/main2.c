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

Compute the adjacentcy matrix as we insert IDs into the the word list???
*/
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "atree.h"
#include "wtree.h"

int main(int argc, char* argv[])
{
    Article arxiv;
    // Setting the initial root to '1701.01948' so the tree can have some help balancing out at first
    arxiv.IDSize = 11;
    arxiv.ID = (char*) calloc(arxiv.IDSize, sizeof(char));
    strcpy(arxiv.ID, "1701.01948");
    arxiv.authorSize = 2;
    arxiv.author = (char*) calloc(arxiv.authorSize, sizeof(char));
    arxiv.titleSize = 1;
    arxiv.title = (char*) calloc(arxiv.titleSize, sizeof(char));

    ArticleNode *aRoot = NULL;
    articleInsert(&aRoot, &arxiv);
    WordNode* wRoot;
    makeRoot(&wRoot);

    int meta;
    long long i, count = 0;
    long long input;
    if(argc < 1 || argc > 2)
    {
        puts("Invalid input");
        exit(1);
    }
    if(argc == 1)
    {
        input = 1614712;
    }
    else if(argc == 2)
    {
        input = (long long) atoi(argv[1]);
    }

    char info[37000];
    memset(info, '\0', sizeof(info));

    meta = open("arxiv-metadata.txt", O_RDONLY);
    if(meta < 0)
    {
        puts("Input file error.");
        exit(1);
    }

    char buf;
    int pos = 0;    // Will determine struct element 0.ID 1.Title 2.Author 3.Abstract 4.++++++
    int size[5];
    for(i = 0; i < 5; i++){
        size[i] = 0;
    }
    puts("Reading in 'arxiv-metadata.txt'...");
    while(read(meta, &buf, 1) > 0 && count != input){
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
            info[size[pos] - 1] = '\0';
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
                //printf("%s\n", info);
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
                count++;
            }
            pos++;
            memset(info, '\0', sizeof(info));
        }
    }
    int running = 1;
    while(running)
    {
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
    return 0;
}
