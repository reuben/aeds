#include <stdio.h>
#include <ctype.h>

// 0000 0011 1111 1111 1111 1111 1111 1111
//    0    3    F    F    F    F    F    F
#define TWENTY_SIX_LOWER_BITS_SET 0x03FFFFFF

int main(void) {
    FILE* fin = stdin; //fopen("input", "r");
    
    int n;
    fscanf(fin, "%d\n", &n);

    char line[10001];
    int i;
    for (i = 0; i < n; ++i) {
        fgets(line, 10001, fin);

        int chars = 0;
        char* p = line;
        while (*p && chars < TWENTY_SIX_LOWER_BITS_SET) {
            if (!isalpha(*p)) {
                p++;
                continue;
            }

            int idx = *p - 'a';
            // assert(idx <= sizeof(chars)*8);
            chars |= 1 << idx;
            p++;
        }

        if (chars >= TWENTY_SIX_LOWER_BITS_SET) {
            printf("frase completa\n");
        } else {
            int bits = 0;
            while (chars) {
                bits += chars & 1;
                chars >>= 1;
            }
            if (bits >= 13) {
                printf("frase quase completa\n");
            } else {
                printf("frase mal elaborada\n");
            }
        }
    }

    return 0;
}
