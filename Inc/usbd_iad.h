
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_IAD_H
#define __USB_IAD_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include  "usbd_ioreq.h"

/** @addtogroup STM32_USB_DEVICE_LIBRARY
 * @{
 */

/** @defgroup usbd_cdc
 * @brief This file is the Header file for usbd_cdc.c
 * @{
 */

/** @defgroup usbd_cdc_Exported_Defines
 * @{
 */
#define IAD_CDC1_IN_EP                                   0x81  /* CDC1 EP for data IN */
#define IAD_CDC1_OUT_EP                                  0x01  /* CDC1 EP for data OUT */
#define IAD_CDC1_CMD_EP                                  0x82  /* CDC1 EP for CDC commands */

#define IAD_CDC2_IN_EP                                   0x83  /* CDC2 EP for data IN */
#define IAD_CDC2_OUT_EP                                  0x03  /* CDC2 EP for data OUT */
#define IAD_CDC2_CMD_EP                                  0x84  /* CDC2 EP for CDC commands */

/* IAD Endpoints parameters: you can fine tune these values depending on the needed baudrates and performance. */
#define IAD_DATA_HS_MAX_PACKET_SIZE                 512  /* Endpoint IN & OUT Packet size */
#define IAD_DATA_FS_MAX_PACKET_SIZE                 64  /* Endpoint IN & OUT Packet size */
#define IAD_CDC_CMD_PACKET_SIZE                     8  /* Control Endpoint Packet size */

#define USB_IAD_CONFIG_DESC_SIZ                     9 + 66 + 66

#define IAD_DATA_HS_IN_PACKET_SIZE                  IAD_DATA_HS_MAX_PACKET_SIZE
#define IAD_DATA_HS_OUT_PACKET_SIZE                 IAD_DATA_HS_MAX_PACKET_SIZE

#define IAD_DATA_FS_IN_PACKET_SIZE                  IAD_DATA_FS_MAX_PACKET_SIZE
#define IAD_DATA_FS_OUT_PACKET_SIZE                 IAD_DATA_FS_MAX_PACKET_SIZE

/*---------------------------------------------------------------------*/
/*  IAD definitions                                                    */
/*---------------------------------------------------------------------*/
#define IAD_SEND_ENCAPSULATED_COMMAND               0x00
#define IAD_GET_ENCAPSULATED_RESPONSE               0x01
#define IAD_SET_COMM_FEATURE                        0x02
#define IAD_GET_COMM_FEATURE                        0x03
#define IAD_CLEAR_COMM_FEATURE                      0x04
#define IAD_SET_LINE_CODING                         0x20
#define IAD_GET_LINE_CODING                         0x21
#define IAD_SET_CONTROL_LINE_STATE                  0x22
#define IAD_SEND_BREAK                              0x23

/**
 * @}
 */

/** @defgroup USBD_CORE_Exported_TypesDefinitions
 * @{
 */

/**
 * @}
 */
typedef struct {
	uint32_t bitrate;
	uint8_t format;
	uint8_t paritytype;
	uint8_t datatype;
} USBD_IAD_LineCodingTypeDef;

typedef struct _USBD_IAD_Itf {
	int8_t (*Init)(void);
	int8_t (*DeInit)(void);
	int8_t (*Control)(uint8_t, uint8_t *, uint16_t);
	int8_t (*Receive_CDC1)(uint8_t *, uint32_t *);
	int8_t (*Receive_CDC2)(uint8_t *, uint32_t *);

} USBD_IAD_ItfTypeDef;

typedef struct {
	uint8_t *RxBuffer;
	uint8_t *TxBuffer;
	uint32_t RxLength;
	uint32_t TxLength;

	__IO uint32_t TxState;
	__IO uint32_t RxState;
} USBD_IAD_CDC_HandleTypeDef;

typedef struct {
	uint32_t data[IAD_DATA_FS_MAX_PACKET_SIZE / 4]; /* Force 32bits alignment */
	uint8_t CmdOpCode;
	uint8_t CmdLength;
	USBD_IAD_CDC_HandleTypeDef cdc1;
	USBD_IAD_CDC_HandleTypeDef cdc2;
} USBD_IAD_HandleTypeDef;

/** @defgroup USBD_CORE_Exported_Macros
 * @{
 */

/**
 * @}
 */

/** @defgroup USBD_CORE_Exported_Variables
 * @{
 */

extern USBD_ClassTypeDef USBD_IAD;
#define USBD_IAD_CLASS    &USBD_IAD
/**
 * @}
 */

/** @defgroup USB_CORE_Exported_Functions
 * @{
 */
uint8_t USBD_IAD_RegisterInterface(USBD_HandleTypeDef *pdev,
		USBD_IAD_ItfTypeDef *fops);

uint8_t USBD_IAD_CDC1_SetTxBuffer(USBD_HandleTypeDef *pdev, uint8_t *pbuff,
		uint16_t length);
uint8_t USBD_IAD_CDC2_SetTxBuffer(USBD_HandleTypeDef *pdev, uint8_t *pbuff,
		uint16_t length);

uint8_t USBD_IAD_CDC1_SetRxBuffer(USBD_HandleTypeDef *pdev, uint8_t *pbuff);
uint8_t USBD_IAD_CDC2_SetRxBuffer(USBD_HandleTypeDef *pdev, uint8_t *pbuff);

uint8_t USBD_IAD_CDC1_ReceivePacket(USBD_HandleTypeDef *pdev);
uint8_t USBD_IAD_CDC2_ReceivePacket(USBD_HandleTypeDef *pdev);

uint8_t USBD_IAD_CDC1_TransmitPacket(USBD_HandleTypeDef *pdev);
uint8_t USBD_IAD_CDC2_TransmitPacket(USBD_HandleTypeDef *pdev);

uint8_t USBD_IAD_CDC1_ReceivePacket(USBD_HandleTypeDef *pdev);
uint8_t USBD_IAD_CDC2_ReceivePacket(USBD_HandleTypeDef *pdev);
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif  /* __USB_IAD_H */
