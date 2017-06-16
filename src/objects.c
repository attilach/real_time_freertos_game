#include "objects.h"
#include "custom_rand.h"
#include "commons.h"

void mouse_state_init(mouse_state_t *mouse_state)
{
  // Get a random mouse pit
  uint32_t pit = rnd_32() % 3;

  mouse_state->vitality = MOUSE_VITALITY_MAX;
  mouse_state->object.dir = NORTH;
  mouse_state->object.pos.x = (pit * 80) + 34;
  mouse_state->object.pos.y = 280;
}

void cat_init(object_t *cat_state)
{
  cat_state->dir = SOUTH;
  cat_state->pos.x = 110;
  cat_state->pos.y = 30;
}

void cat_state_init(cat_state_t *cat_state)
{
  cat_init(&cat_state->object);
  cat_state->active = true;
}
