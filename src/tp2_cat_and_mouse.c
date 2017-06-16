/**
* Name        : tp2_cat_and_mouse.c
* Date        : 5.5.2017
* Description : cat and mouse game with 2 mylab2 boards. FreeRTOS
*               and LCD display used.
*/

#include <cr_section_macros.h>
#include <dtmf_detection.h>
#include <ethmac.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "lcd.h"
#include "debug.h"
#include "trace_mgt.h"
#include "objects.h"
#include "traces_ref.h"

#include "commons.h"
#include "mouse.h"
#include "cat.h"
#include "rendering.h"
#include "custom_rand.h"

#include "dtmf_detection.h"

const unsigned portBASE_TYPE RENDERING_PRIORITY = configMAX_PRIORITIES - 1;
const unsigned portBASE_TYPE MOUSE_PRIORITY = configMAX_PRIORITIES - 2;
const unsigned portBASE_TYPE CAT_PRIORITY = configMAX_PRIORITIES - 2;

int main(void)
{
	ethernet_power_down();	// remove potential noise on ADC due to Ethernet module

    init_rnd32(4);
    init_lcd();
	init_joystick();
	init_queue();
	init_all_module_dtmf();
	my_init_trace();
	init_dtmf();

	load_sprites(&sprites);

	if (xTaskCreate(task_rendering, "display", configMINIMAL_STACK_SIZE, (void *)&sprites, RENDERING_PRIORITY, NULL) != pdPASS)
	EXIT("Unable to start `rendering` task");

	if (xTaskCreate(task_mouse, "mouse", configMINIMAL_STACK_SIZE, NULL, MOUSE_PRIORITY, NULL) != pdPASS)
	EXIT("Unable to start `mouse` task");

	if (xTaskCreate(task_cat, "cat", configMINIMAL_STACK_SIZE, NULL, CAT_PRIORITY, NULL) != pdPASS)
	EXIT("Unable to start `cat` task");


	vTaskStartScheduler();
}
