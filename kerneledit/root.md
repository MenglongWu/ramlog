#文件系统修改日志


* 添加/etc/pointercal  
> 触屏校准后自动生成，以后打算不用校准

* 添加/bin/ip254up  
> 启动双IP

* 添加/bin/ip254down  
> 关闭双IP

* 添加/bin/randmac  
> MAC地址随机生成，生成脚本 randmac.sh

* 添加/etc/init.d/randhw.sh  
> 如果没有randmac.sh则调用randmac

* 修改/etc/init.d/rcS

* 添加/etc/init.d/runapp.sh                     
> 来自于/bin/qt4的修改，运行应用程序

* 添加/opt/qt-4.7.1/lib/fonts/msyh_xxx  
> 微软雅黑字体库

* 添加/usr/4412/install/lib  
> TMSxx系统所需要的动态链接库

* 添加/usr/MenglongWu/nfs  
> 开发板挂载虚拟机nfs挂载目录，可自定义

* 添加/usr/MenglongWu/bin   
> 程序运行目录

* U盘自动升级
> 添加/bin/update.sh  
> 添加/bin/doupdate.sh  
> 添加/bin/bak.sh  
> 添加/usr/MenglongWu/bin/udiskupdate.elf  
> 修改/etc/init.d/rcS创建U盘设备节点，避免未分区设备挂载不上

```
mknod /dev/sda1 b 8 1 
```

* 驱动目录/usr/MenglongWu/driver  
> **all_load.sh** 加载所有驱动  
> tms_gpio.ko TMSxx外设IO
> usb_daq_card.ko STM32 USB采集器驱动
> usb_daq_card_193.ko FPGA USB采集卡驱动

* 防火墙，双网卡
> 安装iptable，目录/usr/4412/install/sbin  
> /bin/deffirewall 自动配置防火墙  
> /bin/defgw 配置默认网关  
> /etc/eth1-setting 网卡配置信息脚本  
> /etc/init.d/ifconfig-eth1/2 网卡执行配置脚本


* 短信猫
> 安装gnokii  
> **/root/.config/gnokii/config** 短信猫配置文件

* 保证交换机能找到内网

**现象**  
设备能找到网口、但是交换机灯不亮，过很久终端显示lan0 become ready。

**愚钝的解决方法**
> 在加载tms_gpio.ko 驱动后控制usb网卡复位引脚
> 电平由0 变成 1，期间有需要一段比较长的延时 **（3s）** 不可少










