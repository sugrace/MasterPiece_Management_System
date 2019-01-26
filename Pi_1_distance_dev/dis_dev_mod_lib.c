#include<sys/fcntl.h>
#include<sys/ioctl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>
#include<pthread.h>

#include"MQTTClient.h"

#include"dis_dev_mod.h"
#include"dis_dev_mod_lib.h"

#define STR_SIZE 64

void DieWithError(char*);
int MsgWithError(char*);
void make_ip_addr(char*);
int init_MQTT();
int dis_dev_open(void);
int dis_dev_close(int);
int MQTT_running(int);
void* readPIR_function(void*);

extern void delivered(void*, MQTTClient_deliveryToken);
extern int msgarrvd(void*, char*, int, MQTTClient_message*);
extern void connlost(void*, char*);

/* Global variables for MQTTClient */
char clientID[STR_SIZE];
char topic[STR_SIZE];
char ip_addr[STR_SIZE];


/* Global variables for MQTT */
MQTTClient client;
MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
MQTTClient_message pubmsg = MQTTClient_message_initializer;
MQTTClient_deliveryToken token;

/* Die the program when error occur */
void DieWithError(char* mesg){
	char tmp[STR_SIZE];
	
	strncpy(tmp, "DEV_LIB : ", STR_SIZE);
	strncat(tmp, mesg, STR_SIZE);
	perror(tmp);
	exit(-1);
}

/* Message when error occur */
int MsgWithError(char* mesg){
	char tmp[STR_SIZE];

	strncpy(tmp, "DEV_LIB : ", STR_SIZE);
	strncat(tmp, mesg, STR_SIZE);
	perror(tmp);
	return -1;
}

/* Make IP Address */
void make_ip_addr(char* ip){
	strncpy(ip_addr, "tcp://", STR_SIZE);
	strncat(ip_addr, ip, STR_SIZE);
        strncat(ip_addr, ":1883", STR_SIZE);
        printf("%s\n",ip_addr);
}

/* Initializing MQTTClient environment */
int init_MQTT(){
	FILE* fp;
	char ip[STR_SIZE];

	fp = fopen(IP_ADDR_PATH, "r");  //open IP DB
	if(fp < 0)	return MsgWithError("DB file open Error");
	
	fgets(ip, STR_SIZE, fp);  //Get MQTT Broker ip address from Database
        
	strncpy(clientID, DEV_ID, STR_SIZE);
	strncpy(topic, TOPIC, STR_SIZE);
	make_ip_addr(ip);

	fclose(fp);
}

/* Device open */
int dis_dev_open(void){
	int fd;
	char dev_addr[STR_SIZE];  //Device file path

	/* Making dev_addr device file path */
	strncpy(dev_addr, "/dev/", STR_SIZE);
	strncat(dev_addr, DEV_NAME, STR_SIZE);

	fd = open(dev_addr, O_RDWR);

	if(fd < 0)	return MsgWithError("Device file open Error");
	else		return fd;
}



long MQTT_publish(int fd){
int rc;
char ch;
char trig[STR_SIZE];
if(ioctl(fd, CHECK, NULL) == 1) 
{
MQTTClient_create(&client, ip_addr, "publisher", MQTTCLIENT_PERSISTENCE_NONE, NULL);
	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = 1;

	MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);

	if((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS){
		printf("Failed to connect, return code %d\n", rc);
		exit(EXIT_FAILURE);
	}


strncpy(trig, DEV_ID, STR_SIZE);	
strncat(trig, "-", STR_SIZE);
strncat(trig, HUB_ID, STR_SIZE);
strncat(trig, ":", STR_SIZE);
strncat(trig, "emergency", STR_SIZE);

pubmsg.payload = trig;
pubmsg.payloadlen = strlen(trig);
pubmsg.qos = QOS;
pubmsg.retained = 0;

MQTTClient_publishMessage(client, topic, &pubmsg, &token);


rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);

        MQTTClient_disconnect(client, 10000);
	MQTTClient_destroy(&client);

return 1;
}



   return 0;

}





