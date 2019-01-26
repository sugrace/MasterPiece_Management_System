/**************************************************************************************/
/* Konkuk Univ. Department of Computer Science Enginnering                            */
/* 2017 Embedded System Software1 Class by prof.JIN                                   */
/* This project is term project for class and the project name is [ Baby in my Home ] */
/* Team 4 consists of KIM MIN SU & MOON JUN YOUNG & LEE CHANG OH                      */
/* This file is API source code header file of basement module for IoT device         */
/**************************************************************************************/
#ifndef H_DEV_MOD_LIB
#define H_DEV_MDO_LIB

#define HUB_ID "HUB"
#define DEV_ID "BED"
#define TOPIC "hub_topic"
#define IP_ADDR_PATH "/home/ipdb/ipdb.txt"
#define QOS 1
#define TIMEOUT 10000L

#define MSG "cry cry"

#define CS_MCP3008  6        // BCM_GPIO 25                                    
#define SPI_CHANNEL 0
#define SPI_SPEED   1000000  // 1MHz

#endif
