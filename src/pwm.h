#ifndef _PWM_H_
#define _PWM_H_


/*
    Connector
    GND
    +5V
    26  PO.28
    25  PO.29
    24  PO.30
    23  PO.31
    22  PO.11
    21  PO.02
*/

//#define THROTTLE 28
#define ROLL 28
#define PITCH 29
#define YAW 31
#define THROTTLE 30


void updateThrust(int controlValue);





void initPWM();

#endif