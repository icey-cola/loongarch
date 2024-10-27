#include "helper.h"
#include "monitor.h"
#include "reg.h"

extern uint32_t instr;
extern char assembly[80];

/* decode I-type instrucion with unsigned immediate */
static void decode_imm_type(uint32_t instr) {

	op_src1->type = OP_TYPE_REG;
	op_src1->reg = (instr & RS_MASK) >> (RT_SIZE + IMM_SIZE);
	op_src1->val = reg_w(op_src1->reg);
	
	op_src2->type = OP_TYPE_IMM;
	op_src2->imm = instr & IMM_MASK;
	op_src2->val = op_src2->imm;

	op_dest->type = OP_TYPE_REG;
	op_dest->reg = (instr & RT_MASK) >> (IMM_SIZE);
}

make_helper(lui) {

	decode_imm_type(instr);
	reg_w(op_dest->reg) = (op_src2->val << 16);
	sprintf(assembly, "lui   %s,   0x%04x", REG_NAME(op_dest->reg), op_src2->imm);
}

make_helper(ori) {

	decode_imm_type(instr);
	reg_w(op_dest->reg) = op_src1->val | op_src2->val;
	sprintf(assembly, "ori   %s,   %s,   0x%04x", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), op_src2->imm);
}

make_helper(andi) {

	decode_imm_type(instr);
	reg_w(op_dest->reg) = op_src1->val & op_src2->val;
	sprintf(assembly, "andi   %s,   %s,   0x%04x", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), op_src2->imm);
}

make_helper(xori) {

	decode_imm_type(instr);
	reg_w(op_dest->reg) = op_src1->val ^ op_src2->val;
	sprintf(assembly, "xori   %s,   %s,   0x%04x", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), op_src2->imm);
}

make_helper(addi){

	decode_imm_type(instr);
	// sign_extend
	int imm;
	if (op_src2->val & 0x8000){
		imm = (0xFFFF << 16) | op_src2->val;
	}
	else{
		imm = op_src2->val;
	}
	int res = (int)op_src1->val + imm;
	// if overflow
	if (((int)op_src1->val > 0 && imm > 0 && res < 0) || ((int)op_src1->val < 0 && imm < 0 && res > 0)){
		// TODO: exception
		/*if (cpu.cp0.status.EXL == 0){
			cpu.cp0.cause.ExcCode = Ov;
			cpu.cp0.EPC = cpu.pc;
			cpu.pc = TRAP_ADDR;
			cpu.cp0.status.EXL = 1;
		}*/
	}
	else{
		reg_w(op_dest->reg) = res;
	}
	sprintf(assembly, "addi %s, %s, 0x%04x", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), op_src2->val);
}
make_helper(lb) {
    decode_imm_type(instr);
    int imm;
    if (op_src2->val & 0x8000) {
        imm = (0xFFFF << 16) | op_src2->val;
    } else {
        imm = op_src2->val;
    }
    uint32_t addr = imm + (uint32_t)op_src1->val;
    uint32_t val = mem_read(addr, 1);
    if (val & 0x80) {
        val = (0xFFFFFF << 8) | val;
    }
    reg_w(op_dest->reg) = val;
    sprintf(assembly, "lb  %s, 0x%08x(%s)", REG_NAME(op_dest->reg),
            op_src2->val, REG_NAME(op_src1->reg));
}

make_helper(lbu) {
    decode_imm_type(instr);
    int imm;
    if (op_src2->val & 0x8000) {
        imm = (0xFFFF << 16) | op_src2->val;
    } else {
        imm = op_src2->val;
    }
    uint32_t addr = imm + (uint32_t)op_src1->val;
    uint32_t val = mem_read(addr, 1);
    reg_w(op_dest->reg) = val;
    sprintf(assembly, "lbu  %s, 0x%08x(%s)", REG_NAME(op_dest->reg),
            op_src2->val, REG_NAME(op_src1->reg));
}

make_helper(lh) {
    decode_imm_type(instr);
    int imm;
    if (op_src2->val & 0x8000) {
        imm = (0xFFFF << 16) | op_src2->val;
    } else {
        imm = op_src2->val;
    }
    uint32_t addr = imm + (uint32_t)op_src1->val;
    if (addr & 0x1) {
        // TODO exception-地址错异常
    } else {
        uint32_t val = mem_read(addr, 2);
        if (val & 0x8000) {
            val = (0xFFFF << 16) | val;
        }
        reg_w(op_dest->reg) = val;
    }
    sprintf(assembly, "lh  %s, 0x%08x(%s)", REG_NAME(op_dest->reg),
            op_src2->val, REG_NAME(op_src1->reg));
}

make_helper(lhu) {
    decode_imm_type(instr);
    int imm;
    if (op_src2->val & 0x8000) {
        imm = (0xFFFF << 16) | op_src2->val;
    } else {
        imm = op_src2->val;
    }
    uint32_t addr = imm + (uint32_t)op_src1->val;
    if (addr & 0x1) {
        // TODO exception-地址错异常
    } else {
        uint32_t val = mem_read(addr, 2);
        reg_w(op_dest->reg) = val;
    }
    sprintf(assembly, "lhu  %s, 0x%08x(%s)", REG_NAME(op_dest->reg),
            op_src2->val, REG_NAME(op_src1->reg));
}

make_helper(lw) {
    decode_imm_type(instr);
    int imm;
    if (op_src2->val & 0x8000) {
        imm = (0xFFFF << 16) | op_src2->val;
    } else {
        imm = op_src2->val;
    }
    uint32_t addr = imm + (uint32_t)op_src1->val;
    if (addr & 0x3) {
        // TODO exception-地址错异常
    } else {
        uint32_t val = mem_read(addr, 4);
        reg_w(op_dest->reg) = val;
    }
    sprintf(assembly, "lw  %s, 0x%08x(%s)", REG_NAME(op_dest->reg),
            op_src2->val, REG_NAME(op_src1->reg));
}

make_helper(sb) {
    decode_imm_type(instr);
    int imm;
    if (op_src2->val & 0x8000) {
        imm = (0xFFFF << 16) | op_src2->val;
    } else {
        imm = op_src2->val;
    }
    uint32_t addr = imm + (uint32_t)op_src1->val;
    mem_write(addr, 1, reg_w(op_dest->reg));
    sprintf(assembly, "sb  %s, 0x%08x(%s)", REG_NAME(op_dest->reg),
            op_src2->val, REG_NAME(op_src1->reg));
}

make_helper(sh) {
    decode_imm_type(instr);
    int imm;
    if (op_src2->val & 0x8000) {
        imm = (0xFFFF << 16) | op_src2->val;
    } else {
        imm = op_src2->val;
    }
    uint32_t addr = imm + (uint32_t)op_src1->val;
    if (addr & 0x1) {
        // TODO exception-地址错异常
    } else {
        mem_write(addr, 2, reg_w(op_dest->reg));
    }
    sprintf(assembly, "sh  %s, 0x%08x(%s)", REG_NAME(op_dest->reg),
            op_src2->val, REG_NAME(op_src1->reg));
}

make_helper(sw) {
    decode_imm_type(instr);
    int imm;
    if (op_src2->val & 0x8000) {
        imm = (0xFFFF << 16) | op_src2->val;
    } else {
        imm = op_src2->val;
    }
    uint32_t addr = imm + (uint32_t)op_src1->val;
    if (addr & 0x3) {
        // TODO exception-地址错异常
    } else {
        mem_write(addr, 4, reg_w(op_dest->reg));
    }
    sprintf(assembly, "sw  %s, 0x%08x(%s)", REG_NAME(op_dest->reg),
            op_src2->val, REG_NAME(op_src1->reg));}
make_helper(addiu)
{
	decode_imm_type(instr);
	// sign_extend
	int imm;
	if (op_src2->val & 0x8000)
	{
		imm = (0xFFFF << 16) | op_src2->val;
	}
	else
	{
		imm = op_src2->val;
	}
	reg_w(op_dest->reg) = imm + op_src1->val;
	sprintf(assembly, "addiu %s, %s, 0x%04x", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), op_src2->val);
}

make_helper(slti)
{
	decode_imm_type(instr);
	// sign_extend
	int imm;
	if (op_src2->val & 0x8000)
	{
		imm = (0xFFFF << 16) | op_src2->val;
	}
	else
	{
		imm = op_src2->val;
	}
	reg_w(op_dest->reg) = ((int)op_src1->val < imm) ? 1 : 0;
	sprintf(assembly, "slti %s, %s, 0x%04x", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), op_src2->val);
}

make_helper(sltiu)
{
	decode_imm_type(instr);
	// sign_extend
	int imm;
	if (op_src2->val & 0x8000)
	{
		imm = (0xFFFF << 16) | op_src2->val;
	}
	else
	{
		imm = op_src2->val;
	}
	reg_w(op_dest->reg) = (op_src1->val < imm) ? 1 : 0;
	sprintf(assembly, "sltiu %s, %s, 0x%04x", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), op_src2->val);
}

make_helper(beq)
{
	
	decode_imm_type(instr);
	if (reg_w(op_dest->reg) == reg_w(op_src1->reg)){
		int imm;
		if (op_src2->val & 0x8000){
			imm = (0xFFFF << 16) | op_src2->val;
		}
		else{
			imm = op_src2->val;
		}
		uint32_t addr = (int)cpu.pc + (imm << 2);
		cpu.pc = addr;
	}
	sprintf(assembly, "beq %s, %s, 0x%04x", REG_NAME(op_src1->reg), REG_NAME(op_dest->reg), op_src2->val);
}

make_helper(bne) {

    decode_imm_type(instr);
    if (reg_w(op_dest->reg) != reg_w(op_src1->reg)) {
        int imm;
        if (op_src2->val & 0x8000) {
            imm = (0xFFFF << 16) | op_src2->val;
        } else {
            imm = op_src2->val;
        }
        uint32_t addr = (int)cpu.pc + (imm << 2);
        cpu.pc = addr;
    }
    sprintf(assembly, "bne   %s, %s, 0x%04x", REG_NAME(op_src1->reg),
            REG_NAME(op_dest->reg), op_src2->val);
}

make_helper(bgez) {

    decode_imm_type(instr);
    if ((int)op_src1->val >= 0) {
        int imm;
        if (op_src2->val & 0x8000) {
            imm = (0xFFFF << 16) | op_src2->val;
        } else {
            imm = op_src2->val;
        }
        uint32_t addr = (int)cpu.pc + (imm << 2);
        cpu.pc = addr;
    }
    sprintf(assembly, "bgez %s, 0x%04x", REG_NAME(op_src1->reg), op_src2->val);
}

make_helper(bltz) {

    decode_imm_type(instr);
    if ((int)op_src1->val < 0) {
        int imm;
        if (op_src2->val & 0x8000) {
            imm = (0xFFFF << 16) | op_src2->val;
        } else {
            imm = op_src2->val;
        }
        uint32_t addr = (int)cpu.pc + (imm << 2);
        cpu.pc = addr;
    }
    sprintf(assembly, "bltz %s, 0x%04x", REG_NAME(op_src1->reg), op_src2->val);
}

make_helper(bltzal) {

    decode_imm_type(instr);
    if (((int)op_src1->val) < 0) {
        int temp;
        if (op_src2->val & 0x8000) {
            temp = (0xFFFF << 16) | op_src2->val;
        } else {
            temp = op_src2->val;
        }
        uint32_t addr = cpu.pc + (temp << 2);
        reg_w(31) = cpu.pc + 8;
        cpu.pc = addr;
    } else {
        reg_w(31) = cpu.pc + 8;
    }
    sprintf(assembly, "BLTZAL %s, 0x%04x", REG_NAME(op_src1->reg),
            op_src2->val);
}

make_helper(bgezal) {

    decode_imm_type(instr);
    if (((int)op_src1->val) >= 0) {
        int temp;
        if (op_src2->val & 0x8000) {
            temp = (0xFFFF << 16) | op_src2->val;
        } else {
            temp = op_src2->val;
        }
        uint32_t addr = cpu.pc + (temp << 2);
        reg_w(31) = cpu.pc + 8;
        cpu.pc = addr;
    } else {
        reg_w(31) = cpu.pc + 8;
    }
    sprintf(assembly, "BGEZAL %s, 0x%04x", REG_NAME(op_src1->reg),
            op_src2->val);
}

make_helper(bgtz) {
    decode_imm_type(instr);
    if ((int)op_src1->val > 0) {
        int imm;
        if (op_src2->val & 0x8000) {
            imm = (0xFFFF << 16) | op_src2->val;
        } else {
            imm = op_src2->val;
        }
        uint32_t addr = (int)cpu.pc + (imm << 2);
        cpu.pc = addr;
    }
    sprintf(assembly, "bgtz %s, 0x%04x", REG_NAME(op_src1->reg), op_src2->val);
}

make_helper(blez) {
	
    decode_imm_type(instr);
    if ((int)op_src1->val <= 0) {
        int imm;
        if (op_src2->val & 0x8000) {
            imm = (0xFFFF << 16) | op_src2->val;
        } else {
            imm = op_src2->val;
        }
        uint32_t addr = (int)cpu.pc + (imm << 2);
        cpu.pc = addr;
    }
    sprintf(assembly, "blez %s, 0x%04x", REG_NAME(op_src1->reg), op_src2->val);
}