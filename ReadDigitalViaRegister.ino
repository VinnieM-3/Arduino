/* This sketch demonstrates reading a digital input using registers.
 * Arduino pin 7 will be read and the LED on pin 13 will mirror pin 7's state.
 * Either connect pin 7 to ground using a current limiting resistor
 * or leave it open and the internal pullup will pull it high.
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


void setup() {
  
  // Set Arduino pin 7 as an input and turn on the pullup resistor.
  //DDRD &= ~(1<<DDD7);   // set pin 7 as an input
  //PORTD |= (1<<PORTD7); // activate internal pullup resistor
  pinMode(7, INPUT_PULLUP); // or set pin 7 using the Arduino function
  
  // Set Arduino pin 13 as an output
  //DDRB |= (1<<DDB5);
  pinMode(LED_BUILTIN, OUTPUT); // or set pin 13 using the Arduino function
}

void loop() {  
  // Read the value on pin 7 and set pin 13 accordingly.

  // simple if-else statement
  if ((1<<PORTD7) & PIND) {
    PORTB |= (1<<PORTB5); 
  } else {
    PORTB &= ~(1<<PORTB5);
  }

  // or put the if-else all on one line
  //if ((1<<PORTD7) & PIND) PORTB |= (1<<PORTB5); else PORTB &= ~(1<<PORTB5);

  // or you could use a ternary operator 
  //((1<<PORTD7) & PIND) ? PORTB |= (1<<PORTB5) : PORTB &= ~(1<<PORTB5);
}
