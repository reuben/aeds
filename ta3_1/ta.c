#include <stdint.h>
#include <stdio.h>

int main(void) {
    int values[100000];
    int n;
    int64_t work;

    scanf("%d\n", &n);
    while (n != 0) {
        int i;
        for (i = 0; i < n; ++i) {
            scanf("%d ", &values[i]);
        }

        work = 0;
        for (i = 0; i < n; ++i) {
            work += values[i] > 0 ? values[i] : -values[i];
            values[i+1] += values[i];
        }
        printf("%lld\n", work);

        scanf("%d\n", &n);
    }

    return 0;
}
