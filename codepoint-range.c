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

static char *pname;

static void get_codepoint(const char in[], unsigned long *cp)
{
   int res;

   if ((res = parse_codepoint(in, cp)) < 0) {
      switch (res) {
      case -1:
         fprintf(stderr, "%s: invalid codepoint %s\n", pname, in);
         exit(EXIT_FAILURE);
      case -2:
         fprintf(stderr, "%s: codepoint %s out of range\n", pname, in);
         exit(EXIT_FAILURE);
      }
   }
}

int main(int argc, char *argv[])
{
    uint8_t buf[6];
    unsigned long low = 0, high, i;
    int len;

    if ((argc < 2) || (argc > 3)) {
        printf("usage: %s [low|high] [high]\n", argv[0]);
        return 0;
    }

    pname = argv[0];

    if (argc == 2) {
       get_codepoint(argv[1], &high);
    } else {                      /* argc == 3 */
       get_codepoint(argv[1], &low);
       get_codepoint(argv[2], &high);
    }

    if (!(low <= high)) {
        fprintf(stderr, "%s: invalid range [%lu, %lu]\n", argv[0], low,
                high);
        exit(EXIT_FAILURE);
    }

    for (i = low; i <= high; i++) {
        len = utf8encode((uint32_t) i, buf);
        fwrite(buf, 1, len, stdout);
    }

    if (fflush(stdout) == EOF) {
        perror("fflush() error");
        exit(EXIT_FAILURE);
    }

    return 0;
}
