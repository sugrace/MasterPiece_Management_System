/**************************************************************************************/
/* Konkuk Univ. Department of Computer Science Enginnering                            */
/* 2017 Embedded System Software1 Class by prof.JIN                                   */
/* This project is term project for class and the project name is [ Baby in my Home ] */
/* Team 4 consists of KIM MIN SU & MOON JUN YOUNG & LEE CHANG OH                      */
/* This file is header file of basement module for IoT device                         */
/**************************************************************************************/
#ifndef H_DEV_MOD
#define H_DEV_MOD

#define SWITCH_SENSOR 6
#define PIR_SENSOR 4
#define DEV_NAME "baby_dev_mod_pir_sensor"

#define IOCTL_START_NUM 0xE551
#define PIR_IOCTL_NUM IOCTL_START_NUM+1
#define SWITCH_IOCTL_NUM IOCTL_START_NUM+2

#define MAGIC_NUM 'p'
#define SIGNALING _IOWR(MAGIC_NUM, PIR_IOCTL_NUM, unsigned long *)
#define CHECK _IOWR(MAGIC_NUM, SWITCH_IOCTL_NUM, unsigned long *)

#endif
