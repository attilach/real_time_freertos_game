#ifndef RENDERING_H
#define RENDERING_H

#include "objects.h"


typedef struct {
  mouse_state_t mouse;
  cat_state_t cat;
  bool game_started;
} world_t;


/**
* Initialize the given world_t object
* @param world
*/
void world_init(world_t* world);

/**
* Display rendering task.
*
* @param params
*/
void task_rendering(void *params);

/**
* Loads sprites into the specified memory location.
*
* @param sprites
*/
void load_sprites(sprites_t *sprites);

/**
* Draw the playground.
*
* @param sprites Pointer to the sprites
*/
void draw_playground(sprites_t *sprites);

/**
* Draw the gauge at 100%
*/
void draw_full_gauge(void);

/**
* Draw the mouse and it's gauge.
*
* @param  sprites 		sprites of the mouse
* @param  previous 	previous mouse state
* @param  sprites 		current mouse state
*/
void draw_mouse(sprites_t *sprites, mouse_state_t *previous, mouse_state_t *mouse);

/**
* Draw the cat
*
* @param  sprites 		sprites of the mouse
* @param  previous 	previous mouse state
* @param  sprites 		current mouse state
*/
void draw_cat(sprites_t *sprites, cat_state_t *previous, cat_state_t *cat);

/**
* Draw the specified message.

* @param line1     First line of text
* @param color1    First line color
* @param line2     Second line of text
* @param color2    Second line color
*/
void draw_message(char *line1, int color1, char *line2, int color2);

/**
* Erase the messages write with draw_message.
*/
void erase_message();

/**
* Erase the specified object.
*
* @param object
* @param images
*/
void erase_object(object_t *object, image_t images[4]);

#endif //RENDERING_H
