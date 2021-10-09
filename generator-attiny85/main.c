//R.G. August 17th, 2021, MIT licence

#define F_CPU 1000000UL  //fuse bits = internal clock 8MHz divided by 8
#include <avr/io.h>        
#include <util/delay.h>
#include <avr/interrupt.h>

// MACROS FOR EASY PIN HANDLING FOR ATMEL GCC-AVR
//these macros are used indirectly by other macros , mainly for string concatenation
#define _SET(type,name,bit)          type ## name  |= _BV(bit)    
#define _CLEAR(type,name,bit)        type ## name  &= ~ _BV(bit)        
#define _TOGGLE(type,name,bit)       type ## name  ^= _BV(bit)    
#define _GET(type,name,bit)          ((type ## name >> bit) &  1)
#define _PUT(type,name,bit,value)    type ## name = ( type ## name & ( ~ _BV(bit)) ) | ( ( 1 & (unsigned char)value ) << bit )

//these macros are used by end user
#define OUTPUT(pin)         _SET(DDR,pin)    
#define INPUT(pin)          _CLEAR(DDR,pin)    
#define HIGH(pin)           _SET(PORT,pin)
#define LOW(pin)            _CLEAR(PORT,pin)    
#define TOGGLE(pin)         _TOGGLE(PORT,pin)    
#define READ(pin)           _GET(PIN,pin)

/*
    BASIC STAMPS STYLE COMMANDS FOR ATMEL GCC-AVR

    Usage Example:
    ———————————————–
    #define pinLed B,5  //define pins like this

    OUTPUT(pinLed);       //typo fixed
    //OUTPUT(pinLED);     //compiles as DDRB |= (1<<5);
    HIGH(pinLed);         //compiles as PORTB |= (1<<5);
    ———————————————–
*/

//max delay with _delay_ms is 262.14 ms / F_CPU in MHz, i.e. 32 ms for 8 MHz.
//so lets iterate 1ms, that looses precision with function call overhead, 
//but we do not care in this application
void delay1ms(uint32_t ms) {
    uint32_t i;
    for(i=0;i<ms;i++) _delay_ms(1);
}

void delay1us(uint32_t us) {
    uint32_t i;
    for(i=0;i<us;i++) _delay_us(1);
}

#define In_UP B,0
#define In_DOWN B,2
#define In_ON B,3
#define OutX B,1

//Max size for uint16_t is 65,535 - not enough in general.
//Max size for uint32_t is 4,294,967,295 - OK.
volatile uint32_t DELAY_NEG_MS = 500;
#define MAX_MS 10000UL
#define MIN_MS 100UL

volatile uint8_t OUT_STATE = 0;

ISR(TIMER0_COMPA_vect) {
    
    if(READ(In_UP) == 0) {   
        DELAY_NEG_MS++;
    }
    
    if(READ(In_DOWN) == 0) {   
        DELAY_NEG_MS--;
    }
	
    DELAY_NEG_MS = (DELAY_NEG_MS < MIN_MS)? MIN_MS : DELAY_NEG_MS;
    DELAY_NEG_MS = (DELAY_NEG_MS > MAX_MS)? MAX_MS : DELAY_NEG_MS;
}

ISR(TIMER1_COMPA_vect) {
    if(READ(In_ON) == 0) {   
        OUT_STATE = !OUT_STATE;
    }
}

int main(void){
    
    INPUT(In_UP);
    INPUT(In_DOWN);
    OUTPUT(OutX);
    HIGH(OutX);
    
    /*
    Timer 1 on attiny85 is interesting as it has a lot of prescalings, check page 90 at
    https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-2586-AVR-8-bit-Microcontroller-ATtiny25-ATtiny45-ATtiny85_Datasheet.pdf
    */
    cli();
    //Timer1 to toggle OUT_STATE
    TCCR1 |= (1 << CTC1);  //CTC
    TCCR1 |= (1 << CS13) | (1 << CS11) | (1 << CS10); //clock prescaler 1024
    OCR1C = 100; // interrupt runs every 100ms.
    TIMSK |= (1 << OCIE1A); // enable compare match interrupt
    
    //Timer0 to inc/dec DELAY_NEG_MS
    TCCR0A |= (1 << WGM01); //CTC
    TCCR0B |= (1 << CS02) | (1 << CS00); //clock prescaler 1024
    OCR0A = 10; //interrupt runs every 10ms. 
    TIMSK |= (1 << OCIE0A); 
    sei();
    
    while(1){
        if(OUT_STATE != 0){
            LOW(OutX);
            delay1ms(30);
            HIGH(OutX);
            delay1ms(DELAY_NEG_MS);
        } else {
            HIGH(OutX);
        }
    }
}
