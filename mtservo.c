#include <stdio.h>
#include <stdlib.h>
#include <wiringPiI2C.h>
#include "mtservo.h"

void mtservo_resetAllPWM( int fd, int on, int off)
{
	wiringPiI2CWriteReg8( fd, ALL_LED_ON_L_REG_ADR, on & 0xFF);
	wiringPiI2CWriteReg8( fd, ALL_LED_ON_H_REG_ADR, on >> 8);
	wiringPiI2CWriteReg8( fd, ALL_LED_OFF_L_REG_ADR, off & 0xFF);
	wiringPiI2CWriteReg8( fd, ALL_LED_OFF_H_REG_ADR, off >> 8);
}


void mtservo_setPwmFreq( int fd, unsigned int pwm_freq)
{
	unsigned int prescaleval = 25000000;
        int oldmode, newmode;

        prescaleval /= 4096;
        prescaleval /= pwm_freq;
        prescaleval -= 1;

        oldmode = wiringPiI2CReadReg8( fd, MODE1_REG_ADR);
        newmode = (oldmode & 0x7F) | 0x10; // sleep

        wiringPiI2CWriteReg8( fd, MODE1_REG_ADR, newmode);
        wiringPiI2CWriteReg8( fd, PRE_SCALE_REG_ADR, prescaleval);
        wiringPiI2CWriteReg8( fd, MODE1_REG_ADR, oldmode);


        wiringPiI2CWriteReg8( fd, MODE1_REG_ADR, oldmode | 0x80);
}


void mtservo_setDuty( int fd, int channel, int on, int off)
{
	wiringPiI2CWriteReg8( fd, LED0_ON_L_REG_ADR+4*channel, on & 0xFF);
	wiringPiI2CWriteReg8( fd, LED0_ON_H_REG_ADR+4*channel, on >> 8);
	wiringPiI2CWriteReg8( fd, LED0_OFF_L_REG_ADR+4*channel, off & 0xFF);
	wiringPiI2CWriteReg8( fd, LED0_OFF_H_REG_ADR+4*channel, off >> 8);
}


void mtservo_init( int fd)
{
	int mode1;

	// zero all PWM ports
	mtservo_resetAllPWM( fd, 0, 0);

	wiringPiI2CWriteReg8( fd, MODE2_REG_ADR, __OUTDRV);
	wiringPiI2CWriteReg8( fd, MODE1_REG_ADR, __ALLCALL);

	mode1 = wiringPiI2CReadReg8( fd, MODE1_REG_ADR);
	mode1 = mode1 & (~__SLEEP);
	wiringPiI2CWriteReg8( fd, MODE1_REG_ADR, mode1);

	mtservo_setPwmFreq( fd, 50);
}

