#include <stdio.h>
#include "utf8encode.h"

int main(void)
{
    uint32_t cdpt;
    uint8_t buf[6];
    int n, i;

    for (cdpt = 0; cdpt <= 0xffff; cdpt++) {
        n = utf8encode(cdpt, buf);
        printf("%04x: %d:", cdpt, n);
        for (i = 0; i < n; i++)
            printf(" %02x", buf[i]);
        printf(": ");
        fwrite(buf, 1, n, stdout);
        puts("");
    }

    return 0;
}
