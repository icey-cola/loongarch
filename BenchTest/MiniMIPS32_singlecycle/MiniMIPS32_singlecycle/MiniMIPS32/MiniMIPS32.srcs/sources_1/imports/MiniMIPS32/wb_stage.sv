`include "defines.sv"

module wb_stage(
    input  logic                   MemtoReg,
	input  logic [`REG_BUS       ] ALUResult,

    // �����ݴ洢�������ĵ�����
    input  logic [`WORD_BUS      ] ReadData,
    // д��Ŀ�ļĴ���������
    output logic [`WORD_BUS      ] RegData
    );

    logic [`WORD_BUS] data;
    assign data = {ReadData[7:0], ReadData[15:8], ReadData[23:16], ReadData[31:24]};

    assign RegData = (MemtoReg  == `MREG_ENABLE) ? data : ALUResult;
endmodule
