/*
 * debug.h
 *
 *  Created on: 3 mai 2015
 *  Author: V. Pilloux
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#include <stdbool.h>
#include "LPC17xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

// if DEBUG_MODE = 1, it enables the debug function int2file()
#define DEBUG_MODE 1

#if DEBUG_MODE==0
	#define int2file(a,b,c,d)
#else
/* Macro storing an array of integers in a text file, one number per line. The size of
 * each element of the array can be 16 or 32 bits.
 * Parameters: filename
 *             sig:  pointer on the array of integers to be saved
 *             len: number of elements in the array
 *             int_len: size of each element [bytes]
 *             is_signed: true if the signal contains signed numbers, false otherwise
 */
	void int2file(char *filename, void *sig, int len, int int_len, bool is_signed);
#endif

/* Macro that can be used to stop any activity on FreeRTOS in case of
 * critical issue.
 * Parameter: str: string to be displayed before freezing the CPU
 */
#define EXIT(str) {vTaskSuspendAll(); printf(str); while(1);}

#endif /* DEBUG_H_ */
