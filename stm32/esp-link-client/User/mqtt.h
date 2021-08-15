/*
 * mqtt.h
 *
 *  Created on: Aug 8, 2021
 *      Author: Lenovo
 */

#ifndef MQTT_H_
#define MQTT_H_

void Mqtt_Init();
void Mqtt_Loop();
void Mqtt_Publish(const char* topic, const uint8_t* data, const uint16_t len);

#endif /* MQTT_H_ */
