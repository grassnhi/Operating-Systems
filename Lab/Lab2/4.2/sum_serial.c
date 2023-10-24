#include <stdio.h>
#include <stdlib.h>

long long sum_serial(int n) {
    long long sum = 0;
    for (int i = 1; i <= n; i++) {
        sum += i;
    }
    return sum;
}

int main(int argc, char *argv[]){
    long n = strtol(argv[1], NULL, 10);
    long long result = sum_serial(n);
    printf("Sum of first %ld numbers is %lld (serial)\n", n, result);

    return 0;
}
