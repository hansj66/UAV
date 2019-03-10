// Nicked this code from https://nicisdigital.wordpress.com/2011/06/27/proportional-integral-derivative-pid-controller/

#include "PID.h"

void pid_zeroize(PID* pid) {
    // set prev and integrated error to zero
    pid->prev_error = 0;
    pid->int_error = 0;
}
 
void pid_update(PID* pid, double curr_error, double dt) 
{
    double diff;
    double p_term;
    double i_term;
    double d_term;
 
    // integration with windup guarding
    pid->int_error += (curr_error * dt);
    if (pid->int_error < -(pid->windup_guard))
        pid->int_error = -(pid->windup_guard);
    else if (pid->int_error > pid->windup_guard)
        pid->int_error = pid->windup_guard;
 
    // differentiation
    diff = ((curr_error - pid->prev_error) / dt);
 
    // scaling
    p_term = (pid->proportional_gain * curr_error);
    i_term = (pid->integral_gain     * pid->int_error);
    d_term = (pid->derivative_gain   * diff);
 
    // summation of terms
    pid->control = p_term + i_term + d_term;
 
    // save current error as previous error for next iteration
    pid->prev_error = curr_error;
}

void PID_Initialize(PID * pid)
{
    pid_zeroize(pid);
    pid->windup_guard = 0; 
    pid->proportional_gain = 1;
    pid->integral_gain = 0;
    pid->derivative_gain = 0;
}