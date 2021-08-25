#include "stm32f4xx_ll_usart.h"

#include "port.h"
#include "config.h"

BOOL  xPortSerialInit( void)
{
  LL_USART_EnableIT_RXNE(ELCLIENT_UART); /* RXNE = RX Not Empty */
  return TRUE;
}
 
BOOL xPortSerialPutByte( CHAR ucByte )
{
  /* Put a byte in the UARTs transmit buffer. This function is called
  * by the protocol stack if pxMBFrameCBTransmitterEmpty( ) has been
  * called. */
  LL_USART_TransmitData8(ELCLIENT_UART, ucByte);
  while(!LL_USART_IsActiveFlag_TC(ELCLIENT_UART)) { }; /* Waitting flag TC ( Transfer Complete) asserted */

  return TRUE;
}

static INLINE void __UartPutChar(CHAR b)
{
	 LL_USART_TransmitData8(ELCLIENT_UART,  b);
	 while(!LL_USART_IsActiveFlag_TC(ELCLIENT_UART)) { };
}

BOOL xPortSerialSendBuffer( CHAR* ucBytePtr, ULONG ulBufferLen )
{
  /* Put a byte in the UARTs transmit buffer. This function is called
  * by the protocol stack if pxMBFrameCBTransmitterEmpty( ) has been
  * called. */
	ULONG i = ulBufferLen;
	while(i--)
	{
#if 0
		 LL_USART_TransmitData8(ELCLIENT_UART,  *ucBytePtr++);
		 while(!LL_USART_IsActiveFlag_TC(ELCLIENT_UART)) { };
#else
		 __UartPutChar(*ucBytePtr++);
		 // xPortSerialPutByte(ucByte);
#endif
	}

  return TRUE;
}
