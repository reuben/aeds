#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(void) {
    FILE* fin = stdin; //fopen("input", "r");
    
    int n;
    fscanf(fin, "%d\n", &n);

    char line[10001];
    int i;
    for (i = 0; i < n; ++i) {
        fgets(line, 10001, fin);

        int len = strlen(line);
        while (!isprint(line[len])) len--;

        int j;
        for (j = 0; j <= len; ++j) {
            if (isalpha(line[j])) {
                line[j] += 3;
            }
        }
        for (j = 0; j <= len/2; ++j) {
            line[j] -= 1;
        }
        for (j = len; j >= 0; --j) {
            printf("%c", line[j]);
        }

        printf("\n");
    }

    return 0;
}
