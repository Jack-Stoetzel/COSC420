#include <stdio.h>

int main(int argc, char* argv[]){
    unsigned long loop = 10000000;
    unsigned long max = 100000;
    unsigned long fact = 1;

    unsigned long i, k;

    for(i = 0; i < loop; i++){
        for(k = 1; k <= max; k++){
            fact *= k;
            if(fact % 10 == 0){
                fact /= 10;
            }
            if(fact / max >= 1){
                fact = fact % max;
            }
        }
    }
    printf("The factorial's trailing digits are %lu \n", fact);
    return 0;
}
