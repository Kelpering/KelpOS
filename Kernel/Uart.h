#ifndef _UART_H_
#define _UART_H_

#include "KLib.h"
#include "FDT.h"

#define UARTREG(reg) ((volatile uint8_t*)(UART0_ADDR+reg))

//* To be used with UARTREG to identify specific UART register addresses
#define RHR 0   // Receive holding register  (Input)
#define THR 0   // Transmit holding register (output)
#define IER 1   // Interrupt enable register
#define FCR 2   // FIFO control register
#define ISR 2   // Interrupt status register
#define LCR 3   // Line control register
#define LSR 5   // Line status register

void init_uart();
void uart_handler();

void uart_putchar(char c);
int uart_getchar();
void uart_print(char *string);

#endif // _UART_H_