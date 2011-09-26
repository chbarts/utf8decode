#include <stdio.h>
#include "utf8encode.h"

int main(void)
{
    uint32_t cdpt;
    uint8_t buf[6];
    int n, i;

    for (cdpt = 0; cdpt <= 0xffff; cdpt++) {
        n = utf8encode(cdpt, buf);
        printf("%04x\t%d\t%02x", cdpt, n, buf[0]);
        for (i = 1; i < n; i++)
            printf(" %02x", buf[i]);
        printf("\t");
        fwrite(buf, 1, n, stdout);
        puts("");
    }

    return 0;
}
