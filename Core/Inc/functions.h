/*
 * functions.h
 *
 *  Created on: Sep 17, 2024
 *      Author: kriti
 */

#ifndef INC_FUNCTIONS_H_
#define INC_FUNCTIONS_H_

#endif
#include "variables.h"
#include "stdarg.h"
#ifdef CDC_USB_DEBUG
#include "usbd_cdc_if.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"

#endif
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
int buffersize(char *buff);
int buffer(uint8_t *get_size);

#ifdef DEBUG_MODE
#ifndef TRACE_MODE
void myprintf(const char *fmt, ...);
#endif //def TRACE_MODE

uint32_t BASIC_IMAGE_CAPTURE();
void Image_len();

uint32_t Receive_Image_Size_NIR(uint32_t ADDRESS);
uint32_t Receive_Image_Size_RGB();
void Image_Quality();



void OBC_HANDSHAKE();

void CS_Enable_RGB();
void CS_Disable_RGB();
void CS_Enable_NIR();
void CS_Disable_NIR();
void OCP_EN();

#endif /* INC_FUNCTIONS_H_ */
