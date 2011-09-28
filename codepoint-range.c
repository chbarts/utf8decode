#include "utf8encode.h"
#include <stdio.h>
#include <stdlib.h>

static char *pname;

static unsigned long read_codepoint(char *str)
{
    char *endptr;
    unsigned long res;

    if ((res = strtoul(str, &endptr, 0)) > 4294967295U) {
        fprintf(stderr, "%s: %s is out of range\n", pname, str);
        exit(EXIT_FAILURE);
    }

    if (endptr == str) {
        fprintf(stderr, "%s: %s is invalid\n", pname, str);
        exit(EXIT_FAILURE);
    }

    return res;
}

int main(int argc, char *argv[])
{
    uint8_t buf[6];
    unsigned long low = 0, high, i;
    int len;

    if ((argc < 2) || (argc > 3)) {
        printf("usage: %s [low|high] [high]\n", argv[0]);
        return 0;
    }

    pname = argv[0];

    if (argc == 2)
        high = read_codepoint(argv[1]);
    else {                      /* argc == 3 */
        low = read_codepoint(argv[1]);
        high = read_codepoint(argv[2]);
    }

    if (!(low <= high)) {
        fprintf(stderr, "%s: invalid range [%d, %d]\n", argv[0], low,
                high);
        exit(EXIT_FAILURE);
    }

    for (i = low; i <= high; i++) {
        len = utf8encode((uint32_t) i, buf);
        fwrite(buf, 1, len, stdout);
    }

    if (fflush(stdout) == EOF) {
        perror("fflush() error");
        exit(EXIT_FAILURE);
    }

    return 0;
}
