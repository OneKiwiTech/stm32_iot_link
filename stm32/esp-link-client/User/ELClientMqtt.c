#include "ELClientMqtt.h"
#include <string.h>

typedef  void (*clientCbFunc_t)(void*);

typedef struct {
  clientCbFunc_t	connectedCb;
  clientCbFunc_t    disconnectedCb;
  clientCbFunc_t	publishedCb;
  clientCbFunc_t	dataCb;
} ELClient_MQTT_Struct_t;

static ELClient_MQTT_Struct_t  EL_Mqtt_Struct;



void ELClientMqtt_ConnectedCbRegister(void* fn)
{
	EL_Mqtt_Struct.connectedCb = fn;
}

void ELClientMqtt_DisconnectedCbRegister(void* fn)
{
	EL_Mqtt_Struct.disconnectedCb = fn;
}

void ELClientMqtt_PublishedCbRegister(void* fn)
{
	EL_Mqtt_Struct.publishedCb = fn;
}

void ELClientMqtt_DataCbRegister(void* fn)
{
	EL_Mqtt_Struct.dataCb = fn;
}

void ELClientMqtt_setup() 
{
  ELClient_RequestArgc(CMD_MQTT_SETUP, 0, 4);
  uint32_t cb = (uint32_t)(EL_Mqtt_Struct.connectedCb);
  ELClient_RequestBuffer(&cb, 4);
  cb = (uint32_t)(EL_Mqtt_Struct.disconnectedCb);
  ELClient_RequestBuffer(&cb, 4);
  cb = (uint32_t)(EL_Mqtt_Struct.publishedCb);
  ELClient_RequestBuffer(&cb, 4);
  cb = (uint32_t)(EL_Mqtt_Struct.dataCb);
  ELClient_RequestBuffer(&cb, 4);
  ELClient_Request();
}

// SUBSCRIBE
void ELClientMqtt_subscribe(const char* topic, uint8_t qos) 
{
  ELClient_RequestArgc(CMD_MQTT_SUBSCRIBE, 0, 2);
  ELClient_RequestBuffer(topic, strlen(topic));
  ELClient_RequestBuffer(&qos, 1);
  ELClient_Request();
}


// PUBLISH
void ELClientMqtt_publish(const uint8_t* topic, const char* data,
    const uint16_t len, uint8_t qos, uint8_t retain)
{
  ELClient_RequestArgc(CMD_MQTT_PUBLISH, 0, 5);
  ELClient_RequestBuffer(topic, strlen(topic));
  ELClient_RequestBuffer(data, len);
  ELClient_RequestBuffer(&len, 2);
  ELClient_RequestBuffer(&qos, 1);
  ELClient_RequestBuffer(&retain, 1);
  ELClient_Request();
}
