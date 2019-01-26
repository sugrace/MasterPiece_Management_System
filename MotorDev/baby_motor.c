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
#include<linux/cdev.h>
#include<linux/errno.h>
#include<linux/fcntl.h>

#include<asm/delay.h>

#include"baby_motor.h"

MODULE_LICENSE("GPL");

static void mdelay(int msec){
	int i;

	for(i=0;i<1000;i++)
		udelay(1);
}

/* Go to angle during time */
static int part_move(int angle, int time){
	int slice, duration, i=0;

	if(angle <= 0 || angle > 30){
		printk("MOTOR :  The angle period is exceed\n");
		return -1;
	}

	slice = angle * 50;
	duration = time / slice;

	while(i<duration){
		gpio_set_value(MOTOR, 1);
		udelay(slice);
		gpio_set_value(MOTOR, 0);
		udelay(30);
		i++;
	}
	
	return 0;
}

/* Motor moving reverse clock direction */
static void moving_reverse_clock(void){
	int i;

	for(i=6;i<30;i++)
		part_move(i, 100000);
}

/* Motor moving clock direction */
static void moving_clock(void){
	int i;

	for(i=6;i<30;i++)
		part_move(30-i, 100000);
}

/* Device Open */
static int baby_motor_mod_open(struct inode* inode, struct file* file){
	printk("MOTOR : Device file open\n");
	gpio_set_value(MOTOR, 1);
	mdelay(5000);
	gpio_set_value(MOTOR, 0);
	return 0;
}

/* Device Release */
static int baby_motor_mod_release(struct inode* inode, struct file* file){
	printk("MOTOR : Device file release\n");
	return 0;
}

/* IOCTL Body */
static long baby_motor_mod_ioctl(struct file* file, unsigned int cmd, unsigned long arg){
	switch(cmd){
	case MOVING_R_CLOCK : 
		moving_reverse_clock();
		break;
	case MOVING_CLOCK : 
		moving_clock();
		break;
	default : 
		printk("MOTOR : IOCTL Command is undefined\n");
		return -1;
	}
	
	return 0;
}

struct file_operations baby_motor_mod_fops =
{
	.unlocked_ioctl = baby_motor_mod_ioctl,
	.open = baby_motor_mod_open,
	.release = baby_motor_mod_release,
};

static dev_t dev_num;
static struct cdev* baby_motor_cdev;

static int __init speaker_init(void){
	int ret;

	printk("MOTOR : Init module\n");

	/* Device Initializing */
	ret = alloc_chrdev_region(&dev_num, 0, 1, DEV_NAME);
	baby_motor_cdev = cdev_alloc();
	cdev_init(baby_motor_cdev, &baby_motor_mod_fops);
	ret = cdev_add(baby_motor_cdev, dev_num, 1);	
	if(ret < 0)
		printk("MOTOR : Fail to add character device\n");
	else
		printk("MOTOR : Success to initialize character device\n");

	/* GPIO Initializing */
	gpio_request_one(MOTOR, GPIOF_OUT_INIT_LOW, "motor");
	mdelay(5000);  //delay in start of motor

	return 0;
}

static void __exit speaker_exit(void){
	printk("MOTOR : Exit module\n");

	/* Device free */
	cdev_del(baby_motor_cdev);
	unregister_chrdev_region(dev_num, 1);
	printk("MOTOR : Device free\n");

	/* GPIO Free */
	gpio_free(MOTOR);
}

module_init(speaker_init);
module_exit(speaker_exit);
