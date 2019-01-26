컴파일 순서
우분투 $ make user@ubuntu~$ scp main.c mm_dev_mod_lib.c mm_dev_mod.h mm_dev_mod_lib.c mm_dev_mod_lib.h
mm_dev_mqtt.c main.c  pi@10.10.10.13:~/

라즈베리파이
/home/ipdb/ipdb.txt 파일 생성후

Pi2 system의 ip 입력
mqtt broker 및 client 설치
paho 설치
root@raspberrypi:/home/pi#insmod mm_motor.ko
root@raspberrypi:/home/pi#sh mknod.sh
root@raspberrypi:/home/pi# gcc -o main  main.c mm_dev_mod_lib.c mm_dev_mqtt.c -lpaho-mqtt3c
root@raspberrypi:/home/pi# ./main