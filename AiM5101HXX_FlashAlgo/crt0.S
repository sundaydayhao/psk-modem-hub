#include "encoding.h"

#if __riscv_xlen == 64
# define LREG ld
# define SREG sd
# define REGBYTES 8
#else
# define LREG lw
# define SREG sw
# define REGBYTES 4
#endif

.global __initial_sp

// <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:4>
// <i> Config the stack size for the flash algorithm
#define StackSize 0x400

.section .app_stack,"aw",@nobits
.align 2
Stack_Mem:
.space StackSize
__initial_sp:

// <o> Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
// <i> Config the heap size for the flash algorithm
#define HeapSize 0x200

.global __heap_start
.global __heap_end
.section .app_heap,"aw",@nobits
.align 2
Heap_Mem:
__heap_start:
.space HeapSize
__heap_end:

.section .exp_table, "aw", @progbits
    .align 6
    .globl __Vectors
    .type __Vectors, @object
__Vectors:
    .rept 48
    .long Default_Handler
    .endr

.global __main
.global handle_trap

.section ".text.init"
.globl Reset_Handler
.type Reset_Handler, %function
Reset_Handler:
    csrci mstatus, 8

#if defined(MSTATUS_FS) && defined(MSTATUS_XS)
    li t0, MSTATUS_FS | MSTATUS_XS
    csrs mstatus, t0
#endif

    li t0, 0x400000
    csrs mxstatus, t0

    la a0, Default_Handler
    ori a0, a0, 3
    csrw mtvec, a0

    la a0, __Vectors
    csrw mtvt, a0

    la sp, __initial_sp
    csrw mscratch, sp

    jal __main

1:
    j 1b

.globl __continue_label
.section ".text"
.align 2
.type __continue_label, %function
__continue_label:
    mv a0, a0
    ret

.globl __bkpt_label
.section ".text"
.align 2
.type __bkpt_label, %function
__bkpt_label:
    ebreak
    ret

.align 2
Default_Handler:
    addi sp, sp, -272

    SREG x1, 1*REGBYTES(sp)
    SREG x2, 2*REGBYTES(sp)
    SREG x3, 3*REGBYTES(sp)
    SREG x4, 4*REGBYTES(sp)
    SREG x5, 5*REGBYTES(sp)
    SREG x6, 6*REGBYTES(sp)
    SREG x7, 7*REGBYTES(sp)
    SREG x8, 8*REGBYTES(sp)
    SREG x9, 9*REGBYTES(sp)
    SREG x10, 10*REGBYTES(sp)
    SREG x11, 11*REGBYTES(sp)
    SREG x12, 12*REGBYTES(sp)
    SREG x13, 13*REGBYTES(sp)
    SREG x14, 14*REGBYTES(sp)
    SREG x15, 15*REGBYTES(sp)

    csrr a0, mcause
    csrr a1, mepc
    mv a2, sp
    jal handle_trap
    csrw mepc, a0

    li t0, MSTATUS_MPP
    csrs mstatus, t0

    LREG x1, 1*REGBYTES(sp)
    LREG x2, 2*REGBYTES(sp)
    LREG x3, 3*REGBYTES(sp)
    LREG x4, 4*REGBYTES(sp)
    LREG x5, 5*REGBYTES(sp)
    LREG x6, 6*REGBYTES(sp)
    LREG x7, 7*REGBYTES(sp)
    LREG x8, 8*REGBYTES(sp)
    LREG x9, 9*REGBYTES(sp)
    LREG x10, 10*REGBYTES(sp)
    LREG x11, 11*REGBYTES(sp)
    LREG x12, 12*REGBYTES(sp)
    LREG x13, 13*REGBYTES(sp)
    LREG x14, 14*REGBYTES(sp)
    LREG x15, 15*REGBYTES(sp)

    addi sp, sp, 272
    mret
