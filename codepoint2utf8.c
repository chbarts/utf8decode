#include <stdio.h>
#include <stdlib.h>
#include "utf8encode.h"
#include "parse_codepoint.h"

int main(int argc, char *argv[])
{
    unsigned long int codepoint;
    unsigned char buf[6];
    int i, j, len, res;

    if (argc < 2) {
        fprintf(stderr, "usage: %s num...\n", argv[0]);
        return 0;
    }

    for (i = 1; i < argc; i++) {
        if ((res = parse_codepoint(argv[i], (uint32_t *) & codepoint)) < 0) {
            switch (res) {
            case -1:
                fprintf(stderr, "%s: invalid codepoint %s.\n", argv[0],
                        argv[i]);
                break;
            case -2:
                fprintf(stderr, "%s: codepoint %s too large.\n", argv[0],
                        argv[i]);
                break;
            }

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
