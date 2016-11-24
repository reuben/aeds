#include <stdio.h>

int fib[39] = {0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987,
               1597, 2584, 4181, 6765, 10946, 17711, 28657, 46368, 75025,
               121393, 196418, 317811, 514229, 832040, 1346269, 2178309,
               3524578, 5702887, 9227465, 14930352, 24157817, 39088169};

int fibcalls_memo[39];

int fibcalls(int n) {
    if (fibcalls_memo[n] >= 0) {
        return fibcalls_memo[n];
    }
    if (n < 2) {
        return 0;
    }
    fibcalls_memo[n] = 2 + fibcalls(n-1) + fibcalls(n-2);
    return fibcalls_memo[n];
}

int main(void) {
    int tests;
    scanf("%d\n", &tests);

    int i;
    for (i = 2; i < 39; ++i) {
        fibcalls_memo[i] = -1;
    }
    fibcalls_memo[0] = fibcalls_memo[1] = 0;

    while (tests--) {
        int n;
        scanf("%d\n", &n);

        printf("fib(%d) = %d calls = %d\n", n, fibcalls(n), fib[n]);
    }

    return 0;
}
