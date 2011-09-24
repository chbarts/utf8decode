#ifndef UTF8ENCODE_H
#define UTF8ENCODE_H
#include <inttypes.h>

/* Encode 32-bit codepoint into bytes in the buffer, 0 byte first.
 * Return number of bytes used, maximum of 6.
 */
int utf8encode(uint32_t cdpt, uint8_t * buf);

#endif                          /* UTF8ENCODE_H */
