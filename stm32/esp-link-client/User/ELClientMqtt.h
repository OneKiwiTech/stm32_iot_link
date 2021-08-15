#ifndef _EL_CLIENT_MQTT_H_
#define _EL_CLIENT_MQTT_H_

#include <stdint.h>
#include "ELClient.h"

void ELClientMqtt_setup();
void ELClientMqtt_subscribe(const char* topic, uint8_t qos);
void ELClientMqtt_publish(const uint8_t* topic, const char* data,
						  const uint16_t len, uint8_t qos, uint8_t retain);

// Callback register function
void  ELClientMqtt_ConnectedCbRegister(void* fn);
void  ELClientMqtt_DisconnectedCbRegister(void* fn);
void  ELClientMqtt_PublishedCbRegister(void* fn);
void  ELClientMqtt_DataCbRegister(void* fn);

#endif // _EL_CLIENT_MQTT_H_
