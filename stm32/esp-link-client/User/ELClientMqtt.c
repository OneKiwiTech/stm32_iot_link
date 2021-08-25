#include "ELClientMqtt.h"
#include "ELClientRequest.h"
#include "ELClient.h"

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

/* Send MQTT Setup Frame */
void ELClientMqtt_setup() 
{
  uint32_t fncCbPtr = 0;

  /* Send frame header */
  ELClient_RequestArgc(CMD_MQTT_SETUP, 0, 4);

  /* Send address of mqtt callback function pointer  */
  fncCbPtr = (uint32_t)(&EL_Mqtt_Struct.connectedCb);
  ELClient_RequestBuffer(&fncCbPtr, 4);

  fncCbPtr = (uint32_t)(&EL_Mqtt_Struct.disconnectedCb);
  ELClient_RequestBuffer(&fncCbPtr, 4);

  fncCbPtr = (uint32_t)(&EL_Mqtt_Struct.publishedCb);
  ELClient_RequestBuffer(&fncCbPtr, 4);

  fncCbPtr = (uint32_t)(&EL_Mqtt_Struct.dataCb);
  ELClient_RequestBuffer(&fncCbPtr, 4);

  ELClient_EndRequest();
}

/* SUBSCRIBE Request */
void ELClientMqtt_subscribe(const char* topic, uint8_t qos) 
{
  /* Send frame header */
  ELClient_RequestArgc(CMD_MQTT_SUBSCRIBE, 0, 2);

  ELClient_RequestBuffer(topic, strlen(topic));
  ELClient_RequestBuffer(&qos, 1);

  ELClient_EndRequest();
}


/* PUBLISH Request */
void ELClientMqtt_publish(const uint8_t* topic, const char* data,
                          const uint16_t len, uint8_t qos, 
                          uint8_t retain)
{
  // Header 
  ELClient_RequestArgc(CMD_MQTT_PUBLISH, 0, 5);

  // Data section
  ELClient_RequestBuffer(topic, strlen(topic)); /*Arg 1*/
  ELClient_RequestBuffer(data, len); /*Arg 2*/
  ELClient_RequestBuffer(&len, 2); /*Arg 3*/
  ELClient_RequestBuffer(&qos, 1); /*Arg 4*/
  ELClient_RequestBuffer(&retain, 1); /*Arg 5*/

  // Footer
  ELClient_EndRequest();
}
