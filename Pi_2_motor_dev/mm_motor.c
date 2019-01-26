#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/time.h>
#include <linux/cdev.h>

MODULE_LICENSE("GPL");

#define PIN1 6
#define PIN2 13
#define PIN3 19
#define PIN4 26

#define STEPS 8

#define DEV_NAME "mm_motor_mod_dev"

#define MOTOR_IOCTL_START_NUM 0xE551
#define MOTOR_IOCTL_NUM1 MOTOR_IOCTL_START_NUM+1
#define MOTOR_IOCTL_NUM2 MOTOR_IOCTL_START_NUM+2

#define MOTOR_MAGIC_NUM 'm'
#define MOVING_UP _IOWR(MOTOR_MAGIC_NUM, MOTOR_IOCTL_NUM1, unsigned long *)
#define MOVING_DOWN _IOWR(MOTOR_MAGIC_NUM, MOTOR_IOCTL_NUM2, unsigned long *)


int steps[STEPS][4] = {
    {1, 0, 0, 1},
    {0, 0, 0, 1},
    {0, 0, 1, 1},
    {0, 0, 1, 0},
    {0, 1, 1, 0},
    {0, 1, 0, 0},
    {1, 1, 0, 0},
    {1, 0, 0, 0},
};

void setStep(int p1, int p2, int p3, int p4);
void moving_down(int round, int delay);
void moving_up(int round, int delay);

void moving_down(int round, int delay) {
    int i,j;
    for (i = 0 ; i < 512*round ; i++) {
        for (j = STEPS-1 ; j >= 0 ; j--) {
            setStep(steps[j][0], steps[j][1], steps[j][2], steps[j][3]);
            udelay(delay);
        }
    }
}

void moving_up(int round, int delay) {
    int i,j;
    for (i = 0 ; i < 512*round ; i++) {
        for (j = 0 ; j < STEPS ; j++) {
            setStep(steps[j][0], steps[j][1], steps[j][2], steps[j][3]);
            udelay(delay);
        }
    }
}
void setStep(int p1, int p2, int p3, int p4) {
    gpio_set_value(PIN1, p1);
    gpio_set_value(PIN2, p2);
    gpio_set_value(PIN3, p3);
    gpio_set_value(PIN4, p4);
}
static int mm_motor_mod_open(struct inode* inode, struct file* file){
	printk("MOTOR : Device file open\n");
	return 0;
}

static int mm_motor_mod_release(struct inode* inode, struct file* file){
	printk("MOTOR : Device file release\n");
	return 0;
}

static long mm_motor_mod_ioctl(struct file* file, unsigned int cmd, unsigned long arg){
	switch(cmd){
	case MOVING_UP : 
		moving_up(5,500);

                break;
	case MOVING_DOWN : 
		moving_down(5,500);
		break;
	default : 
		printk("MOTOR : IOCTL Command is undefined\n");
		return -1;
	}
	
	return 0;
}

struct file_operations mm_motor_mod_fops =
{
	.unlocked_ioctl = mm_motor_mod_ioctl,
	.open = mm_motor_mod_open,
	.release = mm_motor_mod_release,
};

static dev_t dev_num;
static struct cdev* mm_motor_cdev;

static int __init motor_init(void){
	int ret;

	printk("MOTOR : Init module\n");
	ret = alloc_chrdev_region(&dev_num, 0, 1, DEV_NAME);
	mm_motor_cdev = cdev_alloc();
	cdev_init(mm_motor_cdev, &mm_motor_mod_fops);
	ret = cdev_add(mm_motor_cdev, dev_num, 1);	
	if(ret < 0)
		printk("MOTOR : Fail to add character device\n");
	else
		printk("MOTOR : Success to initialize character device\n");

	gpio_request_one(PIN1, GPIOF_OUT_INIT_LOW, "p1");
    	gpio_request_one(PIN2, GPIOF_OUT_INIT_LOW, "p2");
    	gpio_request_one(PIN3, GPIOF_OUT_INIT_LOW, "p3");
    	gpio_request_one(PIN4, GPIOF_OUT_INIT_LOW, "p4");
	return 0;
}

static void __exit motor_exit(void){
	printk("MOTOR : Exit module\n");

	cdev_del(mm_motor_cdev);
	unregister_chrdev_region(dev_num, 1);
	printk("MOTOR : Device free\n");

	 gpio_free(PIN1);
   	 gpio_free(PIN2);
   	 gpio_free(PIN3);
  	 gpio_free(PIN4);	
}

module_init(motor_init);
module_exit(motor_exit);

