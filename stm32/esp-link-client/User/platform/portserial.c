#include "stm32f4xx_ll_usart.h"

#include "port.h"
#include "config.h"

BOOL  xPortSerialInit( void)
{
  LL_USART_EnableIT_RXNE(ELCLIENT_UART);
  return TRUE;
}
 
BOOL xPortSerialPutByte( CHAR ucByte )
{
  /* Put a byte in the UARTs transmit buffer. This function is called
  * by the protocol stack if pxMBFrameCBTransmitterEmpty( ) has been
  * called. */
  LL_USART_TransmitData8(ELCLIENT_UART, ucByte);
  while(!LL_USART_IsActiveFlag_TC(ELCLIENT_UART)) { };
  return TRUE;
}
 
BOOL xPortSerialSendBuffer( CHAR* ucBytePtr, ULONG ulBufferLen )
{
  /* Put a byte in the UARTs transmit buffer. This function is called
  * by the protocol stack if pxMBFrameCBTransmitterEmpty( ) has been
  * called. */
	ULONG i = ulBufferLen;
	while(i--)
	{
		 LL_USART_TransmitData8(ELCLIENT_UART,  *ucBytePtr++);
		 while(!LL_USART_IsActiveFlag_TC(ELCLIENT_UART)) { };
	}

  return TRUE;
}
