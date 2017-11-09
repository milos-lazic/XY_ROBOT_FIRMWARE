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

