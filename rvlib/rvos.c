#include <utypes.h>
#include <rvos.h>

static inline int64 rvos_ecall(uint64 id, uint64 a0, uint64 a1, uint64 a2, uint64 a3, uint64 a4, uint64 a5, uint64 a6)
{
    register uint64 _a0 asm("a0") = a0;
    register uint64 _a1 asm("a1") = a1;
    register uint64 _a2 asm("a2") = a2;
    register uint64 _a3 asm("a3") = a3;
    register uint64 _a4 asm("a4") = a4;
    register uint64 _a5 asm("a5") = a5;
    register uint64 _a6 asm("a6") = a6;
    register uint64 _a7 asm("a7") = id;

    asm volatile("ecall"
                 : "+r"(_a0)
                 : "r"(_a1), "r"(_a2), "r"(_a3), "r"(_a4), "r"(_a5), "r"(_a6), "r"(_a7)
                 : "memory");

    int64 ret = _a0;

    return ret;
}

int64 read(void *buf, uint64 n)
{
    return rvos_ecall(RVOS_SYSCALL_READ, (uint64) buf, n, 0, 0, 0, 0, 0);
}

int64 write(const void *buf, uint64 n)
{
    return rvos_ecall(RVOS_SYSCALL_WRITE, (uint64) buf, n, 0, 0, 0, 0, 0);
}