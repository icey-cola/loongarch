#include "trap.h"
   .set noat
   .globl main
   .text
main:
   li $at, 0x10101010           #$at = 0x10101010
   li $v0, 0x01011111           #$v0 = 0x01011111
   and $t0, $at, $v0            #$t0 = 0x00001010
   andi $t1, $at, 0x00001111    #$t1 = 0x00001010
   lui $t2, 0x1010              #$t2 = 0x10100000
   nor $t3, $t2, $t0            #$t3 = 0xefefefef
   or $t4, $t2, $t0             #$t4 = 0x10101010
   ori $t5, $t2, 0x1010         #$t5 = 0x10101010
   xor $t6, $t2, $t0            #$t6 = 0x10101010
   xori $t7, $t2, 0x1010        #$t7 = 0x10101010

   HIT_GOOD_TRAP		#stop temu
