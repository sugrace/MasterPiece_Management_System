/**************************************************************************************/
/* Konkuk Univ. Department of Computer Science Enginnering                            */
/* 2017 Embedded System Software1 Class by prof.JIN                                   */
/* This project is term project for class and the project name is [ Baby in my Home ] */
/* Team 4 consists of KIM MIN SU & MOON JUN YOUNG & LEE CHANG OH                      */
/* This file is source code file of basement module for IoT device                    */
/**************************************************************************************/
#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>

#include<linux/fs.h>
#include<linux/gpio.h>
#include<linux/interrupt.h>
#include<linux/cdev.h>
#include<linux/errno.h>
#include<linux/fcntl.h>
#include<linux/sched.h>

#include<asm/delay.h>

#include"baby_dev_mod.h"
MODULE_LICENSE("GPL");

static long sig_pir = 0;  //if get signal from pir sensor, this variable setting for 1
static int irq_num;  //irq number for pir sensor
static long switch_flag = 1;
static unsigned long irq_flags = 0;  //irq state store

/* Device Open */
static int baby_dev_mod_open(struct inode* inode, struct file* file){
	printk("DEV_MOD : Device file open\n");
	return 0;
}

/* Device Release */
static int baby_dev_mod_release(struct inode* inode, struct file* file){
	printk("DEV_MOD : Device file release\n");
	return 0;
}

/* IOCTL Body */
static long baby_dev_mod_ioctl(struct file* file, unsigned int cmd, unsigned long arg){
	long tmp = sig_pir;

	switch(cmd){
	case SIGNALING : 
		local_irq_save(irq_flags);
		sig_pir = 0;
		local_irq_restore(irq_flags);
		udelay(1000);
		return tmp;
	case CHECK : 
		return switch_flag;
	default : 
		printk("DEV_MOD : IOCTL Command is undefined\n");
		return -1;
	}
}

/* Interrupt Service Routine(ISR) */
static irqreturn_t pir_sensor_isr(int irq, void* dev_id){
	printk("DEV_MOD : ISR Start\n");
	sig_pir = (long)gpio_get_value(PIR_SENSOR);
	printk("DEV_MOD : ISR End\n");

	return IRQ_HANDLED;
}

static irqreturn_t switch_sensor_isr(int irq, void* dev_id){
	switch_flag ^= 1;  //reverse the flag

	if(switch_flag != 0)
		printk("DEV_MOD : Switch ON\n");
	else
		printk("DEV_MOD : Switch OFF\n");

	return IRQ_HANDLED;
}

struct file_operations baby_dev_mod_fops =
{
	.unlocked_ioctl = baby_dev_mod_ioctl,
	.open = baby_dev_mod_open,
	.release = baby_dev_mod_release,
};

static dev_t dev_num;
static struct cdev* baby_pir_cdev;

/* Initalizing Module */
static int __init baby_dev_mod_init(void){
	int ret;
	
	printk("DEV_MOD : Init Module Start\n");
	
	/* Device Initializing */
	ret = alloc_chrdev_region(&dev_num, 0, 1, DEV_NAME);
	baby_pir_cdev = cdev_alloc();
	cdev_init(baby_pir_cdev, &baby_dev_mod_fops);
	ret = cdev_add(baby_pir_cdev, dev_num, 1);
	if(ret < 0){
		printk("DEV_MOD : Fail to add character device\n");
	}
	printk("DEV_MOD : Success to initialize character device\n");

	/* GPIO Initializing */
	gpio_request_one(PIR_SENSOR, GPIOF_IN, DEV_NAME);
	irq_num = gpio_to_irq(PIR_SENSOR);
	ret = request_irq(irq_num, pir_sensor_isr, IRQF_TRIGGER_RISING, DEV_NAME, NULL);
	if(ret){
		printk("DEV_MOD : Unable to request IRQ : %d\n", ret);
		free_irq(irq_num, NULL);
	}
	gpio_request_one(SWITCH_SENSOR, GPIOF_IN, "switch");
	irq_num = gpio_to_irq(SWITCH_SENSOR);
	ret = request_irq(irq_num, switch_sensor_isr, IRQF_TRIGGER_FALLING, "switch", NULL);
	if(ret){
		printk("DEV_MOD : Unable to request IRQ : %d\n", ret);
		free_irq(irq_num, NULL);
	}

	printk("DEV_MOD : Success to initialize gpio\n");

	return 0;
}

/* Exit Module */
static void __exit baby_dev_mod_exit(void){
	printk("DEV_MOD : Exit Module Start\n");	

	/* Device free */
	cdev_del(baby_pir_cdev);
	unregister_chrdev_region(dev_num, 1);
	printk("DEV_MOD : Device free\n");

	/* IRQ Free */
	irq_num = gpio_to_irq(PIR_SENSOR);
	free_irq(irq_num, NULL);
	printk("DEV_MOD : Interrupt Free\n");
	irq_num = gpio_to_irq(SWITCH_SENSOR);
	free_irq(irq_num, NULL);
	printk("DEV_MOD : Interrupt Free\n");

	/* GPIO Free */
	gpio_free(PIR_SENSOR);
	gpio_free(SWITCH_SENSOR);
	printk("DEV_MOD : GPIO Free\n");
}

module_init(baby_dev_mod_init);
module_exit(baby_dev_mod_exit);
