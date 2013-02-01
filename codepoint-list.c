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

#include "parse_codepoint.h"
#include "utf8encode.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    uint8_t buf[6];
    unsigned long cp;
    int len, i, res;

    for (i = 1; i < argc; i++) {
        if ((res = parse_codepoint(argv[i], &cp)) < 0) {
            switch (res) {
            case -1:
                fprintf(stderr, "%s: invalid codepoint %s\n", argv[0],
                        argv[i]);
                break;
            case -2:
                fprintf(stderr, "%s: codepoint %s out of range\n", argv[0],
                        argv[i]);
                break;
            }

            continue;
        }

        len = utf8encode((uint32_t) cp, buf);
        fwrite(buf, 1, len, stdout);
    }

    if (fflush(stdout) == EOF) {
        perror("fflush() error");
        exit(EXIT_FAILURE);
    }

    return 0;
}
