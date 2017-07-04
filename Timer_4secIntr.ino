/* This sketch demonstrates a timer using Atmega328's 16bit, Timer1.
 * Timer1 can trigger an interrupt every ~4sec or less 
 * The LED tied to Arduino pin 13 will toggle every time the 
 * interrupt is triggered.
 */

void setup(){
  
  // Set builtin LED as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Disable interrupts globally
  cli();

  // Set Timer1 to normal operation mode, OC1A/OC1B disconnected.
  TCCR1A = 0;
  

  /* Set Clear-Timer-on-Compare-Match (CTC) Mode (WGM12) and 
   * prescaler (CS12-CS10) to 1024 so  we can set max time interval.
   * 
   * CS12  CS11  CS10  Description
   *  0     0     1    No prescaling
   *  0     1     0    clk/8
   *  0     1     1    clk/64
   *  1     0     0    clk/256
   *  1     0     1    clk/1024
   */
  TCCR1B = (1<<WGM12) | (1<<CS12) | (1<<CS10);  // up to 4.194secs
  //TCCR1B = (1<<WGM12) | (1<<CS12);            // up to 1.049secs
  

  // Set Top value to maximum value for a 16bit register
  OCR1A = 65535; // (1024*(1+65535))/16MHz = 4.194sec
  
  // Reset counter.  When counter matches OCR1A interrupt will trigger 
  TCNT1 = 0;
  
  // Enable timer interrupt
  TIMSK1 |= (1<<OCIE1A);

  // Enable interrupts globally
  sei();
}

ISR(TIMER1_COMPA_vect){
  PINB |= (1<<PINB5); // Toggle builtin LED pin
}

void loop(){
}
