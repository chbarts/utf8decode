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
#include <limits.h>

void handle_ferr(char *fname, char *pname)
{
#define MSG "%s: error on %s"
    char buf[2 * PATH_MAX + sizeof(MSG) + 1];

    snprintf(buf, sizeof(buf), MSG, pname, fname);
    perror(buf);
}
