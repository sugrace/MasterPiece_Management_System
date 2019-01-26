/**************************************************************************************/
/* Konkuk Univ. Department of Computer Science Enginnering                            */
/* 2017 Embedded System Software1 Class by prof.JIN                                   */
/* This project is term project for class and the project name is [ Baby in my Home ] */
/* Team 4 consists of KIM MIN SU & MOON JUN YOUNG & LEE CHANG OH                      */
/* This file is source code file of paho MQTT Client subscripter for HUB              */
/**************************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>

#include"MQTTClient.h"

#define HUB_ID "HUB"
#define STR_SIZE 64

volatile MQTTClient_deliveryToken token;

/* Function Prototype */
void delivered(void*, MQTTClient_deliveryToken);
int msgarrvd(void*, char*, int, MQTTClient_message*);
void connlost(void*, char*);
void* ringing_speaker(void*);

extern int assm_msg(char*, char*, char*, char*);
extern int baby_speaker_open(void);
extern int baby_speaker_close(int);
extern void run_air_plane(int);

static int flag = 0;

void delivered(void* context, MQTTClient_deliveryToken dt){
	printf("Message with token value %d delivery confirmed\n", dt);
	token = dt;
}

int msgarrvd(void* context, char* topicName, int topicLen, MQTTClient_message* message){
	char* payloadptr;
	char msg[STR_SIZE];
	char snd[STR_SIZE];
	char recv[STR_SIZE];
	char cont[STR_SIZE];
	pthread_t t_speaker;
	int ret;

	payloadptr = message->payload;
	strncpy(msg, payloadptr, message->payloadlen);
	msg[message->payloadlen] = '\0';

	if(assm_msg(msg, snd, recv, cont) == -1)
		return -1;
	if(strncmp(recv, HUB_ID, STR_SIZE) != 0)
		return 0;  //The message destination is not HUB
	printf("sender : %s\n", snd);
	printf("receiver : %s\n", recv);
	printf("message content : %s\n", cont);

	/*********** insert operation by message content **********/
	if(flag == 0 && (strncmp(cont, "pir trigger", STR_SIZE) == 0)){
		flag = 1;

		ret = pthread_create(&t_speaker, NULL, ringing_speaker, NULL);
	}

	return 1;
}

void connlost(void* context, char* cause){
	printf("\nConnection lost\n");
}

/* Ringing speaker */
void* ringing_speaker(void* ptr){
	int speaker;

	speaker = baby_speaker_open();
	run_air_plane(speaker);
	baby_speaker_close(speaker);
	
	flag = 0;
}
