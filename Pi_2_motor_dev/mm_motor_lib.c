#include<sys/fcntl.h>
#include<sys/ioctl.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>


#define STR_SIZE 64

void DieWithError(char*);
int MsgWithError(char*);
int mm_motor_open(void);
int mm_motor_close(int);
void moving_up(int);
void moving_down(int fd);

/* Die the program when error occur */
void DieWithError(char* mesg){
	MsgWithError(mesg);
	exit(-1);
}

/* Message when error occur */
int MsgWithError(char* mesg){
	char tmp[STR_SIZE];

	strncpy(tmp, "MOTOR_LIB : ", STR_SIZE);
	strncat(tmp, mesg, STR_SIZE);
	perror(tmp);
	return -1;
}

int mm_motor_open(void){
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
int mm_motor_close(int fd){
	int ret;

	ret = close(fd);

	if(ret < 0)	return MsgWithError("File close Error");
	else		return 1;
}

/* Start moving reverse clock direction */
void moving_up(int fd){
	ioctl(fd, MOVING_UP, NULL);
}

/* Start moving clock direction */
void moving_down(int fd){
	ioctl(fd, MOVING_DOWN, NULL);
}

