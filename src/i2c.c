#include "i2c.h"

#include "console/console.h"
#include "hal/hal_i2c.h"
#include "mcu/nrf52_hal.h"
#include "os/os.h"


int init_i2c() {
    struct nrf52_hal_i2c_cfg config = {
        .scl_pin = 12,
        .sda_pin = 17,
        .i2c_frequency = 400,
    };
    int err = hal_i2c_init(1, &config);
    if (err != OS_OK) {
        console_printf("hal_i2c_init error: %d\n", err);
    }
    return err;
}