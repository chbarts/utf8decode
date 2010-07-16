#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef UTF8DECODE
#define UTF8DECODE

/* Determine the total length of the UTF-8 sequence given the first
 * byte in the sequence. Returns -1 if the byte cannot validly begin a
 * UTF-8 sequence.
 */
int seqlen(unsigned char c);

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
int next_sequence(FILE * fd, unsigned char seq[]);

/* Return 1 if seq[] is valid UTF-8, 0 if it isn't.
 */
int valid_sequence(unsigned char seq[]);

/* Convert a UTF-8 sequence to a UCS-4 character (essentially
 * equivalent to a Unicode code point) in the native byte
 * ordering. Use htonl() and friends in <arpa/inet.h> to switch the
 * endianness around.
 *
 * Returns 1 if the sequence is valid, 0 if it isn't. If the sequence
 * is invalid, the value of cdpt is undefined.
 */
int sequence_to_ucs4(unsigned char seq[], uint32_t * cdpt);

/* Find the end of the sequence beginning at beg in the nul-terminated
 * string pointed to by str[].
 *
 * Returns the position just past the end of the next sequence, or 0 if
 * anything was hinky. EXCEPT: Will never point past a nul.
 */
size_t find_seq_end(unsigned char str[], size_t beg);

#endif  /* UTF8DECODE */
