#ifndef H_DEV_MOD
#define H_DEV_MOD

#define ULTRA_TRIG 24
#define ULTRA_ECHO 23

#define SWITCH_SENSOR 6
#define DIS_SENSOR 4
#define DEV_NAME "mm_motor_mod_dev"

#define IOCTL_START_NUM 0xE551
#define PIR_IOCTL_NUM IOCTL_START_NUM+1
#define SWITCH_IOCTL_NUM IOCTL_START_NUM+2

#define MAGIC_NUM 'p'
#define SIGNALING _IOWR(MAGIC_NUM, PIR_IOCTL_NUM, unsigned long *)
#define CHECK _IOWR(MAGIC_NUM, SWITCH_IOCTL_NUM, unsigned long *)

#endif
