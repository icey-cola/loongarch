`include "defines.v"

module exe_stage (

    // �� ID �׶λ�õ���Ϣ
    input  wire [`ALUTYPE_BUS	] 	exe_alutype_i,
    input  wire [`ALUOP_BUS	    ] 	exe_aluop_i,
    input  wire [`REG_BUS 		] 	exe_src1_i,
    input  wire [`REG_BUS 		] 	exe_src2_i,
    input  wire [`REG_ADDR_BUS 	] 	exe_wa_i,
    input  wire 					exe_mreg_i,
    input  wire 					exe_wreg_i,
    input  wire [`WORD_BUS 		] 	exe_din_i,
    input  wire [`INST_ADDR_BUS]    exe_debug_wb_pc,  // ������ʹ�õ�PCֵ���ϰ����ʱ���ɾ�����ź�

    // ���� MEM �׶ε���Ϣ
    output wire [`ALUOP_BUS	    ] 	exe_aluop_o,
    output wire [`REG_ADDR_BUS 	] 	exe_wa_o,
    output wire 					exe_mreg_o,
    output wire 					exe_wreg_o,
    output wire [`REG_BUS 		] 	exe_wd_o,
    output wire [`WORD_BUS 		] 	exe_din_o,
    
    output wire [`INST_ADDR_BUS] 	debug_wb_pc  // ������ʹ�õ�PCֵ���ϰ����ʱ���ɾ�����ź�
    );

    // ֱ�Ӵ�����һ�׶�
    assign exe_aluop_o = exe_aluop_i;
    assign exe_din_o   = exe_din_i;
    
	reg [`REG_BUS       ]      arithres;       // ������������Ľ�� alutype 001
    reg [`REG_BUS       ]      logicres;       // �����߼�����Ľ�� alutype 010
	reg [`REG_BUS       ]      shiftres;       // ������λ����Ľ�� alutype 100
	reg [`REG_BUS       ]      finalres;       // �������յ�������
    
    // �����ڲ�������aluop�����߼�����
	always @(*) begin
		case (exe_aluop_i)
			`MINIMIPS32_ADD: arithres = exe_src1_i + exe_src2_i;
            `MINIMIPS32_LB:  arithres = exe_src1_i + exe_src2_i;
            `MINIMIPS32_LW:  arithres = exe_src1_i + exe_src2_i;
            `MINIMIPS32_SB:  arithres = exe_src1_i + exe_src2_i;
            `MINIMIPS32_SW:  arithres = exe_src1_i + exe_src2_i;
            `MINIMIPS32_JAL: arithres = exe_src1_i;
			default:    arithres = `ZERO_WORD;
		endcase
	end
	always @(*) begin
		case (exe_aluop_i)
			`MINIMIPS32_AND: logicres = exe_src1_i & exe_src2_i;
			`MINIMIPS32_OR:  logicres = exe_src1_i | exe_src2_i;
			`MINIMIPS32_XOR: logicres = exe_src1_i ^ exe_src2_i;
			`MINIMIPS32_LUI: logicres = exe_src2_i;
			default:    logicres = `ZERO_WORD;
		endcase
	end
	always @(*) begin
		case (exe_aluop_i)
			`MINIMIPS32_SLL: shiftres = exe_src2_i << exe_src1_i;
			`MINIMIPS32_SRA: shiftres = $signed(exe_src2_i) >>> exe_src1_i;
			default:    shiftres = `ZERO_WORD;
		endcase
	end

    assign exe_mreg_o = exe_mreg_i;
    assign exe_wreg_o = exe_wreg_i;
    assign exe_wa_o   = exe_wa_i;
    
    // ���ݲ�������alutypeȷ��ִ�н׶����յ����������ȿ����Ǵ�д��Ŀ�ļĴ��������ݣ�Ҳ�����Ƿ������ݴ洢���ĵ�ַ��
	always @(*) begin
		case (exe_alutype_i)
			`ARITH: finalres = arithres;
			`LOGIC: finalres = logicres;
			`SHIFT: finalres = shiftres;
			default:    finalres = `ZERO_WORD;
		endcase
	end
    assign exe_wd_o = finalres;
    
    assign debug_wb_pc = exe_debug_wb_pc;    // �ϰ����ʱ���ɾ������� 

endmodule