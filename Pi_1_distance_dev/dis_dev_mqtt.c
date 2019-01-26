#include<string.h>
#include<stdio.h>

#include"MQTTClient.h"

#define STR_SIZE 64

volatile MQTTClient_deliveryToken deliveredtoken;

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

  
	printf("%s\n", msg);

	MQTTClient_freeMessage(&message);
	MQTTClient_free(topicName);
	return 1;
}

void connlost(void* context, char* cause){
	printf("\nConnection lost\n");
}
