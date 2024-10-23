#include "temu.h"

#define ENTRY_START 0xbfc00000

char *exec_file;
uint8_t *hw_mem;
CPU_state cpu;

void init_regex();
void init_wp_pool();
void init_ddr3();

FILE *log_fp = NULL;
FILE *golden_trace_fp = NULL;

static void init_log() {
	log_fp = fopen("log.txt", "w");
	golden_trace_fp = fopen("golden_trace.txt", "w");
	Assert(golden_trace_fp, "Can not open 'golden_trace.txt'");
	Assert(log_fp, "Can not open 'log.txt'");
}

static void welcome() {
	printf("Welcome to TEMU!\nThe executable is %s.\nFor help, type \"help\"\n",
			exec_file);
}

void init_monitor(int argc, char *argv[]) {
	/* Perform some global initialization */

	/* Open the log file. */
	exec_file = argv[1];
	init_log();

	/* Compile the regular expressions. */
	init_regex();

	/* Initialize the watchpoint pool. */
	init_wp_pool();

	/* Display welcome message. */
	welcome();
}

static void load_entry(const char* instFilePath, const char* dataFilePath) {
    int ret;
    // 加载指令文件
    FILE *fp = fopen(instFilePath, "rb");
    Assert(fp, "Can not open '%s'", instFilePath);
    fseek(fp, 0, SEEK_END);
    size_t file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    ret = fread((void *)(hw_mem + (ENTRY_START & 0x1fffffff)), file_size, 1, fp);
    assert(ret == 1);
    fclose(fp); // 关闭文件

    // 加载数据文件
    fp = fopen(dataFilePath, "rb");
    Assert(fp, "Can not open '%s'", dataFilePath);
    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    ret = fread((void *)(hw_mem), file_size, 1, fp); // load .data segment to memory address 0x00000000
    fclose(fp); // 关闭文件
}

void restart(const char* instFilePath, const char* dataFilePath) {
	/* Perform some initialization to restart a program */

	/* Read the entry code into memory. */
	load_entry(instFilePath, dataFilePath);

	/* Set the initial instruction pointer. */
	cpu.pc = ENTRY_START;

	/* Initialize DRAM. */
	init_ddr3();
}
