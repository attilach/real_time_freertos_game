/*
* Description: trace management using UART (and MyLab_lib)
* Created on : 5.5.2017
* Author     : VP
*/
#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif
#include <traces_ref.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "uart.h"
#include "commons.h"
#include "debug.h"


#define UART_BAUDRATE 115200

#define UART_LCR_THRE (1 << 5)
#define UART_LCR_TEMT (1 << 6)



void write_trace(uint8_t trace_id, short val)
{
	trace_t new_trace;
	new_trace = my_write_trace(trace_id, val);

	if (xQueueSendToBack(queue_trace, (void *)&new_trace, 0) != pdPASS) {
		EXIT("Unable to push trace to the dedicated queue");
	}

	//write_trace_ref(trace_id, val);		// to be replaced by your own implementation
}


trace_t my_write_trace(uint8_t trace_id, short val) {

	trace_t sig;

	sig.synchro = SYNCHRO_WORD;
	sig.sig_idx = trace_id;
	sig.val = val;
	sig.time = LPC_TIM0->TC;

	return sig;
}


void uart0_send_trace(trace_t *trace) {
	unsigned size = sizeof(*trace);

	int i = 0;
	for(i = 0; i < size; i++) {
		while(!(LPC_UART0->LSR & UART_LCR_TEMT)); // Wait until TEMT is empty
		LPC_UART0->THR = *(((uint8_t*)trace)+i);
	}
}


void my_init_trace() {
	uart0_init(UART_BAUDRATE);
	LPC_TIM0->TCR = 1;
	LPC_TIM0->TC =0;
}


// implement trace sending here after having set configUSE_IDLE_HOOK to 1 in FreeRTOSConfig.h
// note that portSET_INTERRUPT_MASK() and portCLEAR_INTERRUPT_MASK() can be used
// to protect critical sections if any. Do not use mutexes here, they have no effect!

void vApplicationIdleHook( void )
{
	trace_t trace_to_send;
	while (1)
	{
		// implement trace sending here after having set configUSE_IDLE_HOOK to 1 in FreeRTOSConfig.h
		if (xQueueReceive(queue_trace, (void *)&trace_to_send, 0) == pdTRUE) { //portMAX_DELAY
			uart0_send_trace(&trace_to_send);
		}
		taskYIELD();		// force changement de contexte
	}
}
