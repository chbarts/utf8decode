#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>
#include "utf8encode.h"

int main(int argc, char *argv[])
{
    unsigned long int codepoint;
    unsigned char buf[6];
    char *endptr = NULL;
    int i, j, len;

    if (argc < 2) {
        fprintf(stderr, "usage: %s num...\n", argv[0]);
        return 0;
    }

    for (i = 1; i < argc; i++) {
        if (((codepoint = strtoul(argv[i], &endptr, 0)) == ULONG_MAX)
            && (errno == ERANGE)) {
            fprintf(stderr, "%s: %s out of range\n", argv[0], argv[i]);
            continue;
        }

        if (endptr == argv[i]) {
            fprintf(stderr, "%s: %s invalid\n", argv[0], argv[i]);
            continue;
        }

        len = utf8encode((uint32_t) codepoint, (uint8_t *) buf);

        fwrite(buf, 1, len, stdout);

        printf("\t%02x", buf[0]);

        for (j = 1; j < len; j++)
            printf(" %02x", buf[j]);

        printf("\t%08x\n", codepoint);
    }

    return 0;
}
