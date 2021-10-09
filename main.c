#include "DHT.h"
#include <avr/io.h>

#ifndef F_CPU
#define F_CPU 1000000
#endif

#include <util/delay.h>

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

#define OutX B,1

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

int main(void) {
    
    uint16_t temp;
    uint16_t hum;
    int8_t error_code = 0;
    
    uint16_t atemp_min = 0;
    uint16_t atemp_max = 0;
    uint16_t hum_cond = 0;
    
    OUTPUT(OutX);
    LOW(OutX); 
   
    while (1) {
        
        error_code = dht_GetTempUtil(&temp, &hum);
        temp /= 10;
        hum /= 10;
        if ( error_code < 0 ) {
            // Handle error
           
        } else {
            // Sensor data in temp and hum
            atemp_min = (temp > 4);
            atemp_max = (temp < 8);
            hum_cond = (hum < 60);
            if(atemp_min && atemp_max && hum_cond){
            //if(atemp_min && atemp_max){ 
            //if(hum_cond){   
                HIGH(OutX);
            } else {
                LOW(OutX);
            }
            	
        }
        delay1ms(2000);
        
    }
    
    return 0; // never reached
}
