/*
===============================================================================
Name        : commons.c
Author      : Vincent Berset, Kevin Estalella, Fiorenzo Poroli
Year        : 2017
===============================================================================
*/

#include <stdbool.h>
#include <objects.h>
#include "commons.h"
#include "lcd.h"
#include "debug.h"
#include "traces_ref.h"
#include "semphr.h"


bool detect_collision(object_t obj1, image_t *obj1_im, object_t obj2, image_t *obj2_im, int margin)
{
	return (
		((obj1.pos.x - obj2.pos.x) < (int)(obj2_im[obj2.dir].width + margin - 1))
		&& ((obj2.pos.x - obj1.pos.x) < (int)(obj1_im[obj1.dir].width + margin - 1))
		&& ((obj2.pos.y - obj1.pos.y) < (int)(obj1_im[obj1.dir].height + margin - 1))
		&& ((obj1.pos.y - obj2.pos.y) < (int)(obj2_im[obj2.dir].height + margin - 1))
	);
}

void init_joystick() {
	LPC_GPIO1->FIODIR &= ~JOYSTICK_EAST & ~JOYSTICK_WEST & ~JOYSTICK_NORTH & ~JOYSTICK_SOUTH; // init joystick for input
}


dir_t get_joystick (void) {
	if (!(LPC_GPIO1->FIOPIN & JOYSTICK_NORTH))
	return NORTH;
	else if (!(LPC_GPIO1->FIOPIN & JOYSTICK_EAST))
	return EAST;
	else if (!(LPC_GPIO1->FIOPIN & JOYSTICK_SOUTH))
	return SOUTH;
	else if (!(LPC_GPIO1->FIOPIN & JOYSTICK_WEST))
	return WEST;
	return NOTHING;
}


void init_queue(void)
{
	queue_mouse_out = xQueueCreate(5, sizeof(mouse_state_t));
	if(queue_mouse_out == NULL)
	EXIT("Impossible to create queue mouse_out !");

	queue_mouse_in = xQueueCreate(5, sizeof(game_event_t));
	if (queue_mouse_in == NULL)
	EXIT("Impossible to create queue mouse_in !");

	queue_cat_out = xQueueCreate(5, sizeof(object_t));
	if (queue_cat_out == NULL)
	EXIT("Impossible to create queue cat_out !");

	queue_cat_in = xQueueCreate(5, sizeof(game_event_t));
	if (queue_cat_in == NULL)
	EXIT("Impossible to create queue cat_in !");

	queue_trace = xQueueCreate(100, sizeof(trace_t));
	if (queue_trace == NULL)
	EXIT("Impossible to create queue trace !");

	queue_buf_index = xQueueCreate(50, sizeof(int));
	if (queue_buf_index == NULL)
	EXIT("Impossible to create queue buffer index !");
}


void clear_pix_on_same_dir(int move_dir, int old_x, int old_y, int move, int width, int height) {

	switch (move_dir) {
		case NORTH:
		lcd_filled_rectangle(old_x, old_y+height-move, old_x+width-1, old_y+height, LCD_WHITE);
		break;

		case SOUTH:
		lcd_filled_rectangle(old_x, old_y, old_x+width-1, old_y+move, LCD_WHITE);
		break;

		case WEST:
		lcd_filled_rectangle(old_x+height-move, old_y, old_x+height, old_y+width-1, LCD_WHITE);
		break;

		case EAST:
		lcd_filled_rectangle(old_x, old_y, old_x+move, old_y+width-1, LCD_WHITE);
		break;

		default:
		EXIT("Not recognized direction!");
		break;
	}
}

void clear_pix_on_new_dir(int mov_dir, int old_x, int old_y, int width, int height) {

	if (mov_dir == EAST || mov_dir == WEST) {
		lcd_filled_rectangle(old_x, old_y+width, old_x+width, old_y+height, LCD_WHITE);
	} else if(mov_dir == NORTH || mov_dir == SOUTH) {
		lcd_filled_rectangle(old_x+width, old_y, old_x+height, old_y+width, LCD_WHITE);
	} else {
		EXIT("Not recognized direction!");
	}
}
