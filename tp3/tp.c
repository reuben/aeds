#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "subset_sum.h"

int main(int argc, char** argv)
{
    if (argc != 3) {
        return 1;
    }

    int threads = atoi(argv[2]);
    if (!threads) {
        return 1;
    }

    FILE* fin = stdin; //fopen("input", "r");
    int problems;
    fscanf(fin, "%d\n", &problems);

    while (problems--) {
        int sum;
        fscanf(fin, "%d\n", &sum);
        assert(sum > 0 && sum <= 1000);

        int values[300];
        char line[3300]; // ceil(log10(2^31)+2)*300
        fgets(line, sizeof(line), fin);

        char* start = line;
        char* end;
        int i = 0;
        long val = strtol(start, &end, 10);
        for (; start != end; val = strtol(start, &end, 10)) {
            assert(labs(val) <= INT_MAX);
            values[i++] = (int)val;
            start = end;
        }

        int d = subset_sum(values, i-1, sum, threads);
        printf("%s\n", d ? "sim" : "nao");
    }

    pthread_exit(NULL);
    return 0;
}
