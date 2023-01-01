#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utf8encode.h"
#include "utf8decode.h"

#define MAGICUC 0x1D670
#define MAGICLC 0x1D68A
#define MAGICD  0x1D7F6

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

        if ((cdpt >= ('A' + MAGICUC)) && (cdpt <= ('Z' + MAGICUC))) {
            cdpt -= MAGICUC;
        } else if ((cdpt >= ('a' + MAGICLC)) && (cdpt <= ('z' + MAGICLC))) {
            cdpt -= MAGICLC;
        } else if ((cdpt >= (0 + MAGICD)) && (cdpt <= (9 + MAGICD))) {
            cdpt -= MAGICD;
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

        if ((cdpt >= 'A') && (cdpt <= 'Z')) {
            cdpt += MAGICUC;
        } else if ((cdpt >= 'a') && (cdpt <= 'z')) {
            cdpt += MAGICLC;
        } else if ((cdpt >= '0') && (cdpt <= '9')) {
            cdpt = (cdpt - '0') + MAGICD;
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
        puts("monospace [-u|--undo] [files...]");
        puts("Convert normal text to monospace. With --undo, undoes this operation.");
        return 0;
    }

    if (!strcmp(argv[1], "--version") || !strcmp(argv[1], "-v")) {
        puts("monospace version 1.0");
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
