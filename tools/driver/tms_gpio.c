/**
 ******************************************************************************
 * @file	tms_gpio.c
 * @brief	TMSxx V1.2 公司自己抄的 4412 底板

主要控制通用 IO 管脚，是个测试驱动。
所有 IO 在使用前均先释放（流氓）应为这些 IO 可能已经被内核编译给其他外设使用，但
该外设对本平台无任何意义，所以先强制释放 IO 资源\n\n

建议平台分成3部分，驱动、底层无关dll、应用程序，该程序为 “驱动” 部分
 *
 @section Platform
	-# Exynos 4412 迅维 全功能板

- 2015-07-23,MenglongWoo,MenglongWoo@163.com
 	- 测试工程，代码可能有点凌乱，

*/

// copy from itop4412_leds.c
#include <linux/init.h>
#include <linux/module.h>

#include <linux/kernel.h>
#include <linux/fs.h>
#include <mach/gpio.h>
#include <plat/gpio-cfg.h>
#include <linux/miscdevice.h>
#include <linux/platform_device.h>
//#include <mach/gpio-bank.h>
#include <mach/regs-gpio.h>
#include <asm/io.h>
#include <linux/regulator/consumer.h>
//#include "gps.h"
#include <linux/delay.h>

#include <asm/uaccess.h>
#include "tms_gpio.h"

#ifdef _DEBUG_
// #define dbg_print(type, format, args...) printk(type, format, ##args) 
#define dm9620_print(dev, format, args...) devdbg(dev, format, ##args)
#else
#define dbg_print(type, format, args...) 
#endif

/*
信号名称		I/O接口			备注							GPIO			输入/输出

IP_SET			CAM_HREF		输出,设置IP段					GPJ0_2			out
UNIT16-220V		CAM_D5			输入，若低16槽为交流电源		GPJ1_0			in
UNIT15-220V		CAM_D7			输入，若低15槽为交流电源		GPJ1_2			in
UNIT16-48V		CAM_PCLK		输入，若低16槽为直流电源		GPJ0_0			in
UNIT15-48V		CAM5M_RST		输入，若低15槽为直流电源		GPL1_1			in
UNIT16-12V		CAM_D1			输入，若低16槽12v正常			GPJ0_4			in
UNIT15-12V		CAM_D3			输入，若低15槽12v正常			GPJ0_6			in
UNIT-13			CAM_MCLK		输入，若低13槽单元存在			GPJ1_3			in
UNIT-12			CAM_D0			输入，若低12槽单元存在			GPJ0_3			in
UNIT-11			CAM_D2			输入，若低11槽单元存在			GPJ0_5			in
UNIT-10			CAM_D4			输入，若低10槽单元存在			GPJ0_7			in
UNIT-9			CAM_D6			输入，若低9槽单元存在			GPJ1_1			in
UNIT-8			xispSPIMOSI		输入，若低8槽单元存在			GPM4_7			in
UNIT-7			CAM_VSYNC		输入，若低7槽单元存在			GPJ0_1			in
UNIT-6			CHG_UOK			输入，若低6槽单元存在			GPX1_5			in
UNIT-5			XEINT1_BAK		输入，若低5槽单元存在			GPX0_1			in
UNIT-4			XEINT14_BAK		输入，若低4槽单元存在			GPX1_6			in
UNIT-3			xispSPICLK		输入，若低3槽单元存在			GPM4_4			in
UNIT-2			GM_INT2			输入，若低2槽单元存在			GPX3_1			in
UNIT-1			GM_INT1			输入，若低1槽单元存在			GPX3_0			in
复位按键		PWRON			输入，低有效					---				in
消音按键		CHG_FLT			输入，低有效					GPX1_0			in
告警输出		BK_LED			输出，有告警输出高电平			GPC1_2			out
运行灯			GPC1_1			输出，正常输出脉冲信号			GPC1_1			out
通信告警		I2C_SCL6		输出，有告警输出高电平			GPC1_4			out
串口出			XuTXD2			输出，串口						GPA1_1			out
串口入			XuRXD2			输入，串口						GPA1_0			in
GSM出			GPS_RXD			输出，GSM						GPA1_5			out
GSM入			GPS_TXD			输入，GSM						GPA1_4			in
GSM控制IGT		GPS_RST			输出，上电后给出100ms低电平		GPB_4			out
GSM控制off		GPS33_EN		输出，如要复位给1s低电平		GPK2_2			out
交换机复位		BGF_INT			输出，如要复位给 200 ms低电平	GPX2_4			out
USB-HUBER-DP	UH_DP3			USB-HUBER 主接口				-
USB-HUBER-DM	UH_DM3			USB-HUBER 主接口				-
*/


#if (CONFIG_BOARD == 11)
#define IP_SET_GPIO				(  EXYNOS4212_GPJ0(2)  )		// 	out
#define UNIT16_220V_GPIO		(  EXYNOS4212_GPJ1(0)  )		// 	in
#define UNIT15_220V_GPIO		(  EXYNOS4212_GPJ1(2)  )		// 	in
#define UNIT16_48V_GPIO			(  EXYNOS4212_GPJ0(0)  )		// 	in
#define UNIT15_48V_GPIO			(  EXYNOS4_GPL1(1)     )			// 	in
#define UNIT16_12V_GPIO			(  EXYNOS4212_GPJ0(4)  )		// 	in
#define UNIT15_12V_GPIO			(  EXYNOS4212_GPJ0(6)  )		// 	in
#define UNIT_13_GPIO			(  EXYNOS4212_GPJ1(3)  )		// 	in
#define UNIT_12_GPIO			(  EXYNOS4212_GPJ0(3)  )		// 	in
#define UNIT_11_GPIO			(  EXYNOS4212_GPJ0(5)  )		// 	in
#define UNIT_10_GPIO			(  EXYNOS4212_GPJ0(7)  )		// 	in
#define UNIT_9_GPIO				(  EXYNOS4212_GPJ1(1)  )		// 	in
#define UNIT_8_GPIO				(  EXYNOS4212_GPM4(7)  )		// 	in
#define UNIT_7_GPIO				(  EXYNOS4212_GPJ0(1)  )		// 	in
#define UNIT_6_GPIO				(  EXYNOS4_GPX1(5)     )			// in
#define UNIT_5_GPIO				(  EXYNOS4_GPX0(1)     )			// in
#define UNIT_4_GPIO				(  EXYNOS4_GPX1(6)     )			// in
#define UNIT_3_GPIO				(  EXYNOS4212_GPM4(4)  )		// in
#define UNIT_2_GPIO				(  EXYNOS4_GPX3(1)     )			// in
#define UNIT_1_GPIO				(  EXYNOS4_GPX3(0)     )			// in
#define KEY_ALARM				(  EXYNOS4_GPX1(0)     )			// in
#define LED_ALARM_TOTAL			(  EXYNOS4_GPC1(2)     )			// out
#define LED_RUN					(  EXYNOS4_GPC1(1)     )			// out
#define LED_ALARM_COMM			(  EXYNOS4212_GPJ1(4)   )			// out
#define GSM_RST					(  EXYNOS4_GPB(4)      )			// out
#define GSM_EN					(  EXYNOS4_GPK2(2)     )			// out
#define SW_RESET				(  EXYNOS4_GPX2(4)     )			// out
// #define TOUCH_EN				(  EXYNOS4_GPL0(2)     )			// out

#elif (CONFIG_BOARD == 12)
#define IP_SET_GPIO				(  EXYNOS4212_GPJ0(2)  )		// 	out
#define UNIT16_220V_GPIO		(  EXYNOS4212_GPJ1(0)  )		// 	in
#define UNIT15_220V_GPIO		(  EXYNOS4212_GPJ1(2)  )		// 	in
#define UNIT16_48V_GPIO			(  EXYNOS4212_GPJ0(0)  )		// 	in
#define UNIT15_48V_GPIO			(  EXYNOS4_GPL1(1)     )			// 	in
#define UNIT16_12V_GPIO			(  EXYNOS4212_GPJ0(4)  )		// 	in
#define UNIT15_12V_GPIO			(  EXYNOS4212_GPJ0(6)  )		// 	in
#define UNIT_13_GPIO			(  EXYNOS4212_GPJ1(3)  )		// 	in
#define UNIT_12_GPIO			(  EXYNOS4212_GPJ0(3)  )		// 	in
#define UNIT_11_GPIO			(  EXYNOS4212_GPJ0(5)  )		// 	in
#define UNIT_10_GPIO			(  EXYNOS4212_GPJ0(7)  )		// 	in
#define UNIT_9_GPIO				(  EXYNOS4212_GPJ1(1)  )		// 	in
#define UNIT_8_GPIO				(  EXYNOS4212_GPM4(7)  )		// 	in
#define UNIT_7_GPIO				(  EXYNOS4212_GPJ0(1)  )		// 	in
#define UNIT_6_GPIO				(  EXYNOS4_GPX1(5)     )			// in
#define UNIT_5_GPIO				(  EXYNOS4_GPX0(1)     )			// in
#define UNIT_4_GPIO				(  EXYNOS4_GPX1(6)     )			// in
#define UNIT_3_GPIO				(  EXYNOS4212_GPM4(4)  )		// in
#define UNIT_2_GPIO				(  EXYNOS4_GPX3(1)     )			// in
#define UNIT_1_GPIO				(  EXYNOS4_GPX3(0)     )			// in
#define KEY_ALARM				(  EXYNOS4_GPX1(0)     )			// in
#define LED_ALARM_TOTAL			(  EXYNOS4_GPC1(2)     )			// out
#define LED_RUN					(  EXYNOS4_GPC1(1)     )			// out
#define LED_ALARM_COMM			(  EXYNOS4_GPX3(3)   )			// out
#define GSM_RST					(  EXYNOS4_GPB(4)      )			// out
#define GSM_EN					(  EXYNOS4_GPK2(2)     )			// out
#define SW_RESET				(  EXYNOS4_GPX2(4)     )			// out
// #define TOUCH_EN				(  EXYNOS4_GPL0(2)     )			// out


#define UNSUE0				(  EXYNOS4_GPX3(4)     )			// out
#define RST_NET					(EXYNOS4212_GPJ1(4))			// out
#define LED_BUZZER				(  EXYNOS4_GPX0(6)     )			// out


#endif




ssize_t  led_write(struct file *filp, const char __user *buf, size_t size, loff_t *ppos);
ssize_t  led_read(struct file *filp, char __user *buf, size_t size, loff_t *ppos);
int  led_open(struct inode *inode, struct file *filp);
int  led_release(struct inode *inode, struct file *filp);
long led_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);
static struct file_operations ledfops = 
{
 	.read = led_read,
 	.write = led_write,
 	.open = led_open,
 	.release = led_release,
 	.unlocked_ioctl = led_ioctl,
};
#define GPIO_HUB_RESET EXYNOS4_GPL2(2)
#define GPIO_HUB_CONNECT EXYNOS4_GPK3(2)


static struct gpio_resource gpios_tbl[] = {
	{IP_SET_GPIO,		"IP_SET_GPIO",			IO_OUTPUT},
	{UNIT16_220V_GPIO,		"UNIT16_220V_GPIO",			IO_INPUT},
	{UNIT15_220V_GPIO,		"UNIT15_220V_GPIO",			IO_INPUT},
	{UNIT16_48V_GPIO,		"UNIT16_48V_GPIO",			IO_INPUT},
	{UNIT15_48V_GPIO,		"UNIT15_48V_GPIO",			IO_INPUT},
	{UNIT16_12V_GPIO,		"UNIT16_12V_GPIO",			IO_INPUT},
	{UNIT15_12V_GPIO,		"UNIT15_12V_GPIO",			IO_INPUT},
	{UNIT_13_GPIO,		"UNIT_13_GPIO",			IO_INPUT},
	{UNIT_12_GPIO,		"UNIT_12_GPIO",			IO_INPUT},
	{UNIT_11_GPIO,		"UNIT_11_GPIO",			IO_INPUT},
	{UNIT_10_GPIO,		"UNIT_10_GPIO",			IO_INPUT},
	{UNIT_9_GPIO,		"UNIT_9_GPIO",			IO_INPUT},
	{UNIT_8_GPIO,		"UNIT_8_GPIO",			IO_INPUT},
	{UNIT_7_GPIO,		"UNIT_7_GPIO",			IO_INPUT},
	{UNIT_6_GPIO,		"UNIT_6_GPIO",			IO_INPUT},
	{UNIT_5_GPIO,		"UNIT_5_GPIO",			IO_INPUT},
	{UNIT_4_GPIO,		"UNIT_4_GPIO",			IO_INPUT},
	{UNIT_3_GPIO,		"UNIT_3_GPIO",			IO_INPUT},
	{UNIT_2_GPIO,		"UNIT_2_GPIO",			IO_INPUT},
	{UNIT_1_GPIO,		"UNIT_1_GPIO",			IO_INPUT},
	{KEY_ALARM,		"KEY_ALARM",			IO_INPUT},
	{LED_ALARM_TOTAL,		"LED_ALARM_TOTAL",			IO_OUTPUT},
	{LED_RUN,		"LED_RUN",			IO_OUTPUT},
#ifdef RST_NET
	{RST_NET,		"RST_NET",			IO_OUTPUT},
#endif
	{LED_ALARM_COMM,		"LED_ALARM_COMM",			IO_OUTPUT},
	{GSM_RST,		"GSM_RST",			IO_OUTPUT},
	{GSM_EN,		"GSM_EN",			IO_OUTPUT},
	{SW_RESET,		"SW_RESET",			IO_OUTPUT},
#ifdef TOUCH_EN
	{TOUCH_EN,		"TOUCH_EN",			IO_OUTPUT},
#endif
#ifdef UNSUE0
	{UNSUE0,		"UNSUE0",			IO_OUTPUT},
#endif

#ifdef LED_BUZZER
	{LED_BUZZER,		"LED_BUZZER",			IO_OUTPUT},
#endif
};

#if 0
static struct gpio_resource gpios_old_tbl[] = {
	// EXYNOS4_GPX2(5),
	// EXYNOS4_GPX0(1),

	EXYNOS4_GPC1(2),"GPC1_2",
	EXYNOS4_GPC1(3),"GPC1_3",
	EXYNOS4_GPC1(4),"GPC1_4",
	EXYNOS4_GPC1(1),"GPC1_1",
	EXYNOS4_GPX2(5),"GPX2_5",


	EXYNOS4_GPX0(1),"GPX0_1",
	EXYNOS4_GPX1(6),"GPX1_6",
	EXYNOS4_GPX2(4),"GPX2_4",
	EXYNOS4212_GPM4(4),"GPM4_4",
	EXYNOS4212_GPM4(7),"GPM4_7",
	EXYNOS4_GPX1(5),"GPX1_5",//这个注册失败
	EXYNOS4_GPX2(7),"GPX2_7",
	EXYNOS4_GPX1(0),"GPX1_0",

	// EXYNOS4_GPC0(1),"dm9620_reset",

	EXYNOS4212_GPJ0(0),"GPJ0_0",
	EXYNOS4212_GPJ0(1),"GPJ0_1",
	EXYNOS4212_GPJ0(2),"GPJ0_2",
	EXYNOS4212_GPJ0(3),"GPJ0_3",
	EXYNOS4212_GPJ0(4),"GPJ0_4",
	EXYNOS4212_GPJ0(5),"GPJ0_5",
	EXYNOS4212_GPJ0(6),"GPJ0_6",
	EXYNOS4212_GPJ0(7),"GPJ0_7",

	EXYNOS4212_GPJ1(0),"GPJ1_0",
	EXYNOS4212_GPJ1(1),"GPJ1_1",
	EXYNOS4212_GPJ1(2),"GPJ1_2",
	EXYNOS4212_GPJ1(3),"GPJ1_3",

	EXYNOS4_GPL0(1),"gpx1_0",
};
#endif

#define IO_NUM		ARRAY_SIZE(gpios_tbl)
#define DRIVER_NAME "tms_gpio"


// end copy



#include <linux/module.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/io.h>
#include <linux/device.h>
#include <asm/io.h>






//定义物理地址f的
#define GPBCON               0x56000010
#define GPBDAT               0x56000014
#define GPBUP                0x56000018


struct class *myclass;
struct cdev leddev;
dev_t devno;

unsigned int *rGPBCON;
unsigned int *rGPBDAT;
unsigned int *rGPBUP;



ssize_t  led_read(struct file *filp, char __user *buf, size_t size, loff_t *ppos)
{
	return 0;
	// char val[ IO_NUM];
	// int len,i;
	// int ret;

	// dbg_print(KERN_NOTICE "read led\n");
	// for(i=0; i< IO_NUM; i++) {
	// 	val[i] = gpio_get_value(gpios_tbl[i].gpio);
	// }

	// if ( IO_NUM > size) {
	// 	len = size;
	// }
	// else {
	// 	len =  IO_NUM;
	// }
	
	// ret = copy_to_user(buf, val, len);
	// if (ret != 0) {
	// 	return RET_MEM_ERR;
	// }
	// return len;
}
ssize_t  led_write(struct file *filp, const char __user *buf, size_t size, loff_t *ppos)
{	
	return 0;
}
int  led_open(struct inode *inode, struct file *filp)
{
	// unsigned long tmpreg; 
	// rGPBCON = ioremap(GPBCON,4);
	// rGPBDAT = ioremap(GPBDAT,4);
	// rGPBUP  = ioremap(GPBUP,4);


	// //GPB:5\6\7\8为输出口
	// tmpreg = readl(rGPBCON);
	// tmpreg |=  ((1 << 10) | (1 << 12) | (1 << 14) | (1 << 16));
	// tmpreg &= ~((1 << 11) | (1 << 13) | (1 << 15) | (1 << 17));
	// writel(tmpreg,rGPBCON);

	// //GPB:5\6\7\8使能弱上拉
	// tmpreg = readl(rGPBUP);
	// tmpreg &= ~((1 << 5) | (1 << 6) | (1 << 7) | (1 << 8));
	// writel(tmpreg,rGPBUP);

	// //GPB:5\6\7\8关闭所有LED
	// tmpreg = readl(rGPBDAT);
	// tmpreg |= ((1 << 5) | (1 << 6) | (1 << 7) | (1 << 8));
	// writel(tmpreg,rGPBDAT);
	return 0;
}


int get_count(unsigned long arg)
{
	int __user *argp = (int __user*)arg;
	int ret;
	int sz = ARRAY_SIZE(gpios_tbl);
	
	ret = copy_to_user(argp, &sz, sizeof(int));
	if (ret != 0) {
		return RET_MEM_ERR;
	}

	return RET_SUCCESS;
}

// 对于方向输入端口，调用该方法不能改变端口电平
int set_dir(unsigned long arg)
{
	struct gpio_dir __user *argp = (struct gpio_dir __user*)arg;
	struct gpio_dir input;
	int ret;

	ret = copy_from_user(&input, argp, sizeof(struct gpio_dir));
	if (ret != 0) {
		return RET_MEM_ERR;
	}

	if (input.index >  IO_NUM) {
		return RET_INVAILD;
	}
	

	switch(input.dir) {
	case IO_OUTPUT:
		s3c_gpio_cfgpin(gpios_tbl[input.index].gpio, S3C_GPIO_OUTPUT);
		break;
	default :// INPUT
		s3c_gpio_cfgpin(gpios_tbl[input.index].gpio, S3C_GPIO_INPUT);
		break;
	}

	return RET_SUCCESS;
}

int get_dir(unsigned long arg)
{
	// struct gpio_data __user *argp = (struct gpio_data __user*)arg;
	// struct gpio_data input;
	

	// copy_from_user(&input, argp, sizeof(struct gpio_data));
	// if (input.index >  IO_NUM) {
	// 	return RET_INVAILD;
	// }

	// input.data = gpio_get_value(gpios_tbl[input.index].gpio);
	// copy_to_user(&argp->data, &input.data, sizeof(int));
	return RET_SUCCESS;
}

int set_val(unsigned long arg)
{
	struct gpio_data __user *argp = (struct gpio_data __user*)arg;
	struct gpio_data input;
	int ret;

	ret = copy_from_user(&input, argp, sizeof(struct gpio_data));
	if (ret != 0) {
		return RET_MEM_ERR;
	}
	if (input.index >  IO_NUM) {
		return RET_INVAILD;
	}
	gpio_set_value(gpios_tbl[input.index].gpio, input.data);
	return RET_SUCCESS;
}


int get_val(unsigned long arg)
{
	struct gpio_data __user *argp = (struct gpio_data __user*)arg;
	struct gpio_data input;
	int ret;

	ret = copy_from_user(&input, argp, sizeof(struct gpio_data));
	if (ret != 0) {
		return RET_MEM_ERR;
	}
	if (input.index >  IO_NUM) {
		return RET_INVAILD;
	}

	input.data = gpio_get_value(gpios_tbl[input.index].gpio);
	ret = copy_to_user(&argp->data, &input.data, sizeof(int));
	return RET_SUCCESS;
}


// 获取IO名称
int io_name(unsigned long arg)
{
	struct gpio_name __user *argp = (struct gpio_name __user*)arg;
	struct gpio_name input;
	int ret;

	ret = copy_from_user(&input, argp, sizeof(struct gpio_name));
	if (ret != 0) {
		return RET_MEM_ERR;
	}
	if (input.index >  IO_NUM) {
		return RET_INVAILD;
	}

	// 拷贝字符串，包括最后的 '\0'
	ret = copy_to_user(argp->name, gpios_tbl[input.index].name, MAX_NAME);
	if (ret != 0) {
		return RET_MEM_ERR;
	}
	return RET_SUCCESS;
}

// 获取IO全部信息
int io_inf(unsigned long arg)
{
	return RET_SUCCESS;
}


long led_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	// int err;
	// char buf[ IO_NUM];
	// int i,ret;


	switch(cmd) {
	case CMD_GET_COUNT:
		// dbg_print("input get_count\n");
		return get_count(arg);

	case CMD_SET_DIR:

		return set_dir(arg);

	case CMD_GET_DIR:
		return get_dir(arg);

	case CMD_SET_VAL:
		// dbg_print("set_val\n");
		return set_val(arg);

	case CMD_GET_VAL:
		// dbg_print("get_val\n");
		return get_val(arg);

	case CMD_IO_NAME:
		return io_name(arg);

	case CMD_IO_INF:
		return io_inf(arg);

	default:
		return RET_INVAILD;
	}
    return 0;
	// switch(cmd)
	// {
	// 	case 0:
	// 	case 1:
	// 		if (arg >  IO_NUM) {
	// 			return -EINVAL;
	// 		}

	// 		gpio_set_value(gpios_tbl[arg].gpio, cmd);
	// 		break;
	// 	case 2:
			
	// 		// err = gpio_request_one(, GPIOF_OUT_INIT_HIGH, "DM9620_RESET");
	// 		err= gpio_request(EXYNOS4_GPC0(1), "DM9620_RESET");
	// 		if (err)
	// 			pr_err("failed to request GPC0_1 for DM9620 reset control\n");

	// 		s3c_gpio_setpull(EXYNOS4_GPC0(1), S3C_GPIO_PULL_UP);                                                                       
	// 		gpio_set_value(EXYNOS4_GPC0(1), 0);

	// 		mdelay(50);

	// 		gpio_set_value(EXYNOS4_GPC0(1), 1);
	// 		gpio_free(EXYNOS4_GPC0(1));
	// 		break;
	// 	case 4:
			

	// 		ret = led_read(&ledfops, buf, 12,NULL);
	// 		dbg_print("ret error %d\n",ret);
	// 		for (i = 0;i < ret; i++) {
	// 			dbg_print("%2.2x ",buf[i]);
	// 		}
	// 		// end copy
	// 		break;
	// 		dbg_print("command reset USB HUB down\n");
	// 		err = gpio_request(GPIO_HUB_RESET, "GPIO_HUB_RESET");
	// 		if (err)
	// 			pr_err("failed to request EXYNOS4_GPL2 for GPIO_HUB_RESET\n");
	// 		gpio_direction_output(GPIO_HUB_RESET, 1);
	// 		// s3c_gpio_setpull(GPIO_HUB_RESET, S3C_GPIO_PULL_NONE);
	// 		// s3c_gpio_setpull(GPIO_HUB_RESET, S3C_GPIO_PULL_UP);
	// 		gpio_set_value(GPIO_HUB_RESET, 1);
	// 		msleep(5);
	// 		// s3c_gpio_setpull(GPIO_HUB_RESET, S3C_GPIO_PULL_DOWN);
	// 		gpio_set_value(GPIO_HUB_RESET, 0);
	// 		msleep(5);
	// 		// s3c_gpio_setpull(GPIO_HUB_RESET, S3C_GPIO_PULL_UP);
	// 		gpio_set_value(GPIO_HUB_RESET, 1);
	// 		msleep(5);
	// 		gpio_free(GPIO_HUB_RESET);

	// 		// HUB_CONNECT
	// 		err = gpio_request(GPIO_HUB_CONNECT, "GPIO_HUB_CONNECT");
	// 		if (err)
	// 			pr_err("failed to request EXYNOS4_GPK3 for GPIO_HUB_CONNECT\n");
	// 		gpio_direction_output(GPIO_HUB_CONNECT, 1);
	// 		// s3c_gpio_setpull(GPIO_HUB_CONNECT, S3C_GPIO_PULL_NONE);

	// 		// s3c_gpio_setpull(GPIO_HUB_CONNECT, S3C_GPIO_PULL_NONE);
	// 		// s3c_gpio_setpull(GPIO_HUB_CONNECT, S3C_GPIO_PULL_UP);
	// 		gpio_set_value(GPIO_HUB_CONNECT, 1);
	// 		msleep(5);
	// 		// s3c_gpio_setpull(GPIO_HUB_CONNECT, S3C_GPIO_PULL_DOWN);
	// 		gpio_set_value(GPIO_HUB_CONNECT, 0);
	// 		msleep(5);
	// 		// s3c_gpio_setpull(GPIO_HUB_CONNECT, S3C_GPIO_PULL_UP);
	// 		gpio_set_value(GPIO_HUB_CONNECT, 1);
	// 		msleep(5);
	// 		gpio_free(GPIO_HUB_CONNECT);
	// 	default:
	// 		return -EINVAL;
	// }
	// return 0;
}
int  led_release(struct inode *inode, struct file *filp)
{
	return 0;
}





// #define rGPC1CON    (*(volatile unsigned *)(0x11400000+0x80))
// #define rGPC1DAT	(*(volatile unsigned *)(0x11400000+0x84))


// Menglong Woo 20151120
// void delay_led(unsigned long t)
// {
// 	int i,j;
// 	for (j = 0; j < t;j++) {
// 		for (i = 0; i < 1000;i++) {
// 		}
// 	}
		
// }

// void show_led_buzzer()
// {
// 	unsigned long rGPC1CON, rGPC1DAT, rGPC1PUP; 
// 	int reg;

// 	printk("show led and buzzer\n");
	
// 	// buzzer	EXYNOS4_GPC1(2)
// 	rGPC1CON = ioremap( (0x11400000+0x80),4);
// 	rGPC1DAT = ioremap( (0x11400000+0x84),4);
// 	rGPC1PUP = ioremap( (0x11400000+0x88),4);
// 	printk("reg %x %x %x\n", rGPC1CON,
// 				rGPC1DAT,
// 				rGPC1PUP);
// 	reg = readl(rGPC1CON);
// 	reg &= ~(0xf << 4);
// 	reg |=  (0x1 << 4);
// 	writel(reg,rGPC1CON);

// 	reg = readl(rGPC1PUP);
// 	reg &= ~(0xf << 1);
// 	reg |=  (0x1 << 1);
// 	writel(reg,rGPC1PUP);
	

// 	// turn on buzzer
// 	reg = readl(rGPC1DAT);
// 	reg |=  (0x1 << 1);
// 	writel(reg,rGPC1DAT);
// 	// delay_led(1000);

// 	// reg = rGPC1DAT;
// 	// reg &=  ~(0x1 << 2);	
// 	// rGPC1DAT = reg;

// }

// static struct file_operations ledfops = 
// {
//  	.read = led_read,
//  	.write = led_write,
//  	.open = led_open,
//  	.release = led_release,
//  	.unlocked_ioctl = led_ioctl,
// };


static int __init led_init(void)
{
	int ret, i,k;
	
	cdev_init(&leddev, &ledfops);
	alloc_chrdev_region(&devno, 0, 2, DRIVER_NAME);
	cdev_add(&leddev, devno,2);

	printk(KERN_NOTICE  "dev %d MKJOR %d MINOR %d MKDEV0 %d MKDEV0 %d\n",devno,MAJOR(devno),MINOR(devno),
															MKDEV(devno,0),MKDEV(devno,2));
	
	

	// 强制释放内核已经使用的端口，否则下面的申请可能失败
	k = 0;
	for(i=0; i< IO_NUM; i++)
	{
		gpio_free(gpios_tbl[i].gpio);
		k++;
	}

	// 申请端口，并配置输入输出
	k = 0;
	for(i=0; i< IO_NUM; i++)
	{
		ret = gpio_request(gpios_tbl[i].gpio, "LED");
		if (ret) {
			printk("%s: request GPIO %d for LED failed, ret = %d\n", DRIVER_NAME,
					gpios_tbl[i].gpio, ret);
			goto _FreeGPIO;
			return ret;
		}
		printk("request success :%d %d %s\n",k,gpios_tbl[i].gpio,gpios_tbl[i].name);
		k++;

		if (IO_INPUT == gpios_tbl[i].dir) {
			s3c_gpio_cfgpin(gpios_tbl[i].gpio, S3C_GPIO_OUTPUT);
			gpio_set_value(gpios_tbl[i].gpio, 1);	
		}
		else {
			s3c_gpio_cfgpin(gpios_tbl[i].gpio, S3C_GPIO_INPUT);
		}
		
	}
	// unsigned long tmpreg; 
	// unsigned int *rVCON0;
	// // #define rVCON0    (*(volatile unsigned *)0x11C00000)
	// rVCON0 = ioremap(0x11C00000,4);
	// tmpreg = readl(rVCON0);
	// printk("con0 %x\n", tmpreg);

	// tmpreg |= (1 << 30);
	// writel(tmpreg, rVCON0);

	// tmpreg = readl(rVCON0);
	// printk("con0 %x\n", tmpreg);
	return 0;

	// 对于某些独占的端口释放无效，释放已经申请的所有端口
_FreeGPIO:
	k = 0;
	for(i=0; i< IO_NUM; i++)
	{
		gpio_free(gpios_tbl[i].gpio);
		printk("free success :%d %d\n",k,gpios_tbl[i].gpio);
		k++;
	}
	return -1;
}

static void __exit led_exit(void)
{
	unregister_chrdev_region(devno,2);
	cdev_del(&leddev);
}

void led_print(void)
{
	printk("this is led module");

}
EXPORT_SYMBOL_GPL(led_print);
module_init(led_init);
module_exit(led_exit);

// void mydevled_test(void)
// {
// 	dbg_print(KERN_ALERT "led module\n");
// }
// EXPORT_SIMPOL(mydevled_test);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Menglong Woo");
#if (CONFIG_BOARD == 11)
	MODULE_DESCRIPTION("JW-CUV1.1");
#elif (CONFIG_BOARD == 12)
	MODULE_DESCRIPTION("JW-CUV1.2");
#endif