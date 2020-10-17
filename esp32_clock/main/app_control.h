/**
 * @file  app_control.h
 * @brief main control system;
 *
 */

#ifndef __APP_CONTROL_H
#define __APP_CONTROL_H

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
typedef enum{
	CONTROL_MODE_STARTUP 	= 0,
	CONTROL_MODE_TOTAL		= 1,


}app_control_mode_t;

typedef enum {
	CONTROL_FROM_GUI		= 0,
	CONTROL_FROM_BLE		= 1,
	CONTROL_FROM_WEBCLIENT	= 2,
}control_from_t;

typedef enum {
	CONTROL_CMD_WIFI		= 0,
	CONTROL_CMD_WEBCLIENT	= 1,
}control_cmd_t;

typedef struct {
	char ssid[32];
	char password[32];
}control_cmd_wifi_t;
/*
 * intertask communication to be received
 * by control task;
 */
typedef struct
{
	control_from_t 	from;
	control_cmd_t 	cmd;
	union{
		control_cmd_wifi_t	wifi_cmd;
	};
} control_message_t;

/*****************************************************************************
 *	Public Function Prototypes
 *****************************************************************************/

void control_init(void);

#endif /* __APP_HELLO_H */
