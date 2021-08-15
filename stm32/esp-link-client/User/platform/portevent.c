/* ----------------------- Modbus includes ----------------------------------*/
#include "port.h"
#include "FreeRTOS.h"
#include "event_groups.h"
#include "platform.h"

/* ----------------------- Variables ----------------------------------------*/
static EventGroupHandle_t xSlaveOsEvent_h;
static StaticEventGroup_t xSlaveOsEventGroup;

/* ----------------------- Start implementation -----------------------------*/
BOOL
xMBPortEventInit( void )
{
    xSlaveOsEvent_h = xEventGroupCreateStatic( &xSlaveOsEventGroup );

    return TRUE;
}

BOOL
xMBPortEventPost( eMBEventType eEvent )
{
    xEventGroupSetBits(xSlaveOsEvent_h, eEvent);
    return TRUE;
}

BOOL
xMBPortEventGet( eMBEventType * eEvent )
{
    EventBits_t uxBits;
    const TickType_t xTicksToWait = 100 / portTICK_PERIOD_MS;

    /* waiting forever OS event */
    uxBits  = xEventGroupWaitBits(xSlaveOsEvent_h,
            EV_READY | EV_FRAME_RECEIVED | EV_EXECUTE | EV_FRAME_SENT,
            pdTRUE,
            pdFALSE,
            xTicksToWait );

    if (uxBits & EV_READY) {
        *eEvent = EV_READY;
    }else if (uxBits &EV_FRAME_RECEIVED) {
        *eEvent = EV_FRAME_RECEIVED;
    }else if (uxBits & EV_EXECUTE){
        *eEvent = EV_EXECUTE;
    }else if (uxBits & EV_FRAME_SENT) {
        *eEvent = EV_FRAME_SENT;
    }else {

    }
    
    return TRUE;
}
