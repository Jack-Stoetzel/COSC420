//  TODO
//    Figure out how to add word to the word tree starting with the ID.
//      Start with (buf == ' ' || buf == '\n')??? or  (buf == ' ') at the beginning
//      
//    TEST THIS RB tree
//    Test inserting all articles into the list and searching an article ID
//    If the first RB tree works, make another for words with a LL for IDs with that word
//
//  EXTA
//  Compute the adjacentcy matrix as we insert IDs into the the word list

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "atree2.h"

int main(){
    // 1,614,712
    // Root = 1701.01948
    Article arxiv;

    ArticleNode *root = NULL;
    //char word[400];
    int meta;
    long long i, k;
    char info[37000];

    meta = open("arxiv-metadata.txt", O_RDONLY);
    if(meta < 0)
    {
      puts("Input file error.");
      exit(1);
    }

    char buf;
    int pos = 0;    // Will determine struct element 0. ID 1. Title 2. Author 3. Abstract
    int size[5];   
    while(read(meta, &buf, 1) > 0){
        info[size[pos]] = buf;
        size[pos]++;
        if((buf == ' ' || buf == '\n') && pos == 3){
            // Insert word to word tree w/ ID
            if(buf == '\n'){
                pos++;  
            }
        }
        else if(buf == '\n'){
            if(pos == 0){
                arxiv.ID = (char*) calloc(size[0], sizeof(char));
                strcpy(arxiv.ID, info);
            }
            else if(pos == 1){
                arxiv.title = (char*) calloc(size[1], sizeof(char));
                strcpy(arxiv.title, info);
            }
            else if(pos == 2){
                arxiv.authors = (char*) calloc(size[2], sizeof(char));
                strcpy(arxiv.authors, info);
            }
            else if(pos == 4){
                // Insert into Article Tree
                printf("%s \n", arxiv.ID);
                printf("%s \n", arxiv.title);
                printf("%s \n\n", arxiv.authors);
               // sleep(2);
                memset(arxiv.ID, '\0', sizeof(size[0]));
                memset(arxiv.title, '\0', sizeof(size[1]));
                memset(arxiv.authors, '\0', sizeof(size[2]));

                for(int i = 0; i < 4; i++){
                    size[i] = 0;
                }
                pos = -1;
            }
            pos++;
            memset(info, '\0', sizeof(info));
        }
    }

/*
        // Parsing the article's ID
        read(meta, &buf, 1);
        for(i = 0; buf != '\n' && i < IDSIZE; i++){
            //printf("%c\n", buf);
            arxiv.ID[i] = buf;
            read(meta, &buf, 1);
        }
        arxiv.ID[i] = '\0';
        if(i == IDSIZE){
            for(i = 0; buf != '\n'; i++){
                read(meta, &buf, 1);
            }
        }
        // Parsing the article's title
        read(meta, &buf, 1);
        for(i = 0; buf != '\n' && i < TTSIZE; i++){
            arxiv.title[i] = buf;
            read(meta, &buf, 1);
        }
        arxiv.title[i] = '\0';
        if(i == TTSIZE){
            for(i = 0; buf != '\n'; i++){
                read(meta, &buf, 1);
            }
        }
        // Parsing the article's author
        read(meta, &buf, 1);
        for(i = 0; buf != '\n' && i < AUSIZE; i++){
            arxiv.authors[i] = buf;
            read(meta, &buf, 1);
        }
        arxiv.authors[i] = '\0';
        if(i == AUSIZE){
            for(i = 0; buf != '\n'; i++){
                read(meta, &buf, 1);
            }
        }
        // Parsing the article's abstract
        int wordPos = -2;
        read(meta, &buf, 1);
        for(i = 0; buf != '\n' && i < ABSIZE; i++){
                //printf("%d %c \n", i, buf);
            if(buf != ' ' && i > 1){
                //printf("HERE %c \n", buf);
                word[wordPos + i] = buf;
            }
            else if(buf == ' ' && i > 1){
                    // Add word to tree w/ article ID
                wordPos = (i + 1) * (-1);
                memset(word, 0, sizeof(word));
            }

            arxiv.abstract[i] = buf;
            read(meta, &buf, 1);
        }
        arxiv.abstract[i] = '\0';
        if(i == ABSIZE){
            for(i = 0; buf != '\n'; i++){
                read(meta, &buf, 1);
            }
        }
        // Removing the +++++
        read(meta, &buf, 1);
        for(i = 0; buf != '\n'; i++){
            read(meta, &buf, 1);
        }

        //printf("%s \n", arxiv.ID);
        // printf("%s \n", arxiv.title);
        // printf("%s \n", arxiv.authors);
        // printf("%s \n", arxiv.abstract);
        // puts("");

        // Add arxiv to the article tree
        insert(root, &arxiv);
        printf("main root = %s\n", root -> article -> ID);
        memset(arxiv.ID, '\0', sizeof(arxiv.ID));
        memset(arxiv.title, '\0', sizeof(arxiv.title));
        memset(arxiv.authors, '\0', sizeof(arxiv.authors));
        memset(arxiv.abstract, '\0', sizeof(arxiv.abstract));
    */
    inorder(root);
    //printf("%c \n", buf);
}
