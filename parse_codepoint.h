#ifndef PARSE_CODEPOINT_H
#define PARSE_CODEPOINT_H

/* Verify that in points to a codepoint, and, if so, store it in cp and return 0.
 * Otherwise, return -1 on a malformed codepoint, and -2 on a codepoint that's too large.
 */
int parse_codepoint(const char in[], unsigned long *cp);

#endif                          /* PARSE_CODEPOINT_H */
