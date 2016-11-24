int checked_sum(int lhs, int rhs) {
    if (lhs == INT_MAX || rhs == INT_MAX) {
        return INT_MAX;
    }
    return lhs + rhs;
}

int badness(int distances[501], int i, int j) {
    return distances[j] - distances[i];
}

int dp(int distances[501], int N, int K)
{
    int data[N+1][K+1];
    int parent[N+1];
    parent[N] = N;

    for (int i = 0; i <= K; ++i) {
        data[N][i] = 0;
    }

    for (int i = 0; i <= N; ++i) {
        data[i][K] = 0;
    }

    for (int i = N-1; i >= 0; --i) {
        for (int j = K-1; j >= 0; --j) {
            int argmin = 0;
            int min = INT_MAX;
            for (int k = i+1; k < N+1; ++k) {
                int cost = checked_sum(badness(distances, i, k), data[k][j+1]);
                if (cost < min) {
                    min = cost;
                    argmin = k;
                }
            }
            data[i][j] = min;
            parent[i] = argmin;
        }
    }

    int max = 0;
    int i = 0;
    while (parent[i] != i) {
        if (parent[i] - i > max) {
            max = parent[i] - i;
        }
        printf("%d ", i);
        i = parent[i];
    }
    printf("\n");

    return max;
}
