#include "trap.h"
   .globl main
   .text
main:
   # 加载立即数到 $t0 而不是 $at
   li $t0, 0x10101010           # $t0 = 0x10101010
   li $v0, 0x01011111           # $v0 = 0x01011111
   li $v1, 0xFFFFFFFF           # $v1 = 0xFFFFFFFF

   # 算术操作
   add $t1, $t0, $v0            # $t1 = $t0 + $v0
   sub $t2, $t0, $v0            # $t2 = $t0 - $v0
   mult $t1, $t2                # $lo = $t1 * $t2
   mflo $t3                     # $t3 = low part of mult result
   div $t1, $t2                 # $lo = $t1 / $t2
   mflo $t4                     # $t4 = quotient

   # 逻辑操作
   and $t5, $t0, $v0            # $t5 = $t0 & $v0
   or $t6, $t0, $v0             # $t6 = $t0 | $v0
   xor $t7, $t0, $v0            # $t7 = $t0 ^ $v0
   nor $t8, $t0, $v0            # $t8 = ~($t0 | $v0)

   # 移位操作
   sll $t9, $v0, 4              # $t9 = $v0 << 4
   srl $s0, $v0, 4              # $s0 = $v0 >> 4
   sra $s1, $v0, 4              # 算术右移 $s1 = $v0 >>> 4

   # 比较
   slt $s2, $t0, $v0            # 如果 $t0 < $v0, 则 $s2 = 1
   sltu $s3, $t0, $v1           # 无符号比较 $t0 < $v1，结果在 $s3

   # 分支指令
   beq $t0, $v0, equal_label    # 如果 $t0 == $v0，跳转到 equal_label
   bne $t0, $v0, not_equal_label # 如果 $t0 != $v0，跳转到 not_equal_label
   j end_label                  # 无条件跳转到 end_label

equal_label:
   li $s4, 0x12345678           # $s4 = 0x12345678 (equal case)
   j end_label

not_equal_label:
   li $s5, 0x87654321           # $s5 = 0x87654321 (not equal case)

end_label:
   # 内存加载和存储
   la $s6, memory_address       # 加载内存地址到 $s6
   lw $s7, 0($s6)               # 从 $s6 指向的内存中加载字到 $s7
   sw $t0, 4($s6)               # 将 $t0 存储到 $s6 + 4 的位置

   # 特权指令示例
   mfc0 $s7, $12                # 从协处理器0的寄存器12加载到 $s7

   HIT_GOOD_TRAP                # 触发结束

memory_address:
   .word 0x12345678             # 定义一个内存地址并初始化为 0x12345678
