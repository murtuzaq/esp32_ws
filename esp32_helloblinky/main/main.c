#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "stdbool.h"
#include "task_hello.h"

#define BLINK_GPIO 13

void blinky_task(void *pvParameter)
{
	bool blinky_state = false;

	gpio_pad_select_gpio(BLINK_GPIO);
	gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);


	while(1)
	{
        gpio_set_level(BLINK_GPIO, blinky_state);

        //toggle state;
        blinky_state = !blinky_state;

        vTaskDelay(1000 / portTICK_RATE_MS);
	}
}

void app_main(void)
{
	task_hello_init();
	xTaskCreate(&blinky_task, "blinky_task", 2048, NULL, 5, NULL);
}

