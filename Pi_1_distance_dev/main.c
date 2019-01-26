#include<stdio.h>
#include<unistd.h>


int main(void){
	int fd=0;
        long ret =0;
	
        fd = dis_dev_open();

	init_MQTT();
  while(1){ 
    while(1){
       ret= MQTT_publish(fd);
 	//MQTT_subscribe();
	//MQTT_running(fd);
        
        if(ret==1){
 	  ioctl(fd, 1, NULL);
           break;
	  }
          sleep(0.8);
        }
      sleep(1);
    }
 
        dis_dev_close(fd);

	return 0;
}
