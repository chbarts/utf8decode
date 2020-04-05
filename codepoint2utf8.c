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

#include <stdio.h>
#include <stdlib.h>
#include "utf8encode.h"
#include "parse_codepoint.h"

int main(int argc, char *argv[])
{
    unsigned long int codepoint;
    unsigned char buf[6];
    int i, j, len, res;

    if (argc < 2) {
        fprintf(stderr, "usage: %s num...\n", argv[0]);
        return 0;
    }

    for (i = 1; i < argc; i++) {
        if ((res = parse_codepoint(argv[i], &codepoint)) < 0) {
            switch (res) {
            case -1:
                fprintf(stderr, "%s: invalid codepoint %s.\n", argv[0],
                        argv[i]);
                break;
            case -2:
                fprintf(stderr, "%s: codepoint %s too large.\n", argv[0],
                        argv[i]);
                break;
            }

            continue;
        }

        len = utf8encode((uint32_t) codepoint, (uint8_t *) buf);

        fwrite(buf, 1, len, stdout);

        printf("\t%02x", buf[0]);

        for (j = 1; j < len; j++)
            printf(" %02x", buf[j]);

        printf("\t%08lx\n", codepoint);
    }

    return 0;
}
