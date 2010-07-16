#include "utf8decode.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    size_t n, m;
    int i;

    for (i = 0; i < argc; i++) {
        printf("%d\n", i);
        n = 0;

        while (argv[i][n]) {
            printf("%llu: %d\n", n, seqlen(argv[i][n]));

            if (valid_sequence(argv[i] + n))
                puts("valid");
            else
                puts("invalid");

            if ((m = find_seq_end(argv[i], n)) == 0) {
                fprintf(stderr,
                        "invalid UTF-8 sequence at %llu in %s (%02x)\n", n,
                        argv[i], (unsigned char) argv[i][n]);
                goto next;
            }

            printf("%llu: %s\n", n, argv[i] + n);
            puts("-");

            n = m;
        }

      next:
        puts("%");
    }

    return 0;
}
