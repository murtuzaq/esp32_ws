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
typedef struct {
	EventGroupHandle_t 	event_group;
	uint32_t 			retry;
	bool				initilized;
	bool				connected;

}wifi_esp32_inst_t;
/*****************************************************************************
 *	Public Function Prototypes
 *****************************************************************************/

bool wifi_esp32_init(void);
bool wifi_esp32_connected(void);

#endif /* __wifi_esp32_H */
