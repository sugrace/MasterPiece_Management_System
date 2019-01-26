#include<string.h>
#include <stdio.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>

#include"MQTTClient.h"

#define STR_SIZE 64
#define DEV_NAME "mm_motor_mod_dev"

#define MOTOR_IOCTL_START_NUM 0xE551
#define MOTOR_IOCTL_NUM1 MOTOR_IOCTL_START_NUM+1
#define MOTOR_IOCTL_NUM2 MOTOR_IOCTL_START_NUM+2

#define MOTOR_MAGIC_NUM 'm'
#define MOVING_UP _IOWR(MOTOR_MAGIC_NUM, MOTOR_IOCTL_NUM1, unsigned long *)
#define MOVING_DOWN _IOWR(MOTOR_MAGIC_NUM, MOTOR_IOCTL_NUM2, unsigned long *)

volatile MQTTClient_deliveryToken deliveredtoken;
int fd;

void delivered(void* context, MQTTClient_deliveryToken dt){
	
         printf("delivered active!!\n");
        printf("Message with token value %d delivery confirmed\n", dt);
	deliveredtoken = dt;
}

int msgarrvd(void* context, char* topicName, int topicLen, MQTTClient_message* message){
	int i;
	char* payloadptr;
	char msg[STR_SIZE];

	printf("subscribe active!!\n");
	payloadptr = message->payload;
	strncpy(msg, payloadptr, message->payloadlen);
	msg[message->payloadlen] = '\0';
        fd=topicName[topicLen];
        topicName[topicLen-1]='\0';
  	// if msg에 따라서 motor ioctl activate ...
	printf("%s\n", msg);
        ioctl(fd,MOVING_UP,NULL);
	MQTTClient_freeMessage(&message);
	MQTTClient_free(topicName);
	return 1;
}

void connlost(void* context, char* cause){
	printf("\nConnection lost\n");
}
