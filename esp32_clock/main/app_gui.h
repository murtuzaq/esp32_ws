/**
 * @file  app_gui.h
 * @brief entry point for creating gui graphics on tft display
 *
 */

#ifndef __APP_GUI_H
#define __APP_GUI_H

/*****************************************************************************
 *	Public Includes
 *****************************************************************************/
#include <stdbool.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_freertos_hooks.h"
#include "freertos/semphr.h"
/*****************************************************************************
 *	Public External References
 *****************************************************************************/

/*****************************************************************************
 *	Public Defines & Macros
 *****************************************************************************/

/*****************************************************************************
 *	Public Typedefs & Enumerations
 *****************************************************************************/
/*
 * gui visual background
 * TERMINAL : black console, with text status
 * CLOCK	: graphical clock at the center
 */
typedef enum {
	GUI_VIEW_TERMINAL 	 = 0,
	GUI_VIEW_CLOCK 	 	 = 1,
	GUI_VIEW_TOTAL 	 	 = 2,
}gui_view;

/*
 * gui_cmd are visual commands sent to the display
 * TEXT: update text
 * TIME: update time
 * CLEAR: clear text fields;
 * WIFI: update wifi status
 * BLE:  update BLE status;
 */
typedef enum {
	GUI_CMD_TEXT 	 = 0,
	GUI_CMD_TIME 	 = 1,
	GUI_CMD_CLEAR	 = 2,
	GUI_CMD_WIFI	 = 3,
	GUI_CMD_BLE		 = 4,
}gui_cmd;

/*
 * intertask communication to be received
 * by gui task;
 */
typedef struct
{
	gui_view 	view;
	gui_cmd 	cmd;
	uint32_t 	time_sec;
	char 		msg[80];
} gui_message_t;

typedef struct
{
	bool 			initilized;
	gui_view		current_view;
	QueueHandle_t 	Queue;

}gui_t;
/*****************************************************************************
 *	Public Function Prototypes
 *****************************************************************************/

void app_gui_init(void);
bool app_gui_receive_message(gui_message_t* pxRxedMsg);
bool app_gui_message_send(gui_message_t pxTxdMsg);
#endif /* __APP_GUI_H */
