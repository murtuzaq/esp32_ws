/**
 * @file app_blinky.c
 * @brief endlessly toggle gpio pin
 *
 */

/*****************************************************************************
 *	Private Includes
 *****************************************************************************/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "stdbool.h"
/*****************************************************************************
 *	Private External References
 *****************************************************************************/

/*****************************************************************************
 *	Private Defines & Macros
 *****************************************************************************/
#define BLINK_GPIO 13
/*****************************************************************************
 *	Private Typedefs & Enumerations
 *****************************************************************************/

/*****************************************************************************
 *	Private Variables
 *****************************************************************************/

/*****************************************************************************
 *	Private Function Prototypes
 *****************************************************************************/
static void app_blinky(void *pvParameter);
/*****************************************************************************
 *	Public Functions
 *****************************************************************************/
/*****************************************************************************
 *  task_hello_init
 *  Parameters:
 *    none
 *  Returns:
 *    none
 *  Description:
 *    Initialize module here
 *****************************************************************************/
void app_blinky_init(void)
{
	//setup gpio pin as output;
	gpio_pad_select_gpio(BLINK_GPIO);
	gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

	//create task;
	xTaskCreate(&app_blinky, "app_blinky", 2048, NULL, 5, NULL);
}

/*****************************************************************************
 *	Private Functions
 *****************************************************************************/

/*****************************************************************************
 *  task_hello
 *  Parameters:
 *    none
 *  Returns:
 *    none
 *  Description:
 *    Run Infinite loop here.
 *    Endlessly prints to terminal;
 *****************************************************************************/
static void app_blinky(void *pvParameter)
{
	bool blinky_state = false;

	while(1)
	{
		//set level;
        gpio_set_level(BLINK_GPIO, blinky_state);

        //toggle state;
        blinky_state = !blinky_state;

        //delay;
        vTaskDelay(1000 / portTICK_RATE_MS);
	}
}


