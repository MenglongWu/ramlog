#内核修改日志
* config_for_linux_9.7到.config升级补丁 

>linux9.7-TMSxx.patch** 

* 网卡驱动

1. 更换dm9620官方驱动 **V2.45**
1. 注释掉  mach-itop4412.c里面的         

>//{ EXYNOS4_GPC0(1),  S3C_GPIO_SLP_INPUT,	S3C_GPIO_PULL_DOWN}, //VLED_ON

* 7.0寸LCD支持

>CONFIG_CMDLINE="root=/dev/mmcblk0p2 rootfstype=ext4 init=/linuxrc console=ttySAC2,115200 lcd=7.00000"

> **其实在uboot里修改启动参数也可以支持7.0寸LCD，但是uboot里不能保存环境变量**

* /opt/iTop-4412/iTop4412_Kernel_3.0/arch/arm/mach-exynos/mach-itop4412.c
>设置 **EXYNOS4_GPC1(2)** 为低电平，关闭蜂鸣器。



* 更改看门狗 2015-11-12
> 1. 修改clock-exynos4.c添加.name       = "watchdog",等初始化时钟内容   
> 1. 去除内核里编译看门狗 CONFIG_S3C2410_WATCHDOG 去除
> 1. 另外采用梁祥严改过的watchdog工程编译成Exynos4412_wdt.ko，该模块放在文件系统里启动时候加载

* 去掉CAM功能防止CAM_VSYNC与普通IO冲突

将下列两项用make menuconfig改成不编译
> 1. CONFIG_SOC_CAMERA=y
> 1. CONFIG_VIDEO_OV5640=y



* LCD触屏芯片使能 2015-12-04

如果不这么做某些板子触屏芯片不能运行，I2C检测不到触屏模块，看不到/dev/input/event2、/dev/input/misc设备节点。 **但是讯为的开发板能检测到**
> 1. TP1_EN : **GPL0(1)** 启动时拉高
> 1. 驱动里不能随便使用该端口，如果将该端口设置成输出引脚，瞬间该芯片就失效，再拉高也未必能起作用

