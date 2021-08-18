#include "ELClient.h"
#include "ELClientResponse.h"
#include "ELClientTransport.h"

#include "port.h"
#include "common.h"
#include "platform.h"
#include "debug_log.h"

#include "FreeRTOS.h"
#include "semphr.h"

//static uint32_t testPtr = 0;

SemaphoreHandle_t xCompltSemp;
StaticSemaphore_t xCompltSempBuffer;

static ELClientPacket* compltPacketPtr = 0;
static uint16_t crc; /**< CRC checksum */
static uint8_t _protoBuf[128]; /**< Protocol buffer */
static ELClientProtocol _proto; /**< Protocol structure */
static elclient_callback_fn	wifiCb;

static uint16_t _crc16Add(unsigned char b, uint16_t acc);
static uint16_t _crc16Data(const unsigned char *data, uint16_t len, uint16_t acc);

void ELClient_RequestBuffer(const void* data, uint16_t len);
void ELClient_RequestArgc(uint16_t cmd, uint32_t value, uint16_t argc);

static void protoCompletedCb(void);

//===== Initialization
/* Porting status: DONE  */
void ELClient_Init() 
{
  _proto.buf = _protoBuf;
  _proto.bufSize = sizeof(_protoBuf);
  _proto.dataLen = 0;
  _proto.isEsc = 0;

  xCompltSemp = xSemaphoreCreateBinaryStatic( &xCompltSempBuffer );

  vMBPortSerialEnable(1, 0);
  ELCLient_Transport_Init(0);
}

/* Porting status: DONE  */
void ELClientMqtt_WifiCbRegister(void* fn)
{
  wifiCb = (elclient_callback_fn)fn;
}

BaseType_t ELClient_WaitReturn(void)
{
	return (xSemaphoreTake( xCompltSemp, ( TickType_t ) 3000 ) == pdTRUE);
}

/* Porting status: DONE */
BOOL ELClient_Sync(void)
{
  // send a SLIP END char to make sure we get a clean start
  xPortSerialPutByte(SLIP_END);
  
  // send sync request
//  testPtr = (uint32_t)&wifiCb;
  ELClient_RequestArgc(CMD_SYNC, (uint32_t)&wifiCb, 0);
  ELClient_Request();

  if ( ELClient_WaitReturn() )
  {
	  if (compltPacketPtr->value == (uint32_t)&wifiCb) {
		  return TRUE;
	  }
  }

  return FALSE;
}

/*Porting status: OK */
void ELClient_GetWifiStatus(void)
{
  ELClient_RequestArgc(CMD_WIFI_STATUS, 0, 0);
  ELClient_Request();
}



/*Porting status: NOT YET */
void ELClient_Process(void* arg)
{
  uint8_t value = 0;


  for(;;)
  {
	  value = ELClient_Read();
	  if (value == SLIP_ESC)
	  {
		_proto.isEsc = 1;
	  } else if (value == SLIP_END)
	  {
		  if ( _proto.dataLen >= 8 ) {
			protoCompletedCb();
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

//================= SLIP REQUEST FUNCIONS ==============
void ELClient_RequestArgc(uint16_t cmd, uint32_t value, uint16_t argc)
{
  crc = 0;

  xPortSerialPutByte(SLIP_END);
  ELClient_WriteBuffer(&cmd, 2);
  crc = _crc16Data((unsigned const char*)&cmd, 2, crc);

  ELClient_WriteBuffer(&argc, 2);
  crc = _crc16Data((unsigned const char*)&argc, 2, crc);

  ELClient_WriteBuffer(&value, 4);
  crc = _crc16Data((unsigned const char*)&value, 4, crc);
}

void ELClient_RequestBuffer(const void* data, uint16_t len)
{
  uint16_t l = 0;
  uint8_t *d = (uint8_t*)data;
  uint16_t data_len = len;

  // ELClient_write the length
  ELClient_WriteBuffer(&data_len, 2);
  crc = _crc16Data((unsigned const char*)&data_len, 2, crc);

  // output the data
  for (l=len; l>0; l--) 
  {
    ELClient_Write(*d);
    crc = _crc16Add(*d++, crc);
  }

  // output padding
  uint16_t pad = (4-(len&3))&3;
  uint8_t temp = 0;
  while (pad--) 
  {
    ELClient_Write(temp);
    crc = _crc16Add(temp, crc);
  }
}

/*Porting status: DONE  */
void ELClient_Request(void) 
{
  ELClient_WriteBuffer((uint8_t*)&crc, 2);
  xPortSerialPutByte(SLIP_END);
}

//================ CALLBACK HANDLER ==================
static void protoCompletedCb(void)
{
  compltPacketPtr = (ELClientPacket*)_proto.buf;

  elclient_callback_fn  fp;

  // verify CRC
  uint16_t crc = _crc16Data(_proto.buf, _proto.dataLen-2, 0);
  uint16_t resp_crc = *(uint16_t*)(_proto.buf+_proto.dataLen-2);
  if (crc != resp_crc) {
    DBG_PRINTF("ELC: Invalid CRC");
    return ;
  }

	// dispatch based on command
	switch (compltPacketPtr->cmd)
  {
	  case CMD_RESP_V: // response with a value: return the packet
		  // value response
		  DBG_PRINTF("RESP_V: ");
		  DBG_PRINTF(packet->value);
		 break;

	  case CMD_RESP_CB: // response callback: perform the callback!
		  // callback reponse
		  DBG_PRINTF("RESP_CB: ");
		  DBG_PRINTF(packet->value);
		  DBG_PRINTF(" ");
		  DBG_PRINTF(packet->argc);
		  fp = *((elclient_callback_fn*)(compltPacketPtr->value));
		  if (fp != NULL)
		  {
//			(fp)(compltPacketPtr);
		  }
		 break;

	  case CMD_SYNC: // esp-link is not in sync, it may have reset, signal up the stack
		  DBG_PRINTF("NEED_SYNC!");
		  break;

	  default:
		  DBG_PRINTF("CMD??");
		  break;
	 }
}

//================== CRC helper functions ====================
/*Porting status: DONE  */
uint16_t _crc16Add(unsigned char b, uint16_t sum)
{
  uint16_t acc = sum;

  acc ^= b;
  acc = (acc >> 8) | (acc << 8);
  acc ^= (acc & 0xff00) << 4;
  acc ^= (acc >> 8) >> 4;
  acc ^= (acc & 0xff00) >> 5;

  return acc;
}

/*Porting status: DONE  */
uint16_t _crc16Data(const unsigned char *data, uint16_t len, uint16_t acc)
{
  uint16_t i = 0;
  uint16_t sum = acc;

  for (i=0; i<len; i++)
  {
    sum = _crc16Add(*data++, sum);
  }

  return sum;
}

