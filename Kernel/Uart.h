#ifndef __UART_H__
#define __UART_H__

#include "Klib.h"
#include "Fdt.h"
#include "Riscv.h"
#include <stdarg.h> // Variadic arguments

#define UARTREG(reg) ((volatile uint8_t*)(fdt_list.uart_addr+(reg)))

//* Used with UARTREG macro. Provides proper memory mapped IO ptr.
#define RHR 0   // Receive Holding Register     (Input)
#define THR 0   // Transmit Holding Register    (Output)
#define IER 1   // Interrupt Enable Register
#define FCR 2   // FIFO Control Register
#define ISR 2   // Interrupt Status Register
#define LCR 3   // Line Control Register
#define LSR 5   // Line Status Register

void init_uart();
void uart_handler();

void uart_putchar(int c);
int uart_getchar();

/// @brief A printf clone that works over UART serial port.
/// @param fmt The printf format string. Uses %[d, x, p, s, l, %]
/// @note Uses variadic arguments which are calculated using % signs in the fmt string.
void uart_printf(char *fmt, ...);

#endif // __UART_H__