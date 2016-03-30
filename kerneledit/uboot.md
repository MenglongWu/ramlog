#uboot修改日志
* 添加启动led和buzzer指示

> smdkc210.c 添加show_led_buzzer

* 兼容JW-CU1.1、JW-CU1.2继电器声音  

> 配置文件在include/configs/itop_4412_android.h加入
>
```
//#define CONFIG_GLINK_TMSXX_BOARD_11 0
#define CONFIG_GLINK_TMSXX_BOARD_12 1
```
