/*
* Description: trace management using UART
* Created on : 30 sept. 2013
* Author     : VP
*/
#ifndef TRACE_MGT_H_
#define TRACE_MGT_H_

#include <stdio.h>
#include <stdbool.h>
#include "traces_ref.h"


/* Description: write a trace to a memory buffer. Note that this function is
*              automatically called by FreeRTOS in privileged mode.
*
* Parameters: trace_id: trace ID. Usually the task number in FreeRTOS.
*             val: 1 if task becomes active, 0 otherwise
*/
void write_trace(uint8_t sig_idx, short val);

/**
* Function to write trace to see task
* @param trace_id		trace identificator
* @param val		    value of the trace
* @return				sig
*/
trace_t my_write_trace(uint8_t trace_id, short val);

/**
* Function to send trace on uart
* @param trace 			 trace to send on uart
*/
void uart0_send_trace(trace_t *trace);

/*
* init trace function, start timer 0, default fre. 25Mhz
*/
void my_init_trace();

#endif
