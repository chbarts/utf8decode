#include <inttypes.h>
/* http://ln.hixie.ch/?start=1064324988&order=-1&count=1 
 *
 * http://en.wikipedia.org/wiki/UTF-8
 *
 * It's stupid, but it's stupid in the inspired way that
 * makes me think it's *meant* to be this stupid.
 */

/* Encode 32-bit codepoint into bytes in the buffer, 0 byte first.
 * Return number of bytes used, maximum of 6.
 */
int utf8encode(uint32_t cdpt, uint8_t * buf)
{
    if (cdpt <= 0x7f) {
        buf[0] = 0x7f & cdpt;
        return 1;
    } else if ((0x80 <= cdpt) && (cdpt <= 0x7ff)) {
        buf[0] = 0xc0 | (0x1f & (cdpt >> 6));
        buf[1] = 0x80 | (0x3f & cdpt);
        return 2;
    } else if ((0x800 <= cdpt) && (cdpt <= 0xffff)) {
        buf[0] = 0xe0 | (0xf & (cdpt >> 12));
        buf[1] = 0x80 | (0x3f & (cdpt >> 6));
        buf[2] = 0x80 | (0x3f & cdpt);
        return 3;
    } else if ((0x10000 <= cdpt) && (cdpt <= 0x1fffff)) {
        /* The parts of this above 0x10ffff take us far above the highest
         * astral planes currently defined in Unicode. At that point, this
         * is either non-standard, or a non-ASCII ASCII-armoring scheme.
         *
         * I prefer to think of it as an easter egg. 
         */
        buf[0] = 0xf0 | (0x7 & (cdpt >> 18));
        buf[1] = 0x80 | (0x3f & (cdpt >> 12));
        buf[2] = 0x80 | (0x3f & (cdpt >> 6));
        buf[3] = 0x80 | (0x3f & cdpt);
        return 4;
    } else if ((0x200000 <= cdpt) && (cdpt <= 0x3ffffff)) {
        buf[0] = 0xf8 | (0x3 & (cdpt >> 24));
        buf[1] = 0x80 | (0x3f & (cdpt >> 18));
        buf[2] = 0x80 | (0x3f & (cdpt >> 12));
        buf[3] = 0x80 | (0x3f & (cdpt >> 6));
        buf[4] = 0x80 | (0x3f & cdpt);
        return 5;
    } else {
        buf[0] = 0xfc | (0x1 & (cdpt >> 30));
        buf[1] = 0x80 | (0x3f & (cdpt >> 24));
        buf[2] = 0x80 | (0x3f & (cdpt >> 18));
        buf[3] = 0x80 | (0x3f & (cdpt >> 12));
        buf[4] = 0x80 | (0x3f & (cdpt >> 6));
        buf[5] = 0x80 | (0x3f & cdpt);
        return 6;
    }
}
