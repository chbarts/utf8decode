#include "parse_codepoint.h"
#include "utf8encode.h"
#include <stdio.h>
#include <stdlib.h>

static char *pname;

static void get_codepoint(const char in[], unsigned long *cp)
{
   int res;

   if ((res = parse_codepoint(in, cp)) < 0) {
      switch (res) {
      case -1:
         fprintf(stderr, "%s: invalid codepoint %s\n", pname, in);
         exit(EXIT_FAILURE);
      case -2:
         fprintf(stderr, "%s: codepoint %s out of range\n", pname, in);
         exit(EXIT_FAILURE);
      }
   }
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

    if (argc == 2) {
       get_codepoint(argv[1], &high);
    } else {                      /* argc == 3 */
       get_codepoint(argv[1], &low);
       get_codepoint(argv[2], &high);
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
