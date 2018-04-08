/*
 * File:   main.c
 * Author: vyvoj
 *
 * Created on April 8, 2018, 4:09 PM
 */

#include <stdio.h>
#include <xc.h>
#include "lampaAPI.h"

#pragma config WDTEN = OFF // Watchdog timer - nepouzivame
#pragma config FOSC = INTIO7 // nastavenie vstupu hodin
#pragma config MCLRE = EXTMCLR // nastavenie resetu procesoru na externe 
#pragma config FCMEN = ON // FailSafe Clock monitor zapiname (dalsie info datasheet)
#pragma config CPD = OFF // eeprom
#pragma confing WRTD = OFF // eeprom
// zvysne konfiguracne slova sa daju lahko najst co znamenaju. http://ww1.microchip.com/downloads/en/DeviceDoc/51537a.pdf
// pouzite ich podla uvazenia a danej aplikacie. Zvysne sa dogeneruju automaticky na defaultne hodnoty. (tj ten procesor najskor pobezi))



uint8_t containsStr(char* message, const char* command) {
    uint8_t i;
    uint8_t j;

    for (i = 0; message[i] != '\0'; i++) {
        if (message[i] == command[0]) {
            for (j = i;; j++) {
                if (command[(j - i)] == '\0')
                    return (1);
                if (message[j%10] != command[(j - i)]) {
                    //printf("error");
                    break;
                    }
            }
        }
    }
    return 0;
}

void main(void) {
    init();
    int lastchar = 0;
    int flag = 1;
    while (1) {
        if (bufferFull) {
            asre + message
            bufferClear();
        }
        if(lastchar != lastCharIndex) {
            lastchar = lastCharIndex;
            printf("%s \n\r", message);
        }
        
        
        if (containsStr(message, "color ") && flag) {
            printf("Super");
            flag = 0;
        }
        
    }

    return;
}