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


#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>

/* Determine the total length of the UTF-8 sequence given the first
 * byte in the sequence. Returns -1 if the byte cannot validly begin a
 * UTF-8 sequence.
 */
int seqlen(unsigned char c)
{
    if ((c & 0xfc) == 0xfc) {
        return 6;
    } else if ((c & 0xf8) == 0xf8) {
        return 5;
    } else if ((c & 0xf0) == 0xf0) {
        return 4;
    } else if ((c & 0xe0) == 0xe0) {
        return 3;
    } else if ((c & 0xc0) == 0xc0) {
        return 2;
    } else if ((c & 0x80) == 0) {
        return 1;
    } else
        return -1;
}

/* Grab the next full UTF-8 sequence off the filehandle. seq[] must be
 * (at least) six bytes; this is NOT CHECKED. (If you shoot your eye
 * out, don't come running to me.)
 *
 * Returns 0 on valid UTF-8, -1 on invalid (including a sequence
 * truncated by EOF), and -1 on EOF.
 *
 * Regardless of retval, the bytes read will be in seq[], up to and
 * including the point we realize it's invalid, except it will not
 * include EOF. This will never overrun a seq[] array that is six
 * bytes long.
 */
int next_sequence(FILE * fd, unsigned char seq[])
{
    int i, n;

    if ((seq[0] = getc(fd)) == EOF) {
        seq[0] = 0;
        return -1;
    }

    if ((n = seqlen(seq[0])) == -1)
        return -1;

    if (n == 1)
        return 0;

    for (i = 1; i < n; i++) {
        if ((seq[i] = getc(fd)) == EOF) {
            seq[i] = 0;
            return -1;
        }

        if ((seq[i] & 0xc0) != 0x80)
            return -1;
    }

    return 0;
}

/* Return 1 if seq[] is valid UTF-8, 0 if it isn't.
 */
int valid_sequence(unsigned char seq[])
{
    int n, i;

    if ((n = seqlen(seq[0])) == -1)
        return 0;

    if (n == 1)
        return 1;

    for (i = 1; i < n; i++) {
        if ((seq[i] & 0xc0) != 0x80)
            return 0;
    }

    return 1;
}

/* Convert a UTF-8 sequence to a UCS-4 character (essentially
 * equivalent to a Unicode code point) in the native byte
 * ordering. Use htonl() and friends in <arpa/inet.h> to switch the
 * endianness around.
 *
 * Returns 1 if the sequence is valid, 0 if it isn't. If the sequence
 * is invalid, the value of cdpt is undefined.
 */
int sequence_to_ucs4(unsigned char seq[], uint32_t * cdpt)
{
    int i, n;

    if (!valid_sequence(seq))
        return 0;

    if (!cdpt)
        return 0;

    *cdpt = 0;

    switch (n = seqlen(seq[0])) {
    case 1:
        *cdpt = seq[0];
        return 1;
    case 2:
        *cdpt = seq[0] & 0x1f;
        break;
    case 3:
        *cdpt = seq[0] & 0x0f;
        break;
    case 4:
        *cdpt = seq[0] & 0x07;
        break;
    case 5:
        *cdpt = seq[0] & 0x03;
        break;
    case 6:
        *cdpt = seq[0] & 0x01;
        break;
    default:
        return 0;
    }

    for (i = 1; i < n; i++) {
        *cdpt <<= 6;
        *cdpt |= seq[i] & 0x3f;
    }

    return 1;
}

/* Find the end of the sequence beginning at beg in the nul-terminated
 * string pointed to by str[].
 *
 * Returns the position just past the end of the next sequence, or 0 if
 * anything was hinky. EXCEPT: Will never point past a nul.
 */
size_t find_seq_end(unsigned char str[], size_t beg)
{
    if (!str)
        return 0;

    if (str[beg] == '\0')
        return beg;

    if (!valid_sequence(str + beg))
        return 0;

    return beg + seqlen(str[beg]);
}
