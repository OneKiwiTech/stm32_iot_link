#ifndef _EL_CLIENT_H_
#define _EL_CLIENT_H_

#include "common.h"

/* Enumeration of commands supported by esp-link, this needs to match the definition in esp-link */

typedef enum {
  CMD_NULL = 0,     /**< null, mainly to prevent 0 from doing something bad */
  CMD_SYNC,         /**< Synchronize, starts the protocol */
  CMD_RESP_V,       /**< Response with a value */
  CMD_RESP_CB,      /**< Response with a callback */
  CMD_WIFI_STATUS,  /**< Get the wifi status */
  CMD_CB_ADD,       /**< Add a custom callback */
  CMD_CB_EVENTS,    /**< ??? */
  CMD_GET_TIME,     /**< Get current time in seconds since the unix epoch */
  //CMD_GET_INFO,

  CMD_MQTT_SETUP = 10, /**< Register callback functions */
  CMD_MQTT_PUBLISH,    /**< Publish MQTT topic */
  CMD_MQTT_SUBSCRIBE,  /**< Subscribe to MQTT topic */
  CMD_MQTT_LWT,        /**< Define MQTT last will */

  CMD_REST_SETUP = 20, /**< Setup REST connection */
  CMD_REST_REQUEST,    /**< Make request to REST server */
  CMD_REST_SETHEADER,  /**< Define HTML header */

  CMD_WEB_SETUP = 30,  /**< web-server setup */
  CMD_WEB_DATA,        /**< used for publishing web-server data */

  CMD_SOCKET_SETUP = 40,  /**< Setup socket connection */
  CMD_SOCKET_SEND,        /**< Send socket packet */
} CmdName; /**< Enumeration of commands supported by esp-link, this needs to match the definition in esp-link! */

enum WIFI_STATUS {
  STATION_IDLE = 0,        /**< Idle status */
  STATION_CONNECTING,      /**< Trying to connect */
  STATION_WRONG_PASSWORD,  /**< Connection error, wrong password */
  STATION_NO_AP_FOUND,     /**< Connection error, AP not found */
  STATION_CONNECT_FAIL,    /**< Connection error, connection failed */
  STATION_GOT_IP           /**< Connected, received IP */
}; /**< Enumeration of possible WiFi status */

typedef struct {
  uint8_t* buf;
  uint16_t bufSize;
  uint16_t dataLen;
  uint8_t isEsc;
} ELClientProtocol; /**< Protocol structure  */

void ELClient_Init();

BOOL ELClient_Sync(void);
void ELClient_GetWifiStatus(void);
void ELClient_Process(void* arg);


/* Callback register */
void ELClientMqtt_WifiCbRegister(void* fn);

#endif // _EL_CLIENT_H_
