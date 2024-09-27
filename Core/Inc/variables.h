/*
 * variables.h
 *
 *  Created on: Sep 17, 2024
 *      Author: kriti
 */

#ifndef INC_VARIABLES_H_
#define INC_VARIABLES_H_

#include <Main_config.h>
#include "MT25QL.h"
#include "string.h"


extern uint8_t OBC_HANDSHAKE_RX[5];
extern uint8_t OBC_HANDSHAKE_TX[5];


////FLAGS////
extern uint8_t OBC_HANDSHAKE_SUCCES;
extern uint8_t OBC_RX_FLAG;
extern uint8_t OBC_TX_FLAG;
extern uint8_t CAM_TX_FLAG;
extern uint8_t DEBUG_RX_FLAG;
extern uint8_t DEBUG_TX_FLAG;
extern uint8_t OPEN_MV_FLAG;

extern uint8_t RGB_Img_RDY;
extern uint8_t NIR_Img_RDY;

extern uint8_t decimalvalue[5];

extern uint8_t Whole_Img_RGB[IMG_RCV_LEN];
extern uint8_t Whole_Img_NIR[IMG_RCV_LEN];

extern uint8_t size_num_RGB[5];
extern uint8_t size_rgb_img;
extern uint8_t size_nir_img;
extern uint32_t rgb_img_len;
extern uint32_t total_received;
extern uint16_t RGB_size;
extern uint16_t NIR_size;
////-----OBC_CAM_MISSION_CMMUNICTAION---//
extern uint8_t OBC_COMMAND[ACT_CMD_LEN];
extern uint8_t OBC_CMD_RX[CMD_BUF_LEN];
extern uint8_t OBC_ACK[ACK_LEN] ;
extern uint8_t CAM_CMD[CAM_CMD_LEN];
extern uint8_t CAM_CMD2[CAM_CMD_LEN];
extern uint8_t CAM_ACK_RX[ACK_LEN];
#endif /* INC_VARIABLES_H_ */
