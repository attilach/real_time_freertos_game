/**************
*   UART driver
***************/
#ifndef __UART_H
#define __UART_H

void uart0_init(uint32_t Baudrate);
void uart0_send(uint8_t *BufferPtr, uint32_t Length);

#endif
