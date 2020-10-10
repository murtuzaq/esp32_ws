# Edit following two lines to set component requirements (see docs)
set(COMPONENT_REQUIRES )
set(COMPONENT_PRIV_REQUIRES )

set(COMPONENT_SRCS "main.c" 
                   "app_control.c" 
                   "app_gui.c"
                   "app_webclient.c"
                   "gui_hw.c"
                   "gui_terminal.c"
                   "control_startup.c"	
                   "nvs_esp32.c"
                   "wifi_esp32.c"
                   "ble_esp32.c")
set(COMPONENT_ADD_INCLUDEDIRS "")

register_component()
