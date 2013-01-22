#include "parse_codepoint.h"
#include "utf8encode.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    uint8_t buf[6];
    unsigned long cp;
    int len, i, res;

    for (i = 1; i < argc; i++) {
        if ((res = parse_codepoint(argv[i], &cp)) < 0) {
            switch (res) {
            case -1:
                fprintf(stderr, "%s: invalid codepoint %s\n", argv[0],
                        argv[i]);
                break;
            case -2:
                fprintf(stderr, "%s: codepoint %s out of range\n", argv[0],
                        argv[i]);
                break;
            }

            continue;
        }

        len = utf8encode((uint32_t) cp, buf);
        fwrite(buf, 1, len, stdout);
    }

    if (fflush(stdout) == EOF) {
        perror("fflush() error");
        exit(EXIT_FAILURE);
    }

    return 0;
}
