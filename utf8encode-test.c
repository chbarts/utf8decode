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
#include "utf8encode.h"

int main(void)
{
    uint32_t cdpt;
    uint8_t buf[6];
    int n, i;

    for (cdpt = 0; cdpt <= 0xffff; cdpt++) {
        n = utf8encode(cdpt, buf);
        printf("%04x\t%d\t%02x", cdpt, n, buf[0]);
        for (i = 1; i < n; i++)
            printf(" %02x", buf[i]);
        printf("\t");
        fwrite(buf, 1, n, stdout);
        puts("");
    }

    return 0;
}
