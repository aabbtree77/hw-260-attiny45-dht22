## Notes

This is a periodic signal generator for a certain water pump. The signal is such that within its period the high-bit (active) value duration is fixed to 30ms., while the low-bit value duration varies from 100ms to 10s. depending on the push buttons In_UP (PB0) or In_DOWN (PB2). The actual output is inverted.

In addition, there is a "RUN/PAUSE" button
```console
#define In_ON B,3
```
whose state is sensed via Timer1 interrupt (with "active zero"):
```console
ISR(TIMER1_COMPA_vect) {
    if(READ(In_ON) == 0) {  
        OUT_STATE = !OUT_STATE;
    }
}
```
Timer0 senses the adjustments of the zero/negative level duration in a periodic rectangular signal:
```console
volatile uint32_t DELAY_NEG_MS = 500; initial zero level duration value in ms.  
#define MAX_MS 10000UL - its maximal possible value in ms.
#define MIN_MS 100UL - its minimal possible value in ms.
```

Critical timer values:
```comsole
OCR1C = 100; - Timer1 interrupt period 100ms.
```
Here possible values are 1..255, but ATtiny85 seems to have a very unique feature in that its Timer1 has so many prescalings, see the ATmega data sheets.
A small value reacts quicker, but results in a so called button "debounce" noise. A bigger value is more reliable, but it increases the button response time.

```console
OCR0A = 10; - Timer0 interrupt period 10ms., 
```
Similarly, 1..255 possible values. OCRA=10 adds one millisecond to the zero level duration every 10ms., thus you can control the speed of the generated period (zero level) adjustment.

**TD1**: Perhaps one could write the whole thesis just about how to handle a simple "press button" response correctly. This would involve interrupts, integration aka low pass filtering with optimal time values or their non-existence, async flows and blocking in various situations with loops resulting in an input handling C library/whole async framework for the ATmega chips. 

**TD2**: It would be nice to be able to save the last set DELAY_NEG_MS value and read it via some EEPROM functions once the device is turned on again the next 
time.
