// Arduino Uno Analog-to-Digital Conversion (ADC)
// by setting the Atmega328 registers directly
// This is an example of ADC Noise Reduction mode
// using an interrupt and the A0 port.

#include <avr/sleep.h>

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif 

const float extVolt = 5.137;  //voltage measured at Arduino's voltage regulator.
float res_f;

volatile byte low;
volatile byte high;
volatile word res_w;
volatile boolean newReading;

// registers used for ADC:
// ADMUX:   REFS1  REFS0  ADLAR   -     MUX3   MUX2    MUX1    MUX0
// ADCSRA:  ADEN   ADSC   ADATE  ADIF   ADIE   ADPS2   ADPS1   ADPS0

void setup() {

  Serial.begin(9600);

  // select channel 0 (pin A0 on Arduino Uno)
  ADMUX |= 0x00;
  
  // select Avcc as the reference voltage
  cbi(ADMUX,REFS1);  
  sbi(ADMUX,REFS0);
  
  // right justify result
  cbi(ADMUX, ADLAR);
   
  // set the ADC clock frequency prescaler = 128, ~125KHz w/16MHz Crystal
  ADCSRA |= 0x07;

  // disable the Power Reduction ADC bit, PRADC
  cbi(PRR,PRADC); 
  
  // enable the Interrupt  
  sbi(ADCSRA,ADIE);

  // enable the ADC
  sbi(ADCSRA,ADEN);

  // enable global interrupts
  sei();
 
  //set the adc low noise sleep mode
  set_sleep_mode(SLEEP_MODE_ADC); 
  
  //enter sleep mode
  sleep_mode();
}

ISR(ADC_vect) {
  // read low and high values
  low = ADCL;  // read low byte first
  high = ADCH; // read high byte
  
  // combine high / low into single value
  res_w = word(high,low);
  
  // set flag
  newReading = true;
}

void loop() {
  if (newReading) {
    
    // clear flag
    newReading = false;
        
    // convert to a voltage value
    res_f = extVolt*(float(res_w)/1023); 
    
    // print the result
    Serial.print("Voltage=");  
    Serial.println(res_f, DEC);

    // why not rest a moment? 
    delay(250);
    
    // set the adc low noise sleep mode
    set_sleep_mode(SLEEP_MODE_ADC); 
    
    // enter sleep mode again
    sleep_mode();
  }  
}
