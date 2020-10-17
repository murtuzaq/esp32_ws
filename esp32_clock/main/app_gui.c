/* LVGL Example project
 * 
 *
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_freertos_hooks.h"
#include "freertos/semphr.h"
#include "esp_system.h"
#include "esp_log.h"
#include "gui_hw.h"
#include "app_gui.h"
#include "gui_terminal.h"
#include "driver/gpio.h"

/* Littlevgl specific */
#include "lvgl/lvgl.h"
#include "lvgl_helpers.h"

#include "lv_examples/src/lv_demo_widgets/lv_demo_widgets.h"
/*****************************************************************************
 *	Private External References
 *****************************************************************************/

/*****************************************************************************
 *	Private Defines & Macros
 *****************************************************************************/

/*****************************************************************************
 *	Private Typedefs & Enumerations
 *****************************************************************************/
typedef void (*gui_loop) (void);
/*****************************************************************************
 *	Private Variables
 *****************************************************************************/
static struct
{
	bool 			initilized;
	gui_view		current_view;
	QueueHandle_t 	Queue;

}gui;

gui_loop gui_lp[GUI_VIEW_TOTAL] =
{
			[GUI_VIEW_TERMINAL	] 	=  gui_terminal_exe,
			[GUI_VIEW_CLOCK		]   =  NULL,
};

/*****************************************************************************
 *	Private Function Prototypes
 *****************************************************************************/
static void app_gui(void *pvParameter);
/*****************************************************************************
 *	Public Functions
 *****************************************************************************/

void app_gui_init()
{
	/* Create a queue capable */

	gui.Queue = xQueueCreate( 10, sizeof(  gui_message_t ) );

	if( gui.Queue == NULL )
	{
		/* Queue was not created and must not be used. */
	}

    /* If you want to use a task to create the graphic, you NEED to create a Pinned task
     * Otherwise there can be problem such as memory corruption and so on.
     * NOTE: When not using Wi-Fi nor Bluetooth you can pin the guiTask to core 0 */
    xTaskCreatePinnedToCore(app_gui, "app_gui", 4096*2, NULL, 0, NULL, 1);

    gui.current_view = GUI_VIEW_TERMINAL;
    gui.initilized = true;

    ESP_LOGI(__FUNCTION__, "init complete");
}


bool app_gui_receive_message(gui_message_t* pxRxedMsg)
{

	//update gui hw screen at fixed interval;
	gui_hw_refresh();

	/* Receive a message from the created queue to hold complex struct gui_message_t
	 structure.  Block for 10 ms if a message is not immediately available.
	 The value is stored into input reference pointer pxRxedMsg, so it can be interrogated in
	 parent functioin;
	 */

	if( xQueueReceive( gui.Queue, pxRxedMsg, 10/ portTICK_PERIOD_MS ) == pdPASS )
	{
		/*
		 * pxRxedMsg now contains a copy of the message
	    */
		ESP_LOGI(__FUNCTION__, "message received from queue");
		return(true);
	}

	return(false);
}

bool app_gui_message_send(gui_message_t TxdMsg)
{
    /* Send an message structure.  Wait for 10 ticks for space to become
    available if necessary. */
    if( xQueueSend( gui.Queue, ( void * ) &TxdMsg, 10/ portTICK_PERIOD_MS ) != pdPASS )
    {
        /* Failed to post the message, even after 10 ticks. */
    	ESP_LOGI(__FUNCTION__, "failed to post message");
    	return(false);
    }

    return(true);


}



/*****************************************************************************
 *	Private Functions
 *****************************************************************************/
static void app_gui(void *pvParameter) {
    
    (void) pvParameter;
    gui_hw_init();

    
    while (true)
    {
    	ESP_LOGI(__FUNCTION__, "current view = %d", gui.current_view);
    	gui_lp[gui.current_view]();
    }

}
