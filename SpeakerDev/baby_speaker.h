/**************************************************************************************/
/* Konkuk Univ. Department of Computer Science Enginnering                            */
/* 2017 Embedded System Software1 Class by prof.JIN                                   */
/* This project is term project for class and the project name is [ Baby in my Home ] */
/* Team 4 consists of KIM MIN SU & MOON JUN YOUNG & LEE CHANG OH                      */
/* This file is header file for speaker module                                        */
/**************************************************************************************/
#ifndef H_SPEAKER
#define H_SPEAKER

#define SPEAKER 17
#define SWITCH 5
#define LED 19

#define DEV_NAME "baby_speaker_mod_dev"

#define SPEAKER_IOCTL_START_NUM 0xE551
#define SPEAKER_IOCTL_NUM1 SPEAKER_IOCTL_START_NUM+1
#define SPEAKER_IOCTL_NUM2 SPEAKER_IOCTL_START_NUM+2
#define SWITCH_IOCTL_NUM SPEAKER_IOCTL_START_NUM+3

#define SPEAKER_MAGIC_NUM 's'
#define AIR_PLANE _IOWR(SPEAKER_MAGIC_NUM, SPEAKER_IOCTL_NUM1, unsigned long *)
#define FLAG _IOWR(SPEAKER_MAGIC_NUM, SPEAKER_IOCTL_NUM2, unsigned long *)
#define CHECK _IOWR(SPEAKER_MAGIC_NUM, SWITCH_IOCTL_NUM, unsigned long *)

#endif
