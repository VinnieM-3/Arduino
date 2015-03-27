// Arduino Uno Analog-to-Digital Conversion (ADC)
// by setting the Atmega328 registers directly
// This is an example of how to adjust ADC speed
// and it demonstrates the effect on conversion accuracy.

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif 

const float extVolt = 5.14;  //voltage measured at Arduino's voltage regulator.

//set num conversions to compare speed or accuracy.
const int numIters = 10000; // compare speed.
//const int numIters = 1;   // compare accuracy.  Accuracy decreases as speed increases.

byte low;
byte high;
word res_w;
float res_f, tot_f;
int testRun = 128;
unsigned long startTime, endTime;

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

  // disable the Power Reduction ADC bit, PRADC
  cbi(PRR,PRADC); 
  
  // enable the ADC
  sbi(ADCSRA,ADEN);   
}

void loop() {

  //set the ADC Prescaler Select Bits
  switch(testRun)
  {
  case 128:
    sbi(ADCSRA,ADPS2);
    sbi(ADCSRA,ADPS1);    
    sbi(ADCSRA,ADPS0);
    break;
  case 64:
    sbi(ADCSRA,ADPS2);
    sbi(ADCSRA,ADPS1);    
    cbi(ADCSRA,ADPS0);
    break;     
  case 32:
    sbi(ADCSRA,ADPS2);
    cbi(ADCSRA,ADPS1);    
    sbi(ADCSRA,ADPS0);
    break; 
  case 16:
    sbi(ADCSRA,ADPS2);
    cbi(ADCSRA,ADPS1);    
    cbi(ADCSRA,ADPS0);
    break; 
  case 8:
    cbi(ADCSRA,ADPS2);
    sbi(ADCSRA,ADPS1);    
    sbi(ADCSRA,ADPS0);
    break;    
  case 4:
    cbi(ADCSRA,ADPS2);
    sbi(ADCSRA,ADPS1);    
    cbi(ADCSRA,ADPS0);
    break;  
  case 2:
    cbi(ADCSRA,ADPS2);
    cbi(ADCSRA,ADPS1);    
    sbi(ADCSRA,ADPS0);
    break;
  }

  tot_f = 0;
  startTime = millis();  
  for (int x=0;x<numIters;x++) {

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
    tot_f += extVolt*(float(res_w)/1023);
    ;
  }
  endTime = millis();      
  res_f = tot_f/float(numIters); 

  // print the result
  Serial.print("ADC Prescaler = ");
  Serial.print(testRun);
  Serial.print(", Frequency = ");  
  Serial.print(1.6E7/float(testRun),0); 
  Serial.println("Hz");
  
  Serial.print("Number of conversions = ");
  Serial.print(numIters);
  Serial.print(", Total Run Time = ");
  Serial.print(float((endTime-startTime))/1000, 3);
  Serial.println("secs");
  
  Serial.print("Avg Voltage = ");  
  Serial.print(res_f, 3);
  Serial.println("v\n"); 

  if (testRun > 2) {
    testRun = testRun/2;
  }
  else {
    testRun = 128;
  }

  delay(2000);
}
