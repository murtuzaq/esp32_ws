#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_freertos_hooks.h"
#include "freertos/semphr.h"
#include "esp_system.h"
#include "esp_log.h"
#include "app_gui.h"
#include "gui_hw.h"

/* Littlevgl specific */
#include "lvgl/lvgl.h"
#include "lvgl_helpers.h"

#include "lv_examples/src/lv_demo_widgets/lv_demo_widgets.h"
/*****************************************************************************
 *	Private External References
 *****************************************************************************/

LV_FONT_DECLARE(lv_font_montserrat_16)
/*****************************************************************************
 *	Private Defines & Macros
 *****************************************************************************/

/*****************************************************************************
 *	Private Typedefs & Enumerations
 *****************************************************************************/

/*****************************************************************************
 *	Private Variables
 *****************************************************************************/
static struct  {
	bool exit;
} gui_terminal;

lv_obj_t* obj_scrn;
lv_style_t style_scrn;
lv_style_t style_text;
lv_obj_t* obj_label;
/*****************************************************************************
 *	Private Function Prototypes
 *****************************************************************************/

static void terminal_receive_message(void)
{
	gui_message_t pxRxedMsg;

	if (app_gui_receive_message(&pxRxedMsg))
	{
		if(pxRxedMsg.view != GUI_VIEW_TERMINAL)
		{
			ESP_LOGI(__FUNCTION__, "current view changed, exit");
			gui_terminal.exit = true;
		}
		else
		{
			if(pxRxedMsg.cmd == GUI_CMD_TEXT)
			{
				ESP_LOGI(__FUNCTION__, "cmd to update text on screen");
				lv_obj_add_style(obj_label, LV_OBJ_PART_MAIN, &style_text);
				lv_label_set_align(obj_label, LV_LABEL_ALIGN_CENTER);
				lv_label_set_text(obj_label, pxRxedMsg.msg);

				lv_obj_set_width(obj_label, 150);
				lv_obj_align(obj_label, NULL, LV_ALIGN_CENTER, 0, 0);

			}
		}
	}


}
/*****************************************************************************
 *	Public Functions
 *****************************************************************************/

void gui_terminal_exe(void)
{
	ESP_LOGI(__FUNCTION__, "enter");

	gui_terminal.exit = false;

	//lv_demo_widgets();

	obj_scrn = lv_obj_create(NULL, NULL);

	lv_style_init(&style_scrn);

	lv_style_set_bg_color(&style_scrn, LV_STATE_DEFAULT, LV_COLOR_BLACK);

	lv_obj_set_size(obj_scrn, LV_HOR_RES, LV_VER_RES);
	lv_obj_add_style(obj_scrn, LV_OBJ_PART_MAIN, &style_scrn);


	lv_style_init(&style_text);
	lv_style_set_text_font(&style_text, LV_STATE_DEFAULT, &lv_font_montserrat_16);
	lv_style_set_text_color(&style_text, LV_STATE_DEFAULT, LV_COLOR_WHITE);

	obj_label = lv_label_create(obj_scrn, NULL);


	lv_obj_add_style(obj_label, LV_OBJ_PART_MAIN, &style_text);
	lv_label_set_align(obj_label, LV_LABEL_ALIGN_CENTER);
	lv_label_set_text(obj_label, "Hello World");

	lv_obj_set_width(obj_label, 150);
	lv_obj_align(obj_label, NULL, LV_ALIGN_CENTER, 0, 0);

	lv_scr_load(obj_scrn);

	while(!gui_terminal.exit)
	{

		terminal_receive_message();

	}

	ESP_LOGI(__FUNCTION__, "exit");
}


bool gui_terminal_update_text(char* strmsg)
{
	if (strmsg == NULL)
		return(false);

	gui_message_t TxdMsg;
	TxdMsg.view = GUI_VIEW_TERMINAL;
	TxdMsg.cmd = GUI_CMD_TEXT;

	bzero(TxdMsg.msg, sizeof(TxdMsg.msg) );
	strcpy(TxdMsg.msg, strmsg);

	bool status = app_gui_message_send(TxdMsg);

	return(status);
}

/*****************************************************************************
 *	Private Functions
 *****************************************************************************/

