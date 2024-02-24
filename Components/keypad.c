
#include "common.h"
#include "keypad.h"
#include "uart.h"

#define PORT1_L_TO_H	0x1
#define PORT1_H_TO_L	0x2

//If these change, may have to modify keypad_scan and keypad_lookup functions
#define PORT1_INPUT_PINS (BIT1 | BIT2 | BIT3)
#define PORT3_OUTPUT_PINS (BIT0 | BIT1 | BIT2 | BIT3)

void keypad_enable_interrupts(char dir);
void keypad_disable_interrupts(void);
char keypad_scan(void);
char keypad_lookup(int row, int col);

void keypad_init(void) {

  P1DIR &= ~PORT1_INPUT_PINS;	// Port 1 Input
  P1SEL &= ~PORT1_INPUT_PINS; // From GPIO pin (when its zero its using the gpio)

  P3DIR |= PORT3_OUTPUT_PINS;	// Port 3 Output
  P3SEL &= ~PORT3_OUTPUT_PINS;	// From pin

	// Init interrupts on Port 1 pins but leave disabled
  P1IFG =	0x0;
  P1IE 	&=	~PORT1_INPUT_PINS;
	
  TACCTL0 &= ~CCIE;     // TACCR0 interrupt enabled
  TACCR0 = 0;

  P1IE |= PORT1_INPUT_PINS;
}

/**
 * Get a character from the keypad or debug input
 */
char keypad_get_char(void) {
	char key = '0';
	
  P3OUT |= PORT3_OUTPUT_PINS;				//set P3 output high
	
	do {
		// Sleep until key is pressed in and sensed
		do {
			keypad_enable_interrupts(PORT1_L_TO_H);
		
			_BIS_SR(CPUOFF);
		} while (!(P1IN & PORT1_INPUT_PINS));
		//debug_printf("OMG");
		// Scan for key
		key = keypad_scan();
		//debug_printf("OMG2");
		
		//If key is not pressed when scan complete, ignore it
	} while(!(P1IN & PORT1_INPUT_PINS));
	
		//debug_printf("OMG3");
	
	// Sleep until Key released
	do {
		
		//debug_printf("OMG4");
		keypad_enable_interrupts(PORT1_H_TO_L);
		//debug_printf("OMGZ");
		_BIS_SR(CPUOFF);
		//debug_printf("OMGR");
		
	} while (P1IN & PORT1_INPUT_PINS);
	
		//debug_printf("OMG5");
	keypad_disable_interrupts();
  P3OUT &= ~PORT3_OUTPUT_PINS;				//set P3 output low to conserve power
	
  return key;
}



/**
 * Interrupt handler for PORT1
 * 
 * When a key is pressed/released, start the timer and disable interrupts on port one
 */
void keypad_pressed (void) __interrupt [PORT1_VECTOR] {
  keypad_disable_interrupts();
  P1IFG = 0x0;                      //clearing the interrupt flag
  // Set timer to go off based on current time
  TACCR0 = TAR + 1311;              //should be 0.040 s * (1/32768)
  //debug_printf("%d %d\n", TAR, TACCR0);
  TACCTL0 |= CCIE;	//enable timer A0
}


/**
 * Iterrupt handler for TimerA
 *
 */
void keypad_timerA_wakeup (void) __interrupt[TIMERA0_VECTOR] {
	int t = TAR;
        //debug_printf("%d %d\n", t, TACCR0);
  
	//TACCR0 = 0;	//disable timer A0
	TACCTL0 &= ~CCIE;	//disable timer A0
	_BIC_SR_IRQ(CPUOFF);
}


/**
 * Enable the interrupts on port 1 for the keypad in the direction specified
 */
void keypad_enable_interrupts(char dir) {

	if(dir == PORT1_L_TO_H) {
		P1IES &= ~PORT1_INPUT_PINS; 			//Set low to high interrupts
	}
	else {
		P1IES |= PORT1_INPUT_PINS; 			//Set high to low interrupts
	}
  //P1IE |= PORT1_INPUT_PINS;					//turning on interrupts
}

/**
 * Disable interrupts on the port 1 pins for the keypad
 */
void keypad_disable_interrupts(void) {

  //P1IE &= ~PORT1_INPUT_PINS;        //turning off interrupts
}


/**
 * Scan keypad to determine which key is pressed
 */
char keypad_scan(void) {
  int row, col;
	char keyValue = '0';
  
  P3OUT &= ~PORT3_OUTPUT_PINS;
  
  for (row = 1; row <= 8; row <<= 1) {
    P3OUT &= 0xF0;
    P3OUT |= row;
    for (col = 2; col <= 8; col <<= 1) {
      if (P1IN & col) {
        keyValue = keypad_lookup(row, col);
        P3OUT |= PORT3_OUTPUT_PINS;
        return keyValue;
      }
    }
    
  }
  P3OUT |= PORT3_OUTPUT_PINS;
  return '0';
}


/**
 * Look up a given key value based on a *bitwise* port value
 */
char keypad_lookup(int row, int col) {
  switch(row) {
    case 0x1: //P3.0
      switch(col) {
        case 0x2: //P1.1
          return '*';
        case 0x4: //P1.2
          return '0';
        case 0x8: //P1.3
          return '#';
      }
    case 0x2: //P3.1
      switch(col) {
        case 0x2: //P1.1
          return '7';
        case 0x4: //P1.2
          return '8';
        case 0x8: //P1.3
          return '9';
      }
    case 0x4: //P3.2
      switch(col) {
        case 0x2: //P1.1
          return '4';
        case 0x4: //P1.2
          return '5';
        case 0x8: //P1.3
          return '6';
      }
    case 0x8: //P3.3
      switch(col) {
        case 0x2: //P1.1
          return '1';
        case 0x4: //P1.2
          return '2';
        case 0x8: //P1.3
          return '3';
      }
  }
  return '0';
}

