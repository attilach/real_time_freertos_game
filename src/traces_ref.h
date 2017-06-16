/*
* Description: trace management using UART (and FreeRTOS)
* Created on : 30 sept. 2013
* Author     : VP
*/
#ifndef TRACES_REF_H_
#define TRACES_REF_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>


// definition of the synchronisation word defining the start of each trace
#define SYNCHRO_WORD 0x55

// trace structure
typedef struct {
	uint8_t synchro;
	uint8_t sig_idx;	/* bit 8 indicates other traces than VCD */
	short val;
	uint32_t time;
} trace_t;

/* Description: initialize UART0 and specified timer for traces. If enabled,
*              the interrupts occur after each trace sending.
*              DO NOT CALL send_traces_to_uart0() but only write_trace_ref()
*              if interrupts are enabled.
* Parameters: baudrate: valid UART baudrate [bit/s]
*             timer_id: 0 to 3 to identify the timer used as a free running counter
*             interrupt_enabled: enable UART0 interrupt if true
*/
void init_traces(uint32_t baudrate, int timer_id, bool interrupt_enabled);

/* Description: store a trace in a memory buffer
*
* Parameters: trace_id: trace ID. Usually the task number in FreeRTOS.
*             val: 1 if task becomes active, 0 otherwise
*/
void write_trace_ref(uint8_t trace_id, short val);

/* Description: if new traces are available, send them to UART.
*              Polling is used here instead of interrupts */
void send_traces_to_uart0(void);

void uart0_send_trace(trace_t *trace);

trace_t my_write_trace(uint8_t trace_id, short val);

void my_init_trace();



#endif
