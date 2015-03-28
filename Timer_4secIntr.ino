// This sketch demonstrates a timer using Atmega328's 16bit, Timer1.  
// Timer1 can trigger an interrupt every ~4sec or less
// The LED tied to Arduino pin 13 will toggle every time the 
// interrupt is triggered.

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif 

void setup(){
  
  // Set Arduino pin 13 as an output.
  sbi(DDRB, DDD5);
  
  // Disable interrupts globally (I-flag)
  cli();

  // Set Timer1 to normal operation mode, OC1A/OC1B disconnected.
  TCCR1A = 0;
  
  // Set Clear-Timer-on-Compare-Match (CTC) Mode
  // and set prescaler to 1024.
  TCCR1B = 0;
  sbi(TCCR1B,WGM12);
  sbi(TCCR1B,CS12);
  sbi(TCCR1B,CS10);
  
  // Clear Counter
  TCNT1  = 0;
  
  // Set Top value to max
  OCR1A = 65535;//(1024*(1+65535))/16M = 4.194sec

  // Enable timer interrupt
  sbi(TIMSK1,OCIE1A);

  // Enable interrupts globally
  sei();
}

ISR(TIMER1_COMPA_vect){
  PORTB ^= 32;
}

void loop(){
}
