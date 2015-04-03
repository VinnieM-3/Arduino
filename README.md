# Arduino Sketches

**ArduinoISP_NoP.ino** - This sketch is a modified version of the standard ArduinoISP.ino that permits the user to program an Atmega328 (without the "P" suffix) and not receive a wrong signature error or require the user to modify the avrdude config file.

**ADC_SingleConversion.ino** - This sketch demonstrates a single ADC conversion by setting Atmega328 registers directly.

**ADC_FreeRunning.ino** - This sketch demonstrates running the ADC in Free Running mode by setting the Atmega328 registers directly.

**ADC_NoiseReduction.ino** - This sketch demonstrates the ADC Noise Reduction mode by setting the Atmega328 registers directly.

**ADC_Frequency.ino** - This sketch demonstrates how to adjust ADC speed and also demonstrates the effect of speed on conversion accuracy.

**PWM_Fast_Mode.ino** - This sketch demonstrates Fast PWM Mode, 10-bit resolution, 1024 steps, on Arduino pins 9 and 10.  Arduino pin 9 will increase in ~1/2v (10% duty cycle) increments and pin 10 will decrease.

**TIMER_4secIntr.ino** - This sketch demonstrates Timer1 (Atmega328's 16bit timer).  Timer1 can trigger an interrupt every ~4sec or less. The LED tied to Arduino pin 13 will toggle each time the interrupt is triggered (~4sec).

**WDT_Simple.ino** - This sketch demonstrates the Watchdog Timer in both Interrupt Mode and System Reset Mode.

**ReadDigitalViaRegister.ino** - This sketch demonstrates reading a digital input using registers.

**PWM_Modes.ino** - This sketch demonstrates some of the AtMega328's PWM modes.  It can be used to study the differences between Fast, Phase Correct, and Phase/Frequency Correct PWM Modes, but an oscilloscope is essential.
