/*
 * This sketch demonstrates the Watchdog Timer 
 * along with the Interrupt and System Reset modes.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 * 
 */


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
  
  /* Now choose a timer mode and prescaler from the choices below:
  
      WDTCSR Register Notes:

      WDE WDIE Mode
       0   1   Interrupt Mode Interrupt
       1   0   System Reset Mode Reset
       1   1   Interrupt and System Reset
  
      WDP3 WDP2 WDP1 WDP0
       0    0    0    0     16 msec
       0    0    0    1     32 msec
       0    0    1    0     64 msec
       0    0    1    1    125 msec
       0    1    0    0    250 msec
       0    1    0    1    500 msec
       0    1    1    0      1 sec
       0    1    1    1      2 sec
       1    0    0    0      4 sec
       1    0    0    1      8 sec
 */

  // Uncomment one of these examples or create your own:
  //WDTCSR = (1<<WDIE) | (1<<WDP2) | (1<<WDP1) | (1<<WDP0); // 2sec, Interrupt Mode
  WDTCSR = (1<<WDIE) | (1<<WDP3);                         // 4sec, Interrupt Mode 
  //WDTCSR = (1<<WDE) | (1<<WDP2) | (1<<WDP1) | (1<<WDP0);  // 2sec, System Reset Mode 
  //WDTCSR = (1<<WDE) | (1<<WDP3);                          // 4sec, System Reset Mode

 
  // Enable interrupts globally
  sei();
}

void loop()
{
  /* Resetting the Watchdog (i.e. running wdt_reset()) before the Watchdog timer 
   * expires will prevent the interrupt and/or system reset. Commenting out 
   * wdt_reset() will cause either an interrupt and/or system reset. A system reset 
   * will cause x to start back at 1, while an interrupt will simply display the 
   * "INTERRUPT!!!" message and continue the x loop where it left off.
   */
   
  delay(1000);
  Serial.println(++x); //print incrementer value
  //wdt_reset();
}

ISR(WDT_vect)
{
  Serial.println("INTERRUPT!!!");
}
