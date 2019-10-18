#include <stdio.h>

int main(){
    double trillion = 1000000000;

    double *squares = new double;

    for(double i = 2; i < 10000; i++){
        for(double k = 1; k < a; k++){
            if(a * a * a * b * b * b * b >= trillion){
                if()
            }
        }
    }

    return 0;
}

/*


long limit = (long)1e12;
List<long> progressiveSquares = new List<long>();

for (long a = 2; a < 10000; a++) {
    for (long b = 1; b < a; b++) {
        if (a * a * a * b * b + b * b >= limit) break;
        if (GCD(a, b) > 1) continue;

        for (long c = 1; ; c++) {
            long n = a * a * a * b * c*c + c * b * b;
            if (n >= limit) break;

            if (IsSquare(n) && !progressiveSquares.Contains(n))
                progressiveSquares.Add(n);

        }
    }
}

long result = progressiveSquares.Sum();
*/
