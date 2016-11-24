#include <stdio.h>
#include <ctype.h>

int main(void) {
    FILE* fin = stdin; //fopen("input", "r");

    char line[30000];

    int i = 1;
    int m;
    fscanf(fin, "%d\n", &m);
    while (m != 0) {
        char alphabet[26];

        int j;
        for (j = 0; j < 26; ++j) {
            alphabet[j] = 'A'+j;
        }

        fgets(line, sizeof(line), fin);
        
        printf("Instancia %d\n", i++);
        char* c = line;
        while (*c) {
            if (*c == ' ') {
                c++;
                continue;
            }
            if (*c == '\n') {
                break;
            }

            int n = *c-'0';
            if (isdigit(*(c+1))) {
                n *= 10;
                n += *(c+1)-'0';
                c++;
            }

            n--;

            printf("%c", alphabet[n]);

            char saved = alphabet[n];
            for (j = n; j > 0; --j) {
                alphabet[j] = alphabet[j-1];
            }
            alphabet[0] = saved;

            c++;
        }
        printf("\n\n");

        fscanf(fin, "%d\n", &m);
    }

    return 0;
}
