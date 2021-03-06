/**
 * @file  nvs_esp32.h
 * @brief application interface to nvs flash
 *
 */

#ifndef __NVS_ESP32_H
#define __NVS_ESP32_H

/*****************************************************************************
 *	Public Includes
 *****************************************************************************/

/*****************************************************************************
 *	Public External References
 *****************************************************************************/

/*****************************************************************************
 *	Public Defines & Macros
 *****************************************************************************/

/*****************************************************************************
 *	Public Typedefs & Enumerations
 *****************************************************************************/

/*****************************************************************************
 *	Public Function Prototypes
 *****************************************************************************/

void nvs_esp32_init(void);
bool nvs_esp32_write(char* key, uint8_t* data, uint32_t size);
uint32_t nvs_esp32_get_size(char* key );
bool nvs_esp32_read(char* key, uint8_t* buff, uint32_t* size );

#endif /* __NVS_ESP32_H */
