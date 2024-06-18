#include "Uart.h"

#define BACKSPACE 127
#define RETURN '\r'
#define HEX_UPPER 'A'
#define HEX_LOWER 'a'


static void internal_uart_print_str(const char *string);
static void internal_uart_print_num(int64_t num);
static void internal_uart_print_hex(uint64_t num, char FMT);


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

void uart_printf(char *fmt, ...)
{
    va_list v_ptr;
    va_start(v_ptr, fmt);
    

    for (uint64_t i = 0; fmt[i] != '\0'; i++)
    {
        if (fmt[i] == '%')
        {
            // Should push the string to the next character, skipping the % sign.
            switch (fmt[++i])
            {
            case '%':
                uart_putchar('%');
                break;
            case 's':
            case 'S':
                internal_uart_print_str(va_arg(v_ptr, char*));
                break;
            case 'D':
            case 'd':
                internal_uart_print_num(va_arg(v_ptr, uint64_t));
                break;
            case 'X':
            case 'P':
                internal_uart_print_hex(va_arg(v_ptr, uint64_t), HEX_UPPER);
                break;
            case 'p':
            case 'x':
                internal_uart_print_hex(va_arg(v_ptr, uint64_t), HEX_LOWER);
                break;

            default:
                uart_putchar('%');
                uart_putchar(fmt[i]);
                break;
            }
        }
        else
            uart_putchar(fmt[i]);
    }
    va_end(v_ptr);

    return;
}

static void internal_uart_print_str(const char *string)
{
    // For every character, send it via uart_putchar()
    for (uint64_t i = 0; string[i] != '\0'; i++)
        uart_putchar(string[i]);

    return;
}

static void internal_uart_print_num(int64_t num)
{
    // an int64_t can only be as big as 20 decimal digits (including '-'). Add an extra space for '\0'.
    char str[21] = {0};
    int str_pos = 19;
    int is_negative = false;

    // If num is negative, flip is_negative flag to true and make num positive.
    if (num < 0)
    {
        is_negative = true;
        num = -num;
    }
    // If num is already 0, set accordingly
    else if (num == 0)
    {
        str[str_pos--] = '0';
    }

    // Primary loop: get first digit from the right, then remove said digit until we reach 0.
    while (num > 0)
    {
        str[str_pos--] = (num % 10) + '0';
        num = num / 10;
    }

    // If the is_negative flag is flipped, add a negative sign to the end of the number.
    if (is_negative)
        str[str_pos--] = '-';

    // Get the address of the first position on the string, ignore empty space.
    internal_uart_print_str(&str[++str_pos]);
}

static void internal_uart_print_hex(uint64_t num, char FMT)
{
    
    // The hex output will always be 16 digits long, including 0's.
    char str[19] = {'0','x','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','\0'};
    int str_pos = 17;
    int temp;


    // Primary loop: get the first hex character (16 possible numbers) and adjust num accordingly.
    while (num > 0)
    {
        temp = num % 16;
        num = num / 16;
        
        if (temp > 9)
            str[str_pos--] = (temp - 10) + FMT;
        else
            str[str_pos--] = temp + '0';
    }

    // Get the address of the first position on the string, ignore empty space.
    internal_uart_print_str(str);
}
