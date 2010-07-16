#include "utf8decode.h"
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    uint32_t cdpt;
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

            if (!sequence_to_ucs4(argv[i] + n, &cdpt)) {
                fprintf(stderr, "sequence_to_ucs4() failed\n");
                exit(EXIT_FAILURE);
            }

            printf("%llu: %d:", (unsigned long long int) n, l);

            for (k = n; k < m; k++)
                printf(" %02x", (unsigned char) argv[i][k]);

            printf(": %08x", (unsigned int) cdpt);
            printf(": %s\n", argv[i] + n);

            puts("-");

            n = m;
        }

      next:
        puts("%");
    }

    return 0;
}
