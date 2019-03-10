#include "os/os.h"
#include "bsp/bsp.h"
#include "hal/hal_gpio.h"
#include "hal/hal_spi.h"
#include <os/os_dev.h>
#include <assert.h>
#include <string.h>
#include <console/console.h>
#include "dwm1001.h"

uint8_t RXBUF[256]; 
uint8_t DUMMY[] = {0xFF};


DWM_RETVAL dwmCommand(uint8_t * cmd, uint8_t length)
{
    // console_printf("SPI TX: ");
    // for (int i=0; i<length; i++)
    //     console_printf("%02X ", cmd[i]);
    // console_printf("\n");

    int rc = 0;
    hal_gpio_write(CS_PIN, 0);

    //console_printf("Command length: %d\n", length);

    rc = hal_spi_txrx(0, cmd, RXBUF, length);
    if (rc != 0) {
        console_printf("SPI error: %d\n", rc);
    }
 
    hal_gpio_write(CS_PIN, 1);

    uint8_t sizeByte[] = {0};



    // Wait until size byte is transmitted
    do {
        hal_gpio_write(CS_PIN, 0);
        hal_spi_txrx(0, DUMMY, sizeByte, 1);
        hal_gpio_write(CS_PIN, 1);
    } while (sizeByte[0] == 0);

    os_time_delay(OS_TICKS_PER_SEC/100);
    //console_printf("Response size: %d\n", sizeByte[0]);

    hal_gpio_write(CS_PIN, 0);
    for (int i=0; i<sizeByte[0]; i++)
    {
        hal_spi_txrx(0, DUMMY, &(RXBUF[i]), 1);
    }
    hal_gpio_write(CS_PIN, 1);

    // console_printf("SPI RX: ");
    // for (int i=0; i<sizeByte[0]; i++) {
    //    console_printf("%02X ", RXBUF[i]);
    //   }
    // console_printf("\n");

    if (RXBUF[0] != 0x40)
    {
        console_printf("ERROR: Invalid TLV frame.\n"); 
        return ERR_NOT_TLV_FRAME;
    }

    switch(RXBUF[2]) {
        case 0: /*console_printf("OK\n");*/ break;
        case 1: console_printf("ERROR: Unknown command.\n"); break;
        case 2: console_printf("ERROR: Internal error.\n"); break;
        case 3: console_printf("ERROR: Invalid parameter.\n"); break;
        case 4: console_printf("ERROR: Busy.\n"); break;
        default: console_printf("ERROR: Weird response code. Something is deeply wrong...\n"); break;
    }
   
    return RXBUF[2];
}

DWM_RETVAL dwm_pos_set(dwm_pos_t position)
{
    /*
    This API function set the default position of the node. Default position is not used when in tag mode
    but is stored anyway so the module can be configured to anchor mode and use the value previously
    set by dwm_pos_set. This call does a write to internal flash in case of new value being set, hence
    should not be used frequently as can take, in worst case, hundreds of milliseconds.
    */
    console_printf("dwm_pos_set - not implemented.\n");
    return ERR_NOT_IMPLEMENTED;
}

DWM_RETVAL dwm_pos_get(dwm_pos_t * position)
{
    /*
    This API function obtain position of the node. If the current position of the node is not available, the
    default position previously set by dwm_pos_set will be returned
    */
    console_printf("dwm_pos_get - not implemented.\n");
    return ERR_NOT_IMPLEMENTED;
}

DWM_RETVAL dwm_upd_rate_set(dwm_rate_t rate)
{
    /*
    This API function sets the update rate and the stationary update rate of the position in unit of 100
    milliseconds. Stationary update rate must be greater or equal to normal update rate. This call does a
    write to the internal flash in case of new value being set, hence should not be used frequently as can
    take, in worst case, hundreds of milliseconds.
    */
    
    console_printf("dwm_upd_rate_get.\n");
    uint8_t cmd[6] = {0x03, 0x04, 0, 0, 0, 0};
    cmd[2] = rate.rate & 0x00F;
    cmd[3] = rate.rate >> 8;
    cmd[4] = rate.stationary_rate & 0x00F;
    cmd[5] = rate.stationary_rate >> 8;
    return dwmCommand(cmd, 6);
}

DWM_RETVAL dwm_upd_rate_get(dwm_rate_t * rate)
{
    /*
    This API function gets position update rate. 
    */
    console_printf("dwm_upd_rate_get - not implemented.\n");
    return ERR_NOT_IMPLEMENTED;
}

DWM_RETVAL dwm_cfg_tag_set(dwm_tag_cfg_t tag_cfg)
{
    /*
    This API function configures the node as tag with given options. It automatically resets the DWM
    module. This call does a write to internal flash in case of new value being set, hence should not be
    used frequently as can take, in worst case, hundreds of milliseconds. Note that this function only
    sets the configuration parameters. To make effect of the settings, users should issue a reset
    command, see section 4.3.12 for more detail.
    */

    console_printf("dwm_cfg_tag_set.\n");
 
    uint8_t byte1 = 0;
    uint8_t byte0 = 0;

    if (tag_cfg.accel_en)
        byte1 |= ACCELEROMETER_ENABLE;

    if (tag_cfg.low_power_en)
        byte0 |= LOW_POWER_ENABLE;

    if (tag_cfg.loc_eng_en)
        byte0 |= LOCATION_ENGINE_ENABLE;

    if (tag_cfg.led_en)
        byte0 |= LED_ENABLE;

    if (tag_cfg.ble_en)
        byte0 |= BLE_ENABLE;

    if (tag_cfg.fw_update_en)
        byte0 |= FIRMWARE_UPDATE_ENABLE;

    switch(tag_cfg.uwb_mode) {
        case OFFLINE:   break;
        case PASSIVE:   byte0 |= UWB_MODE_PASSIVE;
                        break;
        case ACTIVE:    byte0 |= UWB_MODE_ACTIVE;
                        break;
    }

    // console_printf("Byte0: %d, byte1: %d\n", byte0, byte1);
    uint8_t cmd[4] = {0x05, 0x02, byte0, byte1};

    return dwmCommand(cmd, 4);
}

DWM_RETVAL dwm_cfg_anchor_set(dwm_anchor_cfg_t anchor_cfg)
{
    /*
    This API function configures the node as tag with given options. It automatically resets the DWM
    module. This call does a write to internal flash in case of new value being set, hence should not be
    used frequently as can take, in worst case, hundreds of milliseconds. Note that this function only
    sets the configuration parameters. To make effect of the settings, users should issue a reset
    command, see section 4.3.12 for more detail.
    */
    console_printf("dwm_cfg_anchor_set - not implemented.\n");
    return ERR_NOT_IMPLEMENTED;
}

DWM_RETVAL dwm_cfg_get(dwm_node_cfg_t * node_cfg)
{
    /*
    This API function obtains the configuration of the node.
    */
    console_printf("dwm_cfg_get - not implemented.\n");
    return ERR_NOT_IMPLEMENTED;
}

DWM_RETVAL dwm_sleep()
{
    /*
    This API function puts the module into sleep mode. Low power option must be enabled otherwise an
    error will be returned.
    */
    console_printf("dwm_sleep - not implemented.\n");
    return ERR_NOT_IMPLEMENTED;
}

DWM_RETVAL dwm_loc_get(dwm_loc_t * loc)
{
    /*
    Get last distances to the anchors (tag is currently ranging to) and the associated position. The
    interrupt is triggered when all TWR measurements have completed and the LE has finished. If the LE
    is disabled, the distances will just be returned. This API works the same way in both Responsive and
    Low-Power tag modes. 
    */

    // console_printf("dwm_loc_get - ");

    uint8_t cmd[2] = {0x0C, 0x00};
    DWM_RETVAL retval =  dwmCommand(cmd, 2);

    if (retval == ERR_OK) 
    {
        loc->distance_count = RXBUF[20];
        loc->position.x = (RXBUF[8] << 24) + (RXBUF[7] << 16) + (RXBUF[6] << 8) + RXBUF[5];
        loc->position.y = (RXBUF[12] << 24) + (RXBUF[11] << 16) + (RXBUF[10] << 8) + RXBUF[9];
        loc->position.z = (RXBUF[16] << 24) + (RXBUF[15] << 16) + (RXBUF[14] << 8) + RXBUF[13];
   }

    return retval;
}

DWM_RETVAL dwm_baddr_set(uint64_t baddr)
{
    /*
    Sets the public Bluetooth address used by device. New address takes effect after reset. This call does
    a write to internal flash in case of new value being set, hence should not be used frequently as can
    take, in worst case, hundreds of milliseconds
    */
    console_printf("dwm_baddr_set - not implemented.\n");
    return ERR_NOT_IMPLEMENTED;
}

DWM_RETVAL dwm_baddr_get(uint64_t * baddr)
{
    /*
    Get Bluetooth address currently used by device.
    */
    console_printf("dwm_baddr_get - not implemented.\n");
    return ERR_NOT_IMPLEMENTED;
}


DWM_RETVAL dwm_reset()
{
    /*
    This API function reboots the module.
    */
    console_printf("dwm_reset.\n");

    uint8_t cmd[2] = {0x14, 0x00};
    return dwmCommand(cmd, 2);
}

DWM_RETVAL dwm_ver_get(dwm_ver_t * ver)
{
    /*
    This API function obtains the firmware version of the module.
    */
    console_printf("dwm_ver_get - not implemented.\n");
    return ERR_NOT_IMPLEMENTED;
}

DWM_RETVAL dwm_gpio_cfg_output(uint8_t gpio_idx, DWM_GPIO_VALUE gpio_value)
{
    /*
    This API function configures a specified GPIO pin as an output and also sets its value to 1 or 0, giving
    a high or low digital logic output value.
    Note: During the module reboot, the bootloader (as part of the firmware image) blinks twice the
    LEDs on GPIOs 22, 30 and 31 to indicate the module has restarted. Thus these GPIOs should be used
    with care during the first 1s of a reboot operation
    */
    console_printf("dwm_gpio_cfg_output - not implemented.\n");
    return ERR_NOT_IMPLEMENTED;
}

DWM_RETVAL dwm_gpio_cfg_input(uint8_t gpio_idx, DWM_PULL_TYPE pullup)
{
    /*
    This API function configure GPIO pin as input.
    Note: During the module reboot, the bootloader (as part of the firmware image) blinks twice the
    LEDs on GPIOs 22, 30 and 31 to indicate the module has restarted. Thus these GPIOs should be used
    with care during the first 1s of a reboot operation.
    */
    console_printf("dwm_gpio_cfg_input - not implemented.\n");
    return ERR_NOT_IMPLEMENTED;
}

DWM_RETVAL dwm_gpio_value_set(uint8_t gpio_idx, DWM_GPIO_VALUE value)
{
    /*
    This API function sets the value of the GPIO pin to high or low.
    Note: During the module reboot, the bootloader (as part of the firmware image) blinks twice the
    LEDs on GPIOs 22, 30 and 31 to indicate the module has restarted. Thus these GPIOs should be used
    with care during the first 1s of a reboot operation.
    */
    console_printf("dwm_gpio_value_set - not implemented.\n");
    return ERR_NOT_IMPLEMENTED;
}

DWM_RETVAL dwm_gpio_value_get(uint8_t gpio_idx, DWM_GPIO_VALUE * value)
{
    /*
    This API function reads the value of the GPIO pin.
    */
    console_printf("dwm_gpio_value_get - not implemented.\n");
    return ERR_NOT_IMPLEMENTED;
}

DWM_RETVAL dwm_gpio_value_toggle(uint8_t gpio_idx)
{
    /*
    This API function toggles the value of the GPIO pin.
    Note: During the module reboot, the bootloader (as part of the firmware image) blinks twice the
    LEDs on GPIOs 22, 30 and 31 to indicate the module has restarted. Thus these GPIOs should be used
    with care during the first 1s of a reboot operation.
    */
    console_printf("dwm_gpio_value_toggle - not implemented.\n");
    return ERR_NOT_IMPLEMENTED;
}

DWM_RETVAL dwm_status_get(DWM_STATUS * status)
{
    /*
    This API function reads the system status: Location Data Ready. Location Data is the calculated result
    of the LE. When the tag finished TWR with the 4 anchors, the derived distance to the LE for position
    calculation. When this calculation is done, the Location Data Ready flag will be turn on. When the
    data is acquired by any API function, the flag will turn off until next calculation is done.
    */
    console_printf("dwm_status_get - not implemented.\n");
    return ERR_NOT_IMPLEMENTED;
}

DWM_RETVAL dwm_int_cfg(DWM_INT_CFG * cfg)
{
    /*
    This API function enable interrupt generation for various events. Interrupts/events are
    communicated to the user by setting of dedicated GPIO pin (pin 19: READY), user can than read the   
    status (dwm_status_get) and react according to the new status. The status is cleared when read.
    This call is available only on UART/SPI interfaces. This call does a write to internal flash in case of
    new value being set, hence should not be used frequently as can take, in worst case, hundreds of
    milliseconds.
    */
    console_printf("dwm_int_cfg - not implemented.\n");
    return ERR_NOT_IMPLEMENTED;
}




DWM_RETVAL dwm_init_tag()
{
    int rc;
    console_printf("Initializing DWM1001\n");
    os_time_delay(OS_TICKS_PER_SEC*5);

    // Init CS pin
    rc = hal_gpio_init_out(CS_PIN, 1);
    if (rc != OS_OK) {
        console_printf("CS_PIN init error: %d\n", rc);
        return ERR_SPI;
    }

    // Config SPI
    int spi_id;
    struct hal_spi_settings my_spi;

    my_spi.data_order = HAL_SPI_MSB_FIRST;
    my_spi.data_mode = HAL_SPI_MODE0;
    my_spi.baudrate = 8000;
    my_spi.word_size = HAL_SPI_WORD_SIZE_8BIT;
    spi_id = 0;
    rc = hal_spi_config(spi_id, &my_spi);
    if (rc != OS_OK) {
        console_printf("hal_spi_config error: %d\n", rc);
        return ERR_SPI;
    }
    hal_spi_set_txrx_cb(0, NULL, NULL);
    hal_spi_enable(0);

    // Config as tag
    dwm_tag_cfg_t cfg;
    cfg.accel_en = false;
    cfg.low_power_en = false;
    cfg.loc_eng_en = true;
    cfg.led_en = false;
    cfg.ble_en = true;
    cfg.fw_update_en = true;
    cfg.uwb_mode = UWB_MODE_ACTIVE;

    while (ERR_OK != dwm_cfg_tag_set(cfg))
    {
         os_time_delay(OS_TICKS_PER_SEC);
    }
    os_time_delay(OS_TICKS_PER_SEC);
   
    // Set update rate (moving and stationary)
    dwm_rate_t rate;
    rate.rate = 1;
    rate.stationary_rate = 1;
    while (ERR_OK != dwm_upd_rate_set(rate))
    {
        os_time_delay(OS_TICKS_PER_SEC);  
    }
    
    // Write to flash and reboot
    while (ERR_OK != dwm_reset()) {
        os_time_delay(OS_TICKS_PER_SEC);
    }

    // Wait 2s for node to reset
    os_time_delay(OS_TICKS_PER_SEC*2);

    return ERR_OK;
 }