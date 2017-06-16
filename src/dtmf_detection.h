#ifndef DTMF_DETECTION_H
#define DTMF_DETECTION_H

#include "objects.h"

/**
* DTMF & Fourier info
*/
#define FREQ_1 852
#define FREQ_2 1336
#define FREQ_3 941
#define FREQ_4 1608
#define FREQ_ECHATILLONAGE 8000
#define SEUIL_FOURIER 2000000
#define PI 3.14159265359
#define BUFSIZE 400

typedef struct {
	unsigned int frequence ;
	float sin_tab[BUFSIZE];
	float cos_tab[BUFSIZE];
} module_dtmf;

// pointer on acquisition double buffer
unsigned short *sig;

/**
* Initialise DMA&ADC to receive samples from the audio line at 8000 kHz.
*/
void init_dtmf();

/**
* Initialise module dtmf, create lookup table for sin and cos operations
* @param  module_dtmf	 	module
* @param  unsigned int	 	freq_research
*/
void init_module_dtmf(module_dtmf *module, unsigned int freq_research);

/**
* Initialise all module dtmf
*/
void init_all_module_dtmf(void);

/**
* Detect colleration of the specify frequency
* @param  module_dtmf	 	module
* @param  unsigned int	 	offset
*/
bool decodage_fourier(module_dtmf *module, unsigned int offset);

/**
* Get cat joystick direction
* @return          	the cat direction
*/
dir_t get_cat_joystick(void);

#endif
