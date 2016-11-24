#include <stdio.h>

int repeats(int n) {
    int count[10] = {0};
    while (n) {
        if (count[n%10] == 1) {
            return 1;
        }
        count[n%10] = 1;
        n /= 10;
    }
    return 0;
}

int main(void) {
    FILE* fin = stdin; //fopen("input", "r");

    while (!feof(fin)) {
        int n, m;
        fscanf(fin, "%d %d\n", &n, &m);

        int count = 0;
        for (; n <= m; ++n) {
            if (!repeats(n)) {
                count++;
            }
        }
        printf("%d\n", count);
    }

    return 0;
}
