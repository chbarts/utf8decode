/*

Copyright © 2017, Chris Barts.

This file is part of utf8decode.

utf8decode is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

utf8decode is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with utf8decode.  If not, see <http://www.gnu.org/licenses/>.  */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "handle_ferr.h"
#include "utf8decode.h"
#include "utf8encode.h"

static uint32_t rot(uint32_t in)
{
    uint32_t res;
    int plane;

    plane = in / 65536;
    res = (in + 32768) % 65536;
    return (res + plane * 65536);
}

static void dofile(FILE * inf, char name[])
{
    unsigned char seq[7];
    uint32_t cdpt;
    size_t n = 0;
    size_t l = 0;
    int len;

    memset(seq, '\0', sizeof(seq) * sizeof(seq[0]));

    while (next_sequence(inf, seq) != -1) {
        n += seqlen(seq[0]);
        sequence_to_ucs4(seq, &cdpt);
        if ('\n' == cdpt)
            l++;
        cdpt = rot(cdpt);
        len = utf8encode(cdpt, (uint8_t *) seq);
        fwrite(seq, 1, len, stdout);
    }

    if (!valid_sequence(seq) && !feof(inf))
        fprintf(stderr,
                "rot32768: invalid UTF-8 sequence at line %llu (byte %llu) in %s\n",
                (unsigned long long int) l, (unsigned long long int) n,
                name);
}

int main(int argc, char *argv[])
{
    FILE *inf;
    int i = 1;

    if (1 == argc) {
        dofile(stdin, "stdin");
        return 0;
    }

    if (!strcmp(argv[1], "--")) {
        i = 2;
        goto loop;
    }

    if (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h")) {
        printf("usage: %s [files...]\n", argv[0]);
        puts("Rotates Unicode codepoints (UTF-8 encoded only) such that they stay within their plane.");
        puts("Self-reversible, the same way rot13 is");
        return 0;
    }

    if (!strcmp(argv[1], "--version") || !strcmp(argv[1], "-v")) {
        puts("rot32768 version 1.0");
        puts("Chris Barts, 2017. Released under the GPL Version 3.0 or later");
        return 0;
    }

  loop:
    for (; i < argc; i++) {
        if ((inf = fopen(argv[i], "rb")) == NULL) {
            handle_ferr(argv[i], "rot32768");
            continue;
        }

        dofile(inf, argv[i]);

        if (ferror(inf)) {
            handle_ferr(argv[i], "rot32768");
            fclose(inf);
            continue;
        }

        if (fclose(inf) == EOF)
            handle_ferr(argv[i], "rot32768");
    }

    return 0;
}
