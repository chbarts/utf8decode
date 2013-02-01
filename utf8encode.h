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

#ifndef UTF8ENCODE_H
#define UTF8ENCODE_H
#include <inttypes.h>

/* Encode 32-bit codepoint into bytes in the buffer, 0 byte first.
 * Return number of bytes used, maximum of 6.
 */
int utf8encode(uint32_t cdpt, uint8_t * buf);

#endif                          /* UTF8ENCODE_H */
