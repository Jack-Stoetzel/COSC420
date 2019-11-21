#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

const int IDSIZE = 32;
const int TTSIZE = 128;
const int AUSIZE = 128;
const int ABSIZE = 2048;

typedef struct ARTICLES{
    char ID [32];
    char title[128];
    char authors[128];
    char abstract[2048];

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
        read(meta, &buf, 1);
        for(i = 0; buf != '\n' && i < IDSIZE; i++){
            arxiv[k].ID[i] = buf;
            read(meta, &buf, 1);
        }
        read(meta, &buf, 1);
        for(i = 0; buf != '\n' && i < TTSIZE; i++){
            arxiv[k].title[i] = buf;
            read(meta, &buf, 1);
        }
        read(meta, &buf, 1);
        for(i = 0; buf != '\n' && i < AUSIZE; i++){
            arxiv[k].authors[i] = buf;
            read(meta, &buf, 1);
        }
        read(meta, &buf, 1);
        for(i = 0; buf != '\n' && i < ABSIZE; i++){
            arxiv[k].abstract[i] = buf;
            read(meta, &buf, 1);
        }
        read(meta, &buf, 1);
        for(i = 0; buf != '\n'; i++){
            read(meta, &buf, 1);
        }
        printf("%s \n", arxiv[k].ID);
        printf("%s \n", arxiv[k].title);
        printf("%s \n", arxiv[k].authors);
        printf("%s \n", arxiv[k].abstract);
        sleep(3);
}
    //printf("%c \n", buf);


}
