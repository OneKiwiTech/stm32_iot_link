#ifndef _EL_CLIENT_RESPONSE_H_
#define _EL_CLIENT_RESPONSE_H_

#include "common.h"

#define VARIABLE_ARG_NUM 255

typedef struct PACKED {
  uint16_t cmd;            /**< Command to execute */
  uint16_t argc;           /**< Number of arguments */
  uint32_t value;          /**< Callback to invoke, NULL if none; or response value */
  uint8_t  args[0];        /**< Arguments */
} ELClientPacket; /**< Packet structure  */

void    ELClientResponseInit(ELClientPacket* packet);
int16_t Response_popArgPtr(void **data);
int16_t Response_popArg(void* d, uint16_t maxLen);
void    Response_popString(uint8_t* buffer, uint16_t* payload_len);

#endif // _EL_CLIENT_RESPONSE_H_
