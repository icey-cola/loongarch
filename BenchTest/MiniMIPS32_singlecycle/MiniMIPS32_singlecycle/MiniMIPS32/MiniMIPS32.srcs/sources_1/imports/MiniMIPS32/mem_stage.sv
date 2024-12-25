`include "defines.sv"

module mem_stage (

    // �������ִ�н׶λ�õ���Ϣ
    input  logic [`REG_BUS       ]      ALUResult,
    input  logic [`REG_BUS       ]      WriteData,
    input  logic                        MemWrite,

    // �������ݴ洢�����ź�
    output logic [`INST_ADDR_BUS ]       A,
    output logic                         WE,
    output logic [`REG_BUS       ]       WD
    );

    // ������ݴ洢���ķ��ʵ�ַ,��д������ݺ�дʹ��
    assign A = ALUResult;
    assign WD = ((ALUResult[31:16] == 16'h8000) | (ALUResult[31:16] == 16'h8004)) ? 
                    WriteData : {WriteData[7 : 0], WriteData[15 : 8], WriteData[23 : 16], WriteData[31 : 24]};
    assign WE = (ALUResult[31] == 1'b1) ? 1'b0 : MemWrite;

endmodule
