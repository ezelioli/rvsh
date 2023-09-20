#include <utypes.h>
#include <rvos.h>

void* memset(void *dst, uint8 c, uint64 n)
{
    char *cdst = (char *) dst;
    for(uint64 i = 0; i < n; i++){
        cdst[i] = c;
    }
    return dst;
}

char* gets(char *buf, uint64 max)
{
    int64 cc;
    char c;

    uint64 i;
    for(i = 0; i + 1 < max; ){
        cc = read(&c, 1);
        if(cc < 1)
            break;
        buf[i++] = c;
        if(c == '\n' || c == '\r')
            break;
    }
    buf[i] = '\0';
    return buf;
}