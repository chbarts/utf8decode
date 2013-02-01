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
