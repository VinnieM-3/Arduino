// This sketch demonstrates many of the AtMega328's PWM modes.
// Uses Arduino pins 9 and 10.

#include <avr/wdt.h> 

#define MODE_CTC_TOP_OCR1A 0
#define MODE_FAST_TOP_ICR1 1
#define MODE_FAST_TOP_OCR1A 2
#define MODE_PHASE_TOP_OCR1A 3
#define MODE_PHASE_FREQ_TOP_OCR1A 4

int PWM_Mode = -1;
int newMode;
unsigned long tick;
unsigned long tickTop;

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif 

void setup() { 
  Serial.begin(9600);
  printMenu();
}

void loop()
{
  if (Serial.available() > 0) {  
    newMode = Serial.parseInt(); 
    if (Serial.read() == '\n') {
      newMode = constrain(newMode, 0, 4);
      if (newMode != PWM_Mode) {
        switch(newMode) {
        case MODE_CTC_TOP_OCR1A:
          CTCtopOCR1A();
          break;
        case MODE_FAST_TOP_ICR1:
          FastTopICR1();
          break;
        case MODE_FAST_TOP_OCR1A:
          FastTopOCR1A();
          break;
        case MODE_PHASE_TOP_OCR1A:
          PhaseTopOCR1A();
          break;
        case MODE_PHASE_FREQ_TOP_OCR1A:
          PhaseFreqTopOCR1A();
          break;
        }
        printMenu();
      }      
    }
  }

  if (tick == tickTop) {
    switch(PWM_Mode) {
    case MODE_CTC_TOP_OCR1A:
      CTCtopOCR1A();
      break;
    case MODE_FAST_TOP_ICR1:
      FastTopICR1();
      break;
    case MODE_FAST_TOP_OCR1A:
      FastTopOCR1A();
      break;
    case MODE_PHASE_TOP_OCR1A:
      PhaseTopOCR1A();
      break;
    case MODE_PHASE_FREQ_TOP_OCR1A:
      PhaseFreqTopOCR1A();
      break;
    }
  }
  else {
    tick++;
  }
}

void printMenu() {
  Serial.println("To change PWM Mode enter a number and hit Return/Enter:");  
  Serial.println("0 = MODE_CTC_TOP_OCR1A");
  Serial.println("1 = MODE_FAST_TOP_ICR1");
  Serial.println("2 = MODE_FAST_TOP_OCR1A1");
  Serial.println("3 = MODE_PHASE_TOP_OCR1A");
  Serial.println("4 = MODE_PHASE_FREQ_TOP_OCR1A");
}

void CTCtopOCR1A() {

  if (MODE_CTC_TOP_OCR1A != PWM_Mode) {  

    // Toggle OCR1A on Compare Match
    sbi(TCCR1A,COM1A0);
    cbi(TCCR1A,COM1A1);

    // OCR1B Disconnected  
    cbi(TCCR1A,COM1B0); 
    cbi(TCCR1A,COM1B1);

    // CTC Mode, TOP=OCR1A  
    cbi(TCCR1A,WGM10);  
    cbi(TCCR1A,WGM11);   
    sbi(TCCR1B,WGM12);
    cbi(TCCR1B,WGM13);

    // No prescaling
    sbi(TCCR1B,CS10);
    cbi(TCCR1B,CS11);
    cbi(TCCR1B,CS12);  

    // TOP
    OCR1A = 2047;

    // Set OCR1A pins as outputs
    sbi(DDRB,DDB1);

    PWM_Mode = MODE_CTC_TOP_OCR1A;
    Serial.println("\n\n\nNow running MODE_CTC_TOP_OCR1A.");
    Serial.println("CTC mode does NOT support double buffering.\n");
    Serial.println("Going from a high to low OCR1A value can cause counter to miss compare match at Top");
    Serial.println("and result in counter having to wrap around.");  
    tick = 0;
    tickTop = 20;
  }
  else if (tick == tickTop) {
    if (OCR1A == 2047) {
      OCR1A = 1023;
    }
    else {
      OCR1A = 2047;
    }
    tick = 0;
  }
}

void FastTopICR1() {

  if (MODE_FAST_TOP_ICR1 != PWM_Mode) {  

    // OCR1A Non-Inverting Mode
    cbi(TCCR1A,COM1A0);
    sbi(TCCR1A,COM1A1);

    // OCR1B Non-Inverting Mode
    cbi(TCCR1A,COM1B0); 
    sbi(TCCR1A,COM1B1);

    // Fast PWM, TOP=ICR1 
    cbi(TCCR1A,WGM10);  
    sbi(TCCR1A,WGM11);   
    sbi(TCCR1B,WGM12); 
    sbi(TCCR1B,WGM13);   

    // No prescaling
    sbi(TCCR1B,CS10);
    cbi(TCCR1B,CS11);
    cbi(TCCR1B,CS12);  

    // TOP
    ICR1 = 1023;

    OCR1A = 511;
    OCR1B = 127;

    // Set OCR1A OCR1B pins as outputs
    sbi(DDRB,DDB1);
    sbi(DDRB,DDB2);

    PWM_Mode = MODE_FAST_TOP_ICR1;
    Serial.println("\n\n\nNow running MODE_FAST_TOP_ICR1.");
    Serial.println("ICR1 is NOT double buffered and changed continuously.");
    Serial.println("Going from a high to low ICR1 value can cause counter to miss");
    Serial.println("compare match at Top and result in counter having to wrap around.");   
    Serial.println("It is better to use OCR1A as Top or switch to Phase/Frequency mode.\n");  
    tick = 0;
    tickTop = 20;
  }
  else if (tick == tickTop) {
    if (ICR1 == 1023) {
      OCR1A = 255;
      ICR1 = 511;
    }
    else {
      ICR1 = 1023;
      OCR1A = 511;
    }
    tick = 0;
  }
}

void FastTopOCR1A() {

  if (MODE_FAST_TOP_OCR1A != PWM_Mode) {  

    // Toggle OCR1A on Compare Match
    sbi(TCCR1A,COM1A0);
    cbi(TCCR1A,COM1A1);

    // OCR1B Non-Inverting Mode
    cbi(TCCR1A,COM1B0); 
    sbi(TCCR1A,COM1B1);

    // Fast PWM, TOP=OCR1A 
    sbi(TCCR1A,WGM10);  
    sbi(TCCR1A,WGM11);   
    sbi(TCCR1B,WGM12); 
    sbi(TCCR1B,WGM13);   

    // No prescaling
    sbi(TCCR1B,CS10);
    cbi(TCCR1B,CS11);
    cbi(TCCR1B,CS12);  

    // Top
    OCR1A = 1023;

    OCR1B = 255;

    // Set OCR1A OCR1B pins as outputs
    sbi(DDRB,DDB1);
    sbi(DDRB,DDB2);

    PWM_Mode = MODE_FAST_TOP_OCR1A;
    Serial.println("\n\n\nNow running MODE_FAST_TOP_OCR1A.");
    Serial.println("OCR1A is Top and is double buffered and changed continuously.");
    Serial.println("No problem with wrap around like when using ICR1 as Top\n");   
    tick = 0;
    tickTop = 20;
  }
  else if (tick == tickTop) {
    if (OCR1A == 1023) {
      OCR1A = 511;
    }
    else {
      OCR1A = 1023;
    }
    tick = 0;
  }
}

void PhaseTopOCR1A() {

  if (MODE_PHASE_TOP_OCR1A != PWM_Mode) {  

    // Toggle OCR1A on Compare Match
    sbi(TCCR1A,COM1A0);
    cbi(TCCR1A,COM1A1);

    // OCR1B Clear on Compare Match when up-counting
    cbi(TCCR1A,COM1B0); 
    sbi(TCCR1A,COM1B1);

    // Phase PWM, TOP=OCR1A 
    sbi(TCCR1A,WGM10);  
    sbi(TCCR1A,WGM11);   
    cbi(TCCR1B,WGM12); 
    sbi(TCCR1B,WGM13);   

    // No prescaling
    sbi(TCCR1B,CS10);
    cbi(TCCR1B,CS11);
    cbi(TCCR1B,CS12);  

    // Top
    OCR1A = 2048;

    OCR1B = 512;

    // Set OCR1A OCR1B pins as outputs
    sbi(DDRB,DDB1);
    sbi(DDRB,DDB2);

    PWM_Mode = MODE_PHASE_TOP_OCR1A;
    Serial.println("\n\n\nNow running MODE_PHASE_TOP_OCR1A.");
    Serial.println("OCR1A is Top and is double buffered and changed continuously.");
    Serial.println("However, changing OCR1A results in unsymmetrical output because"); 
    Serial.println("value changes at Top and PWM period starts and ends at TOP\n");    
    tick = 0;
    tickTop = 20;
  }
  else if (tick == tickTop) {
    if (OCR1A == 2048) {
      OCR1A = 1024;
    }
    else {
      OCR1A = 2048;
    }
    tick = 0;
  }
}

void PhaseFreqTopOCR1A() {

  if (MODE_PHASE_FREQ_TOP_OCR1A != PWM_Mode) {  

    // Toggle OCR1A on Compare Match
    sbi(TCCR1A,COM1A0);
    cbi(TCCR1A,COM1A1);

    // OCR1B Clear on Compare Match when up-counting
    cbi(TCCR1A,COM1B0); 
    sbi(TCCR1A,COM1B1);

    // Phase and Frequency PWM, TOP=OCR1A 
    sbi(TCCR1A,WGM10);  
    cbi(TCCR1A,WGM11);   
    cbi(TCCR1B,WGM12); 
    sbi(TCCR1B,WGM13);   

    // No prescaling
    sbi(TCCR1B,CS10);
    cbi(TCCR1B,CS11);
    cbi(TCCR1B,CS12);  

    // Top
    OCR1A = 2048;

    OCR1B = 512;

    // Set OCR1A OCR1B pins as outputs
    sbi(DDRB,DDB1);
    sbi(DDRB,DDB2);

    PWM_Mode = MODE_PHASE_FREQ_TOP_OCR1A;
    Serial.println("\n\n\nNow running MODE_PHASE_FREQ_TOP_OCR1A.");
    Serial.println("Even with OCR1A changing continuously, output is always");   
    Serial.println("symmetrical because value changes at bottom.\n"); 
    tick = 0;
    tickTop = 20;
  }
  else if (tick == tickTop) {
    if (OCR1A == 2048) {
      OCR1A = 1024;
    }
    else {
      OCR1A = 2048;
    }
    tick = 0;
  }
} 
