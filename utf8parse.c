#include "utf8decode.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    size_t k, n, m;
    int i, l;

    for (i = 0; i < argc; i++) {
        printf("%d\n", i);
        n = 0;

        while (argv[i][n]) {
            if (!valid_sequence(argv[i] + n)) {
                fprintf(stderr,
                        "invalid UTF-8 sequence at %llu in %s (%02x)\n",
                        (unsigned long long int) n, argv[i],
                        (unsigned char) argv[i][n]);
                goto next;
            }

            l = seqlen(argv[i][n]);
            m = find_seq_end(argv[i], n);

            printf("%llu: %d:", (unsigned long long int) n, l);

            for (k = n; k < m; k++)
                printf(" %02x", (unsigned char) argv[i][k]);

            printf(": %s\n", argv[i] + n);

            puts("-");

            n = m;
        }

      next:
        puts("%");
    }

    return 0;
}
