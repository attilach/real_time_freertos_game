/**
* Name        : dtmf_detection.c
* Date        : 5.5.2017
* Description : template of the DTMF detector
*/
#include <cr_section_macros.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "adc.h"
#include "debug.h"
#include "objects.h"

#include "commons.h"

#include "dtmf_detection.h"


#define BUF_SIZE 400
#define FE 8000.
#define PI 3.14159265359


// if you need additional buffers, enable the next line and declare them after it to place them in local RAM (RAM2 could be full!)
__attribute__ ((section(".data"))) module_dtmf module_freq1, module_freq2, module_freq3, module_freq4;

// callback function which is called each time one of both RX buffer filled by the DMA from ADC is full
// buf_index: 0 or 1, indicating which buffer is full
void buffer_filled(int buf_index)
{
	static portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

	if (xQueueSendToBackFromISR(queue_buf_index, (void *)&buf_index, &xHigherPriorityTaskWoken) != pdTRUE) {
		// EXIT("Unable to push value of buffer index to the dedicated queue");
	}

}


void init_dtmf()
{
	// The return pointer points on the the double buffer allocated.
	if ((sig=init_adc_dma(1, FE, BUF_SIZE, buffer_filled))==NULL)
	EXIT("Not enough memory to allocate acquisition buffers!");
}


void init_module_dtmf(module_dtmf *module, unsigned int freq_research) {

	float module_calc=0;

	module->frequence = freq_research;

	module_calc = ((-2*PI * freq_research) / FREQ_ECHATILLONAGE);

	for (int i = 0; i < 400; i++)
	{
		module->sin_tab[i] = sinf(i * module_calc);
		module->cos_tab[i] = cosf(i * module_calc);
	}
}


void init_all_module_dtmf(void) {
	init_module_dtmf(&module_freq1, FREQ_1);
	init_module_dtmf(&module_freq2, FREQ_2);
	init_module_dtmf(&module_freq3, FREQ_3);
	init_module_dtmf(&module_freq4, FREQ_4);
}


bool decodage_fourier(module_dtmf *module, unsigned int offset) {
	unsigned int i;
	float calc_cos=0, calc_sin=0, calc_tot=0, sample;

	for (i = 0; i <= BUFSIZE; i++) {
		sample = sig[i + offset];
		calc_cos += (sample * module->cos_tab[i]);
		calc_sin += (sample * module->sin_tab[i]);
	}
	calc_tot = fabsf(calc_cos)+fabsf(calc_sin);

	if (calc_tot > SEUIL_FOURIER) {
		return true;
	}

	return false;
}


dir_t get_cat_joystick(void) {

	unsigned int buf_index=0, offset=0;


	if (xQueueReceive(queue_buf_index, (void *)&buf_index, 0) == pdTRUE) {

		offset = buf_index * 400;

		if (decodage_fourier(&module_freq1, offset)) {
			if (decodage_fourier(&module_freq2, offset)) {
				return NORTH;
			}
			else if (decodage_fourier(&module_freq4, offset)) {
				return SOUTH;
			}
		}
		else if (decodage_fourier(&module_freq3, offset)) {
			if (decodage_fourier(&module_freq2, offset)) {
				return EAST;
			}
			else if (decodage_fourier(&module_freq4, offset)) {
				return WEST;
			}
		}
	}

	return NOTHING;
}
