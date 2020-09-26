/*
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#ifndef __BLE_ESP32_H
#define __BLE_ESP32_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* Attributes State Machine */
enum
{
    IDX_SVC,      			//HRS_IDX_SVC     			//HEART RATE SERVICE

    IDX_CHAR_A,				//HRS_IDX_HR_MEAS_VAL,		//HEART RATE MEASUREMENT CHARACTERISTIC
    IDX_CHAR_VAL_A,			//HRS_IDX_HR_MEAS_VAL,		//HEART RATE MEASUREMENT VALUE
    IDX_CHAR_CFG_A,			// HRS_IDX_HR_MEAS_NTF_CFG	//HEART RATE MEASUREMENT NOTIFICATION CONFIGURATION

    IDX_CHAR_B,				// HRS_IDX_BOBY_SENSOR_LOC_CHAR	 //SENSOR LOCATION CAHRACTERISTIC
    IDX_CHAR_VAL_B,			//HRS_IDX_BOBY_SENSOR_LOC_VAL	// SENSOR LOCATION VALUE

    IDX_CHAR_C,				//HRS_IDX_HR_CTNL_PT_CHAR		//HEART RATE CONTROL POINT CHARACTERISTIC
    IDX_CHAR_VAL_C,			//HRS_IDX_HR_CTNL_PT_VAL		//CONTROL POINT VALUE

    HRS_IDX_NB,
};

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

void ble_esp32_init(void);

#endif /* __BLE_ESP32_H */
