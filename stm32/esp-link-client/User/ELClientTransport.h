#ifndef EL_CLIENT_TRANSPORT_H_
#define EL_CLIENT_TRANSPORT_H_

#include "common.h"

#define SLIP_END      0300    /**< Indicates end of packet */
#define SLIP_ESC      0333    /**< Indicates byte stuffing */
#define SLIP_ESC_END  0334    /**< ESC ESC_END means END data byte */
#define SLIP_ESC_ESC  0335    /**< ESC ESC_ESC means ESC data byte */


BOOL ELClient_Read(uint8_t* data);
void ELClient_Write(uint8_t data);
void ELClient_WriteBuffer(void* data, uint16_t len);

#endif /* EL_CLIENT_TRANSPORT_H_ */