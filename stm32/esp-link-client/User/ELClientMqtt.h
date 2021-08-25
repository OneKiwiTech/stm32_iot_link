#ifndef _EL_CLIENT_MQTT_H_
#define _EL_CLIENT_MQTT_H_

#include "common.h"

typedef struct {
  clientCbFunc_t	connectedCb;
  clientCbFunc_t    disconnectedCb;
  clientCbFunc_t  	publishedCb;
  clientCbFunc_t  	dataCb;
} ELClient_MQTT_Struct_t;

void ELClientMqtt_setup();

/* Pub/Sub function */
void ELClientMqtt_subscribe(const char* topic, uint8_t qos);
void ELClientMqtt_publish(const uint8_t* topic, const char* data,
						  const uint16_t len, uint8_t qos, uint8_t retain);

/* Callback register functions */
void  ELClientMqtt_ConnectedCbRegister(void* fn);
void  ELClientMqtt_DisconnectedCbRegister(void* fn);
void  ELClientMqtt_PublishedCbRegister(void* fn);
void  ELClientMqtt_DataCbRegister(void* fn);

#endif // _EL_CLIENT_MQTT_H_
