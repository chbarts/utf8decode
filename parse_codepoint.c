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
#include <ctype.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

static bool isxnum(const char in[], size_t len)
{
    size_t i;

    for (i = 0; i < len; i++) {
        if (!isxdigit(in[i])) {
            return false;
        }
    }

    return true;
}

static bool isdnum(const char in[], size_t len)
{
    size_t i;

    for (i = 0; i < len; i++) {
        if (!isdigit(in[i])) {
            return false;
        }
    }

    return true;
}

int parse_codepoint(const char in[], unsigned long *cp)
{
    size_t len = strlen(in);

    if (len == 0)
        return -1;

    if ((len > 2) && ((in[0] == 'U') || (in[0] == 'u'))
        && ((in[1] == '+') || (in[1] == '-'))
        && isxnum(in + 2, len - 2)) {
        *cp = strtoul(in + 2, NULL, 16);
    } else if ((len > 2) && (in[0] == '0')
               && ((in[1] == 'x') || (in[1] == 'X'))
               && isxnum(in + 2, len - 2)) {
        *cp = strtoul(in, NULL, 16);
    } else if (isdnum(in, len)) {
        *cp = strtoul(in, NULL, 0);
    } else {
        return -1;
    }

    if (*cp > 4294967295UL)
        return -2;

    return 0;
}
