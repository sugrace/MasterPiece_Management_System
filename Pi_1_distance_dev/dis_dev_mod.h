
#define ULTRA_TRIG 24
#define ULTRA_ECHO 23
#define LED 4
#define PIR_SENSOR 17

#define SWITCH_SENSOR 6
#define DIS_SENSOR 4
#define DEV_NAME "dis_dev_mod"

#define IOCTL_START_NUM 0xE551
#define RESTORE_IOCTL_NUM IOCTL_START_NUM+1
#define SWITCH_IOCTL_NUM IOCTL_START_NUM+2

#define MAGIC_NUM 'p'
#define RESTORE _IOWR(MAGIC_NUM, RESTORE_IOCTL_NUM, unsigned long *)
#define CHECK _IOWR(MAGIC_NUM, SWITCH_IOCTL_NUM, unsigned long *)


