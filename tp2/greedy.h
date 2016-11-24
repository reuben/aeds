int greater_than(const void* lhs, const void* rhs)
{
    int left = *(const int*)lhs;
    int right = *(const int*)rhs;

    if (left > right) return -1;
    if (left < right) return 1;
    return 0;
}

int greedy(int distances[501], int N, int K)
{
    int* sets = (int*)calloc(K+1, sizeof(int));

    int average = 0;
    for (int i = 0; i < N+1; ++i) {
        average += distances[i];
    }
    average /= (N+1);

    int current_set = 0;
    for (int i = 0; i < N+1; ++i) {
        sets[current_set] += distances[i];
        if (sets[current_set] > average && current_set < K+1) {
            current_set++;
        }
    }

    int largest_set = 0;
    for (int i = 1; i < K+1; ++i) {
        if (sets[i] > sets[largest_set]) {
            largest_set = i;
        }
    }

    return sets[largest_set];
}
