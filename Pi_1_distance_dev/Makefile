obj-m:=dis_dev_mod.o


KERNELDIR := ~/berry/linux-rpi/
ARM := ARCH=arm CROSS_COMPILE=/usr/bin/arm-linux-gnueabi-
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) $(ARM) modules
copy:
	scp mknod.sh speaker.ko   pi@10.10.10.13:~
clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) $(ARM) clean
	
