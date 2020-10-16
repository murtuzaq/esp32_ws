
#include "app_gui.h"
#include "app_control.h"
#include "app_webclient.h"
#include "wifi_esp32.h"
#include "ble_esp32.h"
#include "nvs_esp32.h"


void app_main(void)
{
	nvs_esp32_init();
	wifi_esp32_init("myya", "Qu@!z@r15068");
	ble_esp32_init();

	app_control_init();
	app_gui_init();
	app_webclient_init();


}

