#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

const int IDSIZE = 32;
const int TTSIZE = 256;
const int AUSIZE = 256;
const int ABSIZE = 4096;

typedef struct ARTICLES{
    char ID [32];
    char title[256];
    char authors[256];
    char abstract[4096];

}Articles;

int main(){
    Articles arxiv[1000];
    int meta;
    int i, k;

    meta = open("arxiv-metadata.txt", O_RDONLY);
    if(meta < 0)
	{
		puts("Input file error.");
		exit(1);
	}

    char buf;
    read(meta, &buf, 1);
    for(k = 0; meta; k++){
        // Parsing the article's ID
        read(meta, &buf, 1);
        for(i = 0; buf != '\n' && i < IDSIZE; i++){
            arxiv[k].ID[i] = buf;
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
            arxiv[k].title[i] = buf;
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
            arxiv[k].authors[i] = buf;
            read(meta, &buf, 1);
        }
        if(i == AUSIZE){
            for(i = 0; buf != '\n'; i++){
                read(meta, &buf, 1);
            }
        }
        // Parsing the article's abstract
        read(meta, &buf, 1);
        for(i = 0; buf != '\n' && i < ABSIZE; i++){
            arxiv[k].abstract[i] = buf;
            read(meta, &buf, 1);
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
        printf("%s \n", arxiv[k].ID);
        printf("%s \n", arxiv[k].title);
        printf("%s \n", arxiv[k].authors);
        printf("%s \n", arxiv[k].abstract);
        puts("");
        sleep(3);
}
    //printf("%c \n", buf);


} 