#include <pic18f46k22.h>

#include "lampaAPI.h"

void init(){
    OSCCON |= 0b01110000; // interny oscilator nastaveny na 16MHz 
    TRISB = 0b11111111; // five buttons in + unused + PGC, PGD
    LATB = 0xff;        // pull-up by default
    ANSELB = 0;         // no ADC inputs
    
    TRISD = 0b11000000; // LEDs: 0..5 out; TX2: 6 out (but should be set to '1'); RX2: 7 in (UART2 / USB)
    LATD = 0b00000000;
    ANSELD = 0;
    
    T0CON = 0b10001000;
    INTCON = 0b11100000;
    
    SPBRG2 = 25;
    TXSTA2 = 0b00100000;
    RCSTA2 = 0b10010000;
    
    for (uint8_t i = 0; i < CYCLICBUFFERSIZE; i++) message[i] = 0;
}

inline EndCode setRedDutyCycle(uint8_t val) {
    if (val > 100) return ERROR;
    redDutyCycle = val;
    return SUCCESS;
}

inline EndCode setBlueDutyCycle(uint8_t val) {
    if (val > 100) return ERROR;
    blueDutyCycle = val;
    return SUCCESS;
}

inline EndCode setGreenDutyCycle(uint8_t val) {
    if (val > 100) return ERROR;
    greenDutyCycle = val;
    return SUCCESS;
}

inline EndCode setLed(LED led, uint8_t on){
    switch (led) {
        case LED1:
            LATA0 = on;
            break;
        case LED2:
            LATA1 = on;
            break;
        default:
            return ERROR;
    }
    return SUCCESS;
}

inline MenuPos getMenuPosition(void) {
    return (MenuPos)((~(PORTC & 0x0f)) & 0x0f);
}

inline void pwmEnable(uint8_t on) {
    pwmEnabled = on;
}

inline EndCode setPwmFreq (uint16_t val) {
    if (val < 20 || val > 4000) return ERROR;
    pwmFreq = val;
    return SUCCESS;
}

EndCode saveData(unsigned char adr, unsigned char val) {
    if (adr > 0xfe) return ERROR;
    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS = 0;          
    EEADR = adr;  //nacitaj adresu do registra
    EEDATA = val; // nacitaj hodnotu 
    WREN = 1; // toto je write enable
    GIE = 0;  //vypneme interupty
    asm("MOVLW 55h");
    asm("MOVWF EECON2");
    asm("MOVLW 0AAh");
    asm("MOVWF EECON2");
    WR = 1; // spusti write
    GIE = 1; // nastartuj naspat interupty
    while (WR) {}
    WREN = 0; // koniec zapisovania
    return SUCCESS;
}


unsigned char readData(unsigned char adr) {
    if (adr > 0xfe) return 0;
    while(RD || WR); // zapis alebo citanie (ine) nesmie prebiehat
    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS = 0; 
    EEADR = adr; //odkial citat
    RD = 1; // spusti interne citanie, tj EEDATA naplni hodnotou z danej adresy v EEADR
    return EEDATA; // data
}

inline EndCode setOverallIntensity(uint8_t val) {
    if (val > 100) return ERROR;
    intensity = val;
    return SUCCESS;
}

inline uint16_t readAdc(void) {
    return lastADCvalue;
};

void interrupt IT(void) {
    static uint8_t percent = 0;
    if (TMR0IF) {
        TMR0IF = 0;
        if (pwmEnabled && percent < 100) {
            if (percent < intensity) {
                TMR0 = (unsigned)((0.5 * pwmFreq) + 63525);
                if (percent < redDutyCycle) LATB5 = 1; 
                else LATB5 = 0;
                if (percent < greenDutyCycle) LATB4 = 1; 
                else LATB4 = 0;
                if (percent < blueDutyCycle) LATB3 = 1; 
                else LATB3 = 0;
            } else LATB5 = LATB4 = LATB3 = 0;
            percent++;
        } else {
            percent = 0;
            TMR0 = 0;
        }
    } 
    if (PIR3bits.RC2IF) {
        if (!RCSTA2bits.OERR){
            if (!RCSTA2bits.FERR){ 	
                if (lastCharIndex < CYCLICBUFFERSIZE) {
                    message[lastCharIndex] = RCREG2;
                    lastCharIndex++;
                } else {
                    lastCharIndex = 0;
                    message[0] = RCREG2;
                }
            } else {							
                RCSTA2bits.SPEN = 0;		
                RCSTA2bits.SPEN = 1;		
            }
        } else {					
            RCSTA2bits.CREN = 0;		
            RCSTA2bits.CREN = 1;	
        }
        PIR3bits.RC2IF = 0;
    }
   
    if (ADIF) {
        ADIF = 0;
    }
}

void putch(char c) {
    while(!TXSTA2bits.TRMT) {};
    TXREG2 = c;
}

inline void clearMessageBuffer(void) {
    for(uint8_t i = 0; i < CYCLICBUFFERSIZE; i++) message[i] = 0;
}
