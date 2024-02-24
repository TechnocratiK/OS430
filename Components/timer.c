
#include "common.h"
#include "timer.h"

int ticks = 0;
int seconds = 0;

void timer_init(void) {
  timer_reset();
	
	TACCR2 = TAR + 512;   // Interrupt every 256/32768 = 1/128 = 0.0078125 seconds
	TACCTL2 |= CCIE;   // CCR2 interrupt enabled
}
void timer_reset(void){
	seconds = 0;
  ticks = 0;
}

/**
 * Iterrupt handler for TimerA
 *
 */
void timerA_wakeup (void) __interrupt[TIMERA1_VECTOR] {
  int vector = TAIV;

  if (vector == 0x2){	//from Wait function
    _BIC_SR_IRQ(CPUOFF);
  }
  else if(vector == 0x4) {	//for get_ticks function
    TACCR2 += 512;
    ticks++;
    if(ticks & 0x40) { // every (64*512/32768) = 1 second
      seconds++;
      ticks = 0;
    }
  }
}

/**
 * Wait a given number of clock ticks by using TimerA
 *
 */
void timer_wait(int t){
  TACCR1 = TAR + t; 
  TACCTL1 |= CCIE;	// enable timer A1 
  _BIS_SR(CPUOFF);
  TACCTL1 &= ~CCIE;	//disable timer A1
}

int timer_get_ticks(void) {
	return ticks;
}
int timer_get_seconds(void) {
	return seconds;
}

