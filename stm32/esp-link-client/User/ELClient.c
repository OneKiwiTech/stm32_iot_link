#include "ELClient.h"
#include "ELClientResponse.h"
#include "ELClientTransport.h"

#include "common.h"

/* Port header */
#include "port.h"
#include "platform.h"
#include "debug_log.h"

/* FreeRTOS header */
#include "FreeRTOS.h"
#include "semphr.h"

#include "ELClientCRC.h"
#include "ELClientTransport.h"

SemaphoreHandle_t xCompltSemp;
StaticSemaphore_t xCompltSempBuffer;

static ELClientPacket* compltPacketPtr = 0;

static uint8_t _protoBuf[128]; /**< Protocol buffer */
static ELClientProtocol _proto; /**< Protocol structure */
static elclient_callback_fn	_wifiCb; /**< Wifi status callback */

static void __ProtoCompletedCb(void);

/* ===== Initialization ======== */
void ELClient_Init() 
{
  _proto.buf = _protoBuf;
  _proto.bufSize = sizeof(_protoBuf);
  _proto.dataLen = 0;
  _proto.isEsc = 0;

  /* Semaphore to notify received new frame */
  xCompltSemp = xSemaphoreCreateBinaryStatic( &xCompltSempBuffer );
 
  /* Transport init */
  xPortEventInit();
  ELCLient_Transport_Init();
}


/* ELClient main process */
void ELClient_Process(void* arg)
{
  uint8_t value = 0;

  for(;;)
  {
	  /* Read one-by-one data byte from serial queue */
	  value = ELClient_Read();
	  if (value == SLIP_ESC) /* Check special byte & raise flag */
	  {
		_proto.isEsc = 1;
	  } else if (value == SLIP_END) /* Check End Frame */
	  {
		  if ( _proto.dataLen >= 8 ) { /* 8 = minimum header len*/
			__ProtoCompletedCb();
			xSemaphoreGive( xCompltSemp );
		  }
		  _proto.dataLen = 0;
		  _proto.isEsc = 0;
	  } else
	  {
		if (_proto.isEsc)
		{
		  if (value == SLIP_ESC_END) { value = SLIP_END; }
		  if (value == SLIP_ESC_ESC) { value = SLIP_ESC; }
		  _proto.isEsc = 0;
		}
		if (_proto.dataLen < _proto.bufSize) {
		  _proto.buf[_proto.dataLen++] = value;
		}else
		{
			_proto.dataLen = 0;
		}
	  }
  }
}

void ELClientMqtt_WifiCbRegister(void* fn)
{
  _wifiCb = (elclient_callback_fn)fn;
}

BaseType_t ELClient_WaitReturn(void)
{
	return (xSemaphoreTake( xCompltSemp, ( TickType_t ) 3000 ) == pdTRUE);
}

BOOL ELClient_Sync(void)
{
  /* send a SLIP END char to make sure we get a clean start */
  xPortSerialPutByte(SLIP_END);
  
  /* send sync request */
  ELClient_RequestArgc(CMD_SYNC, (uint32_t)&_wifiCb, 0);
  ELClient_EndRequest();

  if ( ELClient_WaitReturn() )
  {
	  if (compltPacketPtr->value == (uint32_t)&_wifiCb) 
    {
		  return TRUE;
	  }
  }

  return FALSE;
}

void ELClient_GetWifiStatus(void)
{
  ELClient_RequestArgc(CMD_WIFI_STATUS, 0, 0);
  ELClient_EndRequest();
}

/*================ CALLBACK HANDLER ================== */
static void __ProtoCompletedCb(void)
{
  compltPacketPtr = (ELClientPacket*)_proto.buf;

  elclient_callback_fn  fp;

  /* verify CRC */
  /* Calculated from received Frame, excludes 2 last bytes CRC */
  uint16_t crc = _crc16Data(_proto.buf, _proto.dataLen-2, 0);
  uint16_t resp_crc = *(uint16_t*)(_proto.buf+_proto.dataLen-2); /* Respone CRC */
  if (crc != resp_crc) {
    DBG_PRINTF("ELC: Invalid CRC");
    return ;
  }

	/* dispatch based on command */
	switch (compltPacketPtr->cmd)
  {
	  /*VALUE FEILD = normal value */
	  case CMD_RESP_V: // response with a value: return the packet
		  // value response
		  DBG_PRINTF("RESP_V: ");
		  DUMP_BUFFER(&(compltPacketPtr->value), 1);
		 break;

		 /*VALUE FEILD = Pointer */
	  case CMD_RESP_CB: /* response callback: perform the callback! */
		  DBG_PRINTF("RESP_CB: ");
		  DBG_PRINTF(compltPacketPtr->value);
		  DBG_PRINTF(" ");
		  DBG_PRINTF(compltPacketPtr->argc);
		  /* cast to function pointer (elclient_callback_fn*)(compltPacketPtr->value) & get value*/
		  fp = *((elclient_callback_fn*)(compltPacketPtr->value));
		  /* fp = Application Function Pointer */
		  if (fp != NULL)
		  {
		  	(fp)(compltPacketPtr); /*Call back registered USER function, USER process payload */
		  }
		 break;

	  case CMD_SYNC: /* esp-link is not in sync, it may have reset, signal up the stack */
		  DBG_PRINTF("NEED_SYNC!");
		  break;

	  default:
		  DBG_PRINTF("CMD??");
		  break;
	 }
}

