/**
 * @file app_webclient.c
 * @brief web client
 *
 */

/*****************************************************************************
 *	Private Includes
 *****************************************************************************/
#include "wifi_esp32.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>
/*****************************************************************************
 *	Private External References
 *****************************************************************************/

/*****************************************************************************
 *	Private Defines & Macros
 *****************************************************************************/
#define TAG "app webclient"

/*****************************************************************************
 *	Private Typedefs & Enumerations
 *****************************************************************************/
//#define CONFIG_RESOURCE "/"
//#define CONFIG_WEBSITE	"www.google.com"

#define CONFIG_RESOURCE	"/demo/aphorisms.php"
#define CONFIG_WEBSITE	"www.lucadentella.it"

//#define CONFIG_RESOURCE	"/api/ip"
//#define CONFIG_WEBSITE	"www.worldtimeapi.org"

//#define CONFIG_RESOURCE	"/newyork/daily.json?key=476f79503ed9eb872b5b6bff59986bb1"
//#define CONFIG_WEBSITE	"www.muslimsalat.com"



/*****************************************************************************
 *	Private Variables
 *****************************************************************************/

// HTTP request
static const char *REQUEST = 	"GET "CONFIG_RESOURCE" HTTP/1.1\r\n"
								"Host: "CONFIG_WEBSITE"\r\n"
								"User-Agent: ESP32\r\n"
								"\r\n";
/*****************************************************************************
 *	Private Function Prototypes
 *****************************************************************************/
static void app_webclient(void *pvParameter);
/*****************************************************************************
 *	Public Functions
 *****************************************************************************/
/*****************************************************************************
 *  task_webserver_init
 *  Parameters:
 *    none
 *  Returns:
 *    none
 *  Description:
 *    Create Webserver task (thread)
 *****************************************************************************/
void app_webclient_init(void)
{
	//create task;
	xTaskCreate(&app_webclient, "app_webclient", 4096, NULL, 5, NULL);
}

/*****************************************************************************
 *	Private Functions
 *****************************************************************************/

/*****************************************************************************
 *  app_webclient
 *  Parameters:
 *    none
 *  Returns:
 *    none
 *  Description:
 *    Run Infinite loop here.
 *    tcp web client;
 *****************************************************************************/
static void app_webclient(void *pvParameter)
{

	while (!wifi_esp32_connected())
	{
		//wait till wifi is connected;
		vTaskDelay(1000 / portTICK_RATE_MS);
	}
	// define connection parameters
	const struct addrinfo hints =
	{
			.ai_family = AF_INET,
			.ai_socktype = SOCK_STREAM,
	};

	// address info struct and receive buffer
	struct addrinfo *res;
	char recv_buf[2048];

	// resolve the IP of the target website
	int result = getaddrinfo(CONFIG_WEBSITE, "80", &hints, &res);

	if((result != 0) || (res == NULL))
	{
		ESP_LOGE(TAG, "Unable to resolve IP for target website %s", CONFIG_WEBSITE );
		while(1)
		{
			vTaskDelay(1000 / portTICK_RATE_MS);
		}
	}

	ESP_LOGI(TAG, "Target website's IP resolved" );


	// create a new socket
	int s = socket(res->ai_family, res->ai_socktype, 0);
	if(s < 0)
	{
			ESP_LOGE(TAG, "Unable to allocate a new socket" );
			while(1)
			{
				vTaskDelay(1000 / portTICK_RATE_MS);
			}
	}

	ESP_LOGI(TAG, "Socket allocated, id=%d", s);

	// connect to the specified server
	result = connect(s, res->ai_addr, res->ai_addrlen);
	if(result != 0)
	{
		ESP_LOGE(TAG, "Unable to connect to the target website %s", CONFIG_WEBSITE );
		close(s);
		while(1)
		{
			vTaskDelay(1000 / portTICK_RATE_MS);
		}
	}

	ESP_LOGI(TAG, "Connected to the target website");

	// send the request
	result = send(s, REQUEST, strlen(REQUEST), 0);

	ESP_LOGI(TAG, "sent request: %s, # of bytes: %d", REQUEST, sizeof(REQUEST));
	if(result < 0)
	{
		ESP_LOGE(TAG, "Unable to send the HTTP request" );
		close(s);
		while(1)
		{
			vTaskDelay(1000 / portTICK_RATE_MS);
		}
	}

	ESP_LOGI(TAG, "HTTP request sent");

	// print the response
	printf("HTTP response:\n");
	ESP_LOGI(TAG, "HTTP response:");
	ESP_LOGI(TAG, "--------------------------------------------------------------------------------");
	int r;
	do
	{
		bzero(recv_buf, sizeof(recv_buf));
		r = recv(s, recv_buf, sizeof(recv_buf) - 1, 0);
		ESP_LOGI(TAG, "receive # %d bytes", r);

		if(r > 0)
			ESP_LOGI(TAG, "%s", recv_buf);

	} while(r > 0);
	ESP_LOGI(TAG, "--------------------------------------------------------------------------------");

	close(s);
	ESP_LOGI(TAG, "Socket closed");

	while(1)
	{
		//stay here forever;
		vTaskDelay(1000 / portTICK_RATE_MS);
	}




}


