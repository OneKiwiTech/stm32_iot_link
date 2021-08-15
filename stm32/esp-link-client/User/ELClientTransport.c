#include "ELClientTransport.h"
#include "FreeRTOS.h"
#include "queue.h"

/* The queue is to be created to hold a maximum of 10 uint64_t
variables. */
#define SERIAL_QUEUE_LENGTH    256
#define SERIAL_ITEM_SIZE       sizeof( uint8_t )

/* The variable used to hold the queue's data structure. */
static StaticQueue_t xSerialQueueStruct;
QueueHandle_t xSerialQueueHandle;

/* The array to use as the queue's storage area.  This must be at least
uxQueueLength * uxItemSize bytes. */
uint8_t ucSerialQueueStorageArea[ SERIAL_QUEUE_LENGTH * SERIAL_ITEM_SIZE ];

void ELCLient_Transport_Init( void *pvParameters )
{
    /* Create a queue capable of containing 10 uint64_t values. */
    xSerialQueueHandle = xQueueCreateStatic( SERIAL_QUEUE_LENGTH,
                                 SERIAL_ITEM_SIZE,
                                 ucSerialQueueStorageArea,
                                 &xSerialQueueStruct );

 }

BOOL ELClient_Read(uint8_t* data)
{
      if( xQueueReceive( xSerialQueueHandle,
                         data,
                         ( TickType_t ) 0xFFFFFFFF ) == pdPASS )
      {
         /* xRxedStructure now contains a copy of xMessage. */
         return TRUE;
      }

    return FALSE;
}

void ELClient_Write(uint8_t data)
{
  switch (data)
  {
	  case SLIP_END:
		  xPortSerialPutByte(SLIP_ESC);
		  xPortSerialPutByte(SLIP_ESC_END);
		break;
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

void pxFrameCBByteReceived(uint8_t in_byte)
{

}

