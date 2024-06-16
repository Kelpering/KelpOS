#include "Uart.h"

#define BACKSPACE 127
#define RETURN '\r'

void init_uart()
{
    // Disable interrupts on UART
    *UARTREG(IER) = 0x00;

    // Enter Set Baud mode
    *UARTREG(LCR) = (1 << 7);

    //* Full short for a baud rate of 38.4K
    *UARTREG(0) = 0x03; // LSB for baud rate
    *UARTREG(1) = 0x00; // MSB for baud rate

    // Exit Set Baud mode; set line control to 8-bit words, no parity.
    *UARTREG(LCR) = (3 << 0);

    // Reset and Enable FIFO
    *UARTREG(FCR) = (3 << 1) | (1 << 0);

    // Enable transmit and receive interrupts
    *UARTREG(IER) = (1<<1) | (1 << 0);

    return;
}

void uart_handler()
{
    int c = NULL;

    while (true)
    {
        c = uart_getchar();
        if (c == error)
            break;
        uart_putchar(c);

        //^ Handle return (Enter) here for when the buffer is finished and we send it to the operating system.
    }

    return;
}

void uart_putchar(char c)
{
    //* Wait until UART is idle (probably an artifact from xv6, as I don't have any multitasking)
    while ((*UARTREG(LSR) & (1 << 5)) == 0);

    //* Write char to UART transmit register
    switch (c)
    {
    case BACKSPACE:
        *(UARTREG(THR)) = '\b';
        *(UARTREG(THR)) = ' ';
        *(UARTREG(THR)) = '\b';
        break;
        
    case RETURN:
        *(UARTREG(THR)) = '\n';
        break;

    default:
        *(UARTREG(THR)) = c;
        break;
    }
        

    return;
}

int uart_getchar()
{
    // If UART has chars waiting to be input, read them. Otherwise return error for no char.
    if (*UARTREG(LSR) & 0x01)
        return *UARTREG(RHR); 
    
    return error;
}

void uart_print(char *string)
{
    for (uint64_t i = 0; string[i] != '\0'; i++)
        uart_putchar(string[i]);

    return;
}