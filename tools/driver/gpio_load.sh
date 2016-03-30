module="tms_gpio"
device="tms_gpio"
modfile="/usr/MenglongWu/bin/tms_gpio.ko"


insmod $modfile
major=$(cat /proc/devices | awk "\$2==\"$module\"" | awk "{print \$1}")


strout="create /dev/${device}0"
echo $strout
mknod /dev/${device}0 c $major 0

# strout="create /dev/${device}1"
# echo $strout
# mknod /dev/${device}1 c $major 1


