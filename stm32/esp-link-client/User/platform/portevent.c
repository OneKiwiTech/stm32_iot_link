/* -----------------------  includes ----------------------------------*/
#include "port.h"
#include "FreeRTOS.h"
#include "event_groups.h"

/* ----------------------- Variables ----------------------------------------*/
static EventGroupHandle_t xELCientEvent_h;
static StaticEventGroup_t xELClientEventGroup;

/* ----------------------- Start implementation -----------------------------*/
BOOL  xPortEventInit( void )
{
    xELCientEvent_h = xEventGroupCreateStatic( &xELClientEventGroup );
    return TRUE;
}

BOOL  xPortEventPost( uint32_t eEvent )
{
    xEventGroupSetBits(xELCientEvent_h, eEvent);
    return TRUE;
}

BOOL xPortEventGet( uint32_t * eEvent )
{
    EventBits_t uxBits;
    const TickType_t xTicksToWait = 0xFFFFFFFF;

    /* waiting forever OS event */
    uxBits  = xEventGroupWaitBits(xELCientEvent_h,
            0x01,
            pdTRUE,
            pdFALSE,
            xTicksToWait );

    if (uxBits & 0x01) {
        *eEvent = 0x01;
    }
    
    return TRUE;
}
