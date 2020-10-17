/**
 * @file app_control.c
 * @brief thread that host all the control loops that drives the flow for the project
 *
 */

/*****************************************************************************
 *	Private Includes
 *****************************************************************************/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_freertos_hooks.h"
#include "freertos/semphr.h"
#include "app_control.h"
#include "control_startup.h"
#include "esp_log.h"
/*****************************************************************************
 *	Private External References
 *****************************************************************************/

/*****************************************************************************
 *	Private Defines & Macros
 *****************************************************************************/

/*****************************************************************************
 *	Private Typedefs & Enumerations
 *****************************************************************************/
typedef void (*control_loop) (void);

/*****************************************************************************
 *	Private Variables
 *****************************************************************************/
static struct{
	bool 			initilized;
	app_control_mode_t 	current_mode;
	QueueHandle_t 	Queue;
}control;

control_loop cntrl_lp[CONTROL_MODE_TOTAL] =
{
			[CONTROL_MODE_STARTUP] 	=  control_startup_exe,
};
/*****************************************************************************
 *	Private Function Prototypes
 *****************************************************************************/
static void control_task(void *pvParameter);
static void control_engine(control_message_t RxedMsg);
/*****************************************************************************
 *	Public Functions
 *****************************************************************************/
/*****************************************************************************
 *  app_control_init
 *  Parameters:
 *    none
 *  Returns:
 *    none
 *  Description:
 *    Initialize module here
 *****************************************************************************/
void control_init(void)
{
	/* Create a queue capable */

	control.Queue = xQueueCreate( 10, sizeof(  control_message_t ) );

	if( control.Queue == NULL )
	{
		/* Queue was not created and must not be used  */
		ESP_LOGE(__FUNCTION__, "failed to create queue");
		abort();
	}

	//create task;
	xTaskCreate(&control_task, "control_task", 2048, NULL, 5, NULL);

	control.current_mode = CONTROL_MODE_STARTUP;
	control.initilized = true;
}

/*****************************************************************************
 *  control_receive_message
 *  Parameters:
 *    none
 *  Returns:
 *    none
 *  Description:
 *
 *
 *****************************************************************************/
bool control_receive_message(void)
{

	/* Receive a message from Queue */
	control_message_t RxedMsg = {};
	if( xQueueReceive( control.Queue, &RxedMsg, 10/ portTICK_PERIOD_MS ) == pdPASS )
	{
		/*
		 * pxRxedMsg now contains a copy of the message
	    */
		ESP_LOGI(__FUNCTION__, "message received from queue");
		control_engine(RxedMsg);
		return(true);
	}

	return(false);
}
/*****************************************************************************
 *	Private Functions
 *****************************************************************************/

/*****************************************************************************
 *  control_task
 *  Parameters:
 *    none
 *  Returns:
 *    none
 *  Description:
 *
 *
 *****************************************************************************/
static void control_task(void *pvParameter)
{
	while(1)
	{
		cntrl_lp[control.current_mode]();
	}
}

/*****************************************************************************
 *  control_engine
 *  Parameters:
 *    none
 *  Returns:
 *    none
 *  Description:
 *
 *
 *****************************************************************************/
static void control_engine(control_message_t RxedMsg)
{
	switch(RxedMsg.cmd)
	{
	case CONTROL_CMD_WIFI:
		//store wifi credentials, and then reboot;
		abort();
		break;

	case CONTROL_CMD_WEBCLIENT:
		break;
	}
}


