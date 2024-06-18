#ifndef _UART_H_
#define _UART_H_

#include "KLib.h"
#include "FDT.h"

#include <stdarg.h>

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

/// @brief Printf clone that works over uart.
/// @param fmt Format string to be printed
/// @note uses variadic arguments which are calculated using the format (fmt) string
/// @warning All numbers expect uint64_t. Errors might show if you do not specify L or (uint64_t) on numbers.
void uart_printf(char *fmt, ...);

#endif // _UART_H_