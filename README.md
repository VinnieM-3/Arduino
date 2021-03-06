Arduino example sketches demonstrating ADC Single Conversion, ADC in Free Running mode, ADC with Noise Reduction, ADC with Frequency tuning, PWM in Fast mode, Other PWM modes, 16bit Timer1 example, and a Watchdog example using an interrupt and/or system reset.  

The GitHub Project Page is [here](http://vinniem-3.github.io/Arduino/).

**[ADC_SingleConversion.ino](https://github.com/VinnieM-3/Arduino/blob/master/ADC_SingleConversion.ino)** - This sketch demonstrates a single ADC conversion by setting Atmega328 registers directly.

**[ADC_FreeRunning.ino](https://github.com/VinnieM-3/Arduino/blob/master/ADC_FreeRunning.ino)** - This sketch demonstrates running the ADC in Free Running mode by setting the Atmega328 registers directly.

**[ADC_NoiseReduction.ino](https://github.com/VinnieM-3/Arduino/blob/master/ADC_NoiseReduction.ino)** - This sketch demonstrates the ADC Noise Reduction mode by setting the Atmega328 registers directly.

**[ADC_Frequency.ino](https://github.com/VinnieM-3/Arduino/blob/master/ADC_Frequency.ino)** - This sketch demonstrates how to adjust ADC speed and also demonstrates the effect of speed on conversion accuracy.


**[PWM_Fast_Mode.ino](https://github.com/VinnieM-3/Arduino/blob/master/PWM_Fast_Mode.ino)** - This sketch demonstrates Fast PWM Mode, 10-bit resolution, 1024 steps, on Arduino pins 9 and 10.  Arduino pin 9 will increase in ~1/2v (10% duty cycle) increments and pin 10 will decrease.

**[PWM_Modes.ino](https://github.com/VinnieM-3/Arduino/blob/master/PWM_Modes.ino)** - This sketch demonstrates some of the AtMega328's PWM modes.  It can be used to study the differences between Fast, Phase Correct, and Phase/Frequency Correct PWM Modes, but an oscilloscope is essential.


**[Timer_4secIntr.ino](https://github.com/VinnieM-3/Arduino/blob/master/Timer_4secIntr.ino)** - This sketch demonstrates Timer1 (Atmega328's 16bit timer).  Timer1 can trigger an interrupt every ~4sec or less. The LED tied to Arduino pin 13 will toggle each time the interrupt is triggered (~4sec).


**[WDT_Simple.ino](https://github.com/VinnieM-3/Arduino/blob/master/WDT_Simple.ino)** - This sketch demonstrates the Watchdog Timer in both Interrupt Mode and System Reset Mode.


**[ReadDigitalViaRegister.ino](https://github.com/VinnieM-3/Arduino/blob/master/ReadDigitalViaRegister.ino)** - This sketch demonstrates reading a digital input using registers.
