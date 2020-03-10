
#include "task_hello.h"
#include "task_blinky.h"


void app_main(void)
{
	task_hello_init();
	task_blinky_init();
}

