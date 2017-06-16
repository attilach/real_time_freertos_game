/*
===============================================================================
Name        : mouse.h
Author      : Vincent Berset, Kevin Estalella, Fiorenzo Poroli
Year        : 2017
===============================================================================
*/

#ifndef MOUSE_H
#define MOUSE_H


#include "objects.h"


/**
* Handle mouse's moves
*
* @param params	Unused parameter
*/
void task_mouse(void *params);

/**
* Get mouse speed from it's own vitality
* @param  int		 	mouse vitality
* @return	int         mouse speed
*/

int speed_mouse(int vitality);

/**
* Change mouse position function its direction
* @param mouse			Struct who contain mouse information
* @param direction		Direction of the move
*/
void mouse_move(mouse_state_t *mouse, dir_t direction);

/**
* Read joystick value
* @return joystick state
*/
unsigned read_joystick();


/**
* Convert Joystick position to direction
* @return dir 		direction between 0 (NORTH) and 3 (WEST)
*/
dir_t from_joystick(unsigned dir);

void task_mouse(void *params);


#endif
