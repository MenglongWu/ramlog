/**
 ******************************************************************************
 * @file	TMSxxTC\drivers\test\src\gpio.c
 * @brief	应用层到调用GPIO的接口，需要tms_gpio.ko的支持

 *
 @section exynos-4412

 @section Library
	-# 
- 2015-9,Menglong Woo,MenglongWoo@aliyun.com
 	
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <linux/watchdog.h>
#include "tms_gpio.h"
extern void led_print(void);


#define TEST_LED 1


static int fd = 0;




int get_count(int *val)
{
	ioctl(fd, CMD_GET_COUNT, val);
	return 0;	
}

int get_io(struct gpio_data *val)
{
	ioctl(fd, CMD_GET_VAL, val);
	return 0;	
}


int io_name(struct gpio_name *val)
{
	ioctl(fd, CMD_IO_NAME, val);

	// printf("strout %s\n", val->name);

	// char strout[33];
	// int ret = ioctl(fd, CMD_IO_NAME, &myval);
	// printf("ret %d strout %s\n", ret ,strout);
	return 0;
}
int set_io(struct gpio_data *val)
{
	ioctl(fd, CMD_SET_VAL, val);
	return 0;
}

int set_dir(struct gpio_dir *val)
{
	ioctl(fd, CMD_SET_DIR, val);
	return 0;
}
int open_io(char *file)
{
	if (file == NULL) {
		fd = open("/dev/tms_gpio0", O_RDWR);	
	}
	else {
		fd = open(file, O_RDWR);
	}
	
	// /dev/leds-wml0
	if(fd < 0) {
		printf("open device tms_gpio0\n");
		// exit(1);
	}
	return fd;
}

int close_io()
{
	close(fd);
	return 0;
}
