// This sketch demonstrates some of the AtMega328's PWM modes.
// It can be used to study the differences between Fast, Phase Correct, 
// and Phase/Frequency Correct PWM Modes, but an oscilloscope is essential.
// Except for CTC Mode, all other modes use Arduino pin 9 to display
// the PWM period in some manner. For Fast mode pin 9 toggles with each period.
// For Phase and Phase/Frequency mode, pin 9 toggles on the slope so that 
// you can differentiate between the rising and falling of the counter.
// Via the serial interface you can select a mode and turn on/off
// the activity.  By activity I mean it will vary the Top and/or OCR1B values
// so that the advantages and disadvantages of each mode can be seem on your scope.

#define ACTIVITY_OFF 0
#define ACTIVITY_ON 1
#define MODE_CTC_TOP_OCR1A 2
#define MODE_FAST_TOP_ICR1 3
#define MODE_FAST_TOP_OCR1A 4
#define MODE_PHASE_TOP_OCR1A 5
#define MODE_PHASE_FREQ_TOP_OCR1A 6

int PWM_Mode = -1;
int newMode;
unsigned long tick;
unsigned long tickTop;
boolean activity = false;

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
      newMode = constrain(newMode, 0, 6);
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
        case ACTIVITY_OFF:
          activity = false;
          Serial.println("\n\n\nActivity Turned Off\n");   
          break;         
         case ACTIVITY_ON:
          activity = true;
          Serial.println("\n\n\nActivity Turned On\n");   
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
  else if (activity){
    tick++;
  }
}

void printMenu() {
  Serial.println("To change PWM mode enter a number and hit Return/Enter:");
  Serial.println("0 = Activity_Off"); 
  Serial.println("1 = Activity_On");   
  Serial.println("2 = CTC Mode with Top=OCR1A");
  Serial.println("3 = Fast PWM Mode with Top=ICR1");
  Serial.println("4 = Fast PWM Mode with Top=OCR1A1");
  Serial.println("5 = Phase Correct Mode with Top=OCR1A");
  Serial.println("6 = Phase and Frequency Correct Mode with Top=OCR1A");
}

ISR(TIMER1_OVF_vect) {
  sbi(PINB,PINB1);
}

ISR(TIMER1_COMPA_vect) {
  sbi(PINB,PINB1);
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
    OCR1A = 4095;

    // Set OCR1A pin as output and configure interrupts
    sbi(DDRB,DDB1);
    cbi(TIMSK1,OCIE1A);
    cbi(TIMSK1,TOIE1);

    PWM_Mode = MODE_CTC_TOP_OCR1A;
    Serial.println("\n\n\nNow running CTC Mode with Top=OCR1A.");
    Serial.println("CTC Mode does NOT support double buffering.");
    Serial.println("Going from a high to low OCR1A value can cause the");
    Serial.println("counter to miss the compare match at the Top");
    Serial.println("and result in the counter having to wrap around.\n");  
    tick = 0;
    tickTop = 20;
  }
  else if (tick == tickTop) {
    if (OCR1A == 4095) {
      OCR1A = 2047;
    }
    else {
      OCR1A = 4095;
    }
    tick = 0;
  }
}

void FastTopICR1() {

  if (MODE_FAST_TOP_ICR1 != PWM_Mode) {  

    // OCR1A Disconnected
    cbi(TCCR1A,COM1A0);
    cbi(TCCR1A,COM1A1);

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
    ICR1 = 4095;

    OCR1B = 2047;

    // Set OCR1B pin as output
    sbi(DDRB,DDB2);
    
    // Set OCR1A pin as output and configure interrupts
    sbi(DDRB,DDB1);
    cbi(TIMSK1,OCIE1A);
    sbi(TIMSK1,TOIE1);

    PWM_Mode = MODE_FAST_TOP_ICR1;
    Serial.println("\n\n\nNow running Fast PWM Mode with Top=ICR1.");
    Serial.println("ICR1 is not double buffered.");
    Serial.println("Going from a high to low ICR1 value can cause the");
    Serial.println("counter to miss the compare match at the Top");
    Serial.println("and result in the counter having to wrap around.\n");  
    tick = 0;
    tickTop = 20;
  }
  else if (tick == tickTop) {
    if (ICR1 == 4095) {
      OCR1B = 1023;
      ICR1 = 2047;
    }
    else {
      ICR1 = 4095;
      OCR1B = 2047;
    }
    tick = 0;
  }
}

void FastTopOCR1A() {

  if (MODE_FAST_TOP_OCR1A != PWM_Mode) {  

    // OCR1A Disconnected
    cbi(TCCR1A,COM1A0);
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
    OCR1A = 4095;

    OCR1B = 2047;

    // Set OCR1B pin as output
    sbi(DDRB,DDB2);
    
    // Set OCR1A pin as output and configure interrupts
    sbi(DDRB,DDB1);
    cbi(TIMSK1,OCIE1A);
    sbi(TIMSK1,TOIE1);

    PWM_Mode = MODE_FAST_TOP_OCR1A;
    Serial.println("\n\n\nNow running Fast PWM Mode with Top=OCR1A.");
    Serial.println("OCR1A is being used to set the Top and it is");
    Serial.println("double buffered so there is no problem with");
    Serial.println("wrap around like when using ICR1 as Top.");
    Serial.println("However, the output is unsymmetrical.\n");   
    tick = 0;
    tickTop = 20;
  }
  else if (tick == tickTop) {
    if (OCR1A == 4095) {
      OCR1B = 1023;      
      OCR1A = 2047;
    }
    else {
      OCR1A = 4095;
      OCR1B = 2047;      
    }
    tick = 0;
  }
}

void PhaseTopOCR1A() {

  if (MODE_PHASE_TOP_OCR1A != PWM_Mode) {  

    // OCR1A Disconnected
    cbi(TCCR1A,COM1A0);
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

    OCR1B = 1024;

    // Set OCR1B pin as output
    sbi(DDRB,DDB2);
    
    // Set OCR1A pin as output and configure interrupts
    sbi(DDRB,DDB1);
    sbi(TIMSK1,OCIE1A);
    sbi(TIMSK1,TOIE1);
    
    PWM_Mode = MODE_PHASE_TOP_OCR1A;
    Serial.println("\n\n\nNow running Phase Correct Mode with Top=OCR1A.");
    Serial.println("OCR1A is set as Top and is double buffered.");
    Serial.println("The output is symmetrical as long as the");
    Serial.println("Top value is not changed.  Changing Top");
    Serial.println("will make the output unsymmetrical.\n"); 
    tick = 0;
    tickTop = 20;
  }
  else if (tick == tickTop) {
    if (OCR1A == 2048) {
      OCR1B = 512;
      OCR1A = 1024;
    }
    else {
      OCR1A = 2048;
      OCR1B = 1024;      
    }
    tick = 0;
  }
}

void PhaseFreqTopOCR1A() {

  if (MODE_PHASE_FREQ_TOP_OCR1A != PWM_Mode) {  

    // OCR1A Disconnected
    cbi(TCCR1A,COM1A0);
    cbi(TCCR1A,COM1A1);

    // OCR1B Set on Compare Match when up-counting
    sbi(TCCR1A,COM1B0); 
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

    OCR1B = 1024;

    // Set OCR1B pin as output
    sbi(DDRB,DDB2);
    
    // Set OCR1A pin as output and configure interrupts
    sbi(DDRB,DDB1);
    sbi(TIMSK1,OCIE1A);
    sbi(TIMSK1,TOIE1);
    
    PWM_Mode = MODE_PHASE_FREQ_TOP_OCR1A;
    Serial.println("\n\n\nNow running Phase and Frequency Correct Mode with Top=OCR1A.");
    Serial.println("Even with OCR1A changing continuously, output stays");   
    Serial.println("symmetrical because Top value changes occur at");
    Serial.println("bottom/end of the period.\n"); 
    tick = 0;
    tickTop = 20;
  }
  else if (tick == tickTop) {
    if (OCR1A == 2048) {
      OCR1A = 1024;
      OCR1B = 512;
    }
    else {
      OCR1A = 2048;
      OCR1B = 1024;      
    }
    tick = 0;
  }
}
