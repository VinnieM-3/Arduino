// This sketch demonstrates reading a digital input using registers.
// Arduino pin 7 will be read and the LED on pin 13 will mirror pin 7's state.
// Either connect pin 7 to ground (with a current limiting resistor) 
// or leave it open and the pullup will pull it high.

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif 

void setup() {
  
  // Set Arduino pin 7 as an input and turn on the pullup resistor.
  cbi(DDRD, DDD7); //pin 7 is an input
  sbi(PORTD,PORTD7); //pull up turned on
  
  // Set Arduino pin 13 as an output
  sbi(DDRB, DDB5);  
}

void loop() {  
  // Read the value on pin 7 and set pin 13 accordingly.
  //if ((1<<PORTD7) & PIND) sbi(PORTB,PORTB5); else cbi(PORTB, PORTB5); // if-else all on one line 
  ((1<<PORTD7) & PIND) ? sbi(PORTB,PORTB5) : cbi(PORTB, PORTB5);  // or use a ternary operator 
}
