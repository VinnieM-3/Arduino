// This sketch demonstrates the Watchdog Timer in both Interrupt Mode
// and System Reset Mode.

#include <avr/wdt.h> 

unsigned long x=0;

void setup()
{
  Serial.begin(9600);
  
  // Disable interrupts globally
  cli();
  
  // Reset Watchdog Timer
  MCUSR &= ~(1<<WDRF);  
  
  // Prepare watchdog for config change.
  WDTCSR |= (1<<WDCE) | (1<<WDE);
  
  // Now choose timer mode and prescaler
  WDTCSR = (1<<WDIE) | (1<<WDP3);  // 4sec, Interrupt Mode
  //WDTCSR = (1<<WDE) | (1<<WDP3);  // 4sec, System Reset Mode
  //WDTCSR = (1<<WDIE) | (1<<WDP2) | (1<<WDP1) | (1<<WDP0);  // 2sec, Interrupt Mode
  //WDTCSR = (1<<WDE) | (1<<WDP2) | (1<<WDP1) | (1<<WDP0);  // 2sec, System Reset Mode 
   
  // Enable interrupts globally
  sei();	
} 

void loop()
{
  delay(1000);
  Serial.println(++x); 
  
  // Resetting the timer in less than 4sec will prevent the interrupt or system reset.
  // Commenting out the reset will cause either an interrupt or system reset.
  // System reset will cause x to start back at 1, while an interrupt will
  // simply display the "INTERRUPT!!!" message and continue the loop where it left off.
  //wdt_reset();
}

ISR(WDT_vect)
{
  Serial.println("INTERRUPT!!!");
}
