/*
 * common.h
 *
 *  Created on: Aug 8, 2021
 *      Author: Lenovo
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <stdint.h>
#include <stddef.h>
#include "port.h"

#define PACKED __attribute__ ((__packed__))

typedef  void (*elclient_callback_fn)(void*);

uint32_t millis();

#endif /* COMMON_H_ */
