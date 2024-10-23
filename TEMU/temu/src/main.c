
void init_monitor(int, char *[]);
void restart();
void ui_mainloop();

int main(int argc, char *argv[]) {

	/* Initialize the monitor. */
	init_monitor(argc, argv);
	
	char* instFilePath = argv[1];
    char* dataFilePath = argv[2];

	if (argc != 3) {
		instFilePath="inst.bin";
		dataFilePath="data.bin";
	}
	/* Test the implementation of the `CPU_state' structure. */
	//reg_test();

	/* Initialize the virtual computer sysstem. */
	restart(instFilePath, dataFilePath);

	/* Receive commands from user. */
	ui_mainloop();

	return 0;
}
