#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utf8encode.h"
#include "utf8decode.h"

static uint32_t offset = 0;

static int undofile(FILE * inf)
{
    int slen;
    uint8_t seq[6];
    uint32_t cdpt;

    while (next_sequence(inf, seq) != -1) {
        if (!valid_sequence(seq)) {
            return -2;
        }

        sequence_to_ucs4(seq, &cdpt);

        cdpt -= offset;

        slen = utf8encode(cdpt, seq);

        fwrite(seq, 1, slen, stdout);
    }

    if (feof(inf)) {
        return 0;
    }

    return -1;
}

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

        cdpt += offset;

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
    int i = 1, u = 0, r;
    FILE *inf;

    if ((1 == argc) || !strcmp(argv[i], "--help")
        || !strcmp(argv[i], "-h")) {
        puts("utf8char-add-offset [-u|--undo] offset [files...]");
        puts("Add an offset to the codepoints of the UTF-8-encoded codepoints in the files. With --undo, subtracts instead.");
        return 0;
    }

    if (!strcmp(argv[i], "--version") || !strcmp(argv[i], "-v")) {
        puts("utf8char-add-offset version 1.0");
        return 0;
    }

    if (!strcmp(argv[i], "--undo") || !strcmp(argv[i], "-u")) {
        i++;
        u = 1;
    }

    if ((offset = (uint32_t) strtoul(argv[i], NULL, 0)) == 0) {
        fprintf(stderr, "%s: %s is not a valid number, or is 0.\n",
                argv[0], argv[i]);
        exit(EXIT_FAILURE);
    } else {
        i++;
    }

    if (argc == i) {
        if (0 == u) {
            r = dofile(stdin);
        } else {
            r = undofile(stdin);
        }

        if (-2 == r) {
            fprintf(stderr, "%s: Bad UTF-8 on stdin!\n", argv[0]);
            exit(EXIT_FAILURE);
        } else {
            return 0;
        }
    }

    for (; i < argc; i++) {
        if ((inf = fopen(argv[i], "rb")) == NULL) {
            perror(argv[i]);
            continue;
        }

        if (0 == u) {
            r = dofile(inf);
        } else {
            r = undofile(inf);
        }

        switch (r) {
        case -2:
            fprintf(stderr, "%s: Bad UTF-8 in file %s\n", argv[0],
                    argv[i]);
            fclose(inf);
            break;
        case -1:
            perror(argv[i]);
        default:
            fclose(inf);
        }
    }

    return 0;
}
