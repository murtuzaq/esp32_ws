/**
 * @file  wifi_esp32.h
 * @brief application interface to esp wifi module
 *
 */

#ifndef __wifi_esp32_H
#define __wifi_esp32_H

/*****************************************************************************
 *	Public Includes
 *****************************************************************************/
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "stdint.h"
#include "stdbool.h"
/*****************************************************************************
 *	Public External References
 *****************************************************************************/

/*****************************************************************************
 *	Public Defines & Macros
 *****************************************************************************/

/*****************************************************************************
 *	Public Typedefs & Enumerations
 *****************************************************************************/
typedef enum {
	WIFI_NOT_INITILIZED = -1,
	WIFI_CONNECTING 	= 0,
	WIFI_CONNECTED  	= 1,
	WIFI_FAILED 		= 2,_
}wifi_connection_t;
/*****************************************************************************
 *	Public Function Prototypes
 *****************************************************************************/

bool wifi_esp32_init(char* ssid, char* password);
wifi_connection_t wifi_esp32_connection(void);

#endif /* __wifi_esp32_H */
