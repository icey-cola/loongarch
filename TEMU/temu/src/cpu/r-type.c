#include "helper.h"
#include "monitor.h"
#include "reg.h"

extern uint32_t instr;
extern char assembly[80];

/* decode R-type instrucion */
static void decode_r_type(uint32_t instr) {

	op_src1->type = OP_TYPE_REG;
	op_src1->reg = (instr & RS_MASK) >> (RT_SIZE + IMM_SIZE);
	op_src1->val = reg_w(op_src1->reg);
	
	op_src2->type = OP_TYPE_REG;
	op_src2->reg = (instr & RT_MASK) >> (RD_SIZE + SHAMT_SIZE + FUNC_SIZE);
	op_src2->val = reg_w(op_src2->reg);

	op_dest->type = OP_TYPE_REG;
	op_dest->reg = (instr & RD_MASK) >> (SHAMT_SIZE + FUNC_SIZE);
}

make_helper(and) {

	decode_r_type(instr);
	reg_w(op_dest->reg) = (op_src1->val & op_src2->val);
	sprintf(assembly, "and   %s,   %s,   %s", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), REG_NAME(op_src2->reg));
}

make_helper(nor) {

	decode_r_type(instr);
	reg_w(op_dest->reg) = ~(op_src1->val | op_src2->val);
	sprintf(assembly, "nor   %s,   %s,   %s", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), REG_NAME(op_src2->reg));
}

make_helper(or) {

	decode_r_type(instr);
	reg_w(op_dest->reg) = (op_src1->val | op_src2->val);
	sprintf(assembly, "or   %s,   %s,   %s", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), REG_NAME(op_src2->reg));
}

make_helper(xor) {
	
	decode_r_type(instr);
	reg_w(op_dest->reg) = (op_src1->val ^ op_src2->val);
	sprintf(assembly, "xor   %s,   %s,   %s", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), REG_NAME(op_src2->reg));
}

make_helper(sllv) {

	decode_r_type(instr);
	reg_w(op_dest->reg) = (op_src1->val) << (op_src2->val);
	sprintf(assembly, "sllv   %s,   %s,   %s", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), REG_NAME(op_src2->reg));
}

make_helper(sll) {

	decode_r_type(instr);
	reg_w(op_dest->reg) = (op_src2->val) << ((instr & SHAMT_MASK)>>6);
	sprintf(assembly, "sll   %s,   %s,   0x%02x", REG_NAME(op_dest->reg), REG_NAME(op_src2->reg), instr & SHAMT_MASK);
}

make_helper(srav) {

	decode_r_type(instr);
	reg_w(op_dest->reg) = ((int32_t)op_src1->val >> (op_src2->val));
	sprintf(assembly, "srav   %s,   %s,   %s", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), REG_NAME(op_src2->reg));
}

make_helper(sra) {

	decode_r_type(instr);
	reg_w(op_dest->reg) = ((int32_t)op_src2->val >> ((instr & SHAMT_MASK)>>6));
	sprintf(assembly, "sra   %s,   %s,   0x%02x", REG_NAME(op_dest->reg), REG_NAME(op_src2->reg), instr & SHAMT_MASK);
}

make_helper(srlv) {

	decode_r_type(instr);
	reg_w(op_dest->reg) = (uint32_t)(op_src1->val) >> (op_src2->val);
	sprintf(assembly, "srlv   %s,   %s,   %s", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), REG_NAME(op_src2->reg));
}

make_helper(srl) {

	decode_r_type(instr);
	reg_w(op_dest->reg) = (uint32_t)(op_src2->val) >> ((instr & SHAMT_MASK)>>6);
	sprintf(assembly, "srl   %s,   %s,   0x%02x", REG_NAME(op_dest->reg), REG_NAME(op_src2->reg), instr & SHAMT_MASK);
}

make_helper(add) {

	decode_r_type(instr);
	int temp = (int)(op_src1->val) + (int)(op_src2->val);
	if (((int)op_src1->val > 0 && (int)op_src2->val > 0 && temp < 0) || ((int)op_src1->val < 0 && (int)op_src2->val < 0 && temp > 0)) {
		// TODO exception
		if (cpu.cp0.status.EXL == 0) { // 如果此时异常级为0，即还未有异常发生（不支持嵌套异常）
			cpu.cp0.cause.ExcCode = Ov; // 记录发生了溢出异常
			cpu.cp0.EPC = cpu.pc; // 存放异常返回地址（由软件实现具体的返回地址）
			cpu.pc = TRAP_ADDR; // 跳转到异常处理程序地址。ATTENTION!
			cpu.cp0.status.EXL = 1; // 记录已有异常发生
		}
	}else {
		reg_w(op_dest->reg) = temp;
	}
	sprintf(assembly, "add   %s,   %s,   %s", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), REG_NAME(op_src2->reg));
}

make_helper(addu) {

	decode_r_type(instr);
	reg_w(op_dest->reg) = op_src1->val + op_src2->val;
	sprintf(assembly, "addu   %s,   %s,   %s", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), REG_NAME(op_src2->reg));
}

make_helper(sub) {

	decode_r_type(instr);
	int temp = (int)(op_src1->val) - (int)(op_src2->val);
	if(((int)op_src1->val > 0 && (int)op_src2->val < 0 && temp < 0) || ((int)op_src1->val < 0 && (int)op_src2->val > 0 && temp > 0)) {
		// TODO exception
		if (cpu.cp0.status.EXL == 0) { // 如果此时异常级为0，即还未有异常发生（不支持嵌套异常）
			cpu.cp0.cause.ExcCode = Ov; // 记录发生了溢出异常
			cpu.cp0.EPC = cpu.pc; // 存放异常返回地址（由软件实现具体的返回地址）
			cpu.pc = TRAP_ADDR; // 跳转到异常处理程序地址
			cpu.cp0.status.EXL = 1; // 记录已有异常发生
		}
	}else {
		reg_w(op_dest->reg) = temp; // 直接用tmp可以吗
	}
	sprintf(assembly, "sub   %s,   %s,   %s", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), REG_NAME(op_src2->reg));
}

make_helper(subu) {

	decode_r_type(instr);
	reg_w(op_dest->reg) = op_src1->val - op_src2->val;
	sprintf(assembly, "subu   %s,   %s,   %s", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), REG_NAME(op_src2->reg));
}

make_helper(slt) {

	decode_r_type(instr);
	reg_w(op_dest->reg) = ((int)op_src1->val < (int)op_src2->val) ? 1 : 0;
	sprintf(assembly, "slt   %s,   %s,   %s", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), REG_NAME(op_src2->reg));
}

make_helper(sltu) {

	decode_r_type(instr);
	reg_w(op_dest->reg) = (op_src1->val < op_src2->val) ? 1 : 0;
	sprintf(assembly, "sltu   %s,   %s,   %s", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), REG_NAME(op_src2->reg));
}

make_helper(div) { // 什么div zero？

	decode_r_type(instr);
	int quotient = (int)op_src1->val / (int)op_src2->val;
	int remainder = (int)op_src1->val % (int)op_src2->val;
	cpu.lo = quotient;
	cpu.hi = remainder;
	sprintf(assembly, "div   %s,   %s", REG_NAME(op_src1->reg), REG_NAME(op_src2->reg));
}

make_helper(divu) {

	decode_r_type(instr);
	uint32_t quotient = op_src1->val / op_src2->val;
	uint32_t remainder = op_src1->val % op_src2->val;
	cpu.lo = quotient;
	cpu.hi = remainder;
	sprintf(assembly, "divu   %s,   %s", REG_NAME(op_src1->reg), REG_NAME(op_src2->reg));
}

make_helper(mult) {

	decode_r_type(instr);
	long long val1, val2; // 64位
	// 把uint32_t类型的op_src1->val和op_src2->val转化为long long int型
	if((int)op_src1->val < 0) {
		val1 = (0xFFFFFFFFL << 32) | op_src1->val; // L强制编译器把常量作为长整数来处理
	}else {
		val1 = op_src1->val;
	}
	if((int)op_src2->val < 0) {
		val2 = (0xFFFFFFFFL << 32) | op_src2->val;
	}else {
		val2 = op_src2->val;
	}
	uint64_t val = val1 * val2;
	uint32_t lo = val & 0xFFFFFFFF;
	uint32_t hi = (val >> 32) & 0xFFFFFFFF;
	cpu.lo = lo; // cpu的hi，lo为uint32_t类型
	cpu.hi = hi;
	sprintf(assembly, "mult   %s,   %s", REG_NAME(op_src1->reg), REG_NAME(op_src2->reg));
}

make_helper(multu) {

	decode_r_type(instr);
	uint64_t val = (uint64_t)op_src1->val * (uint64_t)op_src2->val;
	uint32_t lo = val & 0xFFFFFFFF;
	uint32_t hi = (val >> 32) & 0xFFFFFFFF;
	cpu.lo = lo;
	cpu.hi = hi;
	sprintf(assembly, "multu   %s,   %s", REG_NAME(op_src1->reg), REG_NAME(op_src2->reg));
}

make_helper(jr) {

    decode_r_type(instr);
    cpu.pc = op_src1->val;
    cpu.pc -= 4;
    sprintf(assembly, "jr   %s", REG_NAME(op_src1->reg));
}

make_helper(jalr) {

    decode_r_type(instr);
    reg_w(op_dest->reg) = cpu.pc + 8;
    cpu.pc = op_src1->val;
    cpu.pc -= 4; // 为了保证在cpu_exec()中的pc += 4
    sprintf(assembly, "jalr   %s", REG_NAME(op_src1->reg));
}
