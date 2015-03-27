// Arduino Uno Analog-to-Digital Conversion (ADC)
// by setting the Atmega328 registers directly
// This is an example of a single conversion
// using the A0 port.

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif 

const float extVolt = 5.14;  //voltage measured at Arduino's voltage regulator.
byte low;
byte high;
word res_w;
float res_f;

// registers used for ADC:
// ADMUX:   REFS1  REFS0  ADLAR   -     MUX3   MUX2    MUX1    MUX0
// ADCSRA:  ADEN   ADSC   ADATE  ADIF   ADIE   ADPS2   ADPS1   ADPS0

void setup() {
  
  Serial.begin(9600);
  
  // select channel 0 (pin A0 on Arduino Uno)
  ADMUX &= 0xF0;
  
  // select Avcc as the reference voltage
  cbi(ADMUX,REFS1);  
  sbi(ADMUX,REFS0);
  
  // right justify result
  cbi(ADMUX, ADLAR);
   
  // set the ADC clock frequency prescaler = 128, ~125KHz w/16MHz Crystal
  ADCSRA |= 0x07;
  
  // disable the Power Reduction ADC bit, PRADC
  cbi(PRR,PRADC); 
  
  // enable the ADC
  sbi(ADCSRA,ADEN);
}

void loop() { 

  // start a conversion
  sbi(ADCSRA,ADSC);
  
  // wait for the conversion to complete
  while( ( ADCSRA & (1<<ADIF) ) == 0 );
  
  // read low and high values
  low = ADCL;  // read low byte first
  high = ADCH; // read high byte
  
  // clear interrupt flag
  sbi(ADCSRA,ADIF);

  // combine high / low into single value
  res_w = word(high,low); 
  
  // convert to a voltage value
  res_f = extVolt*(float(res_w)/1023); 
  
  // print the result
  Serial.print("Voltage=");  
  Serial.println(res_f, DEC); 
  
  // delay one sec and start over.
  delay(1000);
}
