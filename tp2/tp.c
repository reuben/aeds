#include <stdio.h>

#include "bruteforce.h"
#include "greedy.h"
#include "dp.h"

typedef int (*impl_t)(int[501],int,int);

int main(int argc, char** argv)
{
    if (argc < 2) {
        return 1;
    }

    impl_t impl;
    switch (argv[1][1]) {
        case 'P':
            impl = dp;
        break;
        case 'A':
            impl = greedy;
        break;
        case 'F':
            impl = bruteforce;
        break;
        default:
            return 1;
    }

    FILE* fin = stdin; //fopen("input", "r");

    int T;
    fscanf(fin, "%d\n", &T);

    int distances[501];

    while (T--) {
        int N, K;
        fscanf(fin, "%d %d\n", &N, &K);

        printf("accumulated distances: ");
        int i;
        int accum = 0;
        for (i = 0; i < N+1; ++i) {
            int distance;
            fscanf(fin, "%d\n", &distance);
            accum += distance;
            distances[i] = accum;
            printf("%d ", accum);
        }
        printf("\n");

        printf("%d\n", impl(distances, N, K));
    }

    return 0;
}
