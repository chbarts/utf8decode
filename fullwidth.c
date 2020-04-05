#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "utf8encode.h"
#include "utf8decode.h"
#include "handle_ferr.h"

#define MAGIC 65248
#define SPACE 0x3000
#define FWBEG ('!' + MAGIC)
#define FWEND ('~' + MAGIC)

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

        if ((cdpt >= FWBEG) && (cdpt <= FWEND)) {
            cdpt -= MAGIC;
        } else if (SPACE == cdpt) {
            cdpt = ' ';
        }

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

        if ((cdpt >= '!') && (cdpt <= '~')) {
            cdpt += MAGIC;
        } else if (' ' == cdpt) {
            cdpt = SPACE;
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
    int i = 1, u = 0, r;
    FILE *inf;

    if (1 == argc) {
        if (dofile(stdin) == -2) {
            fprintf(stderr, "%s: Bad UTF-8 on stdin!\n", argv[0]);
            exit(EXIT_FAILURE);
        } else {
            return 0;
        }
    }

    if (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h")) {
        puts("fullwidth [-u|--undo] [files...]");
        puts("Convert normal text to fullwidth. With --undo, undoes this operation.");
        return 0;
    }

    if (!strcmp(argv[1], "--version") || !strcmp(argv[1], "-v")) {
        puts("fullwidth version 1.0");
        return 0;
    }

    if (!strcmp(argv[1], "--")) {
        i = 2;
    }

    if (!strcmp(argv[1], "--undo") || !strcmp(argv[1], "-u")) {
        i = 2;
        u = 1;
    }

    if (argc == i) {
        if (u == 0) {
            r = dofile(stdin);
        } else {
            r = undofile(stdin);
        }

        if (r == -2) {
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

        if (u == 0) {
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
