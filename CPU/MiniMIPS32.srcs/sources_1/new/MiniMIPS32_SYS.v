`include "defines.v"

module MiniMIPS32_SYS(
    input wire sys_clk_200M_p,
    input wire sys_clk_200M_n,
    input wire sys_rst_n
    );

    wire [`INST_ADDR_BUS]  debug_wb_pc;       // ������ʹ�õ�PCֵ���ϰ����ʱ���ɾ�����ź�
    wire                   debug_wb_rf_wen;   // ������ʹ�õ�PCֵ���ϰ����ʱ���ɾ�����ź�
    wire [`REG_ADDR_BUS  ] debug_wb_rf_wnum;  // ������ʹ�õ�PCֵ���ϰ����ʱ���ɾ�����ź�
    wire [`WORD_BUS      ] debug_wb_rf_wdata;  // ������ʹ�õ�PCֵ���ϰ����ʱ���ɾ�����ź�

    wire                  cpu_clk_50M;
    clkdiv clocking(
    // Clock out ports
    .cpu_clk(cpu_clk_50M),     // output cpu_clk
   // Clock in ports
    .clk_in1_p(sys_clk_200M_p),    // input clk_in1_p
    .clk_in1_n(sys_clk_200M_n)     // input clk_in1_n
    );    
    
    wire                  ice;
    wire [`INST_ADDR_BUS] iaddr;
    wire [`INST_BUS     ] inst;
    inst_rom inst_rom0 (
      .clka(cpu_clk_50M),    // input wire clka
      .ena(ice),      // input wire ena
      .addra(iaddr[12:2]),  // input wire [9 : 0] addra
      .douta(inst)  // output wire [31 : 0] douta
    );

    wire                  dce;
    wire [3 : 0]          dwe;
    wire [`WORD_BUS]      daddr;
    wire [`WORD_BUS]      din;
    wire [`WORD_BUS]      dout;
    data_ram data_ram0 (
        .clka(cpu_clk_50M),    // input wire clka
        .ena(dce),      // input wire ena
        .wea(dwe),      // input wire [3 : 0] wea
        .addra(daddr[12:2]),  // input wire [9 : 0] addra
        .dina(din),     // input wire [31 : 0] dina
        .douta(dout)  // output wire [31 : 0] douta
    );

    MiniMIPS32 minimips32 (
        .cpu_clk_50M(cpu_clk_50M),
        .cpu_rst_n(sys_rst_n),
        
        .ice(ice),
        .iaddr(iaddr),
        .inst(inst),

        .dce(dce),
        .dwe(dwe),
        .daddr(daddr),
        .din(din),
        .dout(dout),
        
        .debug_wb_pc(debug_wb_pc),            // ������ʹ�õ�PCֵ���ϰ����ʱ���ɾ�����ź�
        .debug_wb_rf_wen(debug_wb_rf_wen),    // ������ʹ�õ�PCֵ���ϰ����ʱ���ɾ�����ź�
        .debug_wb_rf_wnum(debug_wb_rf_wnum),  // ������ʹ�õ�PCֵ���ϰ����ʱ���ɾ�����ź�
        .debug_wb_rf_wdata(debug_wb_rf_wdata) // ������ʹ�õ�PCֵ���ϰ����ʱ���ɾ�����ź�
    );

endmodule
