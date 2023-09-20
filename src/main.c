#include <utypes.h>
#include <ulib.h>
#include <uio.h>
#include <sh.h>

int64 getcmd(char *buf, int nbuf)
{
    printf(PROMPT);
    memset(buf, 0, nbuf);
    gets(buf, nbuf);
    if(buf[0] == 0) // EOF
        return -1L;
    return 0L;
}

static inline uint8 iswhitespace(char c)
{
    return (c == ' ') || (c == '\t');
}

int64 getfirst(const char *buf, char *dst, uint64 n, uint64 *idx)
{
    char c;
    uint64 i = 0;
    uint64 j = 0;

    while(iswhitespace(buf[i]))
        i++;

    while((c = buf[i++]) != '\n')
    {
        if(iswhitespace(c) || c == '\0' || c == '\r')
            break;

        if (j == n)
            return -1;

        dst[j++] = c;
    }

    *idx = i;

    return (int64) j;
}

int64 strcmp(const char *s1, const char *s2, uint64 n)
{
    for(uint64 i = 0; i < n; ++i)
    {
        if(s1[i] < s2[i])
            return -1L;
        else if (s1[i] > s2[i])
            return 1L;
    }
    return 0L;
}

void main()
{
    static char buf[100];
    static char cmd[16];

    int64 ret;

    uint64 idx;

    printf("RV Shell\n");

    while(1) {
        ret = getcmd(buf, sizeof(buf));
        if(ret >= 0) {
            // printf("\n%s\n", buf);
            memset(cmd, 0, 16);
            if(getfirst(buf, cmd, 16, &idx) < 0){
                printf("ERR: cannot parse command\n");
                continue;
            }
            if(strcmp(cmd, "echo", 5) == 0){
                printf("\n%s\n", buf + idx);
            } else if(strcmp(cmd, "uname", 6) == 0){
                printf("\nRVOS\n");
            } else if (cmd[0] == 0) {
                printf("\n");
            } else {
                printf("\nUnknown command\n");
            }
        } else {
            printf("ERR: cannot read command\n");
        }
    }
}

// void main()
// {
//     printf("Hello world\n");
//     while(1);
// }