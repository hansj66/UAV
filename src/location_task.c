#include "hal/hal_i2c.h"
#include "mcu/nrf52_hal.h"
#include "console/console.h"
#include "os/os.h"
#include "dwm1001.h"
#include "ema.h"
#include "pid.h"
#include "pwm.h"

#define dt OS_TICKS_PER_SEC/20
#define TARGET_Z 1000

const uint32_t location_task_time_interval = dt;

static struct os_callout location_task_callout;

dwm_loc_t location;

int avg_height = 0;
float error = 0;
PID pid;

static void reset_location_task_callout() {
    int err = os_callout_reset(&location_task_callout, location_task_time_interval);
    if (err != OS_OK) {
        console_printf("location task os_callout_reset error: %d\n", err);
    }
}


static void location_task_event_callback(struct os_event* event) 
{
    if (ERR_OK != dwm_loc_get(&location))
    {
        console_printf("ERROR: dwm_loc_get failed...\n");
        // return;
    }


   // console_printf("Z:%d, filtered Z:%d\n", (int)location.position.z, (int)EMA(location.position.z));

   console_printf("Location: X:%d Y:%d, Z:%d (A:%d)\n", (int)location.position.x, 
                                                    (int)location.position.y, 
                                                    (int)location.position.z,
                                                    (int)location.distance_count);

    if ((location.position.x != 0) && (location.position.y != 0) && (location.position.z != 0)) {
        error = EMA(location.position.z - TARGET_Z);
        pid_update(&pid, error, dt); 
        console_printf("ERROR: %d Control:%d\n", (int)error, (int)pid.control);
        updateThrust(pid.control);

    }
                                                    

    reset_location_task_callout();
}

void init_location_task() {
    console_printf("Initializing sensor location task\n");

    if (ERR_OK != dwm_init_tag())
    {
        console_printf("ERROR: dwm_init_tag failed.\n");
        return; 
    }
    location.position.x = 0;
    location.position.y = 0;
    location.position.z = 0;

    PID_Initialize(&pid);

    os_callout_init(&location_task_callout, os_eventq_dflt_get(), location_task_event_callback, NULL);
    reset_location_task_callout();
}