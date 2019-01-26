컴파일 순서

우분투 

user@ubuntu~$ make
user@ubuntu~$ scp main.c dis_dev_mod_lib.c dis_dev_mod.h dis_dev_mod_lib.c dis_dev_mod_lib.h dis_dev_mqtt.c main.c  pi@10.10.10.13:~/


라즈베리파이
/home/ipdb/ipdb.txt 파일 생성후 Pi2 system의 ip 입력
mqtt broker 및 client 설치
paho 설치


root@raspberrypi:/home/pi#insmod dis_dev_mod.ko
root@raspberrypi:/home/pi#sh dis_dev_mknod.sh
root@raspberrypi:/home/pi# gcc -o main  main.c dis_dev_mod_lib.c dis_dev_mqtt.c -lpaho-mqtt3c
root@raspberrypi:/home/pi# ./main

