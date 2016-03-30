device="tms_gpio"
modfile="tms_gpio.ko"


rmmod $device


rm /dev/${device}0
# rm /dev/${device}1