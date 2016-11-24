#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

#define MAX(a,b) ((a)>(b)?(a):(b))
#define MAXWEIGHT 300

typedef struct {
    int val;
    int from_j;
} pack_t;

int main(int argc, char** argv)
{
    // if (argc != 3) {
    //     return 1;
    // }

    // int threads = atoi(argv[2]);
    // if (!threads) {
    //     return 1;
    // }

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
        int n = 0;
        long val = strtol(start, &end, 10);
        for (; start != end; val = strtol(start, &end, 10)) {
            assert(labs(val) <= INT_MAX);
            values[n++] = (int)val;
            start = end;
        }

        pack_t DP[1001][301];

        memset(DP, 0, sizeof(DP));

        int i, j;
        for (i = n-1; i >= 0; --i) {
            for (j = 0; j <= MAXWEIGHT; ++j) {
                if (j >= values[i]) {
                    if (DP[j][i+1].val > values[i] + DP[j-values[i]][i+1].val) {
                        DP[j][i].val = DP[j][i+1].val;
                        DP[j][i].from_j = j;
                    } else {
                        DP[j][i].val = values[i] + DP[j-values[i]][i+1].val;
                        DP[j][i].from_j = j-values[i];
                    }
                } else {
                    DP[j][i].val = DP[j][i+1].val;
                    DP[j][i].from_j = j;
                }
            }
        }

        printf("%s\n", DP[sum][0].val == sum ? "sim" : "nao");
    }

    return 0;
}
