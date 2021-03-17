/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_IAD_IF_H
#define __USBD_IAD_IF_H

#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "usbd_iad.h"
/* USER CODE BEGIN INCLUDE */
/* USER CODE END INCLUDE */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @{
  */

/** @defgroup USBD_IAD_IF
  * @brief header
  * @{
  */

/** @defgroup USBD_IAD_IF_Exported_Defines
  * @{
  */
/* USER CODE BEGIN EXPORTED_DEFINES */
/* USER CODE END EXPORTED_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_IAD_IF_Exported_Types
  * @{
  */
/* USER CODE BEGIN EXPORTED_TYPES */
/* USER CODE END EXPORTED_TYPES */

/**
  * @}
  */

/** @defgroup USBD_IAD_IF_Exported_Macros
  * @{
  */
/* USER CODE BEGIN EXPORTED_MACRO */
/* USER CODE END EXPORTED_MACRO */

/**
  * @}
  */

/** @defgroup USBD_AUDIO_IF_Exported_Variables
  * @{
  */
extern USBD_IAD_ItfTypeDef  USBD_IAD_Interface_fops_FS;

/* USER CODE BEGIN EXPORTED_VARIABLES */
/* USER CODE END EXPORTED_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_IAD_IF_Exported_FunctionsPrototype
  * @{
  */
uint8_t IAD_CDC1_Transmit_FS(uint8_t* Buf, uint16_t Len);
uint8_t IAD_CDC2_Transmit_FS(uint8_t* Buf, uint16_t Len);

/* USER CODE BEGIN EXPORTED_FUNCTIONS */

int8_t CDC_Receive_VCP1_FS(uint8_t* Buf, uint32_t *Len);
int8_t CDC_Receive_VCP2_FS(uint8_t* Buf, uint32_t *Len);

/* USER CODE END EXPORTED_FUNCTIONS */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __USBD_IAD_IF_H */

