/**************************************************************************************/
/* Konkuk Univ. Department of Computer Science Enginnering                            */
/* 2017 Embedded System Software1 Class by prof.JIN                                   */
/* This project is term project for class and the project name is [ Baby in my Home ] */
/* Team 4 consists of KIM MIN SU & MOON JUN YOUNG & LEE CHANG OH                      */
/* This file is source code file for speaker module                                   */
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

#include<asm/delay.h>

#include"baby_speaker.h"

MODULE_LICENSE("GPL");

int BassTab[] = {1911, 1702, 1516, 1431, 1275, 1136, 1012};
static int flag = 0;
static int switch_flag = 1;
static int irq_num;

static void sound(int note, int length){
	int i;

	if(switch_flag != 0){
		gpio_set_value(LED, 1);
		for(i=0;i<(length*50);i++){
			gpio_set_value(SPEAKER, 1);
			udelay(BassTab[note]);
		
			gpio_set_value(SPEAKER, 0);
			udelay(BassTab[note]);
		}
	}
	else
		gpio_set_value(LED, 0);
}

static void mdelay(int msec){
	int i;

	for(i=0;i<1000;i++)
		udelay(1);
}

static void sound_airplane(void){
	printk("SPEAKER : sound airplane\n");	

	mdelay(500);

	flag = 1;
	
	sound(2,3);
	sound(1,1);
	sound(0,2);
	sound(1,2);
	sound(2,2);
	sound(2,2);
	sound(2,4);
	sound(1,2);
	sound(1,2);
	sound(1,4);
	sound(2,2);
	sound(2,2);
	sound(2,4);
	sound(2,3);
	sound(1,1);
	sound(0,2);
	sound(1,2);
	sound(2,2);
	sound(2,2);
	sound(2,4);
	sound(1,2);
	sound(1,2);
	sound(2,3);
	sound(1,1);
	sound(0,4);

	flag = 0;

	gpio_set_value(LED, 0);
}

/* Device Open */
static int baby_speaker_mod_open(struct inode* inode, struct file* file){
	printk("SPEAKER : Device file open\n");
	return 0;
}

/* Device Release */
static int baby_speaker_mod_release(struct inode* inode, struct file* file){
	printk("SPEAKER : Device file release\n");
	return 0;
}

/* IOCTL Body */
static long baby_speaker_mod_ioctl(struct file* file, unsigned int cmd, unsigned long arg){
	switch(cmd){
	case AIR_PLANE : 
		sound_airplane();
		break;
	case FLAG : 
		return flag;
	case CHECK : 
		return switch_flag;
	default : 
		printk("SPEAKER : IOCTL Command is undefined\n");
		return -1;
	}
	
	return 0;
}

static irqreturn_t switch_sensor_isr(int irq, void* dev_id){
	switch_flag ^= 1;  //reverse the flag

	if(switch_flag != 1)
		printk("SPEAKER : SWITCH OFF\n");
	else
		printk("SPEAKER : SWITCH ON\n");

	return IRQ_HANDLED;
}

struct file_operations baby_speaker_mod_fops =
{
	.unlocked_ioctl = baby_speaker_mod_ioctl,
	.open = baby_speaker_mod_open,
	.release = baby_speaker_mod_release,
};

static dev_t dev_num;
static struct cdev* baby_speaker_cdev;

static int __init speaker_init(void){
	int ret;

	printk("SPEAKER : Init module\n");

	/* Device Initializing */
	ret = alloc_chrdev_region(&dev_num, 0, 1, DEV_NAME);
	baby_speaker_cdev = cdev_alloc();
	cdev_init(baby_speaker_cdev, &baby_speaker_mod_fops);
	ret = cdev_add(baby_speaker_cdev, dev_num, 1);	
	if(ret < 0)
		printk("SPEAKER : Fail to add character device\n");
	else
		printk("SPEAKER : Success to initialize character device\n");

	/* GPIO Initializing */
	gpio_request_one(LED, GPIOF_OUT_INIT_LOW, "led");
	gpio_request_one(SPEAKER, GPIOF_OUT_INIT_LOW, "speaker");
	gpio_request_one(SWITCH, GPIOF_IN, "switch");
	irq_num = gpio_to_irq(SWITCH);
	ret = request_irq(irq_num, switch_sensor_isr, IRQF_TRIGGER_FALLING, "switch", NULL);
	if(ret){
		printk("SPEAKER : Unable to request IRQ : %d\n", ret);
		free_irq(irq_num, NULL);
	}

	return 0;
}

static void __exit speaker_exit(void){
	printk("SPEAKER : Exit module\n");

	/* Device free */
	cdev_del(baby_speaker_cdev);
	unregister_chrdev_region(dev_num, 1);
	printk("SPEAKER : Device free\n");

	free_irq(irq_num, NULL);

	/* GPIO Free */
	gpio_free(LED);
	gpio_free(SPEAKER);
	gpio_free(SWITCH);
}

module_init(speaker_init);
module_exit(speaker_exit);
