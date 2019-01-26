/**************************************************************************************/
/* Konkuk Univ. Department of Computer Science Enginnering                            */
/* 2017 Embedded System Software1 Class by prof.JIN                                   */
/* This project is term project for class and the project name is [ Baby in my Home ] */
/* Team 4 consists of KIM MIN SU & MOON JUN YOUNG & LEE CHANG OH                      */
/* This file is source code file of paho MQTT Client subscripter                      */
/**************************************************************************************/
#include<string.h>
#include<stdio.h>

#include"MQTTClient.h"

#define STR_SIZE 64

volatile MQTTClient_deliveryToken deliveredtoken;

void delivered(void* context, MQTTClient_deliveryToken dt){
	printf("Message with token value %d delivery confirmed\n", dt);
	deliveredtoken = dt;
}

int msgarrvd(void* context, char* topicName, int topicLen, MQTTClient_message* message){
	int i;
	char* payloadptr;
	char msg[STR_SIZE];

	payloadptr = message->payload;
	strncpy(msg, payloadptr, message->payloadlen);
	msg[message->payloadlen] = '\0';

	printf("%s\n", msg);

	MQTTClient_freeMessage(&message);
	MQTTClient_free(topicName);
	return 1;
}

void connlost(void* context, char* cause){
	printf("\nConnection lost\n");
}
