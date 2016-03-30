# TMSxxTC GPIO驱动以及上层接口使用  
**吴梦龙
2015-10-28**

该文件目录下可编译两个工程目录可生成3个目标，
> tms_gpio生成驱动 **tms_gpio.ko**  
  test生成 **动态链接库lib_tms_gpio.so(a)** 以及演示代码 **test_gpio.elf**

##注意
生成的x86平台下没有tms_gpio.ko文件，并且生成的lib也不能正常运行（没有IO），编译它的目的 **仅仅** 是为了保证不影响其他代码能在x86平台下测试， **QT代码里** 应该检测 **open_io(NULL)的返回值，如果该值为NULL，表示驱动打开失败，后面的任何驱动调用接口都无意义**。



# 如何编译
---
首先所有已经编译好的文件在output目录下，对于驱动的加载可以直接运行 **./gpio_load.sh** ，卸载运行 **./gpio_unload.sh**


##tms_gpio.ko
清除tms_gpio.ko
> make clean

编译IO驱动tms_gpio.ko
> make
生成目录在./src/tms_gpio.ko

##libtms_gpio.so(a)
清除libtms_gpio.so(a)
> make lib clean

编译libtms_gpio.so(a)
> make ARCH=x86 lib  
make ARCH=armv7 lib  
生成目录在./library/libtms_gpio.so(a)

##initgpio.elf
配置环境
> 将上一步骤生成的libtms_gpio.so(a)拷贝到linux环境的lib目录下 
x86拷贝到 **/usr/local/install/lib**  
ARM环境拷贝到 **/usr/4412/install/lib**

清除initgpio.elf
> make file_prj=prj_test.mk 

编译initgpio.elf
> make ARCH=x86 file_prj=prj_test.mk  
make ARCH=armv7 file_prj=prj_test.mk  
生成目录在./release/test_gpio.elf

## 使用方法
1. 在开发板里首先运行./gpio_load.sh，生成/dev/tms_gpio设备节点
1. 拷贝连接库到开发板的/usr/4412/install/lib目录
1. 运行测试程序./test_gpio.elf

```
[root@iTOP-4412]# ./test_gpio.elf
count 27

io 00 name      IP_SET_GPIO     iodata 0
io 01 name UNIT16_220V_GPIO     iodata 1
io 02 name UNIT15_220V_GPIO     iodata 1
io 03 name  UNIT16_48V_GPIO     iodata 1
io 04 name  UNIT15_48V_GPIO     iodata 1
io 05 name  UNIT16_12V_GPIO     iodata 1
io 06 name  UNIT15_12V_GPIO     iodata 1
io 07 name     UNIT_13_GPIO     iodata 1
io 08 name     UNIT_12_GPIO     iodata 1
io 09 name     UNIT_11_GPIO     iodata 1
io 10 name     UNIT_10_GPIO     iodata 1
io 11 name      UNIT_9_GPIO     iodata 1
io 12 name      UNIT_8_GPIO     iodata 1
io 13 name      UNIT_7_GPIO     iodata 1
io 14 name      UNIT_6_GPIO     iodata 1
io 15 name      UNIT_5_GPIO     iodata 1
io 16 name      UNIT_4_GPIO     iodata 1
io 17 name      UNIT_3_GPIO     iodata 1
io 18 name      UNIT_2_GPIO     iodata 1
io 19 name      UNIT_1_GPIO     iodata 1
io 20 name        KEY_ALARM     iodata 1
io 21 name  LED_ALARM_TOTAL     iodata 0
io 22 name          LED_RUN     iodata 1
io 23 name   LED_ALARM_COMM     iodata 0
io 24 name          GSM_RST     iodata 0
io 25 name           GSM_EN     iodata 1
io 26 name         SW_RESET     iodata 1
```
### 输出内容解析

1. 第1行说明输出表示该驱动拥有 **27个端口** 的控制权限
1. 第2列端口序号分别是 **0~26**
1. 第4列表示 **IO口的功能**
1. 第6列表示读取到的IO口状态

如果修改采用带参数的命令./test_gpio.elf 24 1 可将24端口设置成低电平，输出多最后一行

```
[root@iTOP-4412]# ./test_gpio.elf 24 1
count 27

io 00 name      IP_SET_GPIO     iodata 0
io 01 name UNIT16_220V_GPIO     iodata 1
io 02 name UNIT15_220V_GPIO     iodata 1
io 03 name  UNIT16_48V_GPIO     iodata 1
io 04 name  UNIT15_48V_GPIO     iodata 1
io 05 name  UNIT16_12V_GPIO     iodata 1
io 06 name  UNIT15_12V_GPIO     iodata 1
io 07 name     UNIT_13_GPIO     iodata 1
io 08 name     UNIT_12_GPIO     iodata 1
io 09 name     UNIT_11_GPIO     iodata 1
io 10 name     UNIT_10_GPIO     iodata 1
io 11 name      UNIT_9_GPIO     iodata 1
io 12 name      UNIT_8_GPIO     iodata 1
io 13 name      UNIT_7_GPIO     iodata 1
io 14 name      UNIT_6_GPIO     iodata 1
io 15 name      UNIT_5_GPIO     iodata 1
io 16 name      UNIT_4_GPIO     iodata 1
io 17 name      UNIT_3_GPIO     iodata 1
io 18 name      UNIT_2_GPIO     iodata 1
io 19 name      UNIT_1_GPIO     iodata 1
io 20 name        KEY_ALARM     iodata 1
io 21 name  LED_ALARM_TOTAL     iodata 0
io 22 name          LED_RUN     iodata 1
io 23 name   LED_ALARM_COMM     iodata 0
io 24 name          GSM_RST     iodata 0
io 25 name           GSM_EN     iodata 1
io 26 name         SW_RESET     iodata 1

set led output iodate: 1    get led state iodata 1
```

##API的使用
参看test_gpio.elf的源代码test.c，

- 所有接口以open_io开始
- get_count获取该驱动能控制的端口数，目前该驱动控制端口数为27（0~26）
- io_name获取该端口的名称，知道它的用途
- set_dir设置IO口输入输出方向（IO_OUTPUT,IO_INPUT）
- set_io设置IO口的电平 **（IO有个电源转换芯片，如果IO接在电源转换芯片的输出口，那么该端口无法设置，只能读取）**
- get_io读取IO口的电平
- close_io关闭IO设备文件