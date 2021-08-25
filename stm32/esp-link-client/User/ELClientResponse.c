#include "ELClientResponse.h"

static uint16_t _arg_num = 0; /**< Argument number */
static uint8_t* _arg_ptr = NULL; /**< Pointer to argument */
static ELClientPacket* _cmd = NULL; /**< Global Pointer to packet */

/* Call this init function before parsing the received packet */
void ELClientResponseInit(ELClientPacket* packet)
{
  _cmd = packet;
  _arg_ptr = _cmd->args;
  _arg_num = 0;
}

/* Pop pointer argument */
int16_t Response_popArgPtr(void **data)
{
  if (_arg_num >= _cmd->argc) { return -1; }

  /* Get  2 bytes len */
  uint16_t len = *(uint16_t*)_arg_ptr;
  uint16_t pad = ( 4- ( (len + 2) & 3 ) ) & 3;    /* pad to next multiple of 4, including length */

  /* Increase argument pointer */
  _arg_ptr += 2;

  /* Inc argument number */
  _arg_num++;

  /* Get payload data */
  *data = _arg_ptr;
  _arg_ptr += len + pad;

  return len;
}

/* Pop argument data */
int16_t Response_popArg(void* d, uint16_t maxLen)
{
  if (_arg_num >= _cmd->argc) return -1;

  /* Get  2 bytes len */
  uint16_t len = *(uint16_t*)_arg_ptr;
  uint16_t pad = (4-((len+2)&3))&3;    /* pad to next multiple of 4, including length */
  _arg_ptr += 2;
  _arg_num++;

  uint8_t *data = (uint8_t *)d;
  uint16_t l = len > maxLen ? maxLen : len;
  uint8_t *p = _arg_ptr;
  while (l--) {
    *data++ = *p++;
  }

  _arg_ptr += len + pad;

  return len;
}

void Response_popString(uint8_t* buffer, uint16_t* payload_len)
{
  uint8_t i = 0;
  uint16_t len = *(uint16_t*)_arg_ptr;
  uint16_t pad = (4 - ( ( len + 2 ) & 3 ) ) & 3;    /* pad to next multiple of 4, including length */

  _arg_ptr += 2;
  _arg_num++;

  for (i = 0; i < len; i++) {
    buffer[i] = (char)*_arg_ptr++;
  }
  buffer[i] = '\0'; /* String NULL terminated character */

  *payload_len = i + 1;

  _arg_ptr += pad;
}
