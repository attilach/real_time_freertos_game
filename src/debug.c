/*
 * debug.c
 *
 *  Created on: 3 mai 2015
 *      Author: V. Pilloux
 */

#include <stdio.h>
#include <stdbool.h>
#include "debug.h"
#include "adc.h"


#if DEBUG_MODE
void int2file(char *filename, void *sig, int len, int int_len, bool is_signed)
{
	FILE *pf;
	int i;
	int16_t *ptr16;
	int32_t *ptr32;
	uint16_t *ptru16;
	uint32_t *ptru32;

	//read_mic_stop();	// stop interrupts to avoid buffer overwriting during file creation
	if ((pf=fopen(filename,"w"))==NULL)
	{
		printf("Error opening file!\n");
		return;
	}
	for (i=0; i<len; i++)
	{
		if (is_signed)
		{
			switch(int_len)
			{
			case 2:
				ptr16=(int16_t *)sig;
				fprintf(pf,"%d\n", ptr16[i]);
				break;
			case 4:
				ptr32=(int32_t *)sig;
				fprintf(pf,"%d\n", ptr32[i]);
				break;
			}
		}
		else
		{
			switch(int_len)
			{
			case 2:
				ptru16=(uint16_t *)sig;
				fprintf(pf,"%u\n", ptru16[i]);
				break;
			case 4:
				ptru32=(uint32_t *)sig;
				fprintf(pf,"%u\n", ptru32[i]);
				break;
			}
		}
	}

	fclose(pf);
	//LPC_TIM3->TCR = 1;
}
#endif
