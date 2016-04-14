#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "sys/ioctl.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "unistd.h"
#include <linux/watchdog.h>
#include "tms_gpio.h"
#include "ckself.h"
#include "producetest.h"

#include <linux/input.h>

#include "pthread.h"

#define DEV_INPUT "/dev/input/mice"

int get_count(int *val);
int get_io(struct gpio_data *val);
int io_name(struct gpio_name *val);
int set_io(struct gpio_data *val);
int set_dir(struct gpio_dir *val);
int open_io(char *file);
int close_io();


void TickStatusIO()
{
	struct ck_self ckitem[] = {
		{
			"",//(char*)"Checking Lan0 status",
			(char *)"cat /sys/class/net/lan0/carrier",
			(char *)"1",
			COND_EQ,
		},
	};

	struct check_dir checkdir[] = {
		{
			(char *)"/sys/class/net/lan0/", -1,
			(char *)"ls /sys/class/net/lan0/device -l | awk -F '../../../' \"{print \\$2}\""
		},
	};
	struct ck_self cklan0down[] = {
		{
			"",//(char*)"Checking Lan0 status",
			(char *)"cat /sys/class/net/lan0/operstate",
			(char *)"up",
			COND_EQ,
		},
	};
	struct ck_self ckwlan0down[] = {
		{
			"",//(char*)"Checking Lan0 status",
			(char *)"cat /sys/class/net/wlan0/operstate",
			(char *)"up",
			COND_EQ,
		},
	};

	struct gpio_data iodata;
	int ret;


	bool output = false;



	output = false;
	ret = (int)(DIR *)opendir(checkdir[0].dir);
	closedir(ret);
	// printf("ret = %d\n", ret);
	// 希望不存在该目录
	if (checkdir[0].result == 0 && ret == 0) {
		output = true;
	}
	// 希望存在该目录
	else if (checkdir[0].result != 0 && ret != 0) {
		output = true;
	}
	if (output == false) {
		printf("\n\n-------- restart net hub --------\n");
		open_io(NULL);
		iodata.index = IO_RST_NET;
		iodata.data = 0;
		set_io(&iodata);

		// iodata.index = IO_RST_NET;
		// iodata.data = 0;
		// set_io(&iodata);
		sleep(1);
		iodata.index = IO_RST_NET;
		iodata.data = 1;
		set_io(&iodata);

		// iodata.index = IO_RST_NET;
		// iodata.data = 1;
		// set_io(&iodata);
		close_io();

		return ;
	}

	ret = ti_ck_mutil(&cklan0down[0], 1);
	if (1 == ret) {
		printf("\n\n-------- turn up lan0 --------\n");
		system("ifconfig lan0 down");
		sleep(1);
		system("ifconfig lan0 up");
	}

	ret = ti_ck_mutil(&ckwlan0down[0], 1);
	if (1 == ret) {
		printf("\n\n-------- turn up wlan0 --------\n");
		system("ifconfig wlan0 down");
		sleep(1);
		system("ifconfig wlan0 up");
	}


	ret = ti_ck_mutil(&ckitem[0], 1);
	if (1 == ret) {
		// RESOULT_NO_EN();
		printf("\n\n-------- restart switch --------\n");

		open_io(NULL);
		iodata.index = IO_SW_RESET;
		iodata.data = 0;
		set_io(&iodata);

		// iodata.index = IO_RST_NET;
		// iodata.data = 0;
		// set_io(&iodata);
		sleep(1);
		iodata.index = IO_SW_RESET;
		iodata.data = 1;
		set_io(&iodata);

		// iodata.index = IO_RST_NET;
		// iodata.data = 1;
		// set_io(&iodata);
		close_io();

		sleep(3);
		// 强制将端口UP
		system("ifconfig lan0 down ");
		// system("echo down ");
		sleep(3);
		system("ifconfig lan0 up ");
		// system("echo up");
		// sleep(3);
		// system("/etc/init.d/ifconfig-lan0");

	}
	else {
		// RESOULT_OK_EN();
	}
}


// 自动检测 /dev/input 输入，如果30s内没有任何输入关闭LCD背光
// 检测间隔为1s
static pthread_t pth_blacklight;///<预留
void *ThreadBlacklight(void *arg)
{
	int hid_down = 0;
	int fd;
	struct input_event touch_key;

	// fd = open(DEV_INPUT, O_RDWR);
	fd = open(DEV_INPUT, O_RDONLY | O_NONBLOCK);
	if (fd < 0) {
		// printf("fail open device %s\n", DEV_INPUT);
	}
	else {
		// printf("success open device %s\n", DEV_INPUT);
	}

	int len;
	while(1) {
		for (int i = 0; i < 30; i++) {
			len = read(fd, &touch_key, sizeof(touch_key));
			// printf("len %d\n", len);
			if (len != -1) {
				hid_down = 1;
				break;
			}
			// printf(".\n");
			// usleep(500000);// 100ms
			sleep(1);
		}
		if (hid_down) {
			system("echo 0 > /sys/class/backlight/pwm-backlight.0/bl_power");
			// printf("bl on\n");
		}
		else {
			system("echo 1 > /sys/class/backlight/pwm-backlight.0/bl_power");
			// printf("bl off\n");
		}
		hid_down = 0;
	}
	close(fd);
}

int main(int argc, char *argv[])
{

	// int ret;
	// char buf[40] = {0};
	// int i;
	// int ms;
	// int pin, onoff;


	open_io(NULL);

	// struct gpio_name ioname;
	struct gpio_dir iodir;
	struct gpio_data iodata;

	//----------------------------------------------------------
	// 读取IO总数，并读出所有端口状态

	//----------------------------------------------------------
	// 看到端口闪烁
	// 修改端口为输出口，并设置端口电平


	printf("-----------------------------\n"
	       "close alarm\n"
	       "-----------------------------\n");

	// 关闭告警铃声
	iodir.index = IO_LED_ALARM_TOTAL;
	iodir.dir = IO_OUTPUT;
	set_dir(&iodir);

	// 亮
	iodata.index = IO_LED_ALARM_TOTAL;
	iodata.data = 0;
	set_io(&iodata);


	iodir.index = IO_LED_RUN;
	iodir.dir = IO_OUTPUT;
	set_dir(&iodir);
	iodata.index = IO_LED_RUN;
	iodata.data = 0;
	set_io(&iodata);

	iodir.index = IO_LED_ALARM_COMM;
	iodir.dir = IO_OUTPUT;
	set_dir(&iodir);
	iodata.index = IO_LED_ALARM_COMM;
	iodata.data = 0;
	set_io(&iodata);




	// 复位GSM、交换机 H - L - H
	iodir.index = IO_SW_RESET;
	iodir.dir = IO_OUTPUT;
	set_dir(&iodir);

	iodir.index = IO_GSM_EN;
	iodir.dir = IO_OUTPUT;
	set_dir(&iodir);

	iodata.index = IO_SW_RESET;
	iodata.data = 0;
	set_io(&iodata);

	iodata.index = IO_GSM_EN;
	iodata.data = 1;
	set_io(&iodata);
	sleep(1);


	iodata.index = IO_GSM_EN;
	iodata.data = 0;
	set_io(&iodata);

	iodata.index = IO_SW_RESET;
	iodata.data = 1;
	set_io(&iodata);

	iodata.index = IO_GSM_EN;
	iodata.data = 1;
	set_io(&iodata);


	// printf("set led output iodate: 0    get led state iodata %d\n", iodata.data);
	close_io();


	pthread_create(&pth_blacklight, NULL, &ThreadBlacklight, NULL);
	while(1) {
		TickStatusIO();
		sleep(15);

	}
	pthread_cancel(pth_blacklight);
	pthread_join(pth_blacklight, 0);
	return 0;
}
