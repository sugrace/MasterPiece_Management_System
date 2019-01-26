/**************************************************************************************/
/* Konkuk Univ. Department of Computer Science Enginnering                            */
/* 2017 Embedded System Software1 Class by prof.JIN                                   */
/* This project is term project for class and the project name is [ Baby in my Home ] */
/* Team 4 consists of KIM MIN SU & MOON JUN YOUNG & LEE CHANG OH                      */
/* This file is header file of basement module for IoT device                         */
/**************************************************************************************/
#ifndef H_DEV_MOD
#define H_DEV_MOD

#define SENSOR_PIR 5
#define SENSOR_MOTOR 6
#define SENSOR_SWITCH 13
#define LED 20

#define DEV_NAME "test_sensor_dev"

#define IOCTL_START_NUM 0x80
#define IOCTL_NUM1 IOCTL_START_NUM+1
#define IOCTL_NUM2 IOCTL_START_NUM+2
#define IOCTL_NUM3 IOCTL_START_NUM+3
#define IOCTL_NUM4 IOCTL_START_NUM+4
#define IOCTL_NUM5 IOCTL_START_NUM+5

#define SIMPLE_IOCTL_NUM 'z'
#define CHECK_FLAG _IOWR(SIMPLE_IOCTL_NUM, IOCTL_NUM1, unsigned long *)
#define MOVING_R_CLOCK _IOWR(SIMPLE_IOCTL_NUM, IOCTL_NUM2, unsigned long *)
#define MOVING_CLOCK _IOWR(SIMPLE_IOCTL_NUM, IOCTL_NUM3, unsigned long *)
#define LED_ON _IOWR(SIMPLE_IOCTL_NUM, IOCTL_NUM4, unsigned long *)
#define LED_OFF _IOWR(SIMPLE_IOCTL_NUM, IOCTL_NUM5, unsigned long *)

#endif
