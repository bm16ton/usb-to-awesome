/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_cdc_if.c
  * @version        : v1.0_Cube
  * @brief          : Usb device for Virtual Com Port.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "usbd_iad_if.h"

/* USER CODE BEGIN INCLUDE */

/* USER CODE END INCLUDE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief Usb device library.
  * @{
  */

/** @addtogroup USBD_CDC_IF
  * @{
  */

/** @defgroup USBD_CDC_IF_Private_TypesDefinitions USBD_CDC_IF_Private_TypesDefinitions
  * @brief Private types.
  * @{
  */

/* USER CODE BEGIN PRIVATE_TYPES */

/* USER CODE END PRIVATE_TYPES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_Defines USBD_CDC_IF_Private_Defines
  * @brief Private defines.
  * @{
  */

/* USER CODE BEGIN PRIVATE_DEFINES */
/* Define size for the receive and transmit buffer over CDC */
/* It's up to user to redefine and/or remove those define */
#define APP_RX_DATA_SIZE  2048
#define APP_TX_DATA_SIZE  2048
/* USER CODE END PRIVATE_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_Macros USBD_CDC_IF_Private_Macros
  * @brief Private macros.
  * @{
  */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_Variables USBD_CDC_IF_Private_Variables
  * @brief Private variables.
  * @{
  */
/* Create buffer for reception and transmission           */
/* It's up to user to redefine and/or remove those define */
/* Received Data over USB are stored in this buffer       */
uint8_t UserRxBufferFS1[APP_RX_DATA_SIZE];
uint8_t UserRxBufferFS2[APP_RX_DATA_SIZE];

/* Send Data over USB IAD are stored in this buffer       */
uint8_t UserTxBufferFS1[APP_TX_DATA_SIZE];
uint8_t UserTxBufferFS2[APP_TX_DATA_SIZE];

/* USER CODE BEGIN PRIVATE_VARIABLES */
uint32_t UserRxLenFS1;
uint32_t UserRxLenFS2;
/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Exported_Variables USBD_CDC_IF_Exported_Variables
  * @brief Public variables.
  * @{
  */

extern USBD_HandleTypeDef hUsbDeviceFS;

/* USER CODE BEGIN EXPORTED_VARIABLES */

/* USER CODE END EXPORTED_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_FunctionPrototypes USBD_CDC_IF_Private_FunctionPrototypes
  * @brief Private functions declaration.
  * @{
  */

static int8_t IAD_Init_FS(void);
static int8_t IAD_DeInit_FS(void);
static int8_t IAD_Control_FS(uint8_t cmd, uint8_t* pbuf, uint16_t length);
static int8_t IAD_Receive_CDC1_FS(uint8_t* pbuf, uint32_t *Len);
static int8_t IAD_Receive_CDC2_FS(uint8_t* pbuf, uint32_t *Len);
/* USER CODE BEGIN PRIVATE_FUNCTIONS_DECLARATION */

/* USER CODE END PRIVATE_FUNCTIONS_DECLARATION */

/**
  * @}
  */

USBD_IAD_ItfTypeDef USBD_IAD_Interface_fops_FS =
{
	IAD_Init_FS,
	IAD_DeInit_FS,
	IAD_Control_FS,
	IAD_Receive_CDC1_FS,
	IAD_Receive_CDC2_FS
};

/* Private functions ---------------------------------------------------------*/
/**
 * @brief  IAD_Init_FS
 *         Initializes the IAD media low layer over the USB FS IP
 * @param  None
 * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
 */
static int8_t IAD_Init_FS(void) {
	/* USER CODE BEGIN 8 */
	/* Set Application Buffers */
	USBD_IAD_CDC1_SetTxBuffer(&hUsbDeviceFS, UserTxBufferFS1, 0);
	USBD_IAD_CDC1_SetRxBuffer(&hUsbDeviceFS, UserRxBufferFS1);
	UserRxLenFS1 = 0;

	USBD_IAD_CDC2_SetTxBuffer(&hUsbDeviceFS, UserTxBufferFS2, 0);
	USBD_IAD_CDC2_SetRxBuffer(&hUsbDeviceFS, UserRxBufferFS2);
	UserRxLenFS2 = 0;
	return (USBD_OK);
	/* USER CODE END 8 */
}

/**
 * @brief  IAD_DeInit_FS
 *         DeInitializes the IAD media low layer
 * @param  None
 * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
 */
static int8_t IAD_DeInit_FS(void) {
	/* USER CODE BEGIN 9 */
	return (USBD_OK);
	/* USER CODE END 9 */
}

/**
 * @brief  IAD_Control_FS
 *         Manage the IAD class requests
 * @param  cmd: Command code
 * @param  pbuf: Buffer containing command data (request parameters)
 * @param  length: Number of data to be sent (in bytes)
 * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
 */
static int8_t IAD_Control_FS(uint8_t cmd, uint8_t* pbuf, uint16_t length) {
	/* USER CODE BEGIN 10 */
	switch (cmd) {
	case IAD_SEND_ENCAPSULATED_COMMAND:

		break;

	case IAD_GET_ENCAPSULATED_RESPONSE:

		break;

	case IAD_SET_COMM_FEATURE:

		break;

	case IAD_GET_COMM_FEATURE:

		break;

	case IAD_CLEAR_COMM_FEATURE:

		break;

		/*******************************************************************************/
		/* Line Coding Structure                                                       */
		/*-----------------------------------------------------------------------------*/
		/* Offset | Field       | Size | Value  | Description                          */
		/* 0      | dwDTERate   |   4  | Number |Data terminal rate, in bits per second*/
		/* 4      | bCharFormat |   1  | Number | Stop bits                            */
		/*                                        0 - 1 Stop bit                       */
		/*                                        1 - 1.5 Stop bits                    */
		/*                                        2 - 2 Stop bits                      */
		/* 5      | bParityType |  1   | Number | Parity                               */
		/*                                        0 - None                             */
		/*                                        1 - Odd                              */
		/*                                        2 - Even                             */
		/*                                        3 - Mark                             */
		/*                                        4 - Space                            */
		/* 6      | bDataBits  |   1   | Number Data bits (5, 6, 7, 8 or 16).          */
		/*******************************************************************************/
	case IAD_SET_LINE_CODING:

		break;

	case IAD_GET_LINE_CODING:
		pbuf[0] = (uint8_t) (9600 >> 0);
		pbuf[1] = (uint8_t) (9600 >> 8);
		pbuf[2] = (uint8_t) (9600 >> 16);
		pbuf[3] = (uint8_t) (9600 >> 24);
		pbuf[4] = 0x00;
		pbuf[5] = 0x00;
		pbuf[6] = 0x08;
		break;

    case IAD_SET_CONTROL_LINE_STATE:


    break;

  default:
    break;
  }

  return (USBD_OK);
  /* USER CODE END 5 */
}

/**
 * @brief  IAD_Receive_FS
 *         Data received over USB OUT endpoint are sent over IAD interface
 *         through this function.
 *
 *         @note
 *         This function will block any OUT packet reception on USB endpoint
 *         untill exiting this function. If you exit this function before transfer
 *         is complete on IAD interface (ie. using DMA controller) it will result
 *         in receiving more data while previous ones are still not sent.
 *
 * @param  Buf: Buffer of data to be received
 * @param  Len: Number of data received (in bytes)
 * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
 */
static int8_t IAD_Receive_CDC1_FS(uint8_t* Buf, uint32_t *Len) {
	/* USER CODE BEGIN 11 */
	USBD_IAD_CDC1_SetRxBuffer(&hUsbDeviceFS, &Buf[*Len]);
	UserRxLenFS1 += *Len;
	USBD_IAD_CDC1_ReceivePacket(&hUsbDeviceFS);
	return (USBD_OK);
	/* USER CODE END 11 */
}

/**
 * @brief  IAD_Receive_FS
 *         Data received over USB OUT endpoint are sent over IAD interface
 *         through this function.
 *
 *         @note
 *         This function will block any OUT packet reception on USB endpoint
 *         untill exiting this function. If you exit this function before transfer
 *         is complete on IAD interface (ie. using DMA controller) it will result
 *         in receiving more data while previous ones are still not sent.
 *
 * @param  Buf: Buffer of data to be received
 * @param  Len: Number of data received (in bytes)
 * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
 */
static int8_t IAD_Receive_CDC2_FS(uint8_t* Buf, uint32_t *Len) {
	/* USER CODE BEGIN 11 */
	USBD_IAD_CDC2_SetRxBuffer(&hUsbDeviceFS, &Buf[*Len]);
	UserRxLenFS2 += *Len;
	USBD_IAD_CDC2_ReceivePacket(&hUsbDeviceFS);
	return (USBD_OK);
	/* USER CODE END 11 */
}

/**
 * @brief  IAD_Transmit_FS
 *         Data send over USB IN endpoint are sent over IAD interface
 *         through this function.
 *         @note
 *
 *
 * @param  Buf: Buffer of data to be send
 * @param  Len: Number of data to be send (in bytes)
 * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL or USBD_BUSY
 */
uint8_t IAD_CDC1_Transmit_FS(uint8_t* Buf, uint16_t Len) {
	uint8_t result = USBD_OK;
	/* USER CODE BEGIN 12 */
	USBD_IAD_HandleTypeDef *hiad =
			(USBD_IAD_HandleTypeDef*) hUsbDeviceFS.pClassData;
	if (hiad->cdc1.TxState != 0) {
		return USBD_BUSY;
	}
	USBD_IAD_CDC1_SetTxBuffer(&hUsbDeviceFS, Buf, Len);
	result = USBD_IAD_CDC1_TransmitPacket(&hUsbDeviceFS);
	/* USER CODE END 12 */
	return result;
}

/**
 * @brief  IAD_Transmit_FS
 *         Data send over USB IN endpoint are sent over IAD interface
 *         through this function.
 *         @note
 *
 *
 * @param  Buf: Buffer of data to be send
 * @param  Len: Number of data to be send (in bytes)
 * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL or USBD_BUSY
 */
uint8_t IAD_CDC2_Transmit_FS(uint8_t* Buf, uint16_t Len) {
	uint8_t result = USBD_OK;
	/* USER CODE BEGIN 12 */
	USBD_IAD_HandleTypeDef *hiad =
			(USBD_IAD_HandleTypeDef*) hUsbDeviceFS.pClassData;
	if (hiad->cdc2.TxState != 0) {
		return USBD_BUSY;
	}
	USBD_IAD_CDC2_SetTxBuffer(&hUsbDeviceFS, Buf, Len);
	result = USBD_IAD_CDC2_TransmitPacket(&hUsbDeviceFS);
	/* USER CODE END 12 */
	return result;
}

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */
int8_t CDC_Receive_VCP1_FS(uint8_t* Buf, uint32_t *Len) {
	if (*Len > UserRxLenFS1)
		*Len = UserRxLenFS1;
	if (*Len > 0) {
		memcpy(Buf, UserRxBufferFS1, *Len);

		uint32_t bytesRemaining = UserRxLenFS1 - *Len;
		if (bytesRemaining != 0)
			memcpy(UserRxBufferFS1, &UserRxBufferFS1[*Len], bytesRemaining);

		UserRxLenFS1 = bytesRemaining;
		USBD_IAD_CDC1_SetRxBuffer(&hUsbDeviceFS, &UserRxBufferFS1[UserRxLenFS1]);
		USBD_IAD_CDC1_ReceivePacket(&hUsbDeviceFS);
	}
	return USBD_OK;
}

int8_t CDC_Receive_VCP2_FS(uint8_t* Buf, uint32_t *Len) {
	if (*Len > UserRxLenFS2)
		*Len = UserRxLenFS2;
	if (*Len > 0) {
		memcpy(Buf, UserRxBufferFS2, *Len);

		uint32_t bytesRemaining = UserRxLenFS2 - *Len;
		if (bytesRemaining != 0)
			memcpy(UserRxBufferFS2, &UserRxBufferFS2[*Len], bytesRemaining);

		UserRxLenFS2 = bytesRemaining;
		USBD_IAD_CDC2_SetRxBuffer(&hUsbDeviceFS, &UserRxBufferFS2[UserRxLenFS2]);
		USBD_IAD_CDC2_ReceivePacket(&hUsbDeviceFS);
	}
	return USBD_OK;
}
