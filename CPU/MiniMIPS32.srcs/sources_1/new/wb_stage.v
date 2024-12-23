`include "defines.v"

module wb_stage(

    // �ӷô�׶λ�õ���Ϣ
    input  wire                   wb_mreg_i,
	input  wire                   wb_wreg_i,
    input  wire [3 : 0]           wb_dre_i,
    input  wire [`REG_ADDR_BUS  ] wb_wa_i,
	input  wire [`REG_BUS       ] wb_dreg_i,
	input  wire [`INST_ADDR_BUS]  wb_debug_wb_pc,  // ������ʹ�õ�PCֵ���ϰ����ʱ���ɾ�����ź�

    // data_ram
    input  wire [`WORD_BUS]       dm,

    // д��Ŀ�ļĴ���������
    output wire                   wb_wreg_o,
    output wire [`REG_ADDR_BUS  ] wb_wa_o,
    output reg  [`WORD_BUS      ] wb_wd_o,
    
    output wire [`INST_ADDR_BUS]  debug_wb_pc,       // ������ʹ�õ�PCֵ���ϰ����ʱ���ɾ�����ź�
    output wire                   debug_wb_rf_wen,   // ������ʹ�õ�PCֵ���ϰ����ʱ���ɾ�����ź�
    output wire [`REG_ADDR_BUS  ] debug_wb_rf_wnum,  // ������ʹ�õ�PCֵ���ϰ����ʱ���ɾ�����ź�
    output wire [`WORD_BUS      ] debug_wb_rf_wdata  // ������ʹ�õ�PCֵ���ϰ����ʱ���ɾ�����ź�
    );

    // ���� wb_dre_i ������������������
    reg [`WORD_BUS] findm;
    always @(*) begin
        case (wb_dre_i)
            4'b0001: findm = {{24{dm[7]}},dm[7 : 0]};
            4'b0010: findm = {{24{dm[15]}},dm[15 : 8]};
            4'b0100: findm = {{24{dm[23]}},dm[23 : 16]};
            4'b1000: findm = {{24{dm[31]}},dm[31 : 24]};
            4'b1111: findm = {dm[7:0],dm[15:8],dm[23:16],dm[31:24]};
            default: findm = `ZERO_WORD;
        endcase
    end
    // ���� wb_mreg_i ����д�ص�����
    always @(*) begin
        case (wb_mreg_i)
            1'b1: wb_wd_o = findm;
            1'b0: wb_wd_o = wb_dreg_i;
        endcase
    end

    assign wb_wa_o      = wb_wa_i;
    assign wb_wreg_o    = wb_wreg_i;
    
    assign debug_wb_pc         = wb_debug_wb_pc;    // �ϰ����ʱ���ɾ�������
    assign debug_wb_rf_wen     = wb_wreg_i;         // �ϰ����ʱ���ɾ������� 
    assign debug_wb_rf_wnum    = wb_wa_i;           // �ϰ����ʱ���ɾ�������
    assign debug_wb_rf_wdata   = wb_wd_o;         // �ϰ����ʱ���ɾ������� 
    
endmodule
