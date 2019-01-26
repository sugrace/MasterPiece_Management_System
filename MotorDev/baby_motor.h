/**************************************************************************************/
/* Konkuk Univ. Department of Computer Science Enginnering                            */
/* 2017 Embedded System Software1 Class by prof.JIN                                   */
/* This project is term project for class and the project name is [ Baby in my Home ] */
/* Team 4 consists of KIM MIN SU & MOON JUN YOUNG & LEE CHANG OH                      */
/* This file is header file for motor module                                          */
/**************************************************************************************/
#ifndef H_SPEAKER
#define H_SPEAKER

#define MOTOR 17

#define DEV_NAME "baby_motor_mod_dev"

#define MOTOR_IOCTL_START_NUM 0xE551
#define MOTOR_IOCTL_NUM1 MOTOR_IOCTL_START_NUM+1
#define MOTOR_IOCTL_NUM2 MOTOR_IOCTL_START_NUM+2

#define MOTOR_MAGIC_NUM 'm'
#define MOVING_R_CLOCK _IOWR(MOTOR_MAGIC_NUM, MOTOR_IOCTL_NUM1, unsigned long *)
#define MOVING_CLOCK _IOWR(MOTOR_MAGIC_NUM, MOTOR_IOCTL_NUM2, unsigned long *)

#endif
