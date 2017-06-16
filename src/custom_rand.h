
#ifndef CUSTOM_RAND_H
#define CUSTOM_RAND_H

#include <stdint.h>

#ifdef TESTING // testing environment -----------------------------------------

#include <stdlib.h>

#define rnd_32() ((uint32_t)rand())
#define init_rnd32(seed) srand(seed)

#else // embed environment ----------------------------------------------------

uint32_t rnd_32(void);
void init_rnd32(uint32_t seed);

#endif // end environment test ------------------------------------------------


#endif // CUSTOM_RAND_H
