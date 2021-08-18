#include "ELClient.h"
#include "ELClientMqtt.h"
#include "ELClientResponse.h"

#include "port.h"
#include "debug_log.h"

#include "cmsis_os.h"
#include "platform.h"

#define MQTT_FRAME_LEN      128

static BOOL connected;
static uint8_t topic[MQTT_FRAME_LEN] = {0};
static uint8_t data[MQTT_FRAME_LEN] = {0};

// WIFI status callback
void wifiStatusCb(void* response);

// MQTT callback
void mqttConnected(void* response);
void mqttDisconnected(void* response);
void mqttData(void* response);
void mqttPublished(void* response);

void Mqtt_Init()
{

  ELClient_Init();
  ELClientMqtt_WifiCbRegister(wifiStatusCb);
  // Set-up callbacks for events and initialize with es-link.
  ELClientMqtt_ConnectedCbRegister(mqttConnected);
  ELClientMqtt_DisconnectedCbRegister(mqttDisconnected);
  ELClientMqtt_PublishedCbRegister(mqttPublished);
  ELClientMqtt_DataCbRegister(mqttData);
}

void Mqtt_Publish(const char* topic, const uint8_t* data, const uint16_t len)
{
  ELClientMqtt_publish(topic, data, len, 0, 0);
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

// Callback when an MQTT message arrives for one of our subscriptions
void mqttData(void* response)
{
  uint16_t  payloadLen = 0;

//  ELClientResponse *res = (ELClientResponse *)response;
  DBG_PRINTF("Received: topic=");
  Response_popBuffer(&topic[0], &payloadLen);
  DUMP_BUFFER(topic, MQTT_FRAME_LEN);

  Response_popBuffer(&data[0], &payloadLen);
  DBG_PRINTF("Payload len = %d\r\n", payloadLen);
  DBG_PRINTF("data=");
  DUMP_BUFFER(data, MQTT_FRAME_LEN);
}

void mqttPublished(void* response)
{
	DBG_PRINTF("MQTT published");
}

