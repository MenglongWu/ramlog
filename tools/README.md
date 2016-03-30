# TMSxxTC GPIO驱动以及上层接口使用  

# 工程包括
---
###driver  
* tms_gpio.ko  
TMSxxTC项目GPIO控制

### lcd  
* lcdsw.ko  
不记得是干什么的了，它是在tms_gpio.ko基础上做了点修改

---
### script/prj_libgpio  
* libtms_gpio.so  
TMSxxTC GPIO接口


### script/prj_libusb  
* lib_easydaqcard_dll.so  
USB采集卡接口

---
### script/prj_produce  
* prj_produce.elf  
产品自检与出厂检验

### script/prj_test  
* test_gpio.elf  
测试与观察GPIO，能手工设置GPIO状态

### script/prj_usb  
* usbcard.elf  
USB采集卡测试程序

### script/prj_test
* initgpio.elf  
监控网卡并重启网卡

### script/prj_lcd
* lcdtest.elf  
测试LCD画出3块颜色区域




