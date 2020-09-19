# Edit following two lines to set component requirements (see docs)
set(COMPONENT_REQUIRES )
set(COMPONENT_PRIV_REQUIRES )

set(COMPONENT_SRCS "main.c" 
                   "app_hello.c"
                   "app_gui.c" 
                   "gui_hw.c"	)
set(COMPONENT_ADD_INCLUDEDIRS "")

register_component()
