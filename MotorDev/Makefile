KDIR := ./linux-rpi/
KDIRECTORY := /lib/modules/$(shell uname -r)/build
ARM := ARCH=arm CROSS_COMPILE=/usr/bin/arm-linux-gnueabi-
PWD := $(shell pwd)

obj-m := baby_motor.o

default : 
	$(MAKE) -C $(KDIR) M=$(PWD) $(ARM) modules
	arm-linux-gnueabi-gcc main.c baby_motor_lib.c -o test
clean : 
	$(MAKE) -C $(KDIR) M=$(PWD) $(ARM) clean
	rm test
copy : 
	scp baby_motor.ko pi@203.252.166.1:~/
	scp baby_motor_mknod.sh pi@203.252.166.1:~/
	scp test pi@203.252.166.1:~/
