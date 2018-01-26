#include <stdio.h>
#include <stdlib.h>
#include "utf8encode.h"
#include "utf8decode.h"

#define MAGICUC 120003
#define MAGICLC 119997

static int dofile(FILE * inf)
{
    int slen;
    uint8_t seq[6];
    uint32_t cdpt;

    while (next_sequence(inf, seq) != -1) {
        if (!valid_sequence(seq)) {
            return -2;
        }

        sequence_to_ucs4(seq, &cdpt);

        if ((cdpt >= 'A') && (cdpt <= 'Z')) {
            cdpt += MAGICUC;
        } else if ((cdpt >= 'a') && (cdpt <= 'z')) {
            cdpt += MAGICLC;
        }

        slen = utf8encode(cdpt, seq);

        fwrite(seq, 1, slen, stdout);
    }

    if (feof(inf)) {
        return 0;
    }

    return -1;
}

int main(int argc, char *argv[])
{
    int i;
    FILE *inf;

    if (argc == 1) {
        if (dofile(stdin) == -2) {
            fprintf(stderr, "%s: Bad UTF-8 on stdin!\n", argv[0]);
            exit(EXIT_FAILURE);
        } else {
            return 0;
        }
    }

    for (i = 1; i < argc; i++) {
        if ((inf = fopen(argv[i], "rb")) == NULL) {
            perror(argv[i]);
            continue;
        }

        switch (dofile(inf)) {
        case -2:
            fprintf(stderr, "%s: Bad UTF-8 in file %s\n", argv[0],
                    argv[i]);
        case -1:
            perror(argv[i]);
        default:
            fclose(inf);
        }
    }

    return 0;
}
