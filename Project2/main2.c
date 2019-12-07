/*
    arxiv-metadata.txt
    Lines:          8,073,560
    Articles:       1,614,712
    Root Article:   1701.01948

    TODO
        Figure out how to add word to the word tree starting with the ID.
            Start with (buf == ' ' || buf == '\n')??? or  (buf == ' ') at the beginning

        TEST THIS RB tree
        Test inserting all articles into the list and searching an article ID
        If the first RB tree works, make another for words with a LL for IDs with that word

    EXTA
        Compute the adjacentcy matrix as we insert IDs into the the word list
*/
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "atree2.h"
#include "wtree.h"

int main(){
    // 1,614,712
    // Root = 1701.01948
    Article arxiv;

    ArticleNode *root = NULL;
    int meta;
    long long i, k = 0;
    char info[37000];
    memset(info, '\0', sizeof(info));

    meta = open("arxiv-metadata.txt", O_RDONLY);
    if(meta < 0)
    {
      puts("Input file error.");
      exit(1);
    }

    char buf;
    int pos = 0;    // Will determine struct element 0. ID 1. Title 2. Author 3. Abstract
    int size[5];
    for(i = 0; i < 5; i++){
        size[i] = 0;
    }
    while(read(meta, &buf, 1) > 0){
        info[size[pos]] = buf;
        size[pos]++;
        if((buf == ' ' || buf == '\n') && pos == 3){
            // Insert word to word tree w/ ID
            memset(info, '\0', sizeof(info));
            if(buf == '\n'){
                pos++;
            }
            size[pos] = 0;
        }
        else if(buf == '\n'){
            if(pos == 0){
                // ID
                //printf("%d - %s\n", size[0], info);
                arxiv.IDSize = size[pos] + 1;
                size[pos] = 0;

                arxiv.ID = (char*) calloc(arxiv.IDSize, sizeof(char));
                strcpy(arxiv.ID, info);
            }
            else if(pos == 1){
                // Title
                //printf("%d - %s\n", size[1], info);
                arxiv.titleSize = size[pos] + 1;
                size[pos] = 0;

                arxiv.title = (char*) calloc(arxiv.titleSize, sizeof(char));
                strcpy(arxiv.title, info);
            }
            else if(pos == 2){
                // Author
                arxiv.authorSize = size[pos] + 1;
                size[pos] = 0;

                arxiv.author = (char*) calloc(arxiv.authorSize, sizeof(char));
                strcpy(arxiv.author, info);
            }
            else if(pos == 4){
                //printf("Inserting %s\n", arxiv.ID);
                articleInsert(&root, &arxiv);
                //puts("\n In-Order \n");
                //inorder(root);
                //printf("%s \n", arxiv.ID);
                // printf("%s \n", arxiv.title);
                // printf("%s \n\n", arxiv.authors);
                //sleep(2);
                memset(arxiv.ID, '\0', sizeof(size[0]));
                memset(arxiv.title, '\0', sizeof(size[1]));
                memset(arxiv.author, '\0', sizeof(size[2]));

                size[pos] = 0;
                pos = -1;
                sleep(1);
            }
            pos++;
            memset(info, '\0', sizeof(info));
        }
    }
    inorder(root);
    //printf("%c \n", buf);
}
