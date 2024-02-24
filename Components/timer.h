/**
 * Timer H file for wait and get_ticks function
 *
 * Expects:
 * 1. TimerA to be running at 32768Hz
 * 2. TimerA counting up and interrupt A1 is not in use
 * 3. TimerA will be run continuously
 */


#ifndef __INCLUDE_TIMER_H
#define __INCLUDE_TIMER_H

void timer_init(void);
void timer_wait(int ticks);

void timer_reset(void);
int timer_get_ticks(void);
int timer_get_seconds(void);

#endif
