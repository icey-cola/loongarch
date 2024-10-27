#include "monitor.h"
#include "temu.h"
#include "watchpoint.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include<expr.h>

void cpu_exec(uint32_t);

void display_reg();

/* We use the `readline' library to provide more flexibility to read from stdin. */
char* rl_gets() {
	static char *line_read = NULL;

	if (line_read) {
		free(line_read);
		line_read = NULL;
	}

	line_read = readline("(temu) ");

	if (line_read && *line_read) {
		add_history(line_read);
	}

	return line_read;
}

static int cmd_c(char *args) {
	cpu_exec(-1);
	return 0;
}

static int cmd_q(char *args) {
	return -1;
}

static int cmd_help(char *args);

//单步调试
static int cmd_si(char* args)
{	
	char *arg = strtok(NULL, " ");
	int i = 1;
	if (arg != NULL)
	{
		int step = atoi(arg);
		if(step<1) {
			fprintf(stderr, "usage: n must be greater than 1\n");
			return -1;
		}
		else {
		sscanf(arg, "%d", &i);
		}
	}
	cpu_exec(i);
	return 0;
}

//打印程序状态
static int cmd_info(char*args)
{
	char* arg = strtok(args, " ");
	if(args==NULL)
		{
			printf("usage: info r or info w\n");
			return 0;
		}
	else if(strcmp(arg,"r")==0)
		{
			for (int i = R_ZERO; i <= R_RA;i++)
			{
				printf("%s\t 0x%08x\n", regfile[i], reg_w(i));

			}
		}
	else if(strcmp(arg, "w") == 0)
		{
			list_watchpoint();
		}
	else 
		{
			fprintf(stderr, "unknown command\n");
			return -1;
		}
	return 0;
}
//表达式求值
static int cmd_p(char* args){
    uint32_t num;
    bool success;
    num = expr(args, &success);
    if (success)
    {
        printf("Expression %s:\t0x%x\t%d\n", args, num, num);
    }
//    else assert(0);
    return 0;
}




//扫描内存
static int cmd_x(char* args){
    if (args == NULL)//x后面的参数
    {
        printf("usage: x n addr(0x)\n");
        return 0;
    }

    char *arg = strtok(args, " ");

    int n = atoi(arg);
    char *EXPR = strtok(NULL, " ");
    if (EXPR == NULL)
    {
        printf("usage: x n addr(0x)\n");
        return 0;
    }

    uint32_t address;
    bool success;
    address = expr(EXPR, &success);
    if (success)
    {
        for (int i=0;i<n;i++){
            uint32_t data= mem_read(address+i*4,4);
            printf("0x%08x: ",address + i*4);
            for(int j=0;j<4;j++){
                printf("0x%02x ",data&0xff);
                data = data >> 8;
            }
            printf("\n");
        }
    }
    else {
        assert(0);
    }
    return 0;
}

//设置监视点
static int cmd_w(char*args)
{
if (args)
	{
		int NO = set_watchpoint(args);
		if (NO != -1)
		{
			printf("Set watchpoint #%d\n", NO);
		}
		else
		{
			printf("Bad expression\n");
		}
	}
	return 0;

}
//删除监视点
static int cmd_d(char *args)
{
	int NO;
	sscanf(args, "%d", &NO);
	if (!delete_watchpoint(NO))
	{
		printf("Watchpoint #%d does not exist\n", NO);
	}

	return 0;
}

#include <gtk/gtk.h>

#include <stdio.h>
// 表达式求值的 GUI 回调函数
static void on_expr_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog, *entry, *content_area;
    GtkDialogFlags flags = GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT;

    // 创建一个输入对话框，让用户输入表达式
    dialog = gtk_dialog_new_with_buttons("Evaluate Expression",
                                         NULL,
                                         flags,
                                         "_OK",
                                         GTK_RESPONSE_ACCEPT,
                                         "_Cancel",
                                         GTK_RESPONSE_REJECT,
                                         NULL);
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "输入表达式");
    gtk_container_add(GTK_CONTAINER(content_area), entry);
    gtk_widget_show_all(dialog);

    int response = gtk_dialog_run(GTK_DIALOG(dialog));
    if (response == GTK_RESPONSE_ACCEPT) {
        const char *expr_str = gtk_entry_get_text(GTK_ENTRY(entry));
        if (expr_str != NULL && *expr_str != '\0') {
            bool success;
            uint32_t result = expr((char *)expr_str, &success);  // 调用原有表达式求值函数

            if (success) {
                // 创建一个没有图标的对话框来显示计算结果
                GtkWidget *result_dialog = gtk_dialog_new_with_buttons("结果",
                                                                      NULL,
                                                                      GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                                                      "_OK",
                                                                      GTK_RESPONSE_OK,
                                                                      NULL);
                GtkWidget *result_content = gtk_dialog_get_content_area(GTK_DIALOG(result_dialog));

                // 创建标签显示结果
                char result_text[256];
                snprintf(result_text, sizeof(result_text), "结果: 0x%x (%d)", result, result);
                GtkWidget *result_label = gtk_label_new(result_text);
                gtk_container_add(GTK_CONTAINER(result_content), result_label);

                gtk_widget_show_all(result_dialog);
                gtk_dialog_run(GTK_DIALOG(result_dialog));
                gtk_widget_destroy(result_dialog);
            } else {
                // 表达式求值失败，显示错误消息
                GtkWidget *error_dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT,
                                                                 GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
                                                                 "表达式计算失败，请检查输入。");
                gtk_dialog_run(GTK_DIALOG(error_dialog));
                gtk_widget_destroy(error_dialog);
            }
        } else {
            // 用户输入为空，显示错误消息
            GtkWidget *error_dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT,
                                                             GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
                                                             "请输入有效的表达式。");
            gtk_dialog_run(GTK_DIALOG(error_dialog));
            gtk_widget_destroy(error_dialog);
        }
    }
    gtk_widget_destroy(dialog);
}
static void button_click(GtkWidget *widget, gpointer data) {
    
}

// 主窗口激活回调函数
static void activate(GtkApplication *app, gpointer user_data) {
    // 创建主窗口
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "TEMU GUI");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);

    // 创建垂直布局容器
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);


    GtkWidget *button_c = gtk_button_new_with_label("继续执行");
    gtk_box_pack_start(GTK_BOX(vbox), button_c, TRUE, TRUE, 0);
    g_signal_connect(button_c, "clicked", G_CALLBACK(button_click), NULL);

    GtkWidget *button_x = gtk_button_new_with_label("查看内存");
    gtk_box_pack_start(GTK_BOX(vbox), button_x, TRUE, TRUE, 0);
    g_signal_connect(button_x, "clicked", G_CALLBACK(button_click), NULL);

	GtkWidget *button_si = gtk_button_new_with_label("执行x步");
    gtk_box_pack_start(GTK_BOX(vbox), button_si, TRUE, TRUE, 0);
    g_signal_connect(button_si, "clicked", G_CALLBACK(button_click), NULL);

 
    GtkWidget *button_expr = gtk_button_new_with_label("表达式求值");
    gtk_box_pack_start(GTK_BOX(vbox), button_expr, TRUE, TRUE, 0);
    g_signal_connect(button_expr, "clicked", G_CALLBACK(on_expr_button_clicked), NULL);

    GtkWidget *button_set_watchpoint = gtk_button_new_with_label("设置监视点");
    gtk_box_pack_start(GTK_BOX(vbox), button_set_watchpoint, TRUE, TRUE, 0);
    g_signal_connect(button_set_watchpoint, "clicked", G_CALLBACK(button_click), NULL);


    GtkWidget *button_delete_watchpoint = gtk_button_new_with_label("删除监视点");
    gtk_box_pack_start(GTK_BOX(vbox), button_delete_watchpoint, TRUE, TRUE, 0);
    g_signal_connect(button_delete_watchpoint, "clicked", G_CALLBACK(button_click), NULL);
    
	GtkWidget *button_golden_trace = gtk_button_new_with_label("显示golden_trace");
    gtk_box_pack_start(GTK_BOX(vbox), button_golden_trace, TRUE, TRUE, 0);
    g_signal_connect(button_golden_trace, "clicked", G_CALLBACK(button_click), NULL);

    // 显示窗口
    gtk_widget_show_all(window);
}

// 主 GUI 命令
int cmd_visable(char *args) {
    GtkApplication *app;
    int status;

    // 创建一个 GTK 应用程序实例
    app = gtk_application_new("org.temu.gui", G_APPLICATION_FLAGS_NONE);

    // 连接激活信号到回调函数
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    // 运行应用程序
    status = g_application_run(G_APPLICATION(app), 0, NULL);

    // 释放应用程序实例
    g_object_unref(app);

    return status;
}
static struct
{
	char *name;
	char *description;
	int (*handler)(char *);
} cmd_table [] = {
	{ "help", "Display informations about all supported commands", cmd_help },
	{ "c", "Continue the execution of the program", cmd_c },
	{ "q", "Exit TEMU", cmd_q },
	{"si","Single instruction execute",cmd_si},
	{"info","Printf state of registers(r) or watchpoint(w)",cmd_info},
	{"x","Scan memory",cmd_x},
	{"p","Evaluate expression", cmd_p},
	{"w","Create WatchPoint", cmd_w},
	{"d","Delete WatchPoint", cmd_d},
	{ "visable", "Launch GUI window ", cmd_visable },

 
	/* TODO: Add more commands */

};


 
#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int i;

	if(arg == NULL) {
		/* no argument given */
		for(i = 0; i < NR_CMD; i ++) {
			printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
		}
	}
	else {
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(arg, cmd_table[i].name) == 0) {
				printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
				return 0;
			}
		}
		printf("Unknown command '%s'\n", arg);
	}
	return 0;
}

void ui_mainloop() {
	while(1) {
		char *str = rl_gets();
		char *str_end = str + strlen(str);

		/* extract the first token as the command */
		char *cmd = strtok(str, " ");
		if(cmd == NULL) { continue; }

		/* treat the remaining string as the arguments,
		 * which may need further parsing
		 */
		char *args = cmd + strlen(cmd) + 1;
		if(args >= str_end) {
			args = NULL;
		}

		int i;
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(cmd, cmd_table[i].name) == 0) {
				if(cmd_table[i].handler(args) < 0) { return; }
				break;
			}
		}

		if(i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
	}
}
