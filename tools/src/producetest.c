#include "autoconfig.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "sys/ioctl.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <linux/watchdog.h>
// #include "tms_gpio.h"
#include "time.h"

#include "netcard.h"

#include <readline/readline.h>
#include <readline/history.h>

#include <libgpio.h>
#include <tms_gpio.h>
#include <producetest.h>
#include <ckself.h>


void list_ti(struct test_item *pitem, int len)
{
	printf("\n---------------------------------------------\n");
	for (int i = 0; i < len; i++) {
		printf("\t%d. %s\n", i, pitem[i].description);
	}
	printf("---------------------------------------------\n");
}

int select_ti(char *str, struct test_item *pitem, int len)
{
	int index;

	index = atoi(str);
	if (index >= len) {
		printf("没有该选项\n");
		return -1;
	}
	if (str[0] == '\0') {
		return -2;
	}
	// 只检测第一个字母是否为数字
	if (isdigit(str[0])) {
		return pitem[index].fun(NULL);		
	}
	
	return 0;
}

// *****************************************************************************
// TODO

int ti_null(void *arg)
{
	return RET_TI_BACK;
}

int ti_key_reset(void *arg)
{
	printf("按下复位键（RESET）4秒以上等待系统重启\n");

	NOTE_START();
	printf("重启过程\n");
	printf("\t1. uboot启动：蜂鸣器响1声，4秒后加载kernel\n");
	printf("\t2. kernel正确加载：蜂鸣器响1声，RUN闪烁1次\n");
	printf("\t3. 文件系统加载正常：蜂鸣器响1声\n");
	printf("\t4. 自检程序检测“异常”：蜂鸣长响3秒、停1秒，循环5次\n");
	printf("\t5. 软件正确启动：蜂鸣器响1秒、停1秒，循环5次\n");

	NOTE_END();
	ECHO_PAUSE();

	return 0;
}

int ti_buzzer(void *arg)
{
	struct gpio_dir iodir;
	struct gpio_data iodata;
	int io_index;

	
#if (CONFIG_BOARD == 11)
	io_index = IO_LED_ALARM_TOTAL;
#elif (CONFIG_BOARD == 12)
	io_index = IO_LED_BUZZER;
#endif
	
	
	open_io(NULL);
	// 复位
	iodir.index = io_index;
	iodir.dir = IO_OUTPUT;
	set_dir(&iodir);

	iodata.index = io_index;
	iodata.data = 1;
	set_io(&iodata);
	NOTE_START();
	printf("打开蜂鸣器\n");
	NOTE_END();
	ECHO_PAUSE();
	

	iodata.index = io_index;
	iodata.data = 0;
	set_io(&iodata);
	NOTE_START();
	printf("关闭蜂鸣器\n");
	NOTE_END();
	ECHO_PAUSE();
	close_io();
	
	return 0;
}


int ti_ledrun(void *arg)
{
	struct gpio_dir iodir;
	struct gpio_data iodata;

	open_io(NULL);

	iodir.index = IO_LED_RUN_G;
	iodir.dir = IO_OUTPUT;
	set_dir(&iodir);

	iodir.index = IO_LED_RUN_R;
	iodir.dir = IO_OUTPUT;
	set_dir(&iodir);

	iodata.index = IO_LED_RUN_G;
	iodata.data = 1;
	set_io(&iodata);
	iodata.index = IO_LED_RUN_R;
	iodata.data = 0;
	set_io(&iodata);
	NOTE_START();
	printf("\e[32m运行灯绿色\e[0m\n");
	NOTE_END();
	ECHO_PAUSE();

	iodata.index = IO_LED_RUN_G;
	iodata.data = 0;
	set_io(&iodata);
	iodata.index = IO_LED_RUN_R;
	iodata.data = 1;
	set_io(&iodata);
	NOTE_START();
	printf("\e[31m运行灯红色\e[0m\n");
	NOTE_END();
	ECHO_PAUSE();
	


	iodata.index = IO_LED_RUN_G;
	iodata.data = 0;
	set_io(&iodata);
	iodata.index = IO_LED_RUN_R;
	iodata.data = 0;
	set_io(&iodata);
	NOTE_START();
	printf("运行灯熄灭\n");
	NOTE_END();
	ECHO_PAUSE();
	

	close_io();
	return 0;
}

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <linux/input.h>

char g_ch = '\0';

void sigalrm_fn(int sig)
{
	int count;
	struct gpio_name ioname;
	struct gpio_dir iodir;
	struct gpio_data iodata;

	system("clear");

	printf("\n\n逐个插拔本机框，查看板卡状态\n");
	//----------------------------------------------------------
	// 读取IO总数，并读出所有端口状态
	open_io(NULL);
	open_io(NULL);
	get_count(&count);
	for (int i = 0; i < count; i++) 
	{		

		ioname.index = i;
		io_name(&ioname);
		printf("io %2.2d name %16s\t", i , ioname.name);

		iodata.index = i;
		iodata.data = 3;
		get_io(&iodata);
		printf("iodata %d\n", iodata.data);
	}
	close_io();
	printf("\n按键盘 'q' 退出\n");
    	alarm(1);
    	
	return;
}

int ti_subdev_status(void *arg)
{
	int pin;
	// 测试代码
	int count = 0;

	struct gpio_name ioname;
	struct gpio_dir iodir;
	struct gpio_data iodata;

	system("clear");
 	signal(SIGALRM, sigalrm_fn);
	alarm(1);
	g_ch = '3';
	while(1) 
	{
		g_ch = getchar();
		if ( (g_ch | 0x20) == 'q') {
			break;
		}		
	}
	signal(SIGALRM, SIG_IGN);

	return 0;
}

int ti_subnet_io(void *arg)
{
	struct gpio_dir iodir;
	struct gpio_data iodata;

	open_io(NULL);
	iodir.index = IO_IP_SET_GPIO;
	iodir.dir = IO_OUTPUT;
	set_dir(&iodir);
	

	system("clear");
	printf("设置网络IP为192.168.1.3\n");
	printf("用万用表测试背板上 IP_SET 引脚状态\n\n");

	
	
	iodata.index = IO_IP_SET_GPIO;
	iodata.data = 1;
	set_io(&iodata);
	NOTE_START();
	printf("IP_SET 是3.3V 告知业务板卡内网处于 192.168.0.0/24 网段\n");
	NOTE_END();
	ECHO_PAUSE();


	iodata.index = IO_IP_SET_GPIO;
	iodata.data = 0;
	set_io(&iodata);

	NOTE_START();
	printf("IP_SET 是  0V 告知业务板卡内网处于 192.168.0.0/24 网段\n");
	NOTE_END();
	ECHO_PAUSE();

	close_io();
	return 0;
}

int ti_sw_reset(void *arg)
{
	struct gpio_dir iodir;
	struct gpio_data iodata;
	
	char ch;

	system("clear");
	printf("注意：如果现在使用网络连接，如下测试将导致“交换机断网”、“业务”板将不能与MCU通信、LAN口不能与MCU连接，是否继续[Y/N]\n");
	
	ch = getchar();
	if ( (ch | 0x20) == 'y' ) {
		goto _Countinue;
	}
	return 0;

_Countinue:;
	open_io(NULL);
	iodir.index = IO_SW_RESET;
	iodir.dir = IO_OUTPUT;
	set_dir(&iodir);
	
	printf("\n");
	printf("\n按任意键将执行如下:\n");
	printf("\t拉低交换机复位信号，万用表可测SW_RESET信号值\n");
	printf("\t交换机所有灯灭，网络断开\n");
	getchar();
	iodata.index = IO_SW_RESET;
	iodata.data = 0;
	set_io(&iodata);
	sleep(1);
	printf("\n按任意键将执行如下:\n");
	printf("\t拉高交换机复位信号，万用表可测SW_RESET得信号值\n");
	printf("\t交换机所有灯瞬间亮起，然后熄灭，如果有设备连接交换机，对应灯亮起\n");
	getchar();
	// TODO
	iodata.index = IO_SW_RESET;
	iodata.data = 1;
	set_io(&iodata);

	close_io();

	return 0;
}

int ti_set_mac(void *arg)
{
	system("clear");
	printf("注意：必须完成 “设置本机时间” 后执行本操作，否则随机MAC可能冲突,\n");
	printf("电脑与MCU可能会断网\n");
	ECHO_PAUSE();
	system("rm /etc/init.d/randmac*.sh  > /dev/null");

	system("/etc/init.d/randhw.sh  > /dev/null");
	system("/etc/init.d/ifconfig-* > /dev/null");

	system("clear");
	printf("\n\n\n设置后对应网卡接口的MAC地址如下\n");
	system("cat /etc/init.d/randmac*.sh | grep ether");
	printf("当前网络状态\n");
	system("ifconfig | grep eth");
	system("ifconfig | grep lan");
	
	printf("如果此时电脑不能ping通过MCU Wlan，通过如下办法解决\n");
	printf("	1. 开始->运行->CMD\n");
	printf("	2. 输入arp -a\n");
	printf("	3. 输入arp -d\n");
	printf("	4. 已经解决，通过电脑ping MCU Wlan口IP\n");

	ECHO_PAUSE();
	return 0;
}

int ti_gpio_output(void *arg)
{
	struct test_item titem[] = {
		{ti_null,(char*)"返回上一级",},
		{ti_key_reset,(char*)"按键复位重启系统",},
		{ti_buzzer,(char*)"蜂鸣器测试",},
		{ti_ledrun,(char*)"运行灯测试",},
		{ti_subnet_io,(char*)"内网网段IO指示",},
		{ti_sw_reset,(char*)"测试交换机复位信号",},
	};
	char *input = (char*)NULL;


	system("clear");
	while(1) {
		list_ti(titem, sizeof(titem) / sizeof(struct test_item));
		
		input = readline((char*)"输出IO测试>");
		if (!input) {
			free(input);
		}
		else {
			if (RET_TI_BACK == select_ti(input, titem, sizeof(titem) / sizeof(struct test_item)) ) {
				break;
			}

			// // 返回上一级
			// if ( (input[0] | 0x20) == 'q') {
			// 	break;
			// }
		}
	}
	return 0;
}


int ti_ck_netcard_active(void *arg)
{
	system("clear");
	printf("\n当前网络状态\n");
	system("ifconfig  | grep \"eth.\\|lan.\\|lo.\\|inet addr:\"");

	NOTE_START();
	printf("WLAN 口对应 wlan0 网卡\n");
	printf("LAN  口对应 lan0  网卡\n");
	printf("如果上面出现 eth0, eth1 等表示网卡没能正确复位，属于硬件故障\n");
	NOTE_END();

	return 0;
}

int ti_ck_netcard_position(void *arg)
{
	struct check_dir checkdir[] = {
		{	(char*)"/sys/class/net/eth0", 0,
			(char*)"ls /sys/class/net/eth0/device -l | awk -F '../../../' \"{print \\$2}\""
		},
		{	(char*)"/sys/class/net/eth1", 0,
			(char*)"ls /sys/class/net/eth1/device -l | awk -F '../../../' \"{print \\$2}\""
		},
		{	(char*)"/sys/class/net/wlan0", -1,
			(char*)"ls /sys/class/net/wlan0/device -l | awk -F '../../../' \"{print \\$2}\""
		},
		{	(char*)"/sys/class/net/lan0", -1,
			(char*)"ls /sys/class/net/lan0/device -l | awk -F '../../../' \"{print \\$2}\""
		},
	};
	DIR *ret;
	bool output = false;
	
	system("clear");
	NOTE_START();
	printf("该功能检验网卡对应的USB接口是否正确，由于改板发生的改动\n"
		"不应该存在eth0,eth1网卡，应该存在wlan0,lan0网卡\n");
	NOTE_END();

	for (int i = 0; i < sizeof(checkdir) / sizeof(struct check_dir); i++) {
		output = false;
		ret = opendir(checkdir[i].dir);
		// printf("ret = %d\n", ret);
		// 希望不存在该目录
		if (checkdir[i].result == 0 && ret == 0) {
			output = true;
		}
		// 希望存在该目录
		else if (checkdir[i].result != 0 && ret != 0){
			output = true;
		}

		switch (output) {
		case true:
			printf("检测 %s	", checkdir[i].dir);
			if (ret != 0) {
				printf("USB网卡位置 :");
				system(checkdir[i].todo);
			}
			RESOULT_OK();
			break;
		default:
			printf("检测 %s	", checkdir[i].dir);
			if (ret != 0) {
				printf("USB网卡位置 :");
				system(checkdir[i].todo);
			}
			RESOULT_NO();
			break;
		}
	}

	struct ck_self ckitem[] = {
		{
			(char*)"检测 Wan0 是否与交换机连接",
			(char*)"cat /sys/class/net/wlan0/carrier",
			(char*)"1",
			COND_EQ,
		},
		{
			(char*)"检测 Lan0 是否有网线连接",
			(char*)"cat /sys/class/net/lan0/carrier",
			(char*)"1",
			COND_EQ,
		},
		
	};
	int r;
	for (int i = 0; i < sizeof(ckitem) / sizeof(struct ck_self); i++) {
		r = ti_ck_mutil(&ckitem[i], 1);
		if (1 == r) {
			RESOULT_NO();
		}
		else {
			RESOULT_OK();
		}
	}

	NOTE_START();
	printf("上面USB网卡位置表示格式：\n");
	printf(
		"1-3.2:1.0\n"
		"--- ^ ---\n"
		" ^  |  ^\n"
		" |  |  |-----usb 1.0，不用关心\n"
		" |  |----- USB HUB 第2端口，关注该项\n"
		" |----- 不用关心\n");
	NOTE_END();
	ECHO_PAUSE();
	return 0;
}

int ti_ck_netcard_subnet(void *arg)
{
	struct check_dir checkdir[] = {
		{(char*)"/sys/class/net/eth0", -1},
		{(char*)"/sys/class/net/eth1", -1},
		{(char*)"/sys/class/net/wlan0", 0},
		{(char*)"/sys/class/net/lan0", 0},
	};
	DIR *ret;
	bool output = false;

	system("clear");
	for (int i = 0; i < sizeof(checkdir) / sizeof(struct check_dir); i++) {
		output = false;
		ret = opendir(checkdir[i].dir);
		// 希望不存在该目录
		if (checkdir[i].result == 0 && ret == 0) {
			output = true;
		}
		// 希望存在该目录
		else if (checkdir[i].result != 0 && ret != 0){
			output = true;
		}

		switch (output) {
		case true:
			printf("检测 %s		\n", checkdir[i].dir);
			RESOULT_NO();
			break;
		default:
			printf("检测 %s		\n", checkdir[i].dir);
			RESOULT_OK();
			break;
		}
	}
	
	return 0;
}
int ti_set_ip(void *arg)
{
	char *input = (char*)NULL;
	char *strip = (char*)NULL;
	char *strmask = (char*)NULL;
	char *strgw = (char*)NULL;
	char strout[256];

	system("clear");
	printf("\n当前网卡信息\n");
	system("ifconfig  | grep \"eth.\\|lan.\\|lo.\\|inet addr:\"");	
	// ECHO_PAUSE();
	NOTE_START();
	printf("设置wlan0网卡，lan0网卡自动配置\n");
	printf("\t1. 如果wlan0处于  192.168.0.0/x网段，lan0自动处于192.168.1.254/24网段\n");
	printf("\t2. 如果wlan0处于非192.168.0.0/x网段，lan0自动处于192.168.0.254/24网段\n");
	NOTE_END();



	while(1) {
		input = readline((char*)"输入IP:");
		if(!input) {
			return -1;
		}
		else {
			strip = input;
			break;
		}
	}

	while(1) {
		input = readline((char*)"输入掩码:");
		if(!input) {
			return -1;
		}
		else {
			strmask = input;
			break;
		}
	}

	while(1) {
		input = readline((char*)"输入网关:");
		if(!input) {
			return -1;
		}
		else {
			strgw = input;
			break;
		}
	}

	if (0 == netIpIsValid(strip)) {
		printf("\tIP输入不合法，必须x.x.x.x\n");
		return -1;
	}
	if (0 == netMaskIsValid(strmask)) {
		printf("\t掩码输入不合法\n");
		return -1;
	}
	
	if (0 == netGwIsValid(strip, strmask, strgw)) {
		printf("\t网关输入不合法，必须和IP在统一网段\n");
		return -1;
	}


	snprintf(strout, 256, "/usr/MenglongWu/bin/netcard.elf %s %s %s",
		strip, strmask, strgw);
	system(strout);
	// 设置IP并配置防火墙
	system("/etc/init.d/ifconfig-eth0");
	system("/etc/init.d/ifconfig-wlan0");
	system("/etc/init.d/ifconfig-eth1");
	system("/etc/init.d/ifconfig-lan0");
	system("/bin/deffirewall");
	free(strip);
	free(strmask);
	free(strgw);

	return 0;
}

int ti_ck_netcard_mount(void *arg)
{
	struct check_dir checkdir[] = {
		{(char*)"/sys/class/net/wlan0", 0},
		{(char*)"/sys/class/net/lan0", 0},
	};
	DIR *ret;
	bool output = false;
	int status = 0;

	system("clear");
	NOTE_START();
	printf("该功能检测wlan0和lan0网卡是否存在，重启网卡后验证网卡是否依旧存在\n");
	NOTE_END();
_Check:;
	
	for (int i = 0; i < sizeof(checkdir) / sizeof(struct check_dir); i++) {
		ret = opendir(checkdir[i].dir);
		// 希望不存在该目录
		if (checkdir[i].result == 0 && ret == 0) {
			output = true;
		}
		// 希望存在该目录
		else if (checkdir[i].result != 0 && ret != 0){
			output = true;
		}

		switch (output) {
		case true:
			printf("检测 %s		\n", checkdir[i].dir);
			RESOULT_NO();
			return 0;
		default:
			printf("检测 %s		\n", checkdir[i].dir);
			RESOULT_OK();
			break;
		}
	}

	switch (status) {
	case 0:
		status++;
		printf("正在重启网卡...\n");
		// todo 控制IO
		sleep(4);
		goto _Check;
		break;
	case 1:
		printf("网卡自动重启功能");
		RESOULT_OK();
		break;
	default:
		break;
	}

	return 0;
}

int ti_netcard(void *arg)
{
	struct test_item titem[] = {
		{ti_null,(char*)"返回上一级",},
		{ti_set_ip,(char*)"设置Wlan IP，并自动配置LAN IP，配置防火墙",},
		{ti_ck_netcard_active,(char*)"检查当前存在的网卡",},
		{ti_ck_netcard_position,(char*)"检查网卡位置",},
		{ti_ck_netcard_subnet,(char*)"检查双网卡IP是否处于不同网段",},
		{ti_ck_netcard_mount,(char*)"检查网卡自动挂载",},
	};
	char *input = (char*)NULL;

	system("clear");
	while(1) {
		list_ti(titem, sizeof(titem) / sizeof(struct test_item));
		
		input = readline((char*)"输出IO测试>");
		if (!input) {
			free(input);
		}
		else {
			if (RET_TI_BACK == select_ti(input, titem, sizeof(titem) / sizeof(struct test_item)) ) {
				break;
			}
		}
	}
	return 0;
}


int ti_settime(void *arg)
{
	char *input = (char*)NULL;
	char strout[128]; 

	system("clear");
	printf("显示当前系统时间:\n\n");
	printf("             ");
	system("date \"+%Y-%m-%d %H:%M:%S\"");
	printf("\n放入电池，设置本机时间，格式：(20xx-mm-dd hh:mm)\n\n");

_Again:;
	input = readline((char*)"输入时间:   ");
	// printf("%d\n", input[0]);
	if (!input) {
		free(input);
	}
	else if (input[0] == '\0') {
		goto _Again;
	}
	else {
		snprintf(strout, 128, "/bin/settime.sh \"%s\"", input);
		system(strout);
	}
	printf("\n检查时间是否正确：(如果不正确请重新校时)\n\n");
	printf("             ");
	system("date \"+%Y-%m-%d %H:%M:%S\"");
	ECHO_PAUSE();
	return 0;	
}




int ti_sms(void *arg)
{
	return 0;
}
struct test_item g_titem[] = {
	{ti_null,(char*)"退出系统",},
	{ti_ck_self, (char*)"自检"},
	{ti_settime,(char*)"设置本机时间",},
	{ti_gpio_output,(char*)"输出IO测试",},
	{ti_netcard,(char*)"USB网卡信息",},
	{ti_subdev_status,(char*)"测试业务办插拔状态",},
	{ti_set_mac,(char*)"随机生成本机网卡MAC地址",},
	{ti_sms,(char*)"测试短信模块",}
	

};


#include <termios.h>
#include <unistd.h>
static struct termios stored_settings;
 
void set_keypress(void)                               //设置终端为RAW模式，并关闭回显
{
    struct termios new_settings;
    tcgetattr(0,&stored_settings);
    new_settings = stored_settings;
   
    new_settings.c_lflag &= (~ICANON);
    // new_settings.c_lflag &= (~ECHO);
    new_settings.c_cc[VTIME] = 0;
    new_settings.c_cc[VMIN] = 1;
    tcsetattr(0,TCSANOW,&new_settings);
    return;
}
  
void reset_keypress(void)                                //恢复终端属性

{
    tcsetattr(0,TCSANOW,&stored_settings);
    return;
}

int alarm_buzzer(void)
{
	struct gpio_dir iodir;
	struct gpio_data iodata;

	int io_index;

#if (CONFIG_BOARD == 11)
	io_index = IO_LED_ALARM_TOTAL;
#elif (CONFIG_BOARD == 12)
	io_index = IO_LED_BUZZER;
#endif
	open_io(NULL);
	// 复位
	iodir.index = io_index;
	iodir.dir = IO_OUTPUT;
	set_dir(&iodir);

	for (int i = 0; i < 3; i++) {
		iodata.index = io_index;
		iodata.data = 1;
		set_io(&iodata);
		
		usleep(1000*500);
		iodata.index = io_index;
		iodata.data = 0;
		set_io(&iodata);
		usleep(1000*400);	
	}
	
	close_io();
	
	return 0;
}
void Introduct()
{
	system("clear");
	printf(	
		"|--------------------------------------------------------|\n"
		"|                                                        |\n"
		"|             TMSxxTC 6U设备生产测试工具                 |\n"
		"|                                                        |\n"
		"|--------------------------------------------------------|\n"
	// printf(	
		"注意:\n"
		"\t该工具仅用于 'TMSxx项目 MCU 板卡' 功能检测，随着后期开发\n"
		"\t变动软件也需相应升级，使用前先检测 '适应电路板' 是否同手\n"
		"\t上的电路板型号\n");
	printf("软件信息:\n"
		"\t软件名  	%s\n"
		"\t版本    	%d.%d.%d\n"
		"\t编译时间	%s\n\n",
		PRJ_NAME,
		PRJ_VERSION,PRJ_PATCHLEVEL,PRJ_SUBLEVEL,
		BUILD_DATE);
	// printf("%s  %d %d %d",PRJ_NAME,
	// 	PRJ_VERSION,PRJ_PATCHLEVEL,PRJ_SUBLEVEL);
	printf("适应电路板:\n");
#if (CONFIG_BOARD == 11)
	printf("\tJW-CUV1.1\n");
#endif

#if (CONFIG_BOARD == 12) 
	printf("\tJW-CUV1.2\n");
#endif

}
int main(int argc,char *argv[])
{
	int ret;
	char buf[40] = {0};
	char argv1;
	int i;
	int ms;
	int pin,onoff;
	// char strout[16];

	if (argc == 2 && memcmp(argv[1], "-s", strlen(argv[1])) == 0) {
		int a = 2;
		if (0 != ti_ck_self(&a)) {
			alarm_buzzer();
		}
		return 0;
	}
	// 关闭控制台 printf 行缓冲
	setvbuf(stdout, "", _IONBF, 0);
	set_keypress();
	// ti_subdev_status(NULL);
	char shell_prompt[256];
	char *input = (char*)NULL;
	Introduct();
	while(1) {
		list_ti(g_titem, sizeof(g_titem) / sizeof(struct test_item));

		if(input) {
			free(input);
			input = NULL;
		}
		input= readline((char*)"ProduceTest>");
		if(!input) {
			return -1;
		}
		if (*input != '\0') {
			add_history(input);
			// if(0 == strcmp(input,"quit")) {
			// 	printf("\r\n");
			// 	break;
			// }
		}
		if (RET_TI_BACK ==  select_ti(input, g_titem, sizeof(g_titem) / sizeof(struct test_item))) {

			break;
		}
		// select_ti(strout);
		// strout[0] = '\0';
	}
	return 0;


	return 0;
}
