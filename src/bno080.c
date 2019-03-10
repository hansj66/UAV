#include "bno080.h"

#include "hal/hal_i2c.h"
#include "mcu/nrf52_hal.h"
#include "console/console.h"
#include "os/os.h"
#include "i2c.h"

const uint32_t bno_time_interval = OS_TICKS_PER_SEC;

static struct os_callout bno_callout;

  // TODO: soft reset

#define RX_BUFFER_SIZE 1024
#define BECAUSE_RESERVED 0
uint8_t RX_BUFFER[RX_BUFFER_SIZE];

#define SHTP_HEADER_SIZE 4
uint8_t SHTP_HEADER[SHTP_HEADER_SIZE];
#define PACKET_LSB_INDEX 0
#define PACKET_MSB_INDEX 1
#define CHANNEL_NUMBER_INDEX 2
#define SEQUENCE_NUMBER_INDEX 3

uint8_t txSequenceNumber = 0;


void bno_command(uint8_t command, bool flush_result)
{

    
    int rc;
    uint8_t i2c_command[] = {command, BECAUSE_RESERVED};
    struct hal_i2c_master_data txData = {
        .len = sizeof(i2c_command),
        .address = BNO080_ADDRESS,
        .buffer = i2c_command,
    };



//    Ta inn kommandolengde som argument. 
//    Pakk eget TXbuffer med SHTP_HEADER + kommando


    rc = hal_i2c_master_write(1, &txData, bno_time_interval, 1);
    if (OS_OK != rc)
        console_printf("I2C Write failed while executing command...\n");

    struct hal_i2c_master_data i2c_shtp_header = {
        .len = SHTP_HEADER_SIZE,
        .address = BNO080_ADDRESS,
        .buffer = SHTP_HEADER,
    };

    rc = hal_i2c_master_read(1, &i2c_shtp_header, bno_time_interval, 1);
    if (OS_OK != rc)
        console_printf("I2C Read failed...\n");

   
    if (flush_result)
    {
       // uint8_t dump[0];
        
    /*    struct hal_i2c_master_data bno_junk = {
            .len = 1,
            .address = BNO080_ADDRESS,
            .buffer = dump,
        };
*/
      //  skriv om denne. er pakker med SHTP-header som må churnes, ikke egne byte

       // vurder å bytte pullupresistorer hvis i2c-frekvensen er for høy

      //  bestill nytt dronekort. Kjør testrapport i Altium først
/*
        while (OS_OK  = hal_i2c_master_read(1, &i2c_shtp_header, bno_time_interval, 1))
        {
            // Ugly hack. Just chew through garbage on the bus
        }
        */
    }
    else {
        uint16_t packet_size = ((uint16_t)SHTP_HEADER[PACKET_MSB_INDEX] << 8 | SHTP_HEADER[PACKET_LSB_INDEX]);
        console_printf("Packet size: %d\n", (int)packet_size);
        console_printf("Channel number: %d\n", (int)SHTP_HEADER[CHANNEL_NUMBER_INDEX]);
        console_printf("Sequence number: %d\n", (int)SHTP_HEADER[SEQUENCE_NUMBER_INDEX]);

    }
}

static void reset_bno_callout() {
    int err = os_callout_reset(&bno_callout, bno_time_interval);
    if (err != OS_OK) {
        console_printf("BNO os_callout_reset error: %d\n", err);
    }
}

static void bno_event_callback(struct os_event* event) {
    
    /*
    uint8_t data[10] = { 0,0,0,0,0,0,0,0,0,0 };
    // I2C address is 0x4A or 0x4B
    struct hal_i2c_master_data i2c_data = {
        .len = sizeof(data),
        .address = BNO080_ADDRESS,
        .buffer = data,
    };

    uint8_t data_txBuffer[] = {0xF9, 0};
    struct hal_i2c_master_data txData = {
        .len = sizeof(data_txBuffer),
        .address = BNO080_ADDRESS,
        .buffer = data_txBuffer,
    };
    int rc;
    rc = hal_i2c_master_write(I2C_NUM, &txData, bno_time_interval, 1);
    if (OS_OK != rc)
        console_printf("I2C Write failed...\n");
  
    rc = hal_i2c_master_read(I2C_NUM, &i2c_data, bno_time_interval, 1);
    if (OS_OK != rc)
        console_printf("I2C Read failed...\n");

    console_printf("%02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X\n", data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9]);
*/
    reset_bno_callout();
}

void init_bno080() {
    console_printf("Initializing BNO080...\n");

    init_i2c();
   // bno_command(SHTP_REPORT_PRODUCT_ID_REQUEST);

    os_callout_init(&bno_callout, os_eventq_dflt_get(), bno_event_callback, NULL);
    reset_bno_callout();
}