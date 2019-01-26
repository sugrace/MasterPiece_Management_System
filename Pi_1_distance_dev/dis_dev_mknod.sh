MODULE="dis_dev_mod"
MAJOR=$(awk "\$2==\"$MODULE\" {print \$1}" /proc/devices)

mknod /dev/$MODULE c $MAJOR 0
chmod 777 /dev/$MODULE
