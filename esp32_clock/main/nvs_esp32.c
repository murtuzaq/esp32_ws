/**
 * @file nvs_esp32.c
 * @brief manages application use of nvs flash
 *
 */

/*****************************************************************************
 *	Private Includes
 *****************************************************************************/
#include "esp_log.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_freertos_hooks.h"
#include "freertos/semphr.h"
/*****************************************************************************
 *	Private External References
 *****************************************************************************/

/*****************************************************************************
 *	Private Defines & Macros
 *****************************************************************************/
#define STORAGE_NAMESPACE "storage"

/*****************************************************************************
 *	Private Typedefs & Enumerations
 *****************************************************************************/

/*****************************************************************************
 *	Private Variables
 *****************************************************************************/
static struct {
	SemaphoreHandle_t mutex;
	bool initilized;
}nvs;
/*****************************************************************************
 *	Private Function Prototypes
 *****************************************************************************/

/*****************************************************************************
 *	Public Functions
 *****************************************************************************/
/*****************************************************************************
 *  nvs_esp32_init
 *  Parameters:
 *    none
 *  Returns:
 *    none
 *  Description:
 *    Initialize module here
 *****************************************************************************/
void nvs_esp32_init(void)
{
    esp_err_t ret;

    ret = nvs_flash_init();

    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
      ESP_LOGW(__FUNCTION__, "error, nvs flash erase");

      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }

    ESP_ERROR_CHECK(ret);

    /* Create a mutex type semaphore. */
      nvs.mutex = xSemaphoreCreateMutex();

      if( nvs.mutex == NULL )
      {
          /* The semaphore was not created successfully*/
    	  ESP_LOGE(__FUNCTION__, "mutex not created");
    	  abort();
      }

    ESP_LOGI(__FUNCTION__, "init complete");
    nvs.initilized = true;


}
/*****************************************************************************
 *  nvs_esp32_write
 *  Parameters:
 *    none
 *  Returns:
 *    none
 *  Description:
 *    store data into nvs by key;
 *****************************************************************************/
bool nvs_esp32_write(char* key, uint8_t* data, uint32_t size)
{
    bool is_success = false;

    if( xSemaphoreTake( nvs.mutex, ( TickType_t ) 10 ) == pdTRUE )
    {
        nvs_handle_t my_handle;
    	// Open
    	if ( nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle) == ESP_OK)
    	{
    		//Write
    		if (nvs_set_blob(my_handle, key, data, size) == ESP_OK)
    		{
    			// Commit
    			if (nvs_commit(my_handle) == ESP_OK)
    			{
    				is_success = true;
    			}
    			else
    				ESP_LOGE(__FUNCTION__, "failed to nvs_commit");
    		}
    		else
    			ESP_LOGE(__FUNCTION__, "failed to nvs_set_blob");

			// Close
			nvs_close(my_handle);
    	}
    	else
    		ESP_LOGE(__FUNCTION__, "failed to nvs_open");
    }
    else
    	ESP_LOGE(__FUNCTION__, "failed to grab mutex");

    return is_success;
}


/*****************************************************************************
 *  nvs_esp32_get_size
 *  Parameters:
 *    none
 *  Returns:
 *    none
 *  Description:
 *    get the size of data in key object. used to get buffer size before doing
 *    a nvs_esp32_read.  (i.e. application will malloc and free data for read)
 *
 *    for example:
 *    #define WIFI_SSID = "WIFI_SSID"
 *    uint32_t size = nvs_esp32_get_size(WIFI_SSID);
 *    if (size)
 *    {
 *    	uint8_t* read_buff = malloc(size);
 *    	nvs_esp32_read(WIFI_SSID, read_buff, size );
 *    }
 *
 *****************************************************************************/
uint32_t nvs_esp32_get_size(char* key )
{
    nvs_handle_t my_handle;
    esp_err_t err;
    uint32_t get_size = 0;

    if( xSemaphoreTake( nvs.mutex, ( TickType_t ) 10 ) == pdTRUE )
    {
    	// Open
    	if (nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle) == ESP_OK)
    	{
    		//Read
    		// Read the size of memory space required for blob
    		size_t data_size = 0;  // value will default to 0, if not set yet in NVS
    		err =  nvs_get_blob(my_handle, key, NULL, &data_size);
    		if (err == ESP_OK || err == ESP_ERR_NVS_NOT_FOUND)
    			get_size = data_size;
    		else
    			ESP_LOGE(__FUNCTION__, "failed to nvs_get_blob");

    	    // Close
    	    nvs_close(my_handle);
    	}
    	else
    		ESP_LOGE(__FUNCTION__, "failed to nvs_open");
    }
    else
    	ESP_LOGE(__FUNCTION__, "failed to grab mutex");


    return(get_size);
}

/*****************************************************************************
 *  nvs_esp32_read
 *  Parameters:
 *    none
 *  Returns:
 *    none
 *  Description:
 *    Initialize module here
 *****************************************************************************/
bool nvs_esp32_read(char* key, uint8_t* buff, uint32_t* size )
{
    bool is_success = false;

    if( xSemaphoreTake( nvs.mutex, ( TickType_t ) 10 ) == pdTRUE )
    {
    	nvs_handle_t my_handle;

    	// Open
    	if (nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle) == ESP_OK)
    	{
    		//Read
    		if (nvs_get_blob(my_handle, key, buff, size) == ESP_OK)
    			is_success = true;
    		else
    			ESP_LOGE(__FUNCTION__, "failed to nvs_get_blob");

    		// Close
    		nvs_close(my_handle);
    	}
    	else
    		ESP_LOGE(__FUNCTION__, "failed to nvs_open");
    }
    else
    	ESP_LOGE(__FUNCTION__, "failed to grab mutex");

    return(is_success);
}


/*****************************************************************************
 *	Private Functions
 *****************************************************************************/


