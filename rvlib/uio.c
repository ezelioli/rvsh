#include <stdarg.h>
#include <utypes.h>
#include <rvos.h>

static char digits[] = "0123456789abcdef";

static void putc(char c)
{
    write(&c, 1);
}

static void printint(int xx, int base, int sign)
{
    char buf[16];
    int i;
    unsigned int x;

    if(sign && (sign = xx < 0))
        x = -xx;
    else
        x = xx;

    i = 0;
    do {
        buf[i++] = digits[x % base];
    } while((x /= base) != 0);

    if(sign)
        buf[i++] = '-';

    while(--i >= 0)
        putc(buf[i]);
}

static void printptr(uint64 x)
{
    int i;
    putc('0');
    putc('x');
    for (i = 0; i < 16; i++, x <<= 4)
        putc(digits[x >> 60]);
}

static void printstr(char *s)
{
    if(s == 0)
        s = "(null)";
    for(; *s; s++)
        putc(*s);
}

void printf(char *fmt, ...)
{
    va_list ap;
    int i, c;

    if (fmt == 0)
        return;

    va_start(ap, fmt);
    for(i = 0; (c = fmt[i] & 0xff) != 0; i++){
        if(c != '%'){
            putc(c);
            if(c == '\n')
                putc('\r');
            continue;
        }
        c = fmt[++i] & 0xff;
        if(c == 0)
            break;
        switch(c){
            case 'd':
                printint(va_arg(ap, int), 10, 1);
                break;
            case 'x':
                printint(va_arg(ap, int), 16, 0);
                break;
            case 'p':
                printptr(va_arg(ap, uint64));
                break;
            case 's':
                printstr(va_arg(ap, char*));
                break;
            case '%':
                putc('%');
                break;
            default:
                // Print unknown % sequence to draw attention.
                putc('%');
                putc(c);
                break;
        }
    }
    va_end(ap);
}
