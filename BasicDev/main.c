#include<stdio.h>

int main(void){
	int fd;

	fd = baby_dev_open();

	init_MQTT();

	MQTT_running(fd);

	baby_dev_close(fd);

	return 0;
}
