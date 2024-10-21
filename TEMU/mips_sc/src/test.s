#include "trap.h"
   .globl main
   .text
main:
   li $t0, 0x10101010
   li $v0, 0x01011111
   li $v1, 0xFFFFFFFF
   add $t1, $t0, $v0
   sub $t2, $t0, $v0
   mult $t1, $t2
   mflo $t3
   div $t1, $t2
   mflo $t4
   and $t5, $t0, $v0
   or $t6, $t0, $v0
   xor $t7, $t0, $v0
   nor $t8, $t0, $v0
   sll $t9, $v0, 4
   srl $s0, $v0, 4
   sra $s1, $v0, 4
   slt $s2, $t0, $v0
   sltu $s3, $t0, $v1
   beq $t0, $v0, equal_label
   bne $t0, $v0, not_equal_label
   j end_label

equal_label:
   li $s4, 0x12345678
   j end_label

not_equal_label:
   li $s5, 0x87654321

end_label:
   la $s6, memory_address
   lw $s7, 0($s6)
   sw $t0, 4($s6)
   mfc0 $s7, $12
   HIT_GOOD_TRAP

memory_address:
   .word 0x12345678
