#include <limits.h>
#include <stdlib.h>

void enumerate(int* distances, int* planets, int n, int N, int K, int start,
               int startval, int* smallest_global_subdistance)
{
    // we have a subsequence of size K, find its largest subdistance and compare
    // with the smallest largest subdistance we've found so far.
    if (n == K) {
        int largest_local_subdistance = 0;
        int distance = 0;
        // planets are stops, we simply add subdistances between each stop
        for (int i = 0, j = 0; i < N+1; ++i) {
            distance += distances[i];
            if (planets[j] == i) {
                if (largest_local_subdistance < distance) {
                    largest_local_subdistance = distance;
                }
                distance = 0;
                ++j;
            }
        }
        if (largest_local_subdistance < distance) {
            largest_local_subdistance = distance;
        }
        if (largest_local_subdistance < *smallest_global_subdistance) {
            *smallest_global_subdistance = largest_local_subdistance;
        }
        return;
    }

    for (int i = startval; i < N; ++i) {
        planets[start] = i;
        enumerate(distances, planets, n+1, N, K, start+1, i+1,
                  smallest_global_subdistance);
    }
}

int bruteforce(int distances[501], int N, int K)
{
    int* planets = (int*)malloc(K * sizeof(int));
    int smallest_subdistance = INT_MAX;
    enumerate(distances, planets, 0, N, K, 0, 0, &smallest_subdistance);
    return smallest_subdistance;
}
