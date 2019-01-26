#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/fs.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/cdev.h>

#include <linux/workqueue.h>
#include <asm/delay.h>

#include "baby_dev_mod.h"
MODULE_LICENSE("GPL");

static int irq_num_pir, irq_num_switch;
static int flag_on = 1, flag_pir = 0;

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
    gpio_set_value(SENSOR_MOTOR, 1);
    udelay(slice);
    gpio_set_value(SENSOR_MOTOR, 0);
    udelay(30);
    i++;
  }
  
  return 0;
}

/* Motor moving reverse clock direction */
static void moving_reverse_clock(void){
  int i;

  for(i=6;i<30;i++){
    if(!flag_on)
      break;
    part_move(i, 100000);
  }
}

/* Motor moving clock direction */
static void moving_clock(void){
  int i;

  for(i=6;i<30;i++){
    if(!flag_on)
      break;
    part_move(30-i, 100000);
  }
}

static long test_ioctl(struct file *file, unsigned int cmd, unsigned long arg){
  switch(cmd){
  case CHECK_FLAG:
    if(flag_on && flag_pir){
      gpio_set_value(LED, 1);
      return 1;
    }
    else{
      gpio_set_value(LED, 0);
      return 0;
    }
  case MOVING_R_CLOCK:
    moving_reverse_clock();
    return 0;
  case MOVING_CLOCK:
    moving_clock();
    return 0;
  case LED_ON:

    return 0;
  case LED_OFF:
    gpio_set_value(LED, 0);
    return 0;
  default:
    printk("Invalid IOCTL command\n");
    return -1;
  }
}
static int test_open(struct inode *inode, struct file *file){
  printk("test open\n");
  enable_irq(irq_num_pir);
  enable_irq(irq_num_switch);
  return 0;
}

static int test_release(struct inode *inode, struct file *file){
  printk("test release\n");
  disable_irq(irq_num_pir);
  disable_irq(irq_num_switch);
  return 0;
}

struct file_operations test_fops = {
  .unlocked_ioctl = test_ioctl,
  .open = test_open,
  .release = test_release,
};

static irqreturn_t test_pir_isr(int irq, void *dev_id){
  printk("pir\n");
  if(!flag_on)
    return IRQ_HANDLED;
  if(gpio_get_value(SENSOR_PIR)){
    printk("Rising\n");
    flag_pir = 1;
  }
  else{
    printk("Falling\n");
    flag_pir = 0;
  }

  return IRQ_HANDLED;
}

static irqreturn_t test_switch_isr(int irq, void *dev_id){
  flag_on ^= 1;
  if(flag_on){
    enable_irq(irq_num_pir);
    printk("switch on\n");
  }
  else{
    disable_irq(irq_num_pir);
    printk("switch off\n");
  }
  return IRQ_HANDLED;
}
static dev_t dev_num;
static struct cdev *cd_cdev;

static int __init test_init(void){
  int ret_pir, ret_switch;

  printk("Init Module\n");

  /* Allocate Character Device */
  alloc_chrdev_region(&dev_num, 0, 1, DEV_NAME);
  cd_cdev = cdev_alloc();
  cdev_init(cd_cdev, &test_fops);
  cdev_add(cd_cdev, dev_num, 1);

  /* Request GPIO and Interrupt Handler */
  gpio_request_one(SENSOR_PIR, GPIOF_IN, "PIR");
  gpio_request_one(SENSOR_SWITCH, GPIOF_IN, "SWITCH");
  gpio_request_one(LED, GPIOF_OUT_INIT_LOW, "LED");
  gpio_request_one(SENSOR_MOTOR, GPIOF_OUT_INIT_LOW, "MOTOR");
  mdelay(5000);  //delay in start of motor

  irq_num_pir = gpio_to_irq(SENSOR_PIR);
  irq_num_switch = gpio_to_irq(SENSOR_SWITCH);
  ret_pir = request_irq(irq_num_pir, test_pir_isr, IRQF_TRIGGER_RISING|IRQF_TRIGGER_FALLING, "pir_irq", NULL);
  ret_switch = request_irq(irq_num_switch, test_switch_isr, IRQF_TRIGGER_FALLING, "switch_irq", NULL);
  if(ret_pir){
    printk(KERN_ERR "Unable to request IRQ-PIR : %d\n", ret_pir);
    free_irq(irq_num_pir, NULL);
  }
  if(ret_switch){
    printk(KERN_ERR "Unable to request IRQ-PIR : %d\n", ret_switch);
    free_irq(irq_num_switch, NULL);
  }

  return 0;
}

static void __exit test_exit(void){
  printk("Exit Module\n");
  cdev_del(cd_cdev);
  unregister_chrdev_region(dev_num, 1);

  free_irq(irq_num_pir, NULL);
  free_irq(irq_num_switch, NULL);
  gpio_free(SENSOR_PIR);
  gpio_free(SENSOR_MOTOR);
  gpio_free(SENSOR_SWITCH);
  gpio_free(LED);
}

module_init(test_init);
module_exit(test_exit);
