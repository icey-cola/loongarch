`include "defines.v"

module id_stage(
    
    // ��ȡָ�׶λ�õ�PCֵ
    input  wire [`INST_ADDR_BUS]    id_pc_i,
    input  wire [`INST_ADDR_BUS]    id_debug_wb_pc,  // ������ʹ�õ�PCֵ���ϰ����ʱ���ɾ�����ź�

    input  wire [`INST_BUS     ]    id_inst_i,

    // to EXE
    output wire                     id_mreg_o,
    output wire                     id_wreg_o,
    output wire [`ALUTYPE_BUS  ]    id_alutype_o,
    output wire [`ALUOP_BUS    ]    id_aluop_o,
    output wire [`REG_ADDR_BUS ]    id_wa_o,
    output wire [`REG_BUS      ]    id_src1_o,
    output wire [`REG_BUS      ]    id_src2_o,
    output reg [`WORD_BUS     ]    id_din_o,
    
    // REG_FILE
    input  wire [`REG_BUS      ]    rd1,
    input  wire [`REG_BUS      ]    rd2,
    output wire                     rreg1,
    output wire [`REG_ADDR_BUS ]    ra1,
    output wire                     rreg2,
    output wire [`REG_ADDR_BUS ]    ra2,

    // forword
    input  wire                     exe2id_wreg,
    input  wire [`REG_ADDR_BUS ]    exe2id_wa,
    input  wire [`WORD_BUS     ]    exe2id_wd,
    input  wire                     mem2id_wreg,
    input  wire [`REG_ADDR_BUS ]    mem2id_wa,
    input  wire [`WORD_BUS     ]    mem2id_wd,
    
    output       [`INST_ADDR_BUS] 	debug_wb_pc  // ������ʹ�õ�PCֵ���ϰ����ʱ���ɾ�����ź�
    );
    
    // INST_ROM ������ָ���Ǵ�����
    // ��Ҫ����С��ģʽ��ָ֯����
    wire [`INST_BUS] id_inst = {id_inst_i[7:0], id_inst_i[15:8], id_inst_i[23:16], id_inst_i[31:24]};

    // ��ȡָ�����и����ֶε���Ϣ
    wire [5 :0] op   = id_inst[31:26];
    wire [5 :0] func = id_inst[5 : 0];
    wire [4 :0] rd   = id_inst[15:11];
    wire [4 :0] rs   = id_inst[25:21];
    wire [4 :0] rt   = id_inst[20:16];
    wire [4 :0] sa   = id_inst[10: 6];
    wire [15:0] imm  = id_inst[15: 0]; 

    /*-------------------- ��һ�������߼���ȷ����ǰ��Ҫ�����ָ�� --------------------*/
	// R type
    wire inst_reg  = ~|op;
    wire inst_and  = inst_reg& func[5]&~func[4]&~func[3]& func[2]&~func[1]&~func[0];
	wire inst_or   = inst_reg& func[5]&~func[4]&~func[3]& func[2]&~func[1]& func[0];
	wire inst_xor  = inst_reg& func[5]&~func[4]&~func[3]& func[2]& func[1]&~func[0];
	wire inst_addu = inst_reg& func[5]&~func[4]&~func[3]&~func[2]&~func[1]& func[0];
	wire inst_sll  = inst_reg&~func[5]&~func[4]&~func[3]&~func[2]&~func[1]&~func[0]&(|id_inst); // ��ȫ�㣬���� nop
	wire inst_sra  = inst_reg&~func[5]&~func[4]&~func[3]&~func[2]& func[1]& func[0];
	// I type
	wire inst_ori  = ~op[5]&~op[4]& op[3]& op[2]&~op[1]& op[0];
	wire inst_andi = ~op[5]&~op[4]& op[3]& op[2]&~op[1]&~op[0];
	wire inst_lui  = ~op[5]&~op[4]& op[3]& op[2]& op[1]& op[0];
	wire inst_addiu= ~op[5]&~op[4]& op[3]&~op[2]&~op[1]& op[0];
	wire inst_addi = ~op[5]&~op[4]& op[3]&~op[2]&~op[1]&~op[0];
    wire inst_lb   =  op[5]&~op[4]&~op[3]&~op[2]&~op[1]&~op[0];
    wire inst_lw   =  op[5]&~op[4]&~op[3]&~op[2]& op[1]& op[0];
    wire inst_sb   =  op[5]&~op[4]& op[3]&~op[2]&~op[1]&~op[0];
    wire inst_sw   =  op[5]&~op[4]& op[3]&~op[2]& op[1]& op[0];
    /*------------------------------------------------------------------------------*/

    /*-------------------- �ڶ��������߼������ɾ�������ź� --------------------*/
    // д��·ѡ����ʹ���ź�
    assign id_mreg_o       = inst_lb  | inst_lw;
    // дͨ�üĴ���ʹ���ź�
    assign id_wreg_o       = inst_and | inst_or  | inst_xor | inst_addu | inst_sll | inst_sra 
						   | inst_ori | inst_andi| inst_lui | inst_addiu| inst_addi
                           | inst_lb  | inst_lw;

    // ��������alutype
    assign id_alutype_o[2] = inst_sll | inst_sra;
    assign id_alutype_o[1] = inst_and | inst_or | inst_xor | inst_ori | inst_andi | inst_lui;
    assign id_alutype_o[0] = inst_addu| inst_addiu| inst_addi | inst_lb | inst_lw | inst_sb | inst_sw;

	// �ڲ�������aluop
	assign id_aluop_o[7]   = inst_lb  | inst_lw | inst_sb | inst_sw;
	assign id_aluop_o[6]   = 1'b0;
	assign id_aluop_o[5]   = 1'b0;
	assign id_aluop_o[4]   = inst_and | inst_or | inst_xor | inst_addu | inst_sll | inst_sra | inst_ori  | inst_andi | inst_addiu | inst_addi | inst_lb | inst_lw | inst_sb | inst_sw;
	assign id_aluop_o[3]   = inst_and | inst_or | inst_xor | inst_addu | inst_ori | inst_andi| inst_addiu| inst_addi | inst_sb    | inst_sw;
	assign id_aluop_o[2]   = inst_and | inst_or | inst_xor | inst_ori  | inst_andi| inst_lui;
	assign id_aluop_o[1]   = inst_xor | inst_sra| inst_lw  | inst_sw;
	assign id_aluop_o[0]   = inst_or  | inst_sll| inst_ori | inst_lui;

    // ��ͨ�üĴ����ѵĶ��ź�����1����Ϊ���ں����ж��Ƿ�Ҫʹ�ö���������
    assign rreg1 = 1'b1;
    assign rreg2 = 1'b1;

	/*-------------------- ����һЩֻ�� ID �׶�ʹ�õĿ����ź� --------------------*/
	wire shift;
	wire rt_sel;
	wire sext;
	wire upper;
	wire immsel;
    reg [1:0] fwrd1, fwrd2;
	assign shift = inst_sll | inst_sra; 												                             
	assign rt_sel = inst_ori | inst_andi | inst_lui | inst_addiu| inst_addi | inst_lb | inst_lw;	                 
	assign sext = inst_addiu | inst_addi | inst_lb  | inst_lw   | inst_sb   | inst_sw;				                 
	assign upper = inst_lui;															                             
	assign immsel = inst_ori | inst_andi | inst_lui | inst_addiu| inst_addi | inst_lb | inst_lw | inst_sb | inst_sw;
    always @(*) begin
        if(exe2id_wreg && exe2id_wa == rs) fwrd1 = 2'b01;
        else if(mem2id_wreg && mem2id_wa == rs) fwrd1 = 2'b10;
        else fwrd1 = 2'b00;
        if(exe2id_wreg && exe2id_wa == rt) fwrd2 = 2'b01;
        else if(mem2id_wreg && mem2id_wa == rt) fwrd2 = 2'b10;
        else fwrd2 = 2'b00;
    end
    /*------------------------------------------------------------------------------*/

    // ��ͨ�üĴ����Ѷ˿�1�ĵ�ַΪrs�ֶΣ����˿�2�ĵ�ַΪrt�ֶ�
    assign ra1   = rs;
    assign ra2   = rt;
                                            
    // ��ô�д��Ŀ�ļĴ����ĵ�ַ��rt��rd��
    assign id_wa_o  = rt_sel ? rt : rd;

    // ����������
	wire [`REG_BUS] extimm;
	wire [`REG_BUS] uppimm;
	wire [`REG_BUS] finimm;
	assign extimm = sext ? {{16{imm[15]}}, imm} : {16'b0, imm};
	assign uppimm = {imm, 16'b0};
	assign finimm = upper ? uppimm : extimm;

    // ȷ��Դ������
    reg [`WORD_BUS] true_rs, true_rt, id_src1, id_src2;
    always @(*) begin
        if(fwrd1 == 2'b00) true_rs = rd1;
        else if(fwrd1 == 2'b01) true_rs = exe2id_wd;
        else true_rs = mem2id_wd;
        if(fwrd2 == 2'b00) true_rt = rd2;
        else if(fwrd2 == 2'b01) true_rt = exe2id_wd;
        else true_rt = mem2id_wd;

        if(shift) id_src1 = {27'b0, sa};
        else id_src1 = true_rs;
        if(immsel) id_src2 = finimm;
        else id_src2 = true_rt;
    end
    assign id_src1_o = id_src1;
    assign id_src2_o = id_src2;

    // din �� rt �Ĵ�����ֵ
    always @(*) begin
        case(id_aluop_o)
            `MINIMIPS32_SB:  id_din_o = {4{true_rt[7:0]}};
            `MINIMIPS32_SW:  id_din_o = {true_rt[7:0], true_rt[15:8], true_rt[23:16], true_rt[31:24]};
            default:         id_din_o = `ZERO_WORD;
        endcase
    end
    
    assign debug_wb_pc = id_debug_wb_pc;    // �ϰ����ʱ���ɾ�������      

endmodule
