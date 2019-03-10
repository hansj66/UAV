#ifndef _DWM1001_H_
#define _DWM1001_H_

#include <stdint.h>
#include <stdbool.h>

#define CS_PIN 26

#define SPI_BAUDRATE 500
    
#define MEASUREMENT_MODE 0
#define ACCELEROMETER_ENABLE (1 << 2)

#define LOW_POWER_ENABLE (1 << 7)
#define LOCATION_ENGINE_ENABLE (1 << 6)
#define LED_ENABLE (1 << 4)
#define BLE_ENABLE (1 << 3)
#define FIRMWARE_UPDATE_ENABLE (1 << 2)
#define UWB_MODE_OFFLINE 0
#define UWB_MODE_PASSIVE 1
#define UWB_MODE_ACTIVE (1 << 1)

#define STATUS_LOCATION_READY_BIT (1 << 0)
#define STATUS_UWBMAC_JOINED_BIT (1 << 1)

#define INT_LOCATION_READY_BIT (1 << 0)
#define INT_SPI_DATA_READY_BIT (1 << 1) 

enum dwm_status {
    STATUS_LOCATION_READY,
    STATUS_UWBMAC_JOINED
};
typedef enum dwm_status DWM_STATUS;

enum dwm_uwb_mode {
    OFFLINE = 0,
    PASSIVE = 1,
    ACTIVE = 2
};
typedef enum dwm_uwb_mode DWM_UWB_MODE;

enum dwm_int_cfg {
    INT_LOCATION_READY,
    INT_SPI_DATA_READY,
};
typedef enum dwm_int_cfg DWM_INT_CFG;

enum dwm_retval {
    ERR_OK = 0,
    ERR_UNKNOWN_COMMAND = 1,
    ERR_INTERNAL_ERROR = 2,
    ERR_INVALID_PARAMETER = 3,
    ERR_BUSY = 4,
    ERR_NOT_TLV_FRAME = 5,
    ERR_NOT_IMPLEMENTED = 6,
    ERR_SPI = 7
};
typedef enum dwm_retval DWM_RETVAL;

enum dwm_node_type {
    TAG = 0,
    ANCHOR = 1
};

enum dwm_pull_type {
    DWM_GPIO_PIN_NOPULL = 0,
    DWM_GPIO_PIN_PULLDOWN = 1,
    DWM_GPIO_PIN_PULLUP = 3,
};
typedef enum dwm_pull_type DWM_PULL_TYPE;

enum dwm_gpio_value {
    LOW = 0,
    HIGH = 1,
};
typedef enum dwm_gpio_value DWM_GPIO_VALUE;

#define RETURN_MESSAGE 0x40

typedef struct {
    uint32_t x;
    uint32_t y;
    uint32_t z;
    uint8_t qf;
} dwm_pos_t;

typedef struct {
    uint16_t rate;
    uint16_t stationary_rate;
} dwm_rate_t;

typedef struct {
    bool initiator;
    bool bridge;
    bool accel_en;
   bool low_power_en;
    bool loc_eng_en;
     bool led_en;
    bool ble_en;
    bool fw_update_en;
} dwm_node_cfg_t;

typedef struct {
    bool accel_en;
    bool low_power_en;
    bool loc_eng_en;
    bool led_en;
    bool ble_en;
    bool fw_update_en;
    DWM_UWB_MODE uwb_mode;
} dwm_tag_cfg_t;

typedef struct {
    bool initiator;
    bool bridge;
    bool led_en;
    bool ble_en;
    bool fw_update_en;
    bool uwb_mode;
} dwm_anchor_cfg_t;

typedef struct {
    dwm_pos_t position;
    uint8_t distance_count;
    uint64_t uwb_addresses[15];
    uint32_t distances[15];
    uint8_t quality_factors[15];
    uint8_t anchor_position_count;
    dwm_pos_t anchor_positions[15];
} dwm_loc_t;

typedef struct {
    uint32_t fw_version;
    uint32_t cfg_version;
    uint32_t hw_version;
} dwm_ver_t;

DWM_RETVAL dwm_pos_set(dwm_pos_t position);
DWM_RETVAL dwm_pos_get(dwm_pos_t * position);
DWM_RETVAL dwm_upd_rate_set(dwm_rate_t rate);
DWM_RETVAL dwm_upd_rate_get(dwm_rate_t * rate);
DWM_RETVAL dwm_cfg_tag_set(dwm_tag_cfg_t tag_cfg);
DWM_RETVAL dwm_cfg_anchor_set(dwm_anchor_cfg_t anchor_cfg);
DWM_RETVAL dwm_cfg_get(dwm_node_cfg_t * node_cfg);
DWM_RETVAL dwm_sleep();
DWM_RETVAL dwm_loc_get(dwm_loc_t * loc);
DWM_RETVAL dwm_baddr_set(uint64_t baddr);
DWM_RETVAL dwm_baddr_get(uint64_t * baddr);
DWM_RETVAL dwm_reset();
DWM_RETVAL dwm_ver_get(dwm_ver_t * ver);
DWM_RETVAL dwm_gpio_cfg_output(uint8_t gpio_idx, DWM_GPIO_VALUE gpio_value);
DWM_RETVAL dwm_gpio_cfg_input(uint8_t gpio_idx, DWM_PULL_TYPE pullup);
DWM_RETVAL dwm_gpio_value_set(uint8_t gpio_idx, DWM_GPIO_VALUE value);
DWM_RETVAL dwm_gpio_value_get(uint8_t gpio_idx, DWM_GPIO_VALUE * value);
DWM_RETVAL dwm_gpio_value_toggle(uint8_t gpio_idx);
DWM_RETVAL dwm_status_get(DWM_STATUS * status);
DWM_RETVAL dwm_int_cfg(DWM_INT_CFG * cfg);

DWM_RETVAL dwm_init_tag();

DWM_RETVAL dwmCommand(uint8_t * cmd, uint8_t length);

#endif