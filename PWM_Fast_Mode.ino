// Fast PWM Mode, 10-bit resolution, 1024 steps, on Arduino pins 9 and 10 (IC pins 15 and 16)
// Arduino pin 9 will increase in ~1/2v (10% duty cycle) increments and pin 10 will decrease.
// The frequency will be pretty low (16mHz/1024 = 15.6kHz)

int x, y;
int steps = 1023;
int inc = 102;

void setup() { 

  // TCCR1B Register
  // Bit                7      6      5      4      3      2      1      0
  // Bit Name        COM1A1 COM1A0 COM1B1 COM1B0  ----- ------  WGM11  WGM10
  // Initial Value      0      0      0      0      0      0      0      0
  // changed to         1      0      1      0      0      0      1      0
  TCCR1A = B10100010;

  // TCCR1B Register
  // Bit                7      6      5      4      3      2      1      0
  // Bit Name         ICNC1  ICES1  -----  WGM13  WGM12  CS12   CS11   CS10
  // Initial Value      0      0      0      0      0      0      0      0
  // changed to         0      0      0      1      1      0      0      1  
  // CS12,CS11,CS10 = (0,0,1) = System clock, no division
  TCCR1B = B00011001;

  // How many steps do we want (255, 511, 1023)?  Lower number of steps means fewer
  // possible output levels to choose, but higher frequency (i.e. easier to filter).
  ICR1 = steps;

  // set pins IC 15 and 16 as outputs (Arduino pins 9 and 10)
  DDRB = DDRB | B00000110;
  
  x = steps;
  y = 0;
} 

void loop()
{
  // x will slowly increase and then start over.
  x += inc;
  if (x > steps) x = 0;

  // y will slowly decrease and then start over.
  y -= inc;
  if (y < 0) y = steps;
  
  //set pin 15 (Arduino pin 9)
  OCR1A = x;
  
  //set pin 16 (Arduino pin 10)
  OCR1B = y;
  
  // Lets give some time to see what's happening with a scope or multimeter.  Measure DC voltage, frequency, and duty cycle.
  // IC pin 15 (Arduino pin 9) should rise in 1/2 volt increments and IC pin 16 (Arduino pin 10) should decrease in 1/2 volt 
  // increments assuming you are powering the chip with 5v.  Freq should be about 16KHz with a 16MHz crystal.
  delay (3000);    
}
