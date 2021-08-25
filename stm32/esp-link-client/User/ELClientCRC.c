#include "ELClientCRC.h"

uint16_t _crc16Add(unsigned char b, uint16_t sum)
{
  uint16_t acc = sum;

  acc ^= b;
  acc = (acc >> 8) | (acc << 8);
  acc ^= (acc & 0xff00) << 4;
  acc ^= (acc >> 8) >> 4;
  acc ^= (acc & 0xff00) >> 5;

  return acc;
}

uint16_t _crc16Data(const unsigned char *data, uint16_t len, uint16_t acc)
{
  uint16_t i = 0;
  uint16_t sum = acc;

  for (i=0; i<len; i++)
  {
    sum = _crc16Add(*data++, sum);
  }

  return sum;
}
