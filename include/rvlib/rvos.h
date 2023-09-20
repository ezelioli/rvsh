#ifndef RVOS_H
#define RVOS_H

#include <utypes.h>

#define RVOS_SYSCALL_READ  1UL
#define RVOS_SYSCALL_WRITE 2UL

int64 read(void*, uint64);
int64 write(const void*, uint64);

#endif // RVOS_h