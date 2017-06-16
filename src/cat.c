/*
===============================================================================
Name        : cat.c
Author      : Vincent Berset, Kevin Estalella, Fiorenzo Poroli
Year        : 2017
===============================================================================
*/

#include "FreeRTOS.h"
#include <stdbool.h>
#include <stdlib.h>
#include <cat.h>
#include "lcd.h"
#include "debug.h"
#include "dtmf_detection.h"

#include <math.h>

#include "objects.h"
#include "commons.h"


void task_cat (void *params)
{
	portTickType last_exec= xTaskGetTickCount();

	object_t cat, cat_old;
	cat_state_t state;
	game_event_t event;

	cat.dir = SOUTH;
	cat.pos.x = 110;
	cat.pos.y = 30;

	cat_old.pos.x = cat.pos.x;
	cat_old.pos.y = cat.pos.y;
	cat_old.dir = cat.dir;


	cat_state_init(&state);


	while(true) {

		if (xQueueReceive(queue_cat_in, (void *)&event, 0) == pdTRUE) {
			vTaskDelayUntil(&last_exec, MS_TO_TICKS(1000));
			cat_init(&cat);
		}

		cat.dir = get_cat_joystick();

		if (cat.dir != NOTHING) {
			if (cat.dir != cat_old.dir) {

				if ((((cat_old.dir==SOUTH) || (cat_old.dir==NORTH)) && ((cat.dir==EAST) ||(cat.dir==WEST)))) {
					cat.pos.x = clamp(cat.pos.x-CAT_MARGE,CAT_BORDER_LEFT,CAT_BORDER_RIGHT);
					cat.pos.y = clamp(cat.pos.y+CAT_MARGE,CAT_BORDER_TOP,CAT_BORDER_BOTTOM+CAT_WIDTH);
				}
				else if ((((cat_old.dir==EAST) || (cat_old.dir==WEST)) && ((cat.dir==SOUTH) ||(cat.dir==NORTH)))) {
					cat.pos.x = clamp(cat.pos.x+CAT_MARGE,CAT_BORDER_LEFT,CAT_BORDER_RIGHT+CAT_WIDTH);
					cat.pos.y = clamp(cat.pos.y-CAT_MARGE,CAT_BORDER_TOP,CAT_BORDER_BOTTOM);
				}

			} else {
				switch (cat.dir) {
					case NORTH :
					cat.pos.y = clamp(cat.pos.y - CAT_MOVE, CAT_BORDER_TOP, CAT_BORDER_BOTTOM);
					break;
					case EAST :
					cat.pos.x = clamp(cat.pos.x + CAT_MOVE, CAT_BORDER_LEFT, CAT_BORDER_RIGHT);
					break;
					case SOUTH :
					cat.pos.y = clamp(cat.pos.y + CAT_MOVE, CAT_BORDER_TOP, CAT_BORDER_BOTTOM);
					break;
					case WEST :
					cat.pos.x = clamp(cat.pos.x - CAT_MOVE, CAT_BORDER_LEFT, CAT_BORDER_RIGHT);
					break;
					default :
					break;
				}
			}


			state.object = cat;
			if (xQueueSendToBack(queue_cat_out, (void*) &state, 0) != pdPASS) {
				EXIT("Unable to push cat state to the dedicated queue");
			}

			cat_old.pos.x = cat.pos.x;
			cat_old.pos.y = cat.pos.y;
			cat_old.dir = cat.dir;
		}

		vTaskDelayUntil(&last_exec, CAT_PERIOD);
	}
}
