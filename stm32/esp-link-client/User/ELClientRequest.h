#ifndef ELCLIENT_REQUEST_H_
#define ELCLIENT_REQUEST_H_ 

#include <stdint.h>

/* Request function*/
void ELClient_EndRequest(void);
void ELClient_RequestBuffer(const void* data, uint16_t len);
void ELClient_RequestArgc(uint16_t cmd, uint32_t value, uint16_t argc);

#endif /* ELCLIENT_REQUEST_H_ */
