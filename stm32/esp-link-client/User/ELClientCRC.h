#ifndef ELCLIENT_CRC_H_
#define ELCLIENT_CRC_H_
#include <stdint.h>

uint16_t _crc16Add(unsigned char b, uint16_t sum);
uint16_t _crc16Data(const unsigned char *data, uint16_t len, uint16_t acc);

#endif /* ELCLIENT_CRC_H_ */
