#include "trap.h"
   .set noat
   .globl main
   .text
main:
    lui $t0, 0x8934     # Load upper immediate value into $t0
    ori $t0, $t0, 0x5678  # OR immediate value with lower part
    sll $t1, $t0, 4      # Logical shift left $t0 by 4 bits, result in $t1
    srl $t2, $t1, 4      # Logical shift right $t0 by 4 bits, result in $t2
    sra $t3, $t1, 4      # Arithmetic shift right $t0 by 4 bits, result in $t3
    lui $t0, 0x0000
    ori $t0, $t0, 0x0008
    sllv $t4, $t0, $t1   # Logical shift left $t0 by $t1 bits, result in $t4
    srlv $t5, $t0, $t1   # Logical shift right $t0 by $t1 bits, result in $t5
    srav $t6, $t0, $t1   # Arithmetic shift right $t0 by $t1 bits, result in $t6


    ; HIT_GOOD_TRAP		 #stop temu
