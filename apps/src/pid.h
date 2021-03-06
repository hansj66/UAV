#ifndef _PID_H
#define _PID_H

// Shamelessly ripped from the interwebs

typedef struct {
    double windup_guard;
    double proportional_gain;
    double integral_gain;
    double derivative_gain;
    double prev_error;
    double int_error;
    double control;
} PID;
 
void pid_zeroize(PID* pid);
void pid_update(PID* pid, double curr_error, double dt);
void PID_Initialize(PID * pid);
    
#endif