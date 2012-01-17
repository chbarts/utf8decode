#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

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

int parse_codepoint(const char in[], uint32_t * cp)
{
    size_t len = strlen(in);

    if (len == 0)
        return -1;

    if ((len > 2) && ((in[0] == 'U') || (in[0] == 'u')) && (in[1] == '-')
        && isxnum(in + 2, len - 2)) {
        *cp = (uint32_t) strtol(in + 2, NULL, 16);
    } else if ((len > 2) && (in[0] == '0')
               && ((in[1] == 'x') || (in[1] == 'X'))
               && isxnum(in + 2, len - 2)) {
        *cp = (uint32_t) strtol(in, NULL, 16);
    } else if (isdnum(in, len)) {
        *cp = (uint32_t) strtol(in, NULL, 0);
    } else {
        return -1;
    }

    if (errno == ERANGE)
        return -2;

    return 0;
}
