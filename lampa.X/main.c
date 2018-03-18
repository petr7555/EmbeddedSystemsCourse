
#include <xc.h>
#include <stdint.h>
#include "simdelay.h"

#pragma config WDTEN = OFF
#pragma config FOSC = INTIO7
#pragma config MCLRE = EXTMCLR
#pragma config FCMEN = ON

void init(){
    OSCCON = (OSCCON & 0b10001111) | 0b01110000;    // internal oscillator at full speed (16 MHz)

    TRISB = 0b11111111; // five buttons in + unused + PGC, PGD
    LATB = 0xff;        // pull-up by default
    ANSELB = 0;         // no ADC inputs
    
    TRISD = 0b11000000; // LEDs: 0..5 out; TX2: 6 out (but should be set to '1'); RX2: 7 in (UART2 / USB)
    LATD = 0b00000000;
    ANSELD = 0;
    
    T0CON = 0b10000000;
    INTCON = 0b11100000;
    
}

enum ErrorCode {
        BUGGED_PWM,
        WRONG_FORMAT,
        WRONG_EFFICIENCY,
        WRONG_LED,
        SOME_WEIRD_ERROR_IDK,
        ALL_OK
};

typedef enum ErrorCode ErrorCode;

volatile uint16_t cas = 0;
volatile uint16_t cas1 = 0;
const uint16_t timerConstant = 100;

inline ErrorCode pwm(uint8_t dutyCycle) {
    if (cas < timerConstant) {
            if (cas < (timerConstant/100)*dutyCycle) {
                LATD1 = 1;
            }
            else {
                LATD1 = 0;
            }
    } else cas = 0;
    return 5;
}
volatile uint8_t A = 50; //1 to 400 (in 40 ms)

void interrupt IT(void) {
    if(TMR0IF) {
        cas++; // adds 1 every 100 us
        LATD4 = ~LATD4;
        TMR0 = 65335; //65450
        TMR0IF = 0;
    }
}

void main(void) {
    init();
    for(;;){
        pwm(10);
        if(pwm(5) == ALL_OK) {
            
        } else {
            tryRepairPwm();
        }

    }
    return;
    
    /*init();
    
    for(;;){
       LATD = 0b00000000; 
       LATD2 = 1;        
           
       DelayMs(1000);
       LATD3 = 1;
       DelayMs(200);
       DelayMs(200);
       DelayMs(200);
       DelayMs(200);
       DelayMs(200);
       LATD4 = 1;
       DelayMs(200);
       DelayMs(200);
       DelayMs(200);
       DelayMs(200);
       DelayMs(200);
    } */
}
