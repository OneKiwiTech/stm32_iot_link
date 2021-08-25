/*
 * mqtt.h
 *
 *  Created on: Aug 8, 2021
 *      Author: Lenovo
 */

#ifndef MQTT_H_
#define MQTT_H_

void Mqtt_Init();
void Mqtt_Sync();
void Mqtt_Publish(const char* topic, const uint8_t* data, const uint16_t len);
uint8_t Mqtt_IsConnected();

#endif /* MQTT_H_ */
