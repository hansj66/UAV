#include "hal/hal_i2c.h"
#include "mcu/nrf52_hal.h"
#include "console/console.h"
#include "os/os.h"
#include "dwm1001.h"

const uint32_t heading_task_time_interval = OS_TICKS_PER_SEC/2;

static struct os_callout heading_task_callout;

extern dwm_loc_t location;

static void reset_heading_task_callout() {
    int err = os_callout_reset(&heading_task_callout, heading_task_time_interval);
    if (err != OS_OK) {
        console_printf("heading task os_callout_reset error: %d\n", err);
    }
}


static void heading_task_event_callback(struct os_event* event) 
{
    console_printf("Heading task is not implemented.\n");

    reset_heading_task_callout();
}

void init_heading_task() {
    console_printf("Initializing heading task\n");


    os_callout_init(&heading_task_callout, os_eventq_dflt_get(), heading_task_event_callback, NULL);
    reset_heading_task_callout();
}

