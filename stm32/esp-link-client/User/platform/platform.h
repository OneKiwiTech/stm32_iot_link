#ifndef PLATFORM_H_
#define PLATFORM_H_

BOOL xPortSerialInit( void);
BOOL xPortSerialPutByte( CHAR ucByte );
BOOL xPortSerialSendBuffer( CHAR* ucBytePtr, ULONG ulBufferLen );


BOOL  xPortEventInit( void );
BOOL  xPortEventPost( uint32_t eEvent );
BOOL  xPortEventGet( uint32_t * eEvent );

#endif /* PLATFORM_H_ */