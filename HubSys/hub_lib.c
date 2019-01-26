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
#define MAIN_TOPIC "hub_topic"
#define QOS 1
#define TIMEOUT 10000L
#define IP_ADDR_PATH "/home/ipdb/ipdb.txt"

#define STR_SIZE 64

/* Function Prototype */
int hub_MQTT_init(void);

extern int assm_msg(char*, char*, char*, char*);
extern int baby_speaker_open(void);
extern int baby_speaker_close(int);
extern void run_air_plane(int);

extern void delivered(void*, MQTTClient_deliveryToken);
extern int msgarrvd(void*, char*, int, MQTTClient_message*);
extern void connlost(void*, char*);

/* Global variables for HUB MQTT Protocol */
MQTTClient client;
MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
MQTTClient_message pubmsg = MQTTClient_message_initializer;
MQTTClient_deliveryToken token;

char ip_addr[STR_SIZE];

/* Die the program when error occur */
void DieWithError(char* mesg){
	char tmp[STR_SIZE];
	
	strncpy(tmp, "HUB : ", STR_SIZE);
	strncat(tmp, mesg, STR_SIZE);
	perror(tmp);
	exit(-1);
}

/* Message when error occur */
int MsgWithError(char* mesg){
	char tmp[STR_SIZE];

	strncpy(tmp, "HUB : ", STR_SIZE);
	strncat(tmp, mesg, STR_SIZE);
	perror(tmp);
	return -1;
}

/* Make IP Address */
void make_ip_addr(char* ip){
	strncpy(ip_addr, "tcp://", STR_SIZE);
	strncat(ip_addr, ip, STR_SIZE);
	strncat(ip_addr, ":1883", STR_SIZE);
}

int hub_MQTT_init(void){
	FILE* fp;
	char ip[STR_SIZE];

	fp = fopen(IP_ADDR_PATH, "r");  //Open IP DB
	if(fp < 0)	return MsgWithError("DB file open Error");
	
	fgets(ip, STR_SIZE, fp);  //Get MQTT Broker ip address from Database

	make_ip_addr(ip);

	fclose(fp);
}

int hub_MQTT_running(void){
	int rc;
	int ch;
	MQTTClient_create(&client, ip_addr, HUB_ID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = 1;

	MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);

	if((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS){
		printf("HUB_MQTT : Failed to connect, return code %d\n", rc);
		exit(EXIT_FAILURE);
	}
	MQTTClient_subscribe(client, MAIN_TOPIC, QOS);

	do{
		ch = getchar();
	}while(ch!='Q' && ch!='q');

	MQTTClient_disconnect(client, 10000);
	MQTTClient_destroy(&client);
	return rc;
}
