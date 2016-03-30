#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "sys/ioctl.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <linux/watchdog.h>
#include "tms_gpio.h"

int get_count(int *val);
int get_io(struct gpio_data *val);
int io_name(struct gpio_name *val);
int set_io(struct gpio_data *val);
int set_dir(struct gpio_dir *val);
int open_io(char *file);
int close_io();

int main(int argc,char *argv[])
{
	// int ret;
	// char buf[40] = {0};
	// char argv1;
	// int i;
	// int ms;
	int pin,onoff;


	if(argc == 0) {
		printf("Usage:\n");
		printf("\telf pin on/off\n");
		exit(1);
	}

	// fd = open("/dev/leds",O_WRONLY);
	// fd = open("/dev/tms_gpio0",O_WRONLY);
	// fd = open("/dev/tms_gpio0", O_RDWR);
	

	// // /dev/leds-wml0
	// if(fd < 0) {
	// 	printf("open device tms_gpio0\n");
	// 	exit(1);
	// }
	

	// 测试代码
	int count = 0;
	
	struct gpio_name ioname;
	struct gpio_dir iodir;
	struct gpio_data iodata;


	open_io(NULL);
	//----------------------------------------------------------
	// 看到端口闪烁
	// 修改端口为输出口，并设置端口电平
	if (argc == 3) {
		pin = atoi(argv[1]);
		onoff = atoi(argv[2]);

		iodir.index = pin;
		iodir.dir = IO_OUTPUT;
		set_dir(&iodir);

		// 亮
		iodata.index = pin;
		iodata.data = onoff;
		set_io(&iodata);
		iodata.data = 3;
		get_io(&iodata);
		// printf("\nset led output iodate: 1    get led state iodata %d\n", iodata.data);
		return 0;
	}
	

	//----------------------------------------------------------
	// 读取IO总数，并读出所有端口状态
	
	get_count(&count);
	printf("count %d\n\n", count);
	for (int i = 0; i < count; i++) 
	{
		pin = i;		

		ioname.index = pin;
		io_name(&ioname);
		printf("io %2.2d name %16s\t", pin , ioname.name);

		iodata.index = pin;
		iodata.data = 3;
		get_io(&iodata);
		printf("iodata %d\n", iodata.data);
	}

	
	return 0;
	sleep(1);

	// 灭
	iodata.index = 21;
	iodata.data = 0;
	set_io(&iodata);
	iodata.data = 3;
	get_io(&iodata);
	printf("set led output iodate: 0    get led state iodata %d\n", iodata.data);
	sleep(1);

	close_io();
	return 0;
}
