`include "defines.sv"

module exe_stage (

    // ������׶λ�õ���Ϣ
    input  logic [`ALUCTRL_BUS	] 	ALUControl,
    input  logic [`REG_BUS 		] 	SrcA,
    input  logic [`REG_BUS 		] 	SrcB,
    input  logic                    Branch,

    // �����Ĵ���������
    output logic [`REG_BUS 		] 	ALUResult,
    output logic                    PCSrc
    );
    
    logic [`REG_BUS       ]      addres;          // ����ӷ�����Ľ��
    logic [`REG_BUS       ]      orres;           // �����������
    logic [`REG_BUS       ]      sltres;       // ����С����λ�Ľ��
    logic [`REG_BUS       ]      luires;       // ������ظ߰��ֵĽ��
    
    logic [`REG_BUS       ]      SrcB_tmp;
    logic                        Zero;
    
    assign SrcB_tmp = ((ALUControl == `MINIMIPS32_BEQ) | (ALUControl == `MINIMIPS32_BNE)) ? ~SrcB + 1 : SrcB;
    
    assign addres   = SrcA + SrcB_tmp;
    assign orres    = SrcA | SrcB;
    assign luires   = SrcB;    
    assign sltres   = (($signed(SrcA) < $signed(SrcB)) ? 32'b1 : 32'b0);
    
    always_comb begin
        if(ALUControl == `MINIMIPS32_BEQ) begin
            if (addres == 0) Zero = 1;
            else Zero = 0;
        end
        else if(ALUControl == `MINIMIPS32_BNE) begin
            if (addres != 0) Zero = 1;
            else Zero = 0;
        end
        else Zero = 0;
    end
    
    always_comb begin
        case(ALUControl)
            `MINIMIPS32_ADD : ALUResult = addres;
            `MINIMIPS32_OR  : ALUResult = orres;
            `MINIMIPS32_SLT : ALUResult = sltres;
            `MINIMIPS32_LUI : ALUResult = luires;
            default         : ALUResult = 32'b0;
        endcase
    end
    
    assign PCSrc = Zero & Branch;

    // �����ڲ�������aluop�����߼�����
    /*assign logicres = (cpu_rst_n == `RST_ENABLE)  ? `ZERO_WORD : 
                      (ALUControl == `MINIMIPS32_ORI ) ? (SrcA | SrcB) : `ZERO_WORD;

    // �����ڲ�������aluop������λ����
    assign shiftres = (cpu_rst_n == `RST_ENABLE) ? `ZERO_WORD : 
                      (ALUControl == `MINIMIPS32_SLL ) ? (SrcB << SrcA) : `ZERO_WORD;

    // �ж��Ƿ������������쳣
    logic [31: 0] exe_src2_t;
    assign exe_src2_t = (ALUControl == `MINIMIPS32_SUB) ? (~SrcB) + 1 : SrcB;
    logic [31: 0] arith_tmp;
    assign arith_tmp  = SrcA + exe_src2_t;
    // �����ڲ�������aluop������������
    assign arithres = (cpu_rst_n == `RST_ENABLE) ? `ZERO_WORD : 
                      (ALUControl == `MINIMIPS32_ADD  )  ? arith_tmp :
                      (ALUControl == `MINIMIPS32_LW   )  ? arith_tmp :
                      (ALUControl == `MINIMIPS32_SW   )  ? arith_tmp :
                      (ALUControl == `MINIMIPS32_SUB  )  ? arith_tmp[31:0] :
                      (ALUControl == `MINIMIPS32_SLT  )  ? (($signed(SrcA) < $signed(SrcB)) ? 32'b1 : 32'b0) :  `ZERO_WORD;
    
    // ���ݲ�������alutypeȷ��ִ�н׶����յ����������ȿ����Ǵ�д��Ŀ�ļĴ��������ݣ�Ҳ�����Ƿ������ݴ洢���ĵ�ַ��
    assign ALUResult = (cpu_rst_n   == `RST_ENABLE ) ? `ZERO_WORD : 
                       (ALUControl == `MINIMIPS32_ORI) ? logicres  :
                       (ALUControl == `MINIMIPS32_SLL) ? shiftres  :
                       (ALUControl == `MINIMIPS32_ADD) ? arithres  :
                       (ALUControl == `MINIMIPS32_LW ) ? arithres  :
                       (ALUControl == `MINIMIPS32_SW ) ? arithres  :
                       (ALUControl == `MINIMIPS32_SUB) ? arithres  :
                       (ALUControl == `MINIMIPS32_SLT) ? arithres  : `ZERO_WORD;*/

endmodule
