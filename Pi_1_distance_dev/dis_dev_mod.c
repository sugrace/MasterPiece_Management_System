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
#include<linux/delay.h>
#include<linux/time.h>
#include"dis_dev_mod.h"


MODULE_LICENSE("GPL");

static long sig = 0;  //if get signal from led sensor, this variable setting for 1
static int irq_num;  //irq number for pir sensor
//static long switch_flag = 1;
static unsigned long irq_flags = 0;  //irq state store

/* Device Open */
static int dis_dev_mod_open(struct inode* inode, struct file* file){
	printk("DEV_MOD : Device file open\n");
	return 0;
}

/* Device Release */
static int dis_dev_mod_release(struct inode* inode, struct file* file){
	printk("DEV_MOD : Device file release\n");
	return 0;
}



static void run(void)
{

unsigned long distance;
int a,b;
int i=0;

struct timeval tp;
struct timeval tp2;

for(i=0;i<3;i++)
  { 
	gpio_set_value(ULTRA_TRIG,0);
	udelay(2);
	gpio_set_value(ULTRA_TRIG,1);
	udelay(20);
	gpio_set_value(ULTRA_TRIG,0);
	
	while(gpio_get_value(ULTRA_ECHO)==0);
	a=gpio_get_value(ULTRA_ECHO);
        do_gettimeofday(&tp);

	while(gpio_get_value(ULTRA_ECHO)==1);
	b=gpio_get_value(ULTRA_ECHO);
	do_gettimeofday(&tp2);

	distance=(tp2.tv_usec - tp.tv_usec)/58;
	if(distance>19 && distance <26){
            gpio_set_value(LED,1);
       
        mdelay(300);
        
        gpio_set_value(LED,0);
        
        }
 mdelay(500);
         printk("distance:%lu cm\n",distance);
        
   }
 
}







 /*IOCTL Body*/
static long dis_dev_mod_ioctl(struct file* file, unsigned int cmd, unsigned long arg){
	

	switch(cmd){
	
	case 1 :
                 sig=0;
		 printk("restore sig : %d",sig);
                 return sig;
        case CHECK : 
		printk("check sig : %d",sig);
                return sig;
	
        
        default : 
		printk("DEV_MOD : IOCTL Command is undefined\n");
		return -1;
	}
} 

/* Interrupt Service Routine(ISR) */
static irqreturn_t pir_sensor_isr(int irq, void* dev_id){
	printk("DEV_MOD : pir ISR Start\n");
/* RUN DIS_SENSEOR*/	
       run();	
        
	printk("DEV_MOD : pir ISR End\n");

	return IRQ_HANDLED;
}

/* Interrupt Service Routine(ISR) */
static irqreturn_t led_sensor_isr(int irq, void* dev_id){
	printk("DEV_MOD : led ISR Start\n");
	
        sig = 1;
	
        printk("DEV_MOD : led ISR End\n");

	return IRQ_HANDLED;
}

struct file_operations dis_dev_mod_fops =
{
	.unlocked_ioctl = dis_dev_mod_ioctl,
	.open = dis_dev_mod_open,
	.release = dis_dev_mod_release,
};

static dev_t dev_num;
static struct cdev* dis_cdev;

/* Initalizing Module */
static int __init dis_dev_mod_init(void){
	int ret;
	
	printk("dis_dev : Init Module Start\n");
	
	/* Device Initializing */
	ret = alloc_chrdev_region(&dev_num, 0, 1, DEV_NAME);
	dis_cdev = cdev_alloc();
	cdev_init(dis_cdev, &dis_dev_mod_fops);
	ret = cdev_add(dis_cdev, dev_num, 1);
	if(ret < 0){
		printk("dis_dev  : Fail to add character device\n");
	}
	printk("dis_dev  : Success to initialize character device\n");

	/* GPIO Initializing */
	gpio_request_one(ULTRA_TRIG,GPIOF_OUT_INIT_LOW,"ultra_trig");
        gpio_request_one(ULTRA_ECHO,GPIOF_IN,"ultra_echo"); 
        gpio_request_one(LED,GPIOF_OUT_INIT_LOW,"Led");
        gpio_request_one(PIR_SENSOR,GPIOF_IN,"pir");

                
	irq_num = gpio_to_irq(LED);
	ret = request_irq(irq_num, led_sensor_isr, IRQF_TRIGGER_RISING, DEV_NAME, NULL);
	if(ret){
		printk("DEV_MOD : Unable to request IRQ : %d\n", ret);
		free_irq(irq_num, NULL);
	}
        irq_num = gpio_to_irq(PIR_SENSOR);
	ret = request_irq(irq_num, pir_sensor_isr, IRQF_TRIGGER_RISING, DEV_NAME, NULL);
	if(ret){
		printk("DEV_MOD : Unable to request IRQ : %d\n", ret);
		free_irq(irq_num, NULL);
	}




       


	printk("DEV_MOD : Success to initialize gpio\n");

	return 0;
}

/* Exit Module */
static void __exit dis_dev_mod_exit(void){
	printk("DEV_MOD : Exit Module Start\n");	

	/* Device free */
	cdev_del(dis_cdev);
	unregister_chrdev_region(dev_num, 1);
	printk("DEV_MOD : Device free\n");

	/* IRQ Free */
	irq_num = gpio_to_irq(LED);
	free_irq(irq_num, NULL);
	irq_num = gpio_to_irq(PIR_SENSOR);
	free_irq(irq_num, NULL);
        printk("DEV_MOD : Interrupt Free\n");
	
	/* GPIO Free */
	gpio_free(ULTRA_TRIG);
	gpio_free(ULTRA_ECHO);
	gpio_free(PIR_SENSOR);
        gpio_free(LED);
        printk("DEV_MOD : GPIO Free\n");
}

module_init(dis_dev_mod_init);
module_exit(dis_dev_mod_exit);
