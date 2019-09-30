#ifndef __MAIN_H__
#define __MAIN_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>

#define DEBUG

#ifdef DEBUG
    #define Log(debug_level, format, ...)                                                                    \
			do{                                                                                              \
				switch(debug_level)                                                                          \
				{                                                                                            \
				case 0:                                                                                      \
					printf(format, ##__VA_ARGS__);                                                           \
					break;                                                                                   \
				case 1:                                                                                      \
					printf("[%s] "format"\r\n", __FUNCTION__, ##__VA_ARGS__);                                \
					break;                                                                                   \
				case 2:                                                                                      \
					printf("[%s, %d] "format"\r\n", __FUNCTION__, __LINE__, ##__VA_ARGS__);                  \
					break;                                                                                   \
				default:                                                                                     \
					printf("[%s, %s, %d] "format"\r\n", __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__);    \
					break;                                                                                   \
				}                                                                                            \
			}while(0)
#else
    #define Log(DebugLevel, format, ...)
#endif

#define DEVICE_VERSION        "V1.0.0"
#define ALG_VERSION           "V2.0.0"
#define BLE_MAC               "240AC423AD7E"
#define BLE_DEVICE_VERSION    "V3.0.0"
#define CURRENT_TIME          "2019-09-27 14:30:40"
#define MQTT_SERVER           "china-test.mqtt.protontek.com"
#define MQTT_PORT             1883
#define MQTT_USERNAME         "proton"
#define MQTT_PASSWORD         "proton123"
#define UPGRADE_URL           "vdpics.oss-cn-hangzhou.aliyuncs.com/temp/type4/ChargingBaseStand.bin"

#ifdef __cplusplus
}
#endif

#endif    /* End define macor __MAIN_H__ */
