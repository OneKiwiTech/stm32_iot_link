#ifndef EL_CLIENT_TRANSPORT_H_
#define EL_CLIENT_TRANSPORT_H_

#include "common.h"

/* SLIP protocol definition */
#define SLIP_END      0xC0 //0300    /**< Indicates end of packet */
#define SLIP_ESC      0xDB //0333    /**< Indicates byte stuffing */
#define SLIP_ESC_END  0xDC //0334    /**< ESC ESC_END means END data byte */
#define SLIP_ESC_ESC  0xDD //0335    /**< ESC ESC_ESC means ESC data byte */

void ELCLient_Transport_Init( void );
uint8_t ELClient_Read( void );
void ELClient_Write(uint8_t data);
void ELClient_WriteBuffer(void* data, uint16_t len);

#endif /* EL_CLIENT_TRANSPORT_H_ */
