/**************************************************************************************/
/* Konkuk Univ. Department of Computer Science Enginnering                            */
/* 2017 Embedded System Software1 Class by prof.JIN                                   */
/* This project is term project for class and the project name is [ Baby in my Home ] */
/* Team 4 consists of KIM MIN SU & MOON JUN YOUNG & LEE CHANG OH                      */
/* This file is source code file for speaker module library                           */
/**************************************************************************************/
#include<sys/fcntl.h>
#include<sys/ioctl.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include"baby_speaker.h"

#define STR_SIZE 64

extern void DieWithError(char*);
extern int MsgWithError(char*);
int baby_speaker_open(void);
int baby_speaker_close(int);
void run_air_plane(int);

/* Die the program when error occur */
/*void DieWithError(char* mesg){
	MsgWithError(mesg);

	exit(-1);
}
*/
/* Message when error occur */
/*int MsgWithError(char* mesg){
	char tmp[STR_SIZE];

	strncpy(tmp, "SPEAKER_LIB : ", STR_SIZE);
	strncat(tmp, mesg, STR_SIZE);
	perror(tmp);
	return -1;
}
*/
int baby_speaker_open(void){
	int fd;  //file descripter
	char dev_addr[STR_SIZE];  //Device file path

	/* Making dev_addr device file path */
	strncpy(dev_addr, "/dev/", STR_SIZE);
	strncat(dev_addr, DEV_NAME, STR_SIZE);

	fd = open(dev_addr, O_RDWR);
	
	if(fd < 0)	return MsgWithError("Device file open Error");
	else		return fd;
}

/* Device close */
int baby_speaker_close(int fd){
	int ret;

	ret = close(fd);

	if(ret < 0)	return MsgWithError("File close Error");
	else		return 1;
}

/* Start song air plane */
void run_air_plane(int fd){
	ioctl(fd, AIR_PLANE, NULL);
}

/* Get flag value */
int get_flag(int fd){
	return ioctl(fd, FLAG, NULL);
}
