#include "pwm.h"
#include "hal/hal_gpio.h"
#include "os/os.h"
#include <hal/hal_watchdog.h>
#include "console/console.h"
#include <pwm/pwm.h>
#include <bsp/bsp.h>
#include <pwm_nrf52/pwm_nrf52.h>
#include <stdint.h>

extern bool DISABLE_ROTORS;

struct pwm_chan_cfg chan_conf = {
    .pin = THROTTLE,
    .inverted = false,
    .data = NULL
};
uint32_t base_freq;

/* OS device */
struct os_dev dev0;
/* PWM device */
struct pwm_dev *pwm0;
struct pwm_dev *pwm1;
/* Max value for PWM */
uint16_t max_val;


/*
Arming disable flags 0x820

Kan RX rate økes fra 49 ?

ARMING_DISABLED_RX_FAILSAFE
ARMING_DISABLED_BOX_FAILSAFE
ARMING_DISABLED_RUNAWAY_TAKEOFF
ARMING_DISABLED_BOOT_GRACE_TIME
ARMING_DISABLED_NO_PREARM
*/

/*
    arming
 chan_conf.pin = THROTTLE;
    pwm_chan_config(pwm0, 0, &chan_conf);
    pwm_enable_duty_cycle(pwm0, 0, 5140);
  
    chan_conf.pin = YAW;
    pwm_chan_config(pwm0, 2, &chan_conf);
    pwm_enable_duty_cycle(pwm0, 2, 3400);

    chan_conf.pin = PITCH;
    pwm_chan_config(pwm0, 1, &chan_conf);
    pwm_enable_duty_cycle(pwm0, 1, 4231);

    chan_conf.pin = ROLL;
    pwm_chan_config(pwm0, 3, &chan_conf);
    pwm_enable_duty_cycle(pwm0, 3, 4231);


// 1500 : 4231

     os_time_delay(OS_TICKS_PER_SEC*5);

    chan_conf.pin = YAW;
    pwm_chan_config(pwm0, 2, &chan_conf);
    pwm_enable_duty_cycle(pwm0, 2, 4231);

    os_time_delay(OS_TICKS_PER_SEC*2);
    chan_conf.pin = THROTTLE;
    pwm_chan_config(pwm0, 0, &chan_conf);
    pwm_enable_duty_cycle(pwm0, 0, 5000);
*/

uint32_t THRUST = 5140; // 4231

void updateThrust(int controlValue)
{
    if (DISABLE_ROTORS)
        return;
    if (controlValue < 0)
        THRUST -= abs(controlValue);
    else
        THRUST += abs(controlValue);
    chan_conf.pin = THROTTLE;
    //pwm_chan_config(pwm0, 0, &chan_conf);
      pwm_enable_duty_cycle(pwm0, 0, THRUST);

}

void initPWM()
{
    if (DISABLE_ROTORS)
        return;
/*
    hal_gpio_init_out(THROTTLE, 1);
    hal_gpio_init_out(PITCH, 1);
    hal_gpio_init_out(YAW, 1);
    */
//hal_gpio_init_out(ROLL, 1);
    
    console_printf("Initializing PWM...\n");
    // pwm = (struct pwm_dev *) os_dev_open("pwm0", 0, NULL);

    os_dev_create(&dev0,
                  "pwm0",
                  OS_DEV_INIT_KERNEL,
                  OS_DEV_INIT_PRIO_DEFAULT,
                  nrf52_pwm_dev_init,
                  NULL);

    pwm0 = (struct pwm_dev *) os_dev_open("pwm0", 0, NULL);
  
    //pwm = (struct pwm_dev *) os_dev_open("pwm0", 0, NULL);

    /* set the PWM frequency */
    pwm_set_frequency(pwm0, 50);
    base_freq = pwm_get_clock_freq(pwm0);
    max_val = (uint16_t) (base_freq / 50);

    console_printf("base_frequency: %d\n", (int)base_freq);
    console_printf("max val: %d\n", (int)max_val);


    chan_conf.pin = THROTTLE;
    pwm_chan_config(pwm0, 0, &chan_conf);
    pwm_enable_duty_cycle(pwm0, 0, 5140);
    chan_conf.pin = YAW;
    pwm_chan_config(pwm0, 2, &chan_conf);
    pwm_enable_duty_cycle(pwm0, 2, 3400);
    chan_conf.pin = PITCH;
    pwm_chan_config(pwm0, 1, &chan_conf);
    pwm_enable_duty_cycle(pwm0, 1, 4231);
    chan_conf.pin = ROLL;
    pwm_chan_config(pwm0, 3, &chan_conf);
    pwm_enable_duty_cycle(pwm0, 3, 4231);

    os_time_delay(OS_TICKS_PER_SEC*5);

    chan_conf.pin = YAW;
    pwm_chan_config(pwm0, 2, &chan_conf);
    pwm_enable_duty_cycle(pwm0, 2, 4231);

    os_time_delay(OS_TICKS_PER_SEC*2);
    chan_conf.pin = THROTTLE;
    pwm_chan_config(pwm0, 0, &chan_conf);
  //  pwm_enable_duty_cycle(pwm0, 0, 4700);
      pwm_enable_duty_cycle(pwm0, 0, 4231);
    


     /*

    chan_conf.pin = PITCH;
    pwm_chan_config(pwm0, 1, &chan_conf);
    pwm_enable_duty_cycle(pwm0, 1, 4231);

    chan_conf.pin = YAW;
    pwm_chan_config(pwm0, 2, &chan_conf);
    pwm_enable_duty_cycle(pwm0, 2, 4231);

    chan_conf.pin = ROLL;
    pwm_chan_config(pwm0, 3, &chan_conf);
    pwm_enable_duty_cycle(pwm0, 3, 4231);
    */


    console_printf("Big TODO here. Just sayin'.\n");
}

