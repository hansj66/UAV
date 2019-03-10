#pragma once
#include <stdint.h>

#define BNO080_ADDRESS 0x4B

//Registers
#define CHANNEL_COMMAND 0
#define CHANNEL_EXECUTABLE 1
#define CHANNEL_CONTROL 2
#define CHANNEL_REPORTS 3
#define CHANNEL_WAKE_REPORTS 4
#define CHANNEL_GYRO 5

// SH-2 Control channel commands
#define SHTP_REPORT_COMMAND_RESPONSE 0xF1       // [6.3.9]
#define SHTP_REPORT_COMMAND_REQUEST 0xF2        // [6.3.8]
#define SHTP_REPORT_FRS_READ_RESPONSE 0xF3      // [6.3.7]
#define SHTP_REPORT_FRS_READ_REQUEST 0xF4       // [6.3.6]
#define SHTP_REPORT_PRODUCT_ID_RESPONSE 0xF8    // [6.3.2]
#define SHTP_REPORT_PRODUCT_ID_REQUEST 0xF9     // [6.3.1]

#define SHTP_REPORT_SET_FEATURE_COMMAND 0xFD    // [6.5.4]

#define SHTP_REPORT_BASE_TIMESTAMP 0xFB         // [7.2.1]

// Sensors
#define SENSOR_REPORTID_ACCELEROMETER 0x01
#define SENSOR_REPORTID_GYROSCOPE 0x02
#define SENSOR_REPORTID_MAGNETIC_FIELD 0x03
#define SENSOR_REPORTID_LINEAR_ACCELERATION 0x04
#define SENSOR_REPORTID_ROTATION_VECTOR 0x05
#define SENSOR_REPORTID_GRAVITY 0x06
#define SENSOR_REPORTID_GAME_ROTATION_VECTOR 0x08
#define SENSOR_REPORTID_GEOMAGNETIC_ROTATION_VECTOR 0x09
#define SENSOR_REPORTID_TAP_DETECTOR 0x10
#define SENSOR_REPORTID_STEP_COUNTER 0x11
#define SENSOR_REPORTID_STABILITY_CLASSIFIER 0x13
#define SENSOR_REPORTID_PERSONAL_ACTIVITY_CLASSIFIER 0x1E

// Record IDs
#define FRS_RECORDID_ACCELEROMETER 0xE302
#define FRS_RECORDID_GYROSCOPE_CALIBRATED 0xE306
#define FRS_RECORDID_MAGNETIC_FIELD_CALIBRATED 0xE309
#define FRS_RECORDID_ROTATION_VECTOR 0xE30B

// Sensor hub commands [6.4]
#define COMMAND_ERRORS 1
#define COMMAND_COUNTER 2
#define COMMAND_TARE 3
#define COMMAND_INITIALIZE 4
#define COMMAND_DCD 6
#define COMMAND_ME_CALIBRATE 7
#define COMMAND_DCD_PERIOD_SAVE 9
#define COMMAND_OSCILLATOR 10
#define COMMAND_CLEAR_DCD 11

#define CALIBRATE_ACCEL 0
#define CALIBRATE_GYRO 1
#define CALIBRATE_MAG 2
#define CALIBRATE_PLANAR_ACCEL 3
#define CALIBRATE_ACCEL_GYRO_MAG 4
#define CALIBRATE_STOP 5

#define MAX_PACKET_SIZE 1024 
#define MAX_METADATA_SIZE 9 


void init_bno080();
