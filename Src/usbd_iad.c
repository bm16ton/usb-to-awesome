/* Includes ------------------------------------------------------------------*/
#include "usbd_iad.h"
#include "usbd_desc.h"
#include "usbd_ctlreq.h"

/** @addtogroup STM32_USB_DEVICE_LIBRARY
 * @{
 */

/** @defgroup USBD_IAD
 * @brief usbd core module
 * @{
 */

/** @defgroup USBD_IAD_Private_TypesDefinitions
 * @{
 */
/**
 * @}
 */

/** @defgroup USBD_IAD_Private_Defines
 * @{
 */
/**
 * @}
 */

/** @defgroup USBD_IAD_Private_Macros
 * @{
 */

/**
 * @}
 */

/** @defgroup USBD_IAD_Private_FunctionPrototypes
 * @{
 */

static uint8_t USBD_IAD_Init(USBD_HandleTypeDef *pdev, uint8_t cfgidx);

static uint8_t USBD_IAD_DeInit(USBD_HandleTypeDef *pdev, uint8_t cfgidx);

static uint8_t USBD_IAD_Setup(USBD_HandleTypeDef *pdev,
		USBD_SetupReqTypedef *req);

static uint8_t USBD_IAD_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum);

static uint8_t USBD_IAD_DataOut(USBD_HandleTypeDef *pdev, uint8_t epnum);

static uint8_t USBD_IAD_EP0_RxReady(USBD_HandleTypeDef *pdev);

static uint8_t *USBD_IAD_GetFSCfgDesc(uint16_t *length);

static uint8_t *USBD_IAD_GetHSCfgDesc(uint16_t *length);

static uint8_t *USBD_IAD_GetOtherSpeedCfgDesc(uint16_t *length);

static uint8_t *USBD_IAD_GetOtherSpeedCfgDesc(uint16_t *length);

uint8_t *USBD_IAD_GetDeviceQualifierDescriptor(uint16_t *length);

/* USB Standard Device Descriptor */
__ALIGN_BEGIN static uint8_t USBD_IAD_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC] __ALIGN_END
		= {
		USB_LEN_DEV_QUALIFIER_DESC,
		USB_DESC_TYPE_DEVICE_QUALIFIER, 0x00, 0x02, 0x00, 0x00, 0x00, 0x40,
				0x01, 0x00, };

/**
 * @}
 */

/** @defgroup USBD_IAD_Private_Variables
 * @{
 */

/* IAD interface class callbacks structure */
USBD_ClassTypeDef USBD_IAD = {
	USBD_IAD_Init,
	USBD_IAD_DeInit,
	USBD_IAD_Setup,
	NULL, /* EP0_TxSent, */
	USBD_IAD_EP0_RxReady,
	USBD_IAD_DataIn,
	USBD_IAD_DataOut,
	NULL,
	NULL,
	NULL,
	USBD_IAD_GetHSCfgDesc,
	USBD_IAD_GetFSCfgDesc,
	USBD_IAD_GetOtherSpeedCfgDesc,
	USBD_IAD_GetDeviceQualifierDescriptor,
};

__ALIGN_BEGIN uint8_t USBD_IAD_CfgHSDesc[USB_IAD_CONFIG_DESC_SIZ] __ALIGN_END =
{
	//Configuation Descriptor
	0x09,								// 00 bLength: Configuation Descriptor size
	USB_DESC_TYPE_CONFIGURATION,		// 01 bDescriptorType: Configuration
	USB_IAD_CONFIG_DESC_SIZ,			// 02 wTotalLength:no of returned bytes
	0x00,								// 03
	0x04,								// 04 bNumInterfaces: 4 interface
	0x01,								// 05 bConfigurationValue: Configuration value
	0x00,								// 06 iConfiguration: Index of string descriptor describing the configuration
	0xC0,								// 07 bmAttributes: self powered
	0x32,								// 08 MaxPower 100 mA

	// IAD 1 -> CDC1
	0x08,// 09 bLength: Interface Descriptor size
	0x0B,								// 10 bDescriptorType: IAD
	0x00,								// 11 bFirstInterface
	0x02,								// 12 bInterfaceCount
	0x02,								// 13 bFunctionClass: CDC
	0x02,								// 14 bFunctionSubClass
	0x01,								// 15 bFunctionProtocol
	0x02,								// 16 iFunction

	//Interface Descriptor
	0x09,								// 17 bLength: Interface Descriptor size
	USB_DESC_TYPE_INTERFACE,			// 18 bDescriptorType: Interface
	// Interface descriptor type
	0x00,								// 19 bInterfaceNumber: Number of Interface
	0x00,								// 20 bAlternateSetting: Alternate setting
	0x01,								// 21 bNumEndpoints: One endpoints used
	0x02,								// 22 bInterfaceClass: Communication Interface Class
	0x02,								// 23 bInterfaceSubClass: Abstract Control Model
	0x01,								// 24 bInterfaceProtocol: Common AT commands
	0x00,								// 25 iInterface:

	//Header Functional Descriptor
	0x05,								// 26 bLength: Endpoint Descriptor size
	0x24,								// 27 bDescriptorType: CS_INTERFACE
	0x00,								// 28 bDescriptorSubtype: Header Func Desc
	0x10,								// 29 bcdIAD: spec release number
	0x01,								// 30

	//Call Managment Functional Descriptor
	0x05,								// 31 bFunctionLength
	0x24,								// 32 bDescriptorType: CS_INTERFACE
	0x01,								// 33 bDescriptorSubtype: Call Management Func Desc
	0x00,								// 34 bmCapabilities: D0+D1
	0x01,								// 35 bDataInterface: 1

	//ACM CDC1 Functional Descriptor
	0x04,								// 36 bFunctionLength
	0x24,								// 37 bDescriptorType: CS_INTERFACE
	0x02,								// 38 bDescriptorSubtype: Abstract Control Management desc
	0x02,								// 39 bmCapabilities

	//Union Functional Descriptor
	0x05,								// 40 bFunctionLength
	0x24,								// 41 bDescriptorType: CS_INTERFACE
	0x06,								// 42 bDescriptorSubtype: Union func desc
	0x00,								// 43 bMasterInterface: Communication class interface
	0x01,								// 44 bSlaveInterface0: Data Class Interface

	//Endpoint CDC1 CMD Descriptor
	0x07,								// 45 bLength: Endpoint Descriptor size
	USB_DESC_TYPE_ENDPOINT,				// 46 bDescriptorType: Endpoint
	IAD_CDC1_CMD_EP,					// 47 bEndpointAddress: (IN2)
	0x03,								// 48 bmAttributes: Interrupt
	LOBYTE(IAD_CDC_CMD_PACKET_SIZE),	// 49 wMaxPacketSize:
	HIBYTE(IAD_CDC_CMD_PACKET_SIZE),	// 50
	0xFF,								// 51 bInterval:
	//---------------------------------------------------------------------------

	//Data class interface descriptor
	0x09,								// 52 bLength: Endpoint Descriptor size
	USB_DESC_TYPE_INTERFACE,			// 53 bDescriptorType:
	0x01,								// 54 bInterfaceNumber: Number of Interface
	0x00,								// 55 bAlternateSetting: Alternate setting
	0x02,								// 56 bNumEndpoints: Two endpoints used
	0x0A,								// 57 bInterfaceClass: CDC
	0x00,								// 58 bInterfaceSubClass:
	0x00,								// 59 bInterfaceProtocol:
	0x00,								// 60 iInterface:

	//Endpoint CDC1 OUT Descriptor
	0x07,								// 61 bLength: Endpoint Descriptor size
	USB_DESC_TYPE_ENDPOINT,				// 62 bDescriptorType: Endpoint
	IAD_CDC1_OUT_EP,					// 63 bEndpointAddress: (OUT1)
	0x02,								// 64 bmAttributes: Bulk
	LOBYTE(IAD_DATA_HS_MAX_PACKET_SIZE),// 65 wMaxPacketSize:
	HIBYTE(IAD_DATA_HS_MAX_PACKET_SIZE),// 66
	0x00,								// 67 bInterval: ignore for Bulk transfer

	//Endpoint CDC1 IN Descriptor
	0x07,								// 68 bLength: Endpoint Descriptor size
	USB_DESC_TYPE_ENDPOINT,				// 69 bDescriptorType: Endpoint
	IAD_CDC1_IN_EP,						// 70 bEndpointAddress: (IN1)
	0x02,								// 71 bmAttributes: Bulk
	LOBYTE(IAD_DATA_HS_MAX_PACKET_SIZE),// 72 wMaxPacketSize:
	HIBYTE(IAD_DATA_HS_MAX_PACKET_SIZE),// 73
	0x00,								// 74 bInterval

	// IAD 2 -> CDC2
	0x08,// 75 bLength: Interface Descriptor size
	0x0B,								// 76 bDescriptorType: IAD
	0x02,								// 77 bFirstInterface
	0x02,								// 78 bInterfaceCount
	0x02,								// 79 bFunctionClass: CDC
	0x02,								// 80 bFunctionSubClass
	0x01,								// 81 bFunctionProtocol
	0x02,								// 82 iFunction

	//Interface Descriptor
	0x09,// 83 bLength: Interface Descriptor size
	USB_DESC_TYPE_INTERFACE,			// 84 bDescriptorType: Interface
	// Interface descriptor type
	0x02,								// 85 bInterfaceNumber: Number of Interface
	0x00,								// 86 bAlternateSetting: Alternate setting
	0x01,								// 87 bNumEndpoints: One endpoints used
	0x02,								// 88 bInterfaceClass: Communication Interface Class
	0x02,								// 89 bInterfaceSubClass: Abstract Control Model
	0x01,								// 90 bInterfaceProtocol: Common AT commands
	0x00,								// 91 iInterface:

	//Header Functional Descriptor
	0x05,								// 92 bLength: Endpoint Descriptor size
	0x24,								// 93 bDescriptorType: CS_INTERFACE
	0x00,								// 94 bDescriptorSubtype: Header Func Desc
	0x10,								// 95 bcdCDC: spec release number
	0x01,								// 96

	//Call Managment Functional Descriptor
	0x05,								// 97 bFunctionLength
	0x24,								// 98 bDescriptorType: CS_INTERFACE
	0x01,								// 99 bDescriptorSubtype: Call Management Func Desc
	0x00,								// 100 bmCapabilities: D0+D1
	0x03,								// 101 !! bDataInterface:

	//ACM CDC2 Functional Descriptor
	0x04,								// 102 bFunctionLength
	0x24,								// 103 bDescriptorType: CS_INTERFACE
	0x02,								// 104 bDescriptorSubtype: Abstract Control Management desc
	0x02,								// 105 bmCapabilities

	//Union Functional Descriptor
	0x05,								// 106 bFunctionLength
	0x24,								// 107 bDescriptorType: CS_INTERFACE
	0x06,								// 108 bDescriptorSubtype: Union func desc
	0x02,								// 109 !! bMasterInterface: Communication class interface
	0x03,								// 110 !! bSlaveInterface0: Data Class Interface

	//Endpoint CDC2 CMD Descriptor
	0x07,								// 111 bLength: Endpoint Descriptor size
	USB_DESC_TYPE_ENDPOINT,				// 112 bDescriptorType: Endpoint
	IAD_CDC2_CMD_EP,					// 113 bEndpointAddress: (IN4)
	0x03,								// 114 bmAttributes: Interrupt
	LOBYTE(IAD_CDC_CMD_PACKET_SIZE),	// 115 wMaxPacketSize:
	HIBYTE(IAD_CDC_CMD_PACKET_SIZE),	// 116
	0xFF,								// 117 bInterval:
	//---------------------------------------------------------------------------

	//Data class interface descriptor
	0x09,								// 118 bLength: Endpoint Descriptor size
	USB_DESC_TYPE_INTERFACE,			// 119 bDescriptorType:
	0x03,								// 120 bInterfaceNumber: Number of Interface
	0x00,								// 121 bAlternateSetting: Alternate setting
	0x02,								// 122 bNumEndpoints: Two endpoints used
	0x0A,								// 123 bInterfaceClass: CDC
	0x00,								// 124 bInterfaceSubClass:
	0x00,								// 125 bInterfaceProtocol:
	0x00,								// 126 iInterface:

	//Endpoint CDC2 OUT Descriptor
	0x07,								// 127 bLength: Endpoint Descriptor size
	USB_DESC_TYPE_ENDPOINT,				// 128 bDescriptorType: Endpoint
	IAD_CDC2_OUT_EP,					// 129 bEndpointAddress: (OUT3)
	0x02,								// 130 bmAttributes: Bulk
	LOBYTE(IAD_DATA_HS_MAX_PACKET_SIZE),// 131 wMaxPacketSize:
	HIBYTE(IAD_DATA_HS_MAX_PACKET_SIZE),// 132
	0x00,								// 133 bInterval: ignore for Bulk transfer

	//Endpoint CDC2 IN Descriptor
	0x07,								// 134 bLength: Endpoint Descriptor size
	USB_DESC_TYPE_ENDPOINT,				// 135 bDescriptorType: Endpoint
	IAD_CDC2_IN_EP,						// 136 bEndpointAddress: (IN3)
	0x02,								// 137 bmAttributes: Bulk
	LOBYTE(IAD_DATA_HS_MAX_PACKET_SIZE),// 138 wMaxPacketSize:
	HIBYTE(IAD_DATA_HS_MAX_PACKET_SIZE),// 139
	0x00,								// 140 bInterval
};

/**
 * @}
 */

/** @defgroup USBD_IAD_Private_Functions
 * @{
 */

/**
 * @brief  USBD_IAD_Init
 *         Initialize the IAD interface
 * @param  pdev: device instance
 * @param  cfgidx: Configuration index
 * @retval status
 */
static uint8_t USBD_IAD_Init(USBD_HandleTypeDef *pdev, uint8_t cfgidx) {
	uint8_t ret = 0;
	USBD_IAD_HandleTypeDef *hiad;

	uint16_t size = pdev->dev_speed == USBD_SPEED_HIGH ?
		IAD_DATA_HS_IN_PACKET_SIZE : IAD_DATA_FS_IN_PACKET_SIZE;

	/* CDC1 */
	/* Open EP IN */
	USBD_LL_OpenEP(pdev,
		IAD_CDC1_IN_EP,
		USBD_EP_TYPE_BULK,
		size);
	/* Open EP OUT */
	USBD_LL_OpenEP(pdev,
		IAD_CDC1_OUT_EP,
		USBD_EP_TYPE_BULK,
		size);
	/* Open Command IN EP */
	USBD_LL_OpenEP(pdev,
		IAD_CDC1_CMD_EP,
		USBD_EP_TYPE_INTR,
		IAD_CDC_CMD_PACKET_SIZE);

	/* CDC2 */
	/* Open EP IN */
	USBD_LL_OpenEP(pdev,
		IAD_CDC2_IN_EP,
		USBD_EP_TYPE_BULK,
		size);
	/* Open EP OUT */
	USBD_LL_OpenEP(pdev,
		IAD_CDC2_OUT_EP,
		USBD_EP_TYPE_BULK,
		size);
	/* Open CDC2 Command IN EP */
	USBD_LL_OpenEP(pdev,
		IAD_CDC2_CMD_EP,
		USBD_EP_TYPE_INTR,
		IAD_CDC_CMD_PACKET_SIZE);

	pdev->pClassData = USBD_malloc(sizeof(USBD_IAD_HandleTypeDef));

	if (pdev->pClassData == NULL) {
		ret = 1;
	} else {
		hiad = (USBD_IAD_HandleTypeDef*) pdev->pClassData;

		/* Init  physical Interface components */
		((USBD_IAD_ItfTypeDef *) pdev->pUserData)->Init();

		/* Init Xfer states */
		hiad->cdc1.TxState = 0;
		hiad->cdc1.RxState = 0;
		hiad->cdc2.TxState = 0;
		hiad->cdc2.RxState = 0;

		/* Prepare Out endpoint to receive next packet */
		USBD_LL_PrepareReceive(pdev,
			IAD_CDC1_OUT_EP, hiad->cdc1.RxBuffer,
			size);
		USBD_LL_PrepareReceive(pdev,
			IAD_CDC2_OUT_EP, hiad->cdc2.RxBuffer,
			size);
	}
	return ret;
}

/**
 * @brief  USBD_IAD_Init
 *         DeInitialize the IAD layer
 * @param  pdev: device instance
 * @param  cfgidx: Configuration index
 * @retval status
 */
static uint8_t USBD_IAD_DeInit(USBD_HandleTypeDef *pdev, uint8_t cfgidx) {
	uint8_t ret = 0;

	/* CDC1 */
	/* Open EP IN */
	USBD_LL_CloseEP(pdev,
		IAD_CDC1_IN_EP);
	/* Open EP OUT */
	USBD_LL_CloseEP(pdev,
		IAD_CDC1_OUT_EP);
	/* Open Command IN EP */
	USBD_LL_CloseEP(pdev,
		IAD_CDC1_CMD_EP);

	/* CDC2 */
	/* Open EP IN */
	USBD_LL_CloseEP(pdev,
		IAD_CDC2_IN_EP);
	/* Open EP OUT */
	USBD_LL_CloseEP(pdev,
		IAD_CDC2_OUT_EP);
	/* Open Command IN EP */
	USBD_LL_CloseEP(pdev,
		IAD_CDC2_CMD_EP);

	/* DeInit  physical Interface components */
	if (pdev->pClassData != NULL) {
		((USBD_IAD_ItfTypeDef *) pdev->pUserData)->DeInit();
		USBD_free(pdev->pClassData);
		pdev->pClassData = NULL;
	}

	return ret;
}

/**
 * @brief  USBD_IAD_Setup
 *         Handle the IAD specific requests
 * @param  pdev: instance
 * @param  req: usb requests
 * @retval status
 */
static uint8_t USBD_IAD_Setup(USBD_HandleTypeDef *pdev,
		USBD_SetupReqTypedef *req) {

	USBD_IAD_HandleTypeDef *hiad = (USBD_IAD_HandleTypeDef*) pdev->pClassData;
	static uint8_t ifalt = 0;

	switch (req->bmRequest & USB_REQ_TYPE_MASK) {
	case USB_REQ_TYPE_CLASS:
		if (req->wLength) {
			if (req->bmRequest & 0x80) {
				((USBD_IAD_ItfTypeDef *) pdev->pUserData)->Control(
						req->bRequest, (uint8_t *) hiad->data, req->wLength);
				USBD_CtlSendData(pdev, (uint8_t *) hiad->data, req->wLength);
			} else {
				hiad->CmdOpCode = req->bRequest;
				hiad->CmdLength = req->wLength;

				USBD_CtlPrepareRx(pdev, (uint8_t *) hiad->data, req->wLength);
			}

		} else {
			((USBD_IAD_ItfTypeDef *) pdev->pUserData)->Control(req->bRequest,
					(uint8_t*) req, 0);
		}
		break;

	case USB_REQ_TYPE_STANDARD:
		switch (req->bRequest) {
		case USB_REQ_GET_INTERFACE:
			USBD_CtlSendData(pdev, &ifalt, 1);
			break;

		case USB_REQ_SET_INTERFACE:
			break;
		}
		break;

	default:
		break;
	}
	return USBD_OK;
}

/**
 * @brief  USBD_IAD_DataIn
 *         Data sent on non-control IN endpoint
 * @param  pdev: device instance
 * @param  epnum: endpoint number
 * @retval status
 */
static uint8_t USBD_IAD_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum) {
	USBD_IAD_HandleTypeDef *hiad = (USBD_IAD_HandleTypeDef*) pdev->pClassData;

	if (pdev->pClassData != NULL) {
		if (epnum == IAD_CDC1_IN_EP || epnum == IAD_CDC1_OUT_EP)
			hiad->cdc1.TxState = 0;
		else
			hiad->cdc2.TxState = 0;
		return USBD_OK;
	} else {
		return USBD_FAIL;
	}
}

/**
 * @brief  USBD_IAD_DataOut
 *         Data received on non-control Out endpoint
 * @param  pdev: device instance
 * @param  epnum: endpoint number
 * @retval status
 */
static uint8_t USBD_IAD_DataOut(USBD_HandleTypeDef *pdev, uint8_t epnum) {

	/* USB data will be immediately processed, this allow next USB traffic being
	 NAKed till the end of the application Xfer */
	if (pdev->pClassData != NULL) {
		USBD_IAD_HandleTypeDef *hiad = (USBD_IAD_HandleTypeDef*) pdev->pClassData;

		if (epnum == IAD_CDC1_IN_EP || epnum == IAD_CDC1_OUT_EP) {
			/* Get the received data length */
			hiad->cdc1.RxLength = USBD_LL_GetRxDataSize(pdev, epnum);
			/* USB data will be immediately processed, this allow next USB traffic being
			   NAKed till the end of the application Xfer */
			((USBD_IAD_ItfTypeDef *) pdev->pUserData)->Receive_CDC1(
					hiad->cdc1.RxBuffer, &hiad->cdc1.RxLength);

		} else {
			/* Get the received data length */
			hiad->cdc2.RxLength = USBD_LL_GetRxDataSize(pdev, epnum);
			/* USB data will be immediately processed, this allow next USB traffic being
			   NAKed till the end of the application Xfer */
			((USBD_IAD_ItfTypeDef *) pdev->pUserData)->Receive_CDC2(
					hiad->cdc2.RxBuffer, &hiad->cdc2.RxLength);
		}
		return USBD_OK;
	} else {
		return USBD_FAIL;
	}
}

/**
 * @brief  USBD_IAD_DataOut
 *         Data received on non-control Out endpoint
 * @param  pdev: device instance
 * @param  epnum: endpoint number
 * @retval status
 */
static uint8_t USBD_IAD_EP0_RxReady(USBD_HandleTypeDef *pdev) {
	USBD_IAD_HandleTypeDef *hiad = (USBD_IAD_HandleTypeDef*) pdev->pClassData;

	if ((pdev->pUserData != NULL) && (hiad->CmdOpCode != 0xFF)) {
		((USBD_IAD_ItfTypeDef *) pdev->pUserData)->Control(hiad->CmdOpCode,
				(uint8_t *) hiad->data, hiad->CmdLength);
		hiad->CmdOpCode = 0xFF;

	}
	return USBD_OK;
}

/**
 * @brief  USBD_IAD_GetFSCfgDesc
 *         Return configuration descriptor
 * @param  speed : current device speed
 * @param  length : pointer data length
 * @retval pointer to descriptor buffer
 */
static uint8_t *USBD_IAD_GetFSCfgDesc(uint16_t *length) {
	*length = sizeof(USBD_IAD_CfgHSDesc);
	return USBD_IAD_CfgHSDesc;
}

/**
 * @brief  USBD_IAD_GetHSCfgDesc
 *         Return configuration descriptor
 * @param  speed : current device speed
 * @param  length : pointer data length
 * @retval pointer to descriptor buffer
 */
static uint8_t *USBD_IAD_GetHSCfgDesc(uint16_t *length) {
	*length = sizeof(USBD_IAD_CfgHSDesc);
	return USBD_IAD_CfgHSDesc;
}

/**
 * @brief  USBD_IAD_GetCfgDesc
 *         Return configuration descriptor
 * @param  speed : current device speed
 * @param  length : pointer data length
 * @retval pointer to descriptor buffer
 */
static uint8_t *USBD_IAD_GetOtherSpeedCfgDesc(uint16_t *length) {
	*length = sizeof(USBD_IAD_CfgHSDesc);
	return USBD_IAD_CfgHSDesc;
}

/**
 * @brief  DeviceQualifierDescriptor
 *         return Device Qualifier descriptor
 * @param  length : pointer data length
 * @retval pointer to descriptor buffer
 */
uint8_t *USBD_IAD_GetDeviceQualifierDescriptor(uint16_t *length) {
	*length = sizeof(USBD_IAD_DeviceQualifierDesc);
	return USBD_IAD_DeviceQualifierDesc;
}

/**
 * @brief  USBD_IAD_RegisterInterface
 * @param  pdev: device instance
 * @param  fops: CD  Interface callback
 * @retval status
 */
uint8_t USBD_IAD_RegisterInterface(USBD_HandleTypeDef *pdev,
		USBD_IAD_ItfTypeDef *fops) {
	uint8_t ret = USBD_FAIL;

	if (fops != NULL) {
		pdev->pUserData = fops;
		ret = USBD_OK;
	}

	return ret;
}

/**
 * @brief  USBD_IAD_SetTxBuffer
 * @param  pdev: device instance
 * @param  pbuff: Tx Buffer
 * @retval status
 */
uint8_t USBD_IAD_CDC1_SetTxBuffer(USBD_HandleTypeDef *pdev, uint8_t *pbuff,
		uint16_t length) {
	USBD_IAD_HandleTypeDef *hiad = (USBD_IAD_HandleTypeDef*) pdev->pClassData;

	hiad->cdc1.TxBuffer = pbuff;
	hiad->cdc1.TxLength = length;

	return USBD_OK;
}

/**
 * @brief  USBD_IAD_SetTxBuffer
 * @param  pdev: device instance
 * @param  pbuff: Tx Buffer
 * @retval status
 */
uint8_t USBD_IAD_CDC2_SetTxBuffer(USBD_HandleTypeDef *pdev, uint8_t *pbuff,
		uint16_t length) {
	USBD_IAD_HandleTypeDef *hiad = (USBD_IAD_HandleTypeDef*) pdev->pClassData;

	hiad->cdc2.TxBuffer = pbuff;
	hiad->cdc2.TxLength = length;

	return USBD_OK;
}

/**
 * @brief  USBD_IAD_SetRxBuffer
 * @param  pdev: device instance
 * @param  pbuff: Rx Buffer
 * @retval status
 */
uint8_t USBD_IAD_CDC1_SetRxBuffer(USBD_HandleTypeDef *pdev, uint8_t *pbuff) {
	USBD_IAD_HandleTypeDef *hiad = (USBD_IAD_HandleTypeDef*) pdev->pClassData;

	hiad->cdc1.RxBuffer = pbuff;

	return USBD_OK;
}

/**
 * @brief  USBD_IAD_SetRxBuffer
 * @param  pdev: device instance
 * @param  pbuff: Rx Buffer
 * @retval status
 */
uint8_t USBD_IAD_CDC2_SetRxBuffer(USBD_HandleTypeDef *pdev, uint8_t *pbuff) {
	USBD_IAD_HandleTypeDef *hiad = (USBD_IAD_HandleTypeDef*) pdev->pClassData;

	hiad->cdc2.RxBuffer = pbuff;

	return USBD_OK;
}

/**
 * @brief  USBD_IAD_DataOut
 *         Data received on non-control Out endpoint
 * @param  pdev: device instance
 * @param  epnum: endpoint number
 * @retval status
 */
uint8_t USBD_IAD_CDC1_TransmitPacket(USBD_HandleTypeDef *pdev) {
	USBD_IAD_HandleTypeDef *hiad = (USBD_IAD_HandleTypeDef*) pdev->pClassData;

	if (pdev->pClassData != NULL) {
		if (hiad->cdc1.TxState == 0) {
			/* Tx Transfer in progress */
			hiad->cdc1.TxState = 1;

			/* Transmit next packet */
			USBD_LL_Transmit(pdev,
					IAD_CDC1_IN_EP, hiad->cdc1.TxBuffer, hiad->cdc1.TxLength);

			return USBD_OK;
		} else {
			return USBD_BUSY;
		}
	} else {
		return USBD_FAIL;
	}
}

/**
 * @brief  USBD_IAD_DataOut
 *         Data received on non-control Out endpoint
 * @param  pdev: device instance
 * @param  epnum: endpoint number
 * @retval status
 */
uint8_t USBD_IAD_CDC2_TransmitPacket(USBD_HandleTypeDef *pdev) {
	USBD_IAD_HandleTypeDef *hiad = (USBD_IAD_HandleTypeDef*) pdev->pClassData;

	if (pdev->pClassData != NULL) {
		if (hiad->cdc2.TxState == 0) {
			/* Tx Transfer in progress */
			hiad->cdc2.TxState = 1;

			/* Transmit next packet */
			USBD_LL_Transmit(pdev,
					IAD_CDC2_IN_EP, hiad->cdc2.TxBuffer, hiad->cdc2.TxLength);

			return USBD_OK;
		} else {
			return USBD_BUSY;
		}
	} else {
		return USBD_FAIL;
	}
}

/**
 * @brief  USBD_IAD_ReceivePacket
 *         prepare OUT Endpoint for reception
 * @param  pdev: device instance
 * @retval status
 */
uint8_t USBD_IAD_CDC1_ReceivePacket(USBD_HandleTypeDef *pdev) {
	USBD_IAD_HandleTypeDef *hiad = (USBD_IAD_HandleTypeDef*) pdev->pClassData;

	/* Suspend or Resume USB Out process */
	if (pdev->pClassData != NULL) {
		if (pdev->dev_speed == USBD_SPEED_HIGH) {
			/* Prepare Out endpoint to receive next packet */
			USBD_LL_PrepareReceive(pdev,
				IAD_CDC1_OUT_EP, hiad->cdc1.RxBuffer,
				IAD_DATA_HS_OUT_PACKET_SIZE);
		} else {
			/* Prepare Out endpoint to receive next packet */
			USBD_LL_PrepareReceive(pdev,
				IAD_CDC1_OUT_EP, hiad->cdc1.RxBuffer,
				IAD_DATA_FS_OUT_PACKET_SIZE);
		}
		return USBD_OK;
	} else {
		return USBD_FAIL;
	}
}

/**
 * @brief  USBD_IAD_ReceivePacket
 *         prepare OUT Endpoint for reception
 * @param  pdev: device instance
 * @retval status
 */
uint8_t USBD_IAD_CDC2_ReceivePacket(USBD_HandleTypeDef *pdev) {
	USBD_IAD_HandleTypeDef *hiad = (USBD_IAD_HandleTypeDef*) pdev->pClassData;

	/* Suspend or Resume USB Out process */
	if (pdev->pClassData != NULL) {
		if (pdev->dev_speed == USBD_SPEED_HIGH) {
			/* Prepare Out endpoint to receive next packet */
			USBD_LL_PrepareReceive(pdev,
				IAD_CDC2_OUT_EP, hiad->cdc2.RxBuffer,
				IAD_DATA_HS_OUT_PACKET_SIZE);
		} else {
			/* Prepare Out endpoint to receive next packet */
			USBD_LL_PrepareReceive(pdev,
				IAD_CDC2_OUT_EP, hiad->cdc2.RxBuffer,
				IAD_DATA_FS_OUT_PACKET_SIZE);
		}
		return USBD_OK;
	} else {
		return USBD_FAIL;
	}
}
