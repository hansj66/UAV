#include "hal/hal_i2c.h"
#include "mcu/nrf52_hal.h"
#include "console/console.h"
#include "os/os.h"
#include "dwm1001.h"

const uint32_t control_task_time_interval = OS_TICKS_PER_SEC/2;

static struct os_callout control_task_callout;

extern dwm_loc_t location;

static void reset_control_task_callout() {
    int err = os_callout_reset(&control_task_callout, control_task_time_interval);
    if (err != OS_OK) {
        console_printf("control task os_callout_reset error: %d\n", err);
    }
}


static void control_task_event_callback(struct os_event* event) 
{
 
    // Check heading 
    // Check position (dwm_loc_t location)
    // Caclulate PWM-signal
    console_printf("Control task is not implemented.\n");

    reset_control_task_callout();
}

void init_control_task() {
    console_printf("Initializing control task\n");


    os_callout_init(&control_task_callout, os_eventq_dflt_get(), control_task_event_callback, NULL);
    reset_control_task_callout();
}

