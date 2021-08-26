#include "ELClient.h"
#include "ELClientMqtt.h"
#include "ELClientResponse.h"
#include "ELClientRequest.h"

#include "port.h"
#include "debug_log.h"

#include "cmsis_os.h"
#include "platform.h"

#include "main.h"
#include "stm32f4xx_ll_gpio.h"

#define MQTT_FRAME_LEN      256

static BOOL volatile connected;
static uint8_t topic[MQTT_FRAME_LEN] = {0};
static uint8_t data[MQTT_FRAME_LEN] = {0};

/* WIFI status callback */
void wifiStatusCb(void* response);

/* MQTT callbacks */
void mqttConnected(void* response);
void mqttDisconnected(void* response);
void mqttData(void* response);
void mqttPublished(void* response);

void Mqtt_Init()
{
  ELClient_Init();
  ELClientMqtt_WifiCbRegister(wifiStatusCb);

  /* Set-up callbacks for events and initialize with es-link. */
  ELClientMqtt_ConnectedCbRegister(mqttConnected);
  ELClientMqtt_DisconnectedCbRegister(mqttDisconnected);
  ELClientMqtt_PublishedCbRegister(mqttPublished);
  ELClientMqtt_DataCbRegister(mqttData); /* mqttData =  Address of User Function located in Flash */
}

void Mqtt_Publish(const char* topic, const uint8_t* data, const uint16_t len)
{
  ELClientMqtt_publish(topic, data, len, 0, 0);
}

uint8_t Mqtt_IsConnected() 
{
  return connected;
}

void Mqtt_Sync()
{
  BOOL ok = FALSE;
  do{
	 ok = ELClient_Sync();      // sync up with esp-link, blocks for up to 2 seconds
	 osDelay(1000);
  } while(!ok);
}

//============================ LOCAL FUNCTIONS ===============================
// Callback made from esp-link to notify of wifi status changes
void wifiStatusCb(void* arg)
{
	uint8_t status;
	Response_popArg(&status, 1);
	if(status == STATION_GOT_IP) {
		DBG_PRINTF("WIFI CONNECTED");
	} else {
		DBG_PRINTF("WIFI NOT READY: ");
		DBG_PRINTF("%d", status);
	}
}

// Callback when MQTT is connected
void mqttConnected(void* response)
{
  DBG_PRINTF("MQTT connected!");
  connected = TRUE;
}

// Callback when MQTT is disconnected
void mqttDisconnected(void* response)
{
  DBG_PRINTF("MQTT disconnected");
  connected = FALSE;
}

/* Callback when an MQTT message arrives for one of our subscriptions */
void mqttDataCmdHandler(char* payload, uint32_t len)
{
  if (memcmp(payload, "led_on", strlen("led_on")) == 0) {
	  LL_GPIO_SetOutputPin(LD3_GPIO_Port, LD3_Pin);
  } else if (memcmp(payload, "led_off", strlen("led_off")) == 0) {
	  LL_GPIO_ResetOutputPin(LD3_GPIO_Port, LD3_Pin);
  }
}

void mqttData(void* response)
{
  uint8_t  topicLen = 0;
  uint8_t  payloadLen = 0;
  ELClientResponseInit((ELClientPacket*) response); /* Prepare packet */

  /* POP Mqtt topic */
  DBG_PRINTF("Received: topic=");
  Response_popString(&topic[0], &topicLen);
  DUMP_BUFFER(topic, MQTT_FRAME_LEN);

  /*  Data Mqtt payload */
  payloadLen = Response_popArg(&data[0], sizeof(data));
  DBG_PRINTF("Payload len = %d\r\n", 0);
  DBG_PRINTF("data=");
  DUMP_BUFFER(data, MQTT_FRAME_LEN);

  /* Example command handler */
  if (strcmp((char*)topic, "/esp-link/command") == 0)
  {
    mqttDataCmdHandler((char*)data, payloadLen);
  }
}

/* Topic has been published successfully */
void mqttPublished(void* response)
{
	DBG_MSG("MQTT published");
    LL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
}

