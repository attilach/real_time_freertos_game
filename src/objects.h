/*
* objects.h
*
*  Created on: 12 avr. 2017
*      Author: pillouxv
*/

#ifndef OBJECTS_H_
#define OBJECTS_H_

#include <stdint.h>
#include <stdbool.h>

#define TOP_POS 26

typedef enum {
	NORTH=0,
	EAST,
	SOUTH,
	WEST,
	NOTHING
} dir_t;

typedef struct {
	int x;
	int y;
} pos_t;

typedef struct {
	uint16_t *bitmap;			// index 0 is north, 1 east, etc.
	uint16_t width, height;		// of bitmap
} image_t;

typedef struct {
	image_t cat_im[4], mouse_im[4], cheese_im;
} sprites_t;

typedef struct {
	pos_t pos;
	dir_t dir;
} object_t;

typedef struct {
	object_t object;
	unsigned vitality;
	bool active;
} mouse_state_t;

typedef struct {
	object_t object;
	bool active;
} cat_state_t;

typedef enum {
	MOUSE_MARKS,
	CAT_MARKS,
	NEW_GAME,
} game_event_t;

/**
* Initialize the given mouse_state_t object.
*
* @param mouse_state
*/
void mouse_state_init(mouse_state_t *mouse_state);

/**
* Initialize the given cat_state_t object.
*
* @param cat_state
*/
void cat_state_init(cat_state_t *cat_state);

/**
* Initialize the given cat object.
*
* @param cat_state
*/
void cat_init(object_t *cat_state);

#endif /* OBJECTS_H_ */
