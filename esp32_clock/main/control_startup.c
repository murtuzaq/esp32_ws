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
#include "control_startup.h"
#include "app_gui.h"
#include "gui_terminal.h"
#include "esp_log.h"
#include "wifi_esp32.h"
#include <string.h>
/*****************************************************************************
 *	Private External References
 *****************************************************************************/

/*****************************************************************************
 *	Private Defines & Macros
 *****************************************************************************/

/*****************************************************************************
 *	Private Typedefs & Enumerations
 *****************************************************************************/
typedef enum
{
	STARTUP_WIFI_BOOT 				= 0,
	STARTUP_BLE_CONNECT				= 1,
	STARTUP_BLE_WIFI_PROVISION		= 2,
	STARTUP_WIFI_CONNECTING			= 3,
	STARTUP_WIFI_CONNECTED			= 4,

}startup_state;
static struct  {
	bool exit;
	startup_state	state;
} startup;
/*****************************************************************************
 *	Private Variables
 *****************************************************************************/

//place static global variables here...

/*****************************************************************************
 *	Private Function Prototypes
 *****************************************************************************/

/*****************************************************************************
 *	Public Functions
 *****************************************************************************/

/*****************************************************************************
 *  control_startup_exe
 *  Parameters:
 *    none
 *  Returns:
 *    none
 *  Description:
 *    main startup loop
 *****************************************************************************/
void control_startup_exe(void)
{
	startup.exit = false;
	startup.state = STARTUP_WIFI_BOOT;

	ESP_LOGW(__FUNCTION__, "startup");

	gui_terminal_update_text("startup");
	vTaskDelay(5000 / portTICK_PERIOD_MS);

	while(!startup.exit)
	{
		switch(startup.state)
		{
		case STARTUP_WIFI_BOOT:
			if ( (wifi_esp32_connection() == WIFI_CONNECTING ) ||
				(wifi_esp32_connection() == WIFI_CONNECTED) )
			{
				startup.state = STARTUP_WIFI_CONNECTING;
				gui_terminal_update_text("connecting to wifi");
				vTaskDelay(5000 / portTICK_PERIOD_MS);

			}
			else if (wifi_esp32_connection() == WIFI_FAILED)
			{
				startup.state = STARTUP_BLE_CONNECT;
				gui_terminal_update_text("wait for ble provision");
				vTaskDelay(5000 / portTICK_PERIOD_MS);
			}
			else
			{
				//do nothing; stay here...
			}
			break;

		case STARTUP_BLE_CONNECT:
			break;

		case STARTUP_BLE_WIFI_PROVISION:
			break;

		case STARTUP_WIFI_CONNECTING:
			if (wifi_esp32_connection() == WIFI_CONNECTED )
			{
				startup.state = STARTUP_WIFI_CONNECTED;
				gui_terminal_update_text("wifi connected");
				vTaskDelay(5000 / portTICK_PERIOD_MS);
			}
			else if (wifi_esp32_connection() == WIFI_FAILED)
			{
				startup.state = STARTUP_BLE_CONNECT;
				gui_terminal_update_text("wait for ble provision");
				vTaskDelay(5000 / portTICK_PERIOD_MS);
			}
			else
			{
				//do nothing, waiting for state change;
			}
			break;

		case STARTUP_WIFI_CONNECTED:
			//stay here, till i write some mode code, on what to do next
			break;

		}
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}

/*****************************************************************************
 *	Private Functions
 *****************************************************************************/

