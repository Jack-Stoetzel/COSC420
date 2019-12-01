#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"

const int IDSIZE = 32;
const int TTSIZE = 256;
const int AUSIZE = 256;
const int ABSIZE = 4096;

int main(){
    // 1,614,712
    // Root = 1701.01948
    Articles arxiv;
    wordNode *wordTree;
    char word[32];
    int meta;
    long long i, k;

    meta = open("arxiv-metadata.txt", O_RDONLY);
    if(meta < 0)
    {
      puts("Input file error.");
      exit(1);
    }

    char buf;
    read(meta, &buf, 1);
    for(k = 0; k < 1614712; k++){
        // Parsing the article's ID
        read(meta, &buf, 1);
        for(i = 0; buf != '\n' && i < IDSIZE; i++){
            arxiv.ID[i] = buf;
            read(meta, &buf, 1);
        }
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

            //sleep(1);
        }
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

        // printf("%s \n", arxiv.ID);
        // printf("%s \n", arxiv.title);
        // printf("%s \n", arxiv.authors);
        // printf("%s \n", arxiv.abstract);
        // puts("");

        // Add arxiv to the article tree


        memset(arxiv.ID, 0, sizeof(arxiv.ID));
        memset(arxiv.title, 0, sizeof(arxiv.title));
        memset(arxiv.authors, 0, sizeof(arxiv.authors));
        memset(arxiv.abstract, 0, sizeof(arxiv.abstract));
    }
    //printf("%c \n", buf);
}
