/*
 * functions.c
 *
 *  Created on: Sep 17, 2024
 *      Author: kriti
 */
#include "functions.h"
#include <stdlib.h>
#include<math.h>
#include "stm32f4xx_hal.h"
/*
 * @brief	counts the number of non-null data in given array
 *
 * @param	buff	pointer to the array of data to be counted
 * @retval	int		number of non-null values in the array
 */

#ifdef DEBUG_MODE
#ifndef TRACE_MODE

/*
 * @brief	Outputs the string data to the PC, via USB CDC
 *
 * @param	fmt	pointer the array of characters data to be transmitted
 *
 * @retval	none
 */

int buffersize(char *buff) {
	int i = 0;
	while (*buff++ != '\0')
		i++;
	return i;
}
void myprintf(const char *fmt, ...) {
	static char temp[100];
	va_list args;
	va_start(args, fmt);
	vsnprintf(temp, sizeof(temp), fmt, args);
	va_end(args);
	int len = buffersize(temp);
#ifdef CDC_USB_DEBUG
	CDC_Transmit_FS((uint8_t*) temp, len);
//	HAL_Delay(1);
#endif
#ifdef UART_DEBUG

	// Add the code here for printing data to serial monitor via software simulated UART
	HAL_UART_Transmit(&DEBUG_UART, (uint8_t*) temp, len, 1000);
//	while (DEBUG_TX_FLAG != 1) {
//	}
//	DEBUG_TX_FLAG = 0;
#endif
}
#endif
//static volatile bool uart_rx_done = false;

/*
 * @brief	transfer debug information to SWTrace analyzer in Trace Debug mode of STLink
 *
 */
#ifdef TRACE_MODE
int _write(int32_t file, uint8_t *ptr, int32_t len)
{
    for (int i = 0; i < len; i++)
    {
        ITM_SendChar(*ptr++);
    }
    return len;
}
#endif
#endif
typedef enum {
	IDLE, AWAITING_COMMAND, RECEIVING_SIZE, RECEIVING_IMAGE
} CaptureState;

volatile CaptureState current_state = IDLE;
void DMA1_Channel1_IRQHandler(void) {
	HAL_DMA_IRQHandler(RGB_UART.hdmarx);
}

extern DMA_HandleTypeDef hdma_uart8_rx;
extern int a;

//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
//if (huart->Instance == UART8) {
//	total_received += IMG_RCV_LEN;
//	 uint32_t bytes_remaining = __HAL_DMA_GET_COUNTER(huart->hdmarx);
//	uint32_t bytes_received = IMG_RCV_LEN - bytes_remaining;
//	total_received += bytes_received;
//	if (total_received >= 2){
//		if (Whole_Img_RGB[total_received - 2] == 0xFF && Whole_Img_RGB[total_received - 1] == 0xD9) {
//		 rgb_img_len = total_received;  // Set image length
//		 RGB_CAM_RX_FLAG = 1;           // Set the flag to indicate completion
//		 myprintf("End marker (FF D9) detected. Image length: %lu bytes\r\n", rgb_img_len);
//
//		 HAL_UART_DMAStop(&RGB_UART);   // Stop further DMA reception
//		 myprintf("Data reception complete\r\n");
//
//		 RGB_CAM_RX_FLAG = 0;           // Reset the flag after use
//		 HAL_Delay(100);                // Delay for stability (if needed)
//		 return;
//			}
//	   }
//		 HAL_UART_Receive_DMA(&RGB_UART, Whole_Img_RGB + total_received, IMG_RCV_LEN - total_received);
//
//	}
//
//}

//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
//	CAM_CMD2[0] = 'S';
//	CAM_CMD2[1] = 'E';
//	CAM_CMD2[2] = 'N';
//	CAM_CMD2[3] = 'D';
//	if (huart == &RGB_UART) {
//		if (size_rgb_img == 0) {
//			RGB_size = ((Whole_Img_RGB[0] - 48) * 1000)
//					+ ((Whole_Img_RGB[1] - 48) * 100)
//					+ ((Whole_Img_RGB[2] - 48) * 10)
//					+ ((Whole_Img_RGB[3] - 48));
//			myprintf("Image size received: %d bytes\n", RGB_size);
//			size_rgb_img = 1;
//
////			HAL_UART_Transmit(&RGB_UART, CAM_CMD2, sizeof(CAM_CMD2), 1000);
////			HAL_Delay(100);
//
//			HAL_UART_Receive_DMA(&RGB_UART, Whole_Img_RGB, RGB_size);
//		} else if (size_rgb_img == 1) {
//			if (Whole_Img_RGB[RGB_size - 2] == 0xFF
//					&& Whole_Img_RGB[RGB_size - 1] == 0xD9) {
//				myprintf(
//						"End marker (0xFF, 0xD9) detected, Image received successfully.\n");
////				RGB_Img_RDY =1;
//			} else {
//				myprintf(
//						"Warning: End marker (0xFF, 0xD9) not found. Data might be incomplete.\n");
//			}
//			myprintf("Image Data in Hexadecimal:\n");
//			for (uint32_t i = 0; i < RGB_size; i++) {
//				myprintf("0x%02X ", Whole_Img_RGB[i]);
//			}
//			myprintf("\n");
//
//			size_rgb_img = 0;
//			myprintf("Ready to receive next image size.\n");
//
//			if (HAL_UART_Transmit(&RGB_UART, CAM_CMD, sizeof(CAM_CMD), 3000)
//					== HAL_OK) {
//				myprintf("Command transmitted to RGB CAM\r\n");
//
//				HAL_UART_Receive_DMA(&RGB_UART, Whole_Img_RGB, RGB_SIZE_LEN);
//			} else {
//				myprintf("Failed to transmit command\r\n");
//			}
//
//			HAL_UART_Receive_DMA(&RGB_UART, Whole_Img_RGB, RGB_SIZE_LEN);
//		}
//	}
//
//}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	CAM_CMD2[0] = 'S';
	CAM_CMD2[1] = 'E';
	CAM_CMD2[2] = 'N';
	CAM_CMD2[3] = 'D';
	if (huart == &RGB_UART) {
		if (size_rgb_img == 0) {
			RGB_size = ((Whole_Img_RGB[0] - 48) * 1000)
					+ ((Whole_Img_RGB[1] - 48) * 100)
					+ ((Whole_Img_RGB[2] - 48) * 10)
					+ ((Whole_Img_RGB[3] - 48));
			myprintf("Image size received: %d bytes\n", RGB_size);
			size_rgb_img = 1;

			HAL_UART_Receive_DMA(&RGB_UART, Whole_Img_RGB, RGB_size);
		} else if (size_rgb_img == 1) {
			if (Whole_Img_RGB[RGB_size - 2] == 0xFF
					&& Whole_Img_RGB[RGB_size - 1] == 0xD9) {
				myprintf(
						"End marker (0xFF, 0xD9) detected, Image received successfully.\n");
				//				RGB_Img_RDY =1;
			} else {
				myprintf(
						"Warning: End marker (0xFF, 0xD9) not found. Data might be incomplete.\n");
			}
			myprintf("Image Data in Hexadecimal:\n");
			for (uint32_t i = 0; i < RGB_size; i++) {
				myprintf("0x%02X ", Whole_Img_RGB[i]);
			}
			myprintf("\n");

			size_rgb_img = 0;
			myprintf("Ready to receive next image size.\n");

//					if (HAL_UART_Transmit(&RGB_UART, CAM_CMD, sizeof(CAM_CMD), 3000)
//							== HAL_OK) {
//						myprintf("Command transmitted to RGB CAM\r\n");
//
//						HAL_UART_Receive_DMA(&RGB_UART, Whole_Img_RGB, RGB_SIZE_LEN);
//					} else {
//						myprintf("Failed to transmit command\r\n");
//					}
//
//					HAL_UART_Receive_DMA(&RGB_UART, Whole_Img_RGB, RGB_SIZE_LEN);
//				}
		} else if (huart == &NIR_UART) {
			if (size_nir_img == 0) {
				NIR_size = ((Whole_Img_NIR[0] - 48) * 1000)
						+ ((Whole_Img_NIR[1] - 48) * 100)
						+ ((Whole_Img_NIR[2] - 48) * 10)
						+ ((Whole_Img_NIR[3] - 48));
				myprintf("Image size received: %d bytes\n", NIR_size);
				size_nir_img = 1;

				//			HAL_UART_Transmit(&NIR_UART, CAM_CMD2, sizeof(CAM_CMD2), 1000);
				//			HAL_Delay(100);

				HAL_UART_Receive_DMA(&NIR_UART, Whole_Img_NIR, NIR_size);
			} else if (size_nir_img == 1) {
				if (Whole_Img_NIR[NIR_size - 2] == 0xFF
						&& Whole_Img_NIR[NIR_size - 1] == 0xD9) {
					myprintf(
							"End marker (0xFF, 0xD9) detected, Image received successfully.\n");
					//				RGB_Img_RDY =1;
				} else {
					myprintf(
							"Warning: End marker (0xFF, 0xD9) not found. Data might be incomplete.\n");
				}
				myprintf("Image Data in Hexadecimal:\n");
				for (uint32_t i = 0; i < NIR_size; i++) {
					myprintf("0x%02X ", Whole_Img_NIR[i]);
				}
				myprintf("\n");

				size_nir_img = 0;
				myprintf("Ready to receive next image size.\n");

//							if (HAL_UART_Transmit(&NIR_UART, CAM_CMD, sizeof(CAM_CMD), 3000)
//									== HAL_OK) {
//								myprintf("Command transmitted to RGB CAM\r\n");
//
//								HAL_UART_Receive_DMA(&NIR_UART, Whole_Img_NIR, NIR_SIZE_LEN);
//							} else {
//								myprintf("Failed to transmit command\r\n");
//							}
//
//							HAL_UART_Receive_DMA(&NIR_UART, Whole_Img_NIR, NIR_SIZE_LEN);
			}
		}
	}
}

//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
//	CAM_CMD2[0] = 'S';
//	CAM_CMD2[1] = 'E';
//	CAM_CMD2[2] = 'N';
//	CAM_CMD2[3] = 'D';
//	if (huart == &NIR_UART) {
//		if (size_nir_img == 0) {
//			NIR_size = ((Whole_Img_NIR[0] - 48) * 1000)
//					+ ((Whole_Img_NIR[1] - 48) * 100)
//					+ ((Whole_Img_NIR[2] - 48) * 10)
//					+ ((Whole_Img_NIR[3] - 48));
//			myprintf("Image size received: %d bytes\n", NIR_size);
//			size_nir_img = 1;
//
////			HAL_UART_Transmit(&NIR_UART, CAM_CMD2, sizeof(CAM_CMD2), 1000);
////			HAL_Delay(100);
//
//			HAL_UART_Receive_DMA(&NIR_UART, Whole_Img_NIR, NIR_size);
//		} else if (size_nir_img == 1) {
//			if (Whole_Img_NIR[NIR_size - 2] == 0xFF
//					&& Whole_Img_NIR[NIR_size - 1] == 0xD9) {
//				myprintf(
//						"End marker (0xFF, 0xD9) detected, Image received successfully.\n");
////				RGB_Img_RDY =1;
//			} else {
//				myprintf(
//						"Warning: End marker (0xFF, 0xD9) not found. Data might be incomplete.\n");
//			}
//			myprintf("Image Data in Hexadecimal:\n");
//			for (uint32_t i = 0; i < NIR_size; i++) {
//				myprintf("0x%02X ", Whole_Img_NIR[i]);
//			}
//			myprintf("\n");
//
//			size_nir_img = 0;
//			myprintf("Ready to receive next image size.\n");
//
//			if (HAL_UART_Transmit(&NIR_UART, CAM_CMD, sizeof(CAM_CMD), 3000)
//					== HAL_OK) {
//				myprintf("Command transmitted to RGB CAM\r\n");
//
//				HAL_UART_Receive_DMA(&NIR_UART, Whole_Img_NIR, NIR_SIZE_LEN);
//			} else {
//				myprintf("Failed to transmit command\r\n");
//			}
//
//			HAL_UART_Receive_DMA(&NIR_UART, Whole_Img_NIR, NIR_SIZE_LEN);
//		}
//	}
//
//}
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
//
//        if (current_state == RECEIVING_SIZE) {
//            // Process the received size
//            RGB_size = ((Whole_Img_RGB[0] - '0') * 1000) +
//                       ((Whole_Img_RGB[1] - '0') * 100) +
//                       ((Whole_Img_RGB[2] - '0') * 10) +
//                       (Whole_Img_RGB[3] - '0');
//            myprintf("Image size received: %d bytes\n", RGB_size);
//            current_state = RECEIVING_IMAGE; // Set state to indicate waiting for image data
//
//            // Now receive the full image data
//            HAL_UART_Receive_DMA(&RGB_UART, Whole_Img_RGB, RGB_size);
//        } else if (current_state == RECEIVING_IMAGE) {
//            // Process the received image data
//            if (Whole_Img_RGB[RGB_size - 2] == 0xFF && Whole_Img_RGB[RGB_size - 1] == 0xD9) {
//                myprintf("End marker (0xFF, 0xD9) detected, Image received successfully.\n");
//            } else {
//                myprintf("Warning: End marker (0xFF, 0xD9) not found. Data might be incomplete.\n");
//            }
//
//            // Print image data in hexadecimal
//            myprintf("Image Data in Hexadecimal:\n");
//            for (uint32_t i = 0; i < RGB_size; i++) {
//                myprintf("0x%02X ", Whole_Img_RGB[i]);
//            }
//            myprintf("\n");
//
//            // Prepare for the next image size reception
//            current_state = IDLE; // Reset the state to IDLE, ready for next command
//            HAL_UART_Receive_DMA(&RGB_UART, Whole_Img_RGB, 4); // Get next image size
//        }
//
//}

	void INIT_VAR() {

		memset(OBC_CMD_RX, '\0', CMD_BUF_LEN);
		memset(OBC_ACK, '\0', ACK_LEN);
		memset(CAM_CMD, '\0', CAM_CMD_LEN);
		memset(CAM_ACK_RX, '\0', ACK_LEN);
		memset(Whole_Img_RGB, '\0', IMG_RCV_LEN);

	}

	void OBC_HANDSHAKE() {

		HAL_UART_Receive_IT(&OBC_UART, OBC_HANDSHAKE_RX, 5);
		while (OBC_RX_FLAG != 1) {
			myprintf("waiting for data to be received from OBC\r\n");
			HAL_Delay(2000);
		}
		OBC_RX_FLAG = 0;
		myprintf("Data received from OBC\r\n");
		HAL_Delay(1);

		for (int i = 0; i < 5; i++) {
			OBC_HANDSHAKE_TX[i] = OBC_HANDSHAKE_RX[i];
		}
		if (HAL_UART_Transmit(&OBC_UART, OBC_HANDSHAKE_TX, 5, 5000) == HAL_OK) {
			myprintf("ACK Transmitted to OBC successfully...\r\n");
			HAL_Delay(1);
		} else {
			myprintf("data transmission error.....\r\n");
			HAL_Delay(1);
		}
		memset(OBC_HANDSHAKE_RX, '\0', 5);
	}

//uint32_t Receive_IMG_Size() {
//    uint32_t size_img_RGB = 0;
//    uint32_t size_img_NIR = 0;
//    HAL_StatusTypeDef ret_transmit, ret_receive;
////    HAL_StatusTypeDef ret;
//    CAM_CMD[0] = 'S';
//    CAM_CMD[1] = 'E';
//    CAM_CMD[2] = 'N';
//    CAM_CMD[3] = 'D';
//
//
//    ret_transmit = HAL_UART_Transmit(&RGB_UART, CAM_CMD, sizeof(CAM_CMD), 1000);
//    if (ret_transmit == HAL_OK) {
//        myprintf("Command transmitted to RGB CAM\r\n");
//
////	ret_ NIR_transmit = HAL_UART_Transmit(&RGB_UART, CAM_CMD, sizeof(CAM_CMD), 1000);
////		if (ret_transmit == HAL_OK) {
////			myprintf("Command transmitted to NIR CAM\r\n");
//        __HAL_UART_FLUSH_DRREGISTER(&RGB_UART);
//        memset(size_num_RGB, '\0', sizeof(size_num_RGB));
//        ret_receive = HAL_UART_Receive(&RGB_UART, (uint8_t*)size_num_RGB,sizeof(size_num_RGB),1000);
////        size_num_RGB[4] = '\0';
//
//        if (size_num_RGB[0] != '\0') {
//        	 myprintf("Data received from Camera: %c %c %c %c %c \r\n",size_num_RGB[0], size_num_RGB[1], size_num_RGB[2],
//			 size_num_RGB[3],size_num_RGB[4]);
////        	 size_num_RGB++;
//
//		 size_img_RGB = atoi((char*)size_num_RGB);
//		 myprintf("Size received RGB: %u bytes\r\n", size_img_RGB);
//		 } else {
//			 // Only print an error message if the data is genuinely empty
//			 myprintf("Error: Data not received correctly from camera\r\n");
//		 }
//	 } else {
//		 myprintf("Error transmitting command to RGB CAM\r\n", ret_transmit);
//	 }
//
//	 return size_img_RGB;
//
//}

	uint32_t BASIC_IMAGE_CAPTURE() {
//	uint32_t rgb_img_len = 1;
//	uint32_t nir_img_len = 1;
//	uint32_t RGB_IMG_ADDRESS = ADDRESS;
//	uint32_t NIR_IMG_ADDRESS = ADDRESS + (3 * 4096);
		if (a) {
			a = 0;
			CAM_CMD[0] = 'C';
			CAM_CMD[1] = 'A';
			CAM_CMD[2] = 'M';
			CAM_CMD[3] = 'O';
			CAM_CMD[4] = 'N';
			OCP_EN();

			if (HAL_UART_Transmit(&RGB_UART, CAM_CMD, sizeof(CAM_CMD), 1000)
					== HAL_OK) {
				myprintf("Command transmitted to RGB CAM\r\n");
				if (HAL_UART_Transmit(&NIR_UART, CAM_CMD, sizeof(CAM_CMD), 1000)
						== HAL_OK) {
					myprintf("Command transmitted to NIR CAM\r\n");
//			__HAL_UART_FLUSH_DRREGISTER(&NIR_UART);
					HAL_UART_Receive_DMA(&RGB_UART, Whole_Img_NIR,
					NIR_SIZE_LEN);
//			__HAL_UART_FLUSH_DRREGISTER(&RGB_UART);
					HAL_UART_Receive_DMA(&RGB_UART, Whole_Img_RGB,
					RGB_SIZE_LEN);
				} else {
					myprintf("Failed to transmit command\r\n");
					//return 0;
				}
			}
			HAL_UART_Receive_DMA(&RGB_UART, Whole_Img_RGB, RGB_SIZE_LEN);
			HAL_UART_Receive_DMA(&RGB_UART, Whole_Img_NIR, NIR_SIZE_LEN);
			while (RGB_Img_RDY == 1 && NIR_Img_RDY == 1) {
//				Page_Write_4B(&FM, RGB_IMG_ADDRESS, Whole_Img_RGB, rgb_img_len);
//				Page_Write_4B(&FM, NIR_IMG_ADDRESS, Whole_Img_NIR, nir_img_len);
				RGB_Img_RDY = 0;
				NIR_Img_RDY = 0;
				HAL_Delay(100);
			}
			return;
		}
	}

		void CS_Enable_RGB() {
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET);
			HAL_Delay(1);
		}

		void CS_Disable_RGB() {
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_RESET);
			HAL_Delay(1);
		}
		void CS_Enable_NIR() {
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
			HAL_Delay(1);
		}

		void CS_Disable_NIR() {
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
			HAL_Delay(1);
		}

		void OCP_EN() {
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
			HAL_Delay(100);
		}

