#include "ELClientTransport.h"

/* Queue */
#include "FreeRTOS.h"
#include "queue.h"

/* Porting */
#include "platform.h"

/* UART queue */
#define SERIAL_QUEUE_LENGTH    512
#define SERIAL_ITEM_SIZE       sizeof( uint8_t )

static StaticQueue_t xSerialQueueStruct;
QueueHandle_t xSerialQueueHandle;

uint8_t ucSerialQueueStorageArea[ SERIAL_QUEUE_LENGTH * SERIAL_ITEM_SIZE ];

/* Init uart transport */
void ELCLient_Transport_Init(  )
{
    xPortSerialInit();
    xSerialQueueHandle = xQueueCreateStatic( SERIAL_QUEUE_LENGTH,
                                 SERIAL_ITEM_SIZE,
                                 ucSerialQueueStorageArea,
                                 &xSerialQueueStruct );
 }

/* Read data from the serial queue - wait forever*/
uint8_t ELClient_Read()
{
	uint8_t data = 0;
  xQueueReceive( xSerialQueueHandle, &data, ( TickType_t ) 0xFFFFFFFF );

  return data;
}
/* For each byte in the packet, send the appropriate character
* sequence
*/
void ELClient_Write(uint8_t data)
{
  switch (data)
  {
  /* if it’s the same code as an END character, we send a
  * special two character code so as not to make the
  * receiver think we sent an END
  */    
	  case SLIP_END:
		  xPortSerialPutByte(SLIP_ESC);
		  xPortSerialPutByte(SLIP_ESC_END);
		break;
    /* if it’s the same code as an ESC character,
    * we send a special two character code so as not
    * to make the receiver think we sent an ESC
    */    
	  case SLIP_ESC:
		  xPortSerialPutByte(SLIP_ESC);
		  xPortSerialPutByte(SLIP_ESC_ESC);
		break;
	  default:
		xPortSerialPutByte(data);
  }
  
}

void ELClient_WriteBuffer(void* data, uint16_t len)
{
  uint8_t *d = (uint8_t*)data;

  while (len--)
  {
	  ELClient_Write(*d++);
  }
}

