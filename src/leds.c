#include "leds.h"
#include "hal/hal_gpio.h"
#include "os/os.h"
#include "console/console.h"

void initLEDs()
{
    console_printf("Initializing LEDs\n");
    hal_gpio_init_out(N_LED, 1);
    hal_gpio_init_out(S_LED, 1);
    hal_gpio_init_out(E_LED, 1);
    hal_gpio_init_out(W_LED, 1);

    BootGraceTime();
}


void BootGraceTime()
{
    // Light show while we prevent ARMING_DISABLED_BOOT_GRACE_TIME
    for (int i=0; i<5; i++)
    {
        hal_gpio_write(N_LED,1);
        hal_gpio_write(S_LED, 0);
        hal_gpio_write(E_LED, 0);
        hal_gpio_write(W_LED, 0);
        os_time_delay(OS_TICKS_PER_SEC/20);
        hal_gpio_write(N_LED,0);
        hal_gpio_write(S_LED, 0);
        hal_gpio_write(E_LED, 1);
        hal_gpio_write(W_LED, 0);
        os_time_delay(OS_TICKS_PER_SEC/20);
        hal_gpio_write(N_LED,0);
        hal_gpio_write(S_LED, 1);
        hal_gpio_write(E_LED, 0);
        hal_gpio_write(W_LED, 0);
        os_time_delay(OS_TICKS_PER_SEC/20);
        hal_gpio_write(N_LED,0);
        hal_gpio_write(S_LED, 0);
        hal_gpio_write(E_LED, 0);
        hal_gpio_write(W_LED, 1);
        os_time_delay(OS_TICKS_PER_SEC/20);
    }
    hal_gpio_write(N_LED,0);
    hal_gpio_write(S_LED, 0);
    hal_gpio_write(E_LED, 0);
    hal_gpio_write(W_LED, 0);
    os_time_delay(OS_TICKS_PER_SEC/2);
    hal_gpio_write(N_LED,1);
    hal_gpio_write(S_LED, 1);
    hal_gpio_write(E_LED, 1);
    hal_gpio_write(W_LED, 1);

}
