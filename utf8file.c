#include "handle_ferr.h"
#include "utf8decode.h"
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static void dofile(FILE * fin, char fname[])
{
    unsigned char seq[7];
    uint32_t cdpt;
    size_t n = 0;
    int k, l;

    memset(seq, '\0', sizeof(seq) * sizeof(seq[0]));

    puts(fname);

    while (next_sequence(fin, seq) != -1) {
        l = seqlen(seq[0]);
        sequence_to_ucs4(seq, &cdpt);
        printf("%llu: %d:", (unsigned long long int) n, l);

        for (k = 0; k < l; k++)
            printf(" %02x", (unsigned char) seq[k]);

        printf(": %08x", (unsigned int) cdpt);
        printf(": %s\n", seq);

        memset(seq, '\0', sizeof(seq) * sizeof(seq[0]));
        n += l;
    }

    if (!valid_sequence(seq) && !feof(fin))
        fprintf(stderr, "invalid UTF-8 sequence at %llu in %s\n",
                (unsigned long long int) n, fname);
}

int main(int argc, char *argv[])
{
    FILE *fin;
    int i;

    if (argc == 1) {
        dofile(stdin, "stdin");

        if (ferror(stdin))
            handle_ferr("stdin", "utf8file");

        return 0;
    }

    for (i = 1; i < argc; i++) {
        if ((fin = fopen(argv[i], "rb")) == NULL) {
            handle_ferr(argv[i], "utf8file");
            continue;
        }

        dofile(fin, argv[i]);

        if (ferror(fin))
            handle_ferr(argv[i], "utf8file");

        fclose(fin);
    }

    return 0;
}
