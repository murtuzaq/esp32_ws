/**
 * @file wifi_esp32.c
 * @brief manages application use of wifi module
 *
 */

/*****************************************************************************
 *	Private Includes
 *****************************************************************************/
#include "sdkconfig.h"
#include "wifi_esp32.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_event.h"
#include "esp_wifi.h"
/*****************************************************************************
 *	Private External References
 *****************************************************************************/

/*****************************************************************************
 *	Private Defines & Macros
 *****************************************************************************/
#define TAG	"wifi esp32"

/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

#define WIFI_SSID		"myya"
#define WIFI_PASSWORD	"Qu@!z@r15068"
#define WIFI_MAX_RETRY	 5

/*****************************************************************************
 *	Private Typedefs & Enumerations
 *****************************************************************************/

/*****************************************************************************
 *	Private Variables
 *****************************************************************************/
wifi_esp32_inst_t	wifi_esp32 = {};

/*****************************************************************************
 *	Private Function Prototypes
 *****************************************************************************/
static void wifi_esp32_event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data);
/*****************************************************************************
 *	Public Functions
 *****************************************************************************/
/*****************************************************************************
 *  wifi_esp32_init
 *  Parameters:
 *    none
 *  Returns:
 *    true -> init success
 *    false -> init failed;
 *  Description:
 *    Initialize module here
 *****************************************************************************/
bool wifi_esp32_init(void)
{

	wifi_esp32.event_group = xEventGroupCreate();

	tcpip_adapter_init();

    esp_event_loop_create_default();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_esp32_event_handler, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_esp32_event_handler, NULL);

    wifi_config_t wifi_config =
    {
    	.sta =
    	{
    		.ssid = WIFI_SSID,
			.password = WIFI_PASSWORD
    	},
    };

    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config);
    esp_wifi_start() ;

    ESP_LOGI(TAG, "wifi_init_sta finished.");

    wifi_esp32.initilized = true;

    return(true);


}

/*****************************************************************************
 *  wifi_esp32_connected
 *  Parameters:
 *    none
 *  Returns:
 *    true -> wifi is connected;
 *    false -> wifi not connected;
 *  Description:
 *    checks if wifi is connected
 *****************************************************************************/
bool wifi_esp32_connected(void)
{
	if (wifi_esp32.initilized)
		return(wifi_esp32.connected);

	return(false);
}

/*****************************************************************************
 *	Private Functions
 *****************************************************************************/

static void wifi_esp32_event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
    	esp_wifi_connect();
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
    	if (wifi_esp32.retry < WIFI_MAX_RETRY)
    	{
    		esp_wifi_connect();
            wifi_esp32.retry++;
            ESP_LOGI(TAG, "retry to connect to the AP");
        }
    	else
    	{
            xEventGroupSetBits(wifi_esp32.event_group, WIFI_FAIL_BIT);
            wifi_esp32.connected = false;
        }
        ESP_LOGI(TAG,"connect to the AP fail");
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        wifi_esp32.retry = 0;
        xEventGroupSetBits(wifi_esp32.event_group, WIFI_CONNECTED_BIT);
    	wifi_esp32.connected = true;
    }
}

