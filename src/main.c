#include <assert.h>
#include <string.h>

#include "sysinit/sysinit.h"
#include "os/os.h"
#include "bsp/bsp.h"
#include "hal/hal_gpio.h"
#ifdef ARCH_sim
#include "mcu/mcu_sim.h"
#endif
#include "console/console.h"
#include "i2c.h"
#include "bno080.h"
#include "hal/hal_uart.h"
#include "leds.h"
#include "pwm.h"
#include "dwm1001.h"
#include "location_task.h"
#include "control_task.h"
#include "heading_task.h"
#include "bno080.h"


bool DISABLE_ROTORS = false;

//led indikasjon på antall anker


int main(int argc, char **argv)
{
    int rc;

#ifdef ARCH_sim
    mcu_sim_parse_args(argc, argv);
#endif

    sysinit();

    hal_gpio_write(22,1);
    hal_gpio_write(CS_PIN, 1);

    console_printf("------------------------------------------\n");
    console_printf("Autonomous Aerial Meat Grinder says hello!\n");
    console_printf("------------------------------------------\n");
    initLEDs();
    initPWM();
    init_location_task();
    //init_bno080();
    //init_heading_task();
    // init_control_task();
  
    while (1) {
        os_eventq_run(os_eventq_dflt_get());
    }
    assert(0);

    return rc;
}

