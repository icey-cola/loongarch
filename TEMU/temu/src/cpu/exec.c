#include "helper.h"
#include "all-instr.h"
#include "monitor.h"

typedef void (*op_fun)(uint32_t);
static make_helper(_2byte_esc);

Operands ops_decoded;
uint32_t instr;
	

/* TODO: Add more instructions!!! */

op_fun opcode_table [64] = {
/* 0x00 */	_2byte_esc, inv, inv, inv,
/* 0x04 */	inv, inv, inv, inv,
/* 0x08 */	inv, inv, inv, inv,
/* 0x0c */	andi,ori, xori, lui,
/* 0x10 */	inv, inv, temu_trap, inv,
/* 0x14 */	inv, inv, inv, inv,
/* 0x18 */	inv, inv, inv, inv,
/* 0x1c */	inv, inv, inv, inv,
/* 0x20 */	inv, inv, inv, inv,
/* 0x24 */	inv, inv, inv, inv,
/* 0x28 */	inv, inv, inv, inv,
/* 0x2c */	inv, inv, inv, inv,
/* 0x30 */	inv, inv, inv, inv,
/* 0x34 */	inv, inv, inv, inv,
/* 0x38 */	inv, inv, inv, inv,
/* 0x3c */	inv, inv, inv, inv
};

op_fun _2byte_opcode_table [64] = {
/* 0x00 */	sll, inv, srl, sra, 
/* 0x04 */	sllv, inv, srlv, srav, 
/* 0x08 */	inv, inv, inv, inv, 
/* 0x0c */	inv, inv, inv, inv, 
/* 0x10 */	inv, inv, inv, inv, 
/* 0x14 */	inv, inv, inv, inv, 
/* 0x18 */	inv, inv, inv, inv, 
/* 0x1c */	inv, inv, inv, inv, 
/* 0x20 */	inv, inv, inv, inv, 
/* 0x24 */	and, or, xor, nor,
/* 0x28 */	inv, inv, inv, inv, 
/* 0x2c */	inv, inv, inv, inv, 
/* 0x30 */	inv, inv, inv, inv, 
/* 0x34 */	inv, inv, inv, inv,
/* 0x38 */	inv, inv, inv, inv, 
/* 0x3c */	inv, inv, inv, inv
};

make_helper(exec) {
	// 判断地址错误例外
	if(pc & 0x3) {
		if(cpu.cp0.status.EXL == 0){
			cpu.cp0.cause.ExcCode = AdEL;
			cpu.cp0.EPC = pc;
			pc = TRAP_ADDR;
			cpu.cp0.status.EXL = 1;
		}
		return;
	}
	if(pc == 0x1FC00380 && cpu.cp0.cause.ExcCode != 0) {
		// trap_handler(pc);   //处理异常
		return;
	}


	instr = instr_fetch(pc, 4);
	ops_decoded.opcode = instr >> 26;
	opcode_table[ ops_decoded.opcode ](pc);

	/*----------Gloden_trace----------*/
	if(temu_state != END){
	int Rd_num = op_dest->reg;
	//根据测试更新
	if(Rd_num==-1)
	Trace("%08x		%02d(%s)		%08x\n", cpu.pc, -1, "$HI", cpu.hi);
	else if(Rd_num==-2)
	Trace("%08x		%02d(%s)		%08x\n", cpu.pc, -1, "$LO", cpu.lo);
	else if(Rd_num==-3)
	Trace("%08x		%08x(%s)		%08x(%s)\n", cpu.pc, cpu.hi, "$HI", cpu.lo, "$LO");
	else
	Trace("%08x		%02d(%s)		%08x\n", cpu.pc, Rd_num, REG_NAME(Rd_num), reg_w(Rd_num));
	// 处理hilo寄存器
	// Trace(cpu.pc, Rd_num,regfile[Rd_num], cpu.gpr[Rd_num]._32);
	// Trace(cpu.pc, Rd_num,regfile[Rd_num], cpu.gpr[Rd_num]._32);
	/*-------------------------------*/
	}
}

static make_helper(_2byte_esc) {
	ops_decoded.func = instr & FUNC_MASK;
	_2byte_opcode_table[ops_decoded.func](pc); 
}
