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
#include "app_control.h"
#include "control_startup.h"
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
app_control_t control = {};

control_loop cntrl_lp[APP_CONTROL_MODE_TOTAL] =
{
			[APP_CONTROL_MODE_STARTUP] 	=  control_startup_exe,
};
/*****************************************************************************
 *	Private Function Prototypes
 *****************************************************************************/
static void app_control(void *pvParameter);
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
void app_control_init(void)
{
	//create task;
	xTaskCreate(&app_control, "app_control", 2048, NULL, 5, NULL);
}

/*****************************************************************************
 *	Private Functions
 *****************************************************************************/

/*****************************************************************************
 *  app_control
 *  Parameters:
 *    none
 *  Returns:
 *    none
 *  Description:
 *
 *
 *****************************************************************************/
static void app_control(void *pvParameter)
{
	control.mode = APP_CONTROL_MODE_STARTUP;

	while(1)
	{
		cntrl_lp[control.mode]();
	}
}


