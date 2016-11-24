#include <stdio.h>
#include <string.h>

#define MAX(a,b) ((a)>(b)?(a):(b))
#define MAXWEIGHT 50

typedef struct {
    int val;
    int from_j;
} pack_t;

int main(void) {
    int tests;
    scanf("%d\n", &tests);

    int quantity[100];
    int weight[100];
    pack_t DP[51][101];

    while (tests--) {
        int packages;
        scanf("%d\n", &packages);

        int i;
        for (i = 0; i < packages; ++i) {
            scanf("%d %d\n", &quantity[i], &weight[i]);
        }

        memset(DP, 0, sizeof(DP));

        int j;
        for (i = packages-1; i >= 0; --i) {
            for (j = 0; j <= MAXWEIGHT; ++j) {
                if (j >= weight[i]) {
                    if (DP[j][i+1].val > quantity[i] + DP[j-weight[i]][i+1].val) {
                        DP[j][i].val = DP[j][i+1].val;
                        DP[j][i].from_j = j;
                    } else {
                        DP[j][i].val = quantity[i] + DP[j-weight[i]][i+1].val;
                        DP[j][i].from_j = j-weight[i];
                    }
                } else {
                    DP[j][i].val = DP[j][i+1].val;
                    DP[j][i].from_j = j;
                }
            }
        }

        int missingPackages = 0;
        int totalWeight = 0;
        
        j = MAXWEIGHT;
        for (i = 0; i < packages; ++i) {
            if (DP[j][i].from_j == j) {
                missingPackages++;
            } else {
                totalWeight += weight[i];
            }
            j = DP[j][i].from_j;
        }

#if MAXWEIGHT<6
        for (i = 0; i <= MAXWEIGHT; ++i) {
            for (j = 0; j <= packages; ++j) {
                printf("%2d (%d, %d) ", DP[i][j].val, DP[i][j].from_j, j+1);
            }
            printf("\n");
        }
#endif

        printf("%d brinquedos\nPeso: %d kg\nsobra(m) %d pacote(s)\n\n", DP[MAXWEIGHT][0].val, totalWeight, missingPackages);
    }

    return 0;
}
