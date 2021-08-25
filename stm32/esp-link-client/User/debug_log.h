#ifndef DEBUG_LOG_H
#define DEBUG_LOG_H

#include <stdint.h>
#include <stdio.h>  /* vsnprintf*/
#include <stdarg.h> /* Xu ly argument co nhieu doi so*/

#include "config.h"


#if DEBUG_LOG_EN > 0
  void debug_puts ( const char* msg);
  void  debug_printf ( const char * format, ... );
  void debug_send_buffer(const uint8_t* msg, uint32_t len);

  #define DBG_MSG(msg)             debug_puts(msg);
  #define DBG_PRINTF(...)          debug_printf(__VA_ARGS__);
  #define DUMP_BUFFER(buf, len)    debug_send_buffer(buf, len);
#else
  #define DBG_MSG(msg)
  #define DBG_PRINTF(...)
  #define DUMP_BUFFER(buf, len)
#endif /* DEBUG_LOG_EN */

#endif
