
#ifndef __ADC_H
#define __ADC_H

#include <stdint.h>

#define ADC_OFFSET          0x10
#define ADC_INDEX           4

#define ADC_DONE            0x80000000
#define ADC_OVERRUN         0x40000000
#define ADC_ADINT           0x00010000

#define ADC_NUM			1		/* for LPCxxxx */
#define ADC_CLK			1000000		/* set to 1Mhz */


/* Prototype of callback function called when one of both reception buffers
 * are filled.
 * Parameter: buffer_index: index of the buffer just filled
 */
typedef void (*func_t)(int buffer_index) ;


//void ADC_IRQHandler( void );
void adc_init_rgb( uint32_t adc_clk );

/* Description: Create a double buffer (size of 2*single_buf_length) to store data provided by ADC 0, 1, 2 or 3.
 * On Mylab2, ADC0 is connected to the microphone, ADC1 and 2 on left and right wires of the audio input.
 * The sampling frequency is specified in freq_hz. NOTE that the timer 1 is used to trim this sampling
 * frequency. The specified ADC is then read with GPDMA channel 1 until one of both buffer is filled.
 * If the callback function (func) is be specified, this function will be called each time a buffer has
 * been filled by the DMA. The parameter buffer_index is the index of the buffer filled.
 * Parameters:
 *   adc_idx: ADC index to be used (and read by the DMA)
 *   freq_hz: sampling frequency
 *   single_buf_length: single buffer size (max: 4095)
 *   func: callback function called after a buffer completion or NULL if not used
 * Return: a valid pointer on the allocated buffer which size is 2*single_buf_length or NULL if the malloc failed
 */
uint16_t *init_adc_dma(int adc_idx, uint32_t freq_hz, int16_t single_buf_length, func_t func);

void adc_init(int adc_idx);
short adc_read(int adc_idx);


#endif /* end __ADC_H */
