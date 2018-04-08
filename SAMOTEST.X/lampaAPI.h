/* 
 * File:   lampaAPI.h
 * Author: Samo Urban
 *
 * Created on 2018, march 20, 12:55
 */

#ifndef LAMPAAPI_H
#define	LAMPAAPI_H
#include <stdlib.h>
#include <stdint.h>
#include <xc.h>

#define CYCLICBUFFERSIZE 10

volatile uint8_t lastCharIndex = 0;
volatile char message[CYCLICBUFFERSIZE]; 
volatile uint16_t pwmFreq = 0;
volatile uint8_t redDutyCycle = 0;
volatile uint8_t blueDutyCycle = 0;
volatile uint8_t greenDutyCycle = 0;
volatile uint8_t pwmEnabled = 0;
volatile uint8_t intensity = 0;
volatile uint16_t lastADCvalue = 0;

enum EndCode { 
    SUCCESS,
    ERROR
};

enum LED {
    LED1,
    LED2,
    NONE
};

enum MenuPosition {
    MENU0 = 0,
    MENU1,
    MENU2,
    MENU3,
    MENU4,
    MENU5,
    MENU6,
    MENU7,
    MENU8,
    MENU9,
    MENU10,
    MENU11,
    MENU12,
    MENU13,
    MENU14,
    MENU15
};

typedef enum MenuPosition MenuPos;
typedef enum EndCode EndCode;
typedef enum LED LED;

inline EndCode setRedDutyCycle(uint8_t);
inline EndCode setBlueDutyCycle(uint8_t);
inline EndCode setGreenDutyCycle(uint8_t);
inline EndCode setLed(LED led, uint8_t);
inline void pwmEnable(uint8_t);
inline EndCode setOverallIntensity(uint8_t );

inline MenuPos getMenuPosition(void);

EndCode saveData(unsigned char, unsigned char);
unsigned char readData(unsigned char);

inline uint16_t readAdc(void);
void init(void);
void putch(char);
inline void clearMessageBuffer(void); 

#endif	/* LAMPAAPI_H */

