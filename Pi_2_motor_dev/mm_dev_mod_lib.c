#include<sys/fcntl.h>
#include<sys/ioctl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>
#include<pthread.h>
 
#include <errno.h> 
#include <wiringPi.h> 
#include <wiringPiSPI.h> 
 
#define CS_MCP3208 8 //GPIO 8 
#define SPI_CHANNEL 0
#define SPI_SPEED 1000000 //1Mhz
#include"MQTTClient.h"

#include"mm_dev_mod_lib.h"
#include"mm_motor.h"


#define DEV_NAME "mm_motor_mod_dev"

#define MOTOR_IOCTL_START_NUM 0xE551
#define MOTOR_IOCTL_NUM1 MOTOR_IOCTL_START_NUM+1
#define MOTOR_IOCTL_NUM2 MOTOR_IOCTL_START_NUM+2

#define MOTOR_MAGIC_NUM 'm'
#define MOVING_UP _IOWR(MOTOR_MAGIC_NUM, MOTOR_IOCTL_NUM1, unsigned long *)
#define MOVING_DOWN _IOWR(MOTOR_MAGIC_NUM, MOTOR_IOCTL_NUM2, unsigned long *)

#define STR_SIZE 64

void DieWithError(char*);
int MsgWithError(char*);
void make_ip_addr(char*);
int init_MQTT();
int mm_dev_open(void);
int mm_dev_close(int);
int MQTT_running(int);
 void* light_function(void* ptr);
int read_mcp3208_adc(unsigned char adcChannel);

extern void delivered(void*, MQTTClient_deliveryToken);
extern int msgarrvd(void*, char*, int, MQTTClient_message*);
extern void connlost(void*, char*);

char clientID[STR_SIZE];
char topic[STR_SIZE];
char ip_addr[STR_SIZE];

MQTTClient client;
MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
MQTTClient_message pubmsg = MQTTClient_message_initializer;
MQTTClient_deliveryToken token;

void DieWithError(char* mesg){
	char tmp[STR_SIZE];
	
	strncpy(tmp, "DEV_LIB : ", STR_SIZE);
	strncat(tmp, mesg, STR_SIZE);
	perror(tmp);
	exit(-1);
}

int MsgWithError(char* mesg){
	char tmp[STR_SIZE];

	strncpy(tmp, "DEV_LIB : ", STR_SIZE);
	strncat(tmp, mesg, STR_SIZE);
	perror(tmp);
	return -1;
}

void make_ip_addr(char* ip){
	strncpy(ip_addr, "tcp://", STR_SIZE);
	strncat(ip_addr, ip, STR_SIZE);
        strncat(ip_addr, ":1883", STR_SIZE);
        printf("%s\n",ip_addr);
}

int init_MQTT(){
	FILE* fp;
	char ip[STR_SIZE];

	fp = fopen(IP_ADDR_PATH, "r");  
	if(fp < 0)	return MsgWithError("DB file open Error");
	
	fgets(ip, STR_SIZE, fp);  
        
	strncpy(clientID, DEV_ID, STR_SIZE);
	strncpy(topic, TOPIC, STR_SIZE);
	make_ip_addr(ip);

	fclose(fp);
}

int mm_dev_open(void){
	int fd; 
	char dev_addr[STR_SIZE];  
	strncpy(dev_addr, "/dev/", STR_SIZE);
	strncat(dev_addr, DEV_NAME, STR_SIZE);

	fd = open(dev_addr, O_RDWR);

	if(fd < 0)	return MsgWithError("Device file open Error");
	else		return fd;
}

int mm_dev_close(int fd){
	int ret;
	
	ret = close(fd);

	if(ret < 0)	return MsgWithError("File close Error");
	else		return 1;
}

int MQTT_subscribe(int fd){
int rc;
	int ch;
	int tret;
   	MQTTClient_create(&client, ip_addr, "subscriber", MQTTCLIENT_PERSISTENCE_NONE, NULL);
	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = 1;

          MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);

	if((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS){
		printf("Failed to connect, return code %d\n", rc);
		exit(EXIT_FAILURE);
	}
MQTTClient_subscribe(client, topic, QOS);
       

    
    printf("start");
     if(wiringPiSetupGpio() == -1)
    {
        fprintf(stdout, "Unable to start wiringPi :%s\n", strerror(errno));
        return 1;
    }
    
    if(wiringPiSPISetup(SPI_CHANNEL, SPI_SPEED) == -1)
    {
        fprintf(stdout, "wiringPiSPISetup Failed :%s\n", strerror(errno));
        return 1;
    }
    pinMode(CS_MCP3208, OUTPUT);

do {
 ch =getchar();
}while(ch!='Q' && ch!='q');
   
	
	MQTTClient_disconnect(client, 10000);
	MQTTClient_destroy(&client);
	return 0;
}

int read_mcp3208_adc(unsigned char adcChannel) 
{
    unsigned char buff[3];
    int adcValue = 0;
    
 
    buff[0] = 0x06 | ((adcChannel & 0x07) >> 2);
    buff[1] = ((adcChannel & 0x07) << 6);
    buff[2] = 0x00;
    
    digitalWrite(CS_MCP3208, 0);
    wiringPiSPIDataRW(SPI_CHANNEL, buff, 3);
    
    buff[1] = 0x0f & buff[1];
    adcValue = (buff[1] << 8 ) | buff[2];
    
    digitalWrite(CS_MCP3208, 1);

    return adcValue;
}



void* light_function(void* ptr){
unsigned char adcChannel_light = 0;
 
    int adcValue_light = 0;
 
    float vout_light;
 float vout_oftemp;
    float percentrh = 0;
    float supsiondo = 0;
  

  while(1)
    {
        adcValue_light = read_mcp3208_adc(adcChannel_light);
        printf("light sensor = %u\n", adcValue_light);

         delay(1000);
    }
}




