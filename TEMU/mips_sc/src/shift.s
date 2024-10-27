#include "trap.h"
   .set noat
   .globl main
   .text
main:
    lui $t0, 0x8934     
    ori $t0, $t0, 0x5678  
    sll $t1, $t0, 4      
    srlv $t5, $t0, $t1   


    ; HIT_GOOD_TRAP		
