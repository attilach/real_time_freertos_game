#include "rendering.h"

// FreeRTOS headers
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

// Board utilities headers
#include "lcd.h"
#include "debug.h"

// Project headers
#include "commons.h"
#include "objects.h"


void world_init(world_t* world)
{
  mouse_state_init(&world->mouse);
  cat_state_init(&world->cat);
  world->game_started = false;
}

void task_rendering(void *params)
{
  portTickType last_wakeup;

  sprites_t *sprites = (sprites_t *)params;

  world_t world;

  draw_playground(sprites);

  // Initialize the `last_wakeup` with the current tick count
  last_wakeup = xTaskGetTickCount ();

  world_init(&world);

  bool mouse_has_moved = false;
  bool cat_has_moved = false;
  mouse_state_t new_mouse_state;
  cat_state_t new_cat_state;

  unsigned cat_score = 0;
  unsigned mouse_score = 0;

  draw_message("Move to start a new game", LCD_BLUE, NULL, LCD_BLACK);

  for (;;) {
    // Schedule the next execution
    vTaskDelayUntil(&last_wakeup, RENDERING_PERIOD);

    // Get event from the mouse
    mouse_has_moved = xQueueReceive(queue_mouse_out, (void *)&new_mouse_state, 0) == pdTRUE;
    if (!mouse_has_moved) {
      new_mouse_state = world.mouse;
    }

    // Get event from the cat
    cat_has_moved = xQueueReceive(queue_cat_out, (void *)&new_cat_state, 0) == pdTRUE;
    if (!cat_has_moved) {
      new_cat_state = world.cat;
    }

    // Start a new party on move of cat or mouse
    if (!world.game_started && (cat_has_moved || mouse_has_moved)) {
      erase_message();
      world.game_started = true;

      if (mouse_has_moved) {
        // force cat rendering
        cat_has_moved = true;
        world.cat.object.dir = NORTH;
        cat_state_init(&new_cat_state);
      }

      if (cat_has_moved) {
        mouse_state_init(&world.mouse);
      }
    }

    // Check collision
    if (world.game_started && (cat_has_moved || mouse_has_moved)) {

      if (detect_collision(new_mouse_state.object, sprites->mouse_im, new_cat_state.object, sprites->cat_im, 0)) {
        game_event_t event = CAT_MARKS;

        if (xQueueSendToBack(queue_mouse_in, (void *)&event, 0) != pdPASS)
        EXIT("Unable to push game event to the mouse queue");

        if (xQueueSendToBack(queue_cat_in, (void *)&event, 0) != pdPASS)
        EXIT("Unable to push game event to the cat queue");

        cat_score++;

        draw_full_gauge();
        erase_object(&new_cat_state.object, sprites->cat_im);
        erase_object(&world.cat.object, sprites->cat_im);
        erase_object(&new_mouse_state.object, sprites->mouse_im);
        erase_object(&world.mouse.object, sprites->mouse_im);
        if (cat_score == 2) {
          draw_message("Cat wins !", LCD_GREEN, "Move to start a new game", LCD_BLUE);
          cat_score = 0;
        } else {
          draw_message("Cat scores !", LCD_GREEN, "Move to start a new game", LCD_BLUE);
        }

        world.game_started = false;
        continue;
      }
    }

    if (mouse_has_moved) {
      // Check cheese
      if (new_mouse_state.object.pos.y == MOUSE_BORDER_TOP) {
        game_event_t event = MOUSE_MARKS;

        if (xQueueSendToBack(queue_mouse_in, (void *)&event, 0) != pdPASS)
        EXIT("Unable to push game event to the mouse queue");

        if (xQueueSendToBack(queue_cat_in, (void *)&event, 0) != pdPASS)
        EXIT("Unable to push game event to the cat queue");

        mouse_score++;

        draw_full_gauge();
        erase_object(&new_cat_state.object, sprites->cat_im);
        erase_object(&world.cat.object, sprites->cat_im);
        erase_object(&new_mouse_state.object, sprites->mouse_im);
        erase_object(&world.mouse.object, sprites->mouse_im);
        if (mouse_score == 2) {
          draw_message("Mouse wins !", LCD_GREEN, "Move to start a new game", LCD_BLUE);
          mouse_score = 0;
        } else {
          draw_message("Mouse scores !", LCD_GREEN, "Move to start a new game", LCD_BLUE);
        }

        world.game_started = false;
        continue;
        // Otherwise simply draw
      } else {
        draw_mouse(sprites, &world.mouse, &new_mouse_state);
      }

      world.mouse = new_mouse_state;
    }

    if (cat_has_moved) {
      draw_cat(sprites, &world.cat, &new_cat_state);
      world.cat = new_cat_state;
    }
  }
}


void load_sprites(sprites_t *sprites)
{
  unsigned i;

  // Loads cheese's sprite
  if ((sprites->cheese_im.bitmap=read_bmp_file("cheese 32x26.bmp", &sprites->cheese_im.width, &sprites->cheese_im.height)) == NULL)
  EXIT("Not enough space to create image!");

  // Loads cat's sprites
  sprites->cat_im[0].bitmap=read_bmp_file("cat 24x48 up.bmp", &sprites->cat_im[0].width, &sprites->cat_im[0].height);
  sprites->cat_im[1].bitmap=read_bmp_file("cat 24x48 right.bmp", &sprites->cat_im[1].width, &sprites->cat_im[1].height);
  sprites->cat_im[2].bitmap=read_bmp_file("cat 24x48 down.bmp", &sprites->cat_im[2].width, &sprites->cat_im[2].height);
  sprites->cat_im[3].bitmap=read_bmp_file("cat 24x48 left.bmp", &sprites->cat_im[3].width, &sprites->cat_im[3].height);
  for (i=0; i<4; i++)
  if (sprites->cat_im[i].bitmap==NULL)
  EXIT("Not enough space to create image!");

  // Loads mouse's sprites
  sprites->mouse_im[0].bitmap=read_bmp_file("mouse 12x20 up.bmp", &sprites->mouse_im[0].width, &sprites->mouse_im[0].height);
  sprites->mouse_im[1].bitmap=read_bmp_file("mouse 12x20 right.bmp", &sprites->mouse_im[1].width, &sprites->mouse_im[1].height);
  sprites->mouse_im[2].bitmap=read_bmp_file("mouse 12x20 down.bmp", &sprites->mouse_im[2].width, &sprites->mouse_im[2].height);
  sprites->mouse_im[3].bitmap=read_bmp_file("mouse 12x20 left.bmp", &sprites->mouse_im[3].width, &sprites->mouse_im[3].height);
  for (i=0; i<4; i++)
  if (sprites->mouse_im[i].bitmap==NULL)
  EXIT("Not enough space to create image!");
}


void draw_playground(sprites_t *sprites)
{
  unsigned i;

  // Clear the screen
  clear_screen(LCD_WHITE);

  for (i = 0; i < 3; i++)
  {
    // Draw cheeses
    display_bitmap16(sprites->cheese_im.bitmap, 28+80*i, 0, sprites->cheese_im.width, sprites->cheese_im.height);

    // Draw mouse's pits
    lcd_filled_rectangle(30+80*i, 313, 30+12+80*i, 319, LCD_BLACK);
    lcd_filled_circle(30+6+80*i, 313, 6, LCD_BLACK);
  }

  draw_full_gauge();
}

void draw_full_gauge(void)
{
  lcd_empty_rectangle(219, 299, 233, 319, LCD_BLACK);
  lcd_filled_rectangle(220, 300, 232, 318, LCD_GREEN);
}


void draw_mouse(sprites_t *sprites, mouse_state_t *previous, mouse_state_t *mouse)
{
  dir_t dir = mouse->object.dir;
  int mouse_x = mouse->object.pos.x;
  int mouse_y = mouse->object.pos.y;

  // Compute gauge's levels
  int previous_level = ((int)previous->vitality) >> 2;
  int new_level = ((int)mouse->vitality) >> 2;

  // Levels' delta
  int dlevel = new_level - previous_level;

  int gauge_top = 318 - previous_level;

  if (dlevel > 0) {  // new level is higher
    // draw needed line
    lcd_filled_rectangle(220, gauge_top - dlevel, 232, gauge_top, LCD_GREEN);
  } else if (dlevel < 0){  // new level is lower
    // delete needed line
    lcd_filled_rectangle(220, gauge_top, 232, gauge_top - dlevel, LCD_WHITE);
  }

  lcd_empty_rectangle(219, 299, 233, 319, LCD_BLACK);

  // X and Y deltas
  int dx = mouse_x - previous->object.pos.x;
  int dy = mouse_y - previous->object.pos.y;

  // Redraw the mouse only if it has moved
  if (dx != 0 || dy != 0 || dir != previous->object.dir) {
    // Erase old mouse parts
    if ((dx != 0 || dy != 0) && dir != previous->object.dir) {
      lcd_filled_rectangle(
        previous->object.pos.x,
        previous->object.pos.y,
        previous->object.pos.x + sprites->mouse_im[previous->object.dir].width,
        previous->object.pos.y + sprites->mouse_im[previous->object.dir].height,
        LCD_WHITE
      );
    } else if (dx > 0) {
      lcd_filled_rectangle(mouse_x - dx, mouse_y, mouse_x, mouse_y + sprites->mouse_im[dir].height, LCD_WHITE);
    } else if (dx < 0) {
      int x = mouse_x + sprites->mouse_im[dir].width;
      lcd_filled_rectangle(x, mouse_y, x - dx, mouse_y + sprites->mouse_im[dir].height, LCD_WHITE);
    } else if (dy > 0) {
      lcd_filled_rectangle(mouse_x, mouse_y - dy, mouse_x + sprites->mouse_im[dir].width, mouse_y, LCD_WHITE);
    } else if (dy < 0) {
      int y = mouse_y + sprites->mouse_im[dir].height;
      lcd_filled_rectangle(mouse_x, y, mouse_x + sprites->mouse_im[dir].width, y - dy, LCD_WHITE);
    } else if (dir == NORTH || dir == SOUTH) {
      lcd_filled_rectangle(
        previous->object.pos.x + sprites->mouse_im[dir].width,
        previous->object.pos.y,
        previous->object.pos.x + sprites->mouse_im[previous->object.dir].width,
        previous->object.pos.y + sprites->mouse_im[previous->object.dir].height,
        LCD_WHITE
      );
    } else if (dir == EAST || dir == WEST) {
      lcd_filled_rectangle(
        previous->object.pos.x,
        previous->object.pos.y + sprites->mouse_im[dir].height,
        previous->object.pos.x + sprites->mouse_im[previous->object.dir].width,
        previous->object.pos.y + sprites->mouse_im[previous->object.dir].height,
        LCD_WHITE
      );
    }

    // Draw the mouse's sprite
    display_bitmap16(
      sprites->mouse_im[dir].bitmap,
      mouse_x,
      mouse_y,
      sprites->mouse_im[dir].width,
      sprites->mouse_im[dir].height
    );
  }
}


void draw_cat(sprites_t *sprites, cat_state_t *previous, cat_state_t *cat)
{
  int cat_x = cat->object.pos.x;
  int cat_y = cat->object.pos.y;
  dir_t dir = cat->object.dir;

  // old cat position
  int old_cat_x = previous->object.pos.x;
  int old_cat_y = previous->object.pos.y;
  dir_t old_dir = previous->object.dir;

  // X and Y deltas
  int dx = cat_x - old_cat_x;
  int dy = cat_y - old_cat_y;


  if (dx != 0 || dy != 0 || dir != old_dir) {
    if ((((old_dir==SOUTH) || (old_dir==NORTH)) && ((dir==EAST) ||(dir==WEST)))) {
      lcd_filled_rectangle(old_cat_x, old_cat_y, old_cat_x+CAT_WIDTH, old_cat_y+CAT_HEIGHT, LCD_WHITE);
    } else if ((((old_dir==EAST) || (old_dir==WEST)) && ((dir==SOUTH) ||(dir==NORTH)))) {
      lcd_filled_rectangle(old_cat_x, old_cat_y, old_cat_x+CAT_HEIGHT, old_cat_y+CAT_WIDTH, LCD_WHITE);
    } else {
      clear_pix_on_same_dir(dir, old_cat_x, old_cat_y, CAT_MOVE, CAT_WIDTH, CAT_HEIGHT);
    }

    display_bitmap16(sprites->cat_im[dir].bitmap, cat_x, cat_y, sprites->cat_im[dir].width, sprites->cat_im[dir].height);
  }
}

void draw_message(char *line1, int color1, char *line2, int color2)
{
  lcd_print(10, 40, 13, LCD_BLUE, LCD_WHITE, line1);

  if (line2 != NULL) {
    lcd_print(10, 60, 13, LCD_BLUE, LCD_WHITE, line2);
  }
}

void erase_message()
{
  lcd_filled_rectangle(0, 40, 239, 80, LCD_WHITE);
}

void erase_object(object_t *object, image_t images[4])
{
  unsigned x1, x2, y1, y2;
  x1 = object->pos.x;
  y1 = object->pos.y;
  x2 = x1 + images[object->dir].width;
  y2 = y1 + images[object->dir].height;

  lcd_filled_rectangle(x1, y1, x2, y2, LCD_WHITE);
}
