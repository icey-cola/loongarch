
build/shift:     file format elf32-tradlittlemips
build/shift


Disassembly of section .text:

80000000 <main>:
80000000:	3c088934 	lui	t0,0x8934
80000004:	35085678 	ori	t0,t0,0x5678
80000008:	00084900 	sll	t1,t0,0x4
8000000c:	01286806 	srlv	t5,t0,t1
80000010:	4a000000 	c2	0x0

Disassembly of section .reginfo:

00000000 <.reginfo>:
   0:	00002300 	sll	a0,zero,0xc
	...
