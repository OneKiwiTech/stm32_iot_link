#ifndef DEBUG_LOG_H_
#define DEBUG_LOG_H_


#include "usart.h"
#include "debug_log.h"

#define CONSOLE_UART       USART3  /* Macro dinh nghia khoi UART dang su dung*/
#define MAX_DEBUG_BUF_LEN  512
static char debug_buf[MAX_DEBUG_BUF_LEN];

void __uart_putchar(char c);
void debug_send_buffer(const uint8_t* msg, uint32_t len);


void debug_puts ( const char* msg)
{
    debug_send_buffer(msg, strlen(msg) );
	__uart_putchar('\r');
	__uart_putchar('\n');
}

void  debug_printf ( const char * format, ... )
{
  va_list args;

	/* Tao chuoi tu format nhieu doi so*/
  va_start (args, format);
  vsnprintf (debug_buf, MAX_DEBUG_BUF_LEN, format, args);
	/* Gui chuoi debug ra UART */
  debug_send_buffer(debug_buf, strlen(debug_buf));
  va_end (args);
  __uart_putchar('\r');
  __uart_putchar('\n');
}

void debug_send_buffer(const uint8_t* msg, uint32_t len)
{
   /* Lap den khi nao ket thuc chuoi, va kiem tra pointer NOT NULL*/
	while(len--)
	{
		__uart_putchar(*msg++);
	}
	__uart_putchar('\r');
	__uart_putchar('\n');
}

//=========================================
void __uart_putchar(char c)
{
	/*Step 2: Ghi vao DR*/
	  LL_USART_TransmitData8(CONSOLE_UART, c);

	  /*Step 3: cho den khi TC = 1 - Trang 1007 Ref manual*/
	  while(!LL_USART_IsActiveFlag_TC(CONSOLE_UART)) {};
}



#endif /* DEBUG_LOG_H_ */
