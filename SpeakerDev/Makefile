KDIR := ./linux-rpi/
KDIRECTORY := /lib/modules/$(shell uname -r)/build
ARM := ARCH=arm CROSS_COMPILE=/usr/bin/arm-linux-gnueabi-
PWD := $(shell pwd)

obj-m := baby_speaker.o

default : 
	$(MAKE) -C $(KDIR) M=$(PWD) $(ARM) modules
clean : 
	$(MAKE) -C $(KDIR) M=$(PWD) $(ARM) clean
