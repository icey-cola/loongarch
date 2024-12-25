#include "trap.h"
#include "include/inst_test.h"
#include "include/regdef.h"    
   .set noat
   .globl main
   .text

main:
    ori   s0, zero, 0x2     # s0 = 2 
    ori   s1, zero, 0x3     # s1 = 3  
    lui   s2, 0x8050        # s2 = 0x80500000 
    addiu s3, s2, 0x80      # s3 = 0x80500080  
loop:
    addu  s4, s0, s1        # s4 = s0 + s1
    ori   s0, s1, 0x0       # s0 = s1
    ori   s1, s4, 0x0       # s1 = s4
    sw    s1, 0(s2)         
    lw    s5, 0(s2)        
    nop
    addiu s2, s2, 0x4       # s2 += 4

    bne   s2, s3, loop
    nop
    
    HIT_GOOD_TRAP           # stop temu
