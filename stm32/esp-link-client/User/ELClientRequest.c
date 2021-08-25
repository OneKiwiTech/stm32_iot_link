#include "ELClientRequest.h"
#include "ELClientTransport.h"
#include "ELClientCRC.h"

#include "platform.h"
#include "common.h"

static uint16_t crc = 0; /**< CRC checksum */

void ELClient_RequestArgc(uint16_t cmd, uint32_t value, uint16_t argc)
{
  crc = 0;

  /* send an initial END character to flush out any data that may
  * have accumulated in the receiver due to line noise
  */
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

void ELClient_EndRequest(void)
{
  ELClient_WriteBuffer((uint8_t*)&crc, 2);

  /* tell the receiver that we’re done sending the packet
  */
  xPortSerialPutByte(SLIP_END);
}
