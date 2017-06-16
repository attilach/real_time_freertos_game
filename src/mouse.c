/*
===============================================================================
Name        : mouse.c
Author      : Vincent Berset, Kevin Estalella, Fiorenzo Poroli
Year        : 2017
===============================================================================
*/

#include "FreeRTOS.h"
#include <stdbool.h>
#include <stdlib.h>
#include "mouse.h"
#include "lcd.h"
#include "debug.h"

#include "objects.h"
#include "commons.h"
#include "custom_rand.h"


/**
* Get mouse speed from it's own vitality
* @param  int		 	mouse vitality
* @return	int         mouse speed
*/
int speed_mouse(int vitality) {
	return ((3 * vitality) + (MOUSE_VITALITY_MAX / 2)) / MOUSE_VITALITY_MAX + 1;
}


void mouse_move(mouse_state_t *mouse, dir_t direction) {
	int old_x, old_y;
	int stride = speed_mouse(mouse->vitality); // The stride is computed each time based on the mouse's vitality

	mouse->object.dir = direction;
	old_x = mouse->object.pos.x;
	old_y = mouse->object.pos.y;

	switch (direction) {
		case NORTH:
		stride = -stride;
		case SOUTH:
		mouse->object.pos.y = clamp(mouse->object.pos.y + stride,
			MOUSE_BORDER_TOP, MOUSE_BORDER_BOTTOM);
			break;

			case WEST:
			stride = -stride;
			case EAST:
			mouse->object.pos.x = clamp(mouse->object.pos.x + stride,
				MOUSE_BORDER_LEFT, MOUSE_BORDER_RIGHT);
				break;
				case NOTHING:
				//ignore
				break;
			}

			if (old_x != mouse->object.pos.x || old_y != mouse->object.pos.y) {
				if (mouse->vitality > 0)
				mouse->vitality--;
			} else if (mouse->vitality < MOUSE_VITALITY_MAX) {
				mouse->vitality++;
			}
		}


		unsigned read_joystick() {
			return (~(LPC_GPIO1->FIOPIN >> 20)) & 0xF;
		}


		dir_t from_joystick(unsigned dir) {
			switch (dir) {
				case JOYSTICK_NORTH:
				return NORTH;
				case JOYSTICK_EAST:
				return EAST;
				case JOYSTICK_SOUTH:
				return SOUTH;
				case JOYSTICK_WEST:
				return WEST;
				default:
				return NOTHING;
			}
		}

		void task_mouse(void *params) {
			portTickType last_exec;
			mouse_state_t mouse;
			bool has_changed;
			unsigned joystick_state;

			mouse_state_init(&mouse);

			last_exec = xTaskGetTickCount();

			game_event_t event;

			while (true) {
				if (xQueueReceive(queue_mouse_in, (void *)&event, 0) == pdTRUE) {
					vTaskDelayUntil(&last_exec, MS_TO_TICKS(1000));
					mouse_state_init(&mouse);
				}

				if (mouse.active) {
					joystick_state = read_joystick();
					has_changed = false;

					if (joystick_state != 0) {
						mouse_move(&mouse, from_joystick(joystick_state));
						has_changed = true;
					} else {
						if (mouse.vitality < MOUSE_VITALITY_MAX) {
							mouse.vitality++;
							has_changed = true;
						}
					}

					if (has_changed) {
						if (xQueueSendToBack(queue_mouse_out, (void * )&mouse,
						0) != pdPASS)
						EXIT("Unable to push mouse state to the dedicated queue");
					}
				}

				vTaskDelayUntil(&last_exec, MOUSE_PERIOD);
			}
		}
