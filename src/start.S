.globl _start
_start:
    li    a0, 0
    li    a1, 0
    call  main

hang:
    j hang
