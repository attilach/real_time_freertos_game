/*
===============================================================================
Name        : commons.h
Author      : Vincent Berset, Kevin Estalella, Fiorenzo Poroli
Year        : 2017
===============================================================================
*/

#ifndef COMMONS_H
#define COMMONS_H

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <stdbool.h>
#include "FreeRTOS.h"
#include "objects.h"
#include "queue.h"
#include "semphr.h"

static sprites_t sprites;

/**
* Macro for testing out of bound indexes
*
* @param x : (int) - the index to test
* @param min: (int) - the min value  allowed
* @param max : (int) - the max value allowed
*/
#define clamp(x, min, max) (((min) < (x)) ? (((x) < (max)) ? (x) : (max)) : (min))

/**
* Convert ms to tick
*/
#define MS_TO_TICKS(ms) ((portTickType) (ms/2))

/**
* Joystick's properties
*/
#define JOYSTICK_NORTH  (1 << 0)
#define JOYSTICK_EAST   (1 << 1)
#define JOYSTICK_SOUTH  (1 << 2)
#define JOYSTICK_WEST   (1 << 3)

/**
* Rendering info
*/
#define SCREEN_WIDTH	240
#define RENDERING_PERIOD    MS_TO_TICKS(16)

/**
* Mouse info
*/
#define MOUSE_VITALITY_MAX		75
#define MOUSE_HEIGHT    		20
#define MOUSE_WIDTH     		12
#define MOUSE_BORDER_TOP    	26
#define MOUSE_BORDER_BOTTOM     280
#define MOUSE_BORDER_LEFT       0
#define MOUSE_BORDER_RIGHT      (SCREEN_WIDTH - MOUSE_HEIGHT - 1)
#define MOUSE_PERIOD 			MS_TO_TICKS(20)

/**
* Cat info
*/
#define CAT_HEIGHT			48
#define CAT_WIDTH			24
#define CAT_MOVE			4
#define CAT_BORDER_TOP		26
#define CAT_BORDER_BOTTOM	252-CAT_HEIGHT
#define CAT_BORDER_LEFT		0
#define CAT_BORDER_RIGHT	SCREEN_WIDTH-CAT_HEIGHT-1
#define CAT_MARGE			12
#define CAT_PERIOD 			MS_TO_TICKS(50)


/**
* Queue for cat and mouse
*/
xQueueHandle queue_mouse_out;
xQueueHandle queue_mouse_in;
xQueueHandle queue_cat_out;
xQueueHandle queue_cat_in;

/**
* Queue for trace
*/
xQueueHandle queue_trace;

/**
* Queue for buffer index (DMA)
*/
xQueueHandle queue_buf_index;

/* Description: detects collisions between 2 rectangular objects. As soon as the distance between
* both objects if lower than or equal to the margin, the collision is detected.
* Parameters:
* 	obj1 and obj2: positions and directions of the objects concerned
* 	obj1_im, obj2_im: reference to the object images (containing the dimensions)
* Return: true if the collision is detected, false otherwise
*/
bool detect_collision(object_t obj1, image_t *obj1_im, object_t obj2, image_t *obj2_im, int margin);

/**
* Clear sprite pixels depending on its direction
* @param  move_dir 		direction of the object move
* @param  old_x 			old position on X
* @param  old_y 			old position on Y
* @param  move 			  number of pixels to move
* @param  width 			width to clear
* @param  height 			height to clear
*/
void clear_pix_on_same_dir(int move_dir, int old_x, int old_y, int move, int width, int height);

/**
* Clear sprite pixels on new direction
* @param  move_dir 		direction of the object move
* @param  old_x 			  old position on X
* @param  old_y 			  old position on Y
* @param  width 			  width to clear
* @param  height 			height to clear
*/
void clear_pix_on_new_dir(int mov_dir, int old_x, int old_y, int width, int height);

/**
* Init GPIO1 for joystick
*/
void init_joystick();

/**
* Get joystick value
* @return		         The direction of joystick
*/
dir_t get_joystick (void);


/**
* Initialise queue
*/
void init_queue(void);


/**
* Initialise semaphore
*/
void init_semaphore(void);

#endif //GHOSTBUSTER_CONFIG_H_H
