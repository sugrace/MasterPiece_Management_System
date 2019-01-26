#include <stdio.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#define DEV_NAME "mm_motor_mod_dev"

#define MOTOR_IOCTL_START_NUM 0xE551
#define MOTOR_IOCTL_NUM1 MOTOR_IOCTL_START_NUM+1
#define MOTOR_IOCTL_NUM2 MOTOR_IOCTL_START_NUM+2

#define MOTOR_MAGIC_NUM 'm'
#define MOVING_UP _IOWR(MOTOR_MAGIC_NUM, MOTOR_IOCTL_NUM1, unsigned long *)
#define MOVING_DOWN _IOWR(MOTOR_MAGIC_NUM, MOTOR_IOCTL_NUM2, unsigned long *)

int main(void){
  	int fd;
	unsigned long value = 0;
	fd = open("/dev/mm_motor_mod_dev",O_RDWR);
	printf("Before ioctl_success\n");	
	
        MQTT_subscribe(fd);

         
	printf("after ioctl_success\n");	
	//ioctl(fd,MOVING_DOWN,&value);

	//init_MQTT(); 
 	//MQTT_subscribe();
	//MQTT_running(fd);
	//dis_dev_close(fd);

	close(fd);
 return 0;
}
