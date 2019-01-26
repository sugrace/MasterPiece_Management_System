/**************************************************************************************/
/* Konkuk Univ. Department of Computer Science Enginnering                            */
/* 2017 Embedded System Software1 Class by prof.JIN                                   */
/* This project is term project for class and the project name is [ Baby in my Home ] */
/* Team 4 consists of KIM MIN SU & MOON JUN YOUNG & LEE CHANG OH                      */
/* This file is API source code file of basement module for IoT device                */
/**************************************************************************************/
#include<sys/fcntl.h>
#include<sys/ioctl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>
#include<pthread.h>
#include<errno.h>
#include<wiringPi.h>
#include<wiringPiSPI.h>

#include"MQTTClient.h"

#include"baby_dev_mod.h"
#include"baby_dev_mod_lib.h"

#define STR_SIZE 64

void DieWithError(char*);
int MsgWithError(char*);
void make_ip_addr(char*);
int init_MQTT();
int baby_dev_open(void);
int baby_dev_close(int);
int MQTT_running(int);
void* readPIR_function(void*);
int read_mcp3008_adc(unsigned char);

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
int baby_dev_open(void){
  int fd;  //file descriptor
  char dev_addr[STR_SIZE];  //Device file path
  
  /* Making dev_addr device file path */
  strncpy(dev_addr, "/dev/", STR_SIZE);
  strncat(dev_addr, DEV_NAME, STR_SIZE);
  
  fd = open(dev_addr, O_RDWR);
      
  if(wiringPiSetup() == -1){
    fprintf (stdout, "Unable to start wiringPi: %s\n", strerror(errno));
    return 1 ;
  }
  
  if(wiringPiSPISetup(SPI_CHANNEL, SPI_SPEED) == -1){
    fprintf (stdout, "wiringPiSPISetup Failed: %s\n", strerror(errno));
    return 1 ;
  }

  pinMode(CS_MCP3008, OUTPUT);

  if(fd < 0)
    return MsgWithError("Device file open Error");
  else
    return fd;

}

/* Device close */
int baby_dev_close(int fd){
	int ret;
	
	ret = close(fd);

	if(ret < 0)	return MsgWithError("File close Error");
	else		return 1;
}

/* MQTT Running */
int MQTT_running(int fd){
	pthread_t readPIR;
	int rc;
	int ch;
	int tret;

	MQTTClient_create(&client, ip_addr, clientID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = 1;

	MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);

	if((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS){
		printf("Failed to connect, return code %d\n", rc);
		exit(EXIT_FAILURE);
	}
	MQTTClient_subscribe(client, topic, QOS);

	tret = pthread_create(&readPIR, NULL, readPIR_function, (void*)(&fd));
	if(tret)
		MsgWithError("Create Thread Error");

	do{
		ch = getchar();
	}while(ch!='Q' && ch!='q');

	MQTTClient_disconnect(client, 10000);
	MQTTClient_destroy(&client);
	return rc;
}

/* Read PIR */
void* readPIR_function(void* ptr){
  int fd = *((int*)ptr);
  int adcChannel = 0;
  int adcValue   = 0;
  int rc;
  char trig[STR_SIZE];
  
  strncpy(trig, DEV_ID, STR_SIZE);	
  strncat(trig, "-", STR_SIZE);
  strncat(trig, HUB_ID, STR_SIZE);
  strncat(trig, ":", STR_SIZE);
  strncat(trig, MSG, STR_SIZE);
  
  while(1){
    if(ioctl(fd, CHECK_FLAG, NULL)){
      adcValue = read_mcp3008_adc(adcChannel);
      printf("adc0 Value = %u\n", adcValue);
      
      if(adcValue > 150){
	ioctl(fd, MOVING_CLOCK, NULL);
	pubmsg.payload = trig;
	pubmsg.payloadlen = strlen(trig);
	pubmsg.qos = QOS;
	pubmsg.retained = 0;
	MQTTClient_publishMessage(client, topic, &pubmsg, &token);
	rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
      }
      usleep(100000);
      
    }
  }
}

int read_mcp3008_adc(unsigned char adcChannel)
{
  unsigned char buff[3];
  int adcValue = 0;
  
  buff[0] = 0x01;
  buff[1] = 0x80 | ((adcChannel & 0x07) << 4);
  buff[2] = 0x00;
  
  digitalWrite(CS_MCP3008, 0);  // Low : CS Active

  wiringPiSPIDataRW(SPI_CHANNEL, buff, 3);

  buff[1] = 0x03 & buff[1];
  adcValue = ( buff[1] << 8) | buff[2];

  digitalWrite(CS_MCP3008, 1);  // High : CS Inactive

  return adcValue;
}
