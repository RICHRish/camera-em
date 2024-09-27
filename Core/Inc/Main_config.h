/*
 * MAIN_CONFIG.h
 *
 *  Created on: Sep 17, 2023
 *      Author: Kriti
 */

#ifndef INC_MAIN_CONFIG_H_
#define INC_MAIN_CONFIG_H_
#include "main.h"

#define DEBUG_MODE
//#define CDC_USB_DEBUG
#define UART_DEBUG


#define IMAGE_SIZE      7000

#define RGB_UART	    huart8
#define NIR_UART	    huart6
#define OBC_UART  	    huart4
#define DEBUG_UART	    huart2
#define FM           	hspi2

#define HEADER 		 	0xBA
#define FOOTER 		 	0xFE


#define ACT_CMD_LEN		3
#define IMG_RCV_LEN		12000
#define QUALITY_LEN		8
#define CMD_BUF_LEN   	13
#define CAM_CMD_LEN   	5
#define ACK_LEN       	5
#define PCKT_DATA_LEN 	80
#define RGB_SIZE_LEN    4
#define NIR_SIZE_LEN    4

#define TIMEOUT_DURATION  50000 // milliseconds
#define TIME_LEN		4

#define SUCCESS_LEN		13

#define SECTOR_SIZE 65536
#define CAM_MSN_DATA_START_ADDRESS SECTOR_SIZE*765
#define CAM_MSN_DATA_END_ADDRESS   SECTOR_SIZE*1614
#endif /* INC_MAIN_CONFIG_H_ */
