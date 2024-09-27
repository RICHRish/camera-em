/*
 * variables.c
 *
 *  Created on: Sep 17, 2024
 *      Author: kriti
 */
#include "variables.h"

///***-----OBC HANDSHAKE------****///
uint8_t OBC_HANDSHAKE_RX[5];
uint8_t OBC_HANDSHAKE_TX[5];

///FLAGS
uint8_t OBC_HANDSHAKE_SUCCES = 0;
uint8_t OBC_RX_FLAG = 0;
uint8_t OBC_TX_FLAG = 0;
uint8_t NIR_CAM_RX_FLAG = 0;
uint8_t RGB_CAM_RX_FLAG=0;
uint8_t CAM_TX_FLAG = 0;
uint8_t DEBUG_RX_FLAG = 0;
uint8_t DEBUG_TX_FLAG = 0;
uint8_t RGB_Img_RDY = 0;
uint8_t NIR_Img_RDY = 0;
////-----FLASH_MEMORY---//
uint32_t Flash_memory = 0x02fea000;// starting address of the flash memory

////-----OBC_CAM_MISSION_COMMUNICTAION---//
uint8_t OBC_COMMAND[ACT_CMD_LEN];
uint8_t OBC_CMD_RX[CMD_BUF_LEN];
uint8_t OBC_ACK[ACK_LEN];
uint8_t CAM_CMD[CAM_CMD_LEN];
uint8_t CAM_CMD2[CAM_CMD_LEN];
uint8_t Whole_Img_RGB[IMG_RCV_LEN];
uint8_t Whole_Img_NIR[IMG_RCV_LEN];
uint8_t decimalvalue[5];
uint8_t size_num_RGB[5];
uint8_t size_rgb_img = 0;
uint8_t size_nir_img = 0;
//uint8_t size_nir_img[SIZE_NIR_LEN];
uint32_t total_image_size_RGB = 0;
uint32_t rgb_img_len = 0;
uint32_t total_received = 0;
uint32_t start_time;
uint16_t RGB_size = 0;
uint16_t NIR_size = 0;
