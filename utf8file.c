/*

Copyright © 2013, Chris Barts.

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
