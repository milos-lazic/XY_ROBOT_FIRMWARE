#ifndef _MT_SERVO_H_
#define _MT_SERVO_H

/* Register defines for RPi adafruit servo hat */
/* For more detailed description see: https://cdn-shop.adafruit.com/datasheets/PCA9685.pdf */

#define MODE1_REG_ADR               0x00
#define MODE2_REG_ADR               0x01
#define SUBADR1_REG_ADR             0x02
#define SUBADR2_REG_ADR             0x03
#define SUBADR3_REG_ADR             0x04
#define ALLCALLADR_REG_ADR          0x05
#define LED0_ON_L_REG_ADR           0x06
#define LED0_ON_H_REG_ADR           0x07
#define LED0_OFF_L_REG_ADR          0x08
#define LED0_OFF_H_REG_ADR          0x09
#define ALL_LED_ON_L_REG_ADR        0xFA
#define ALL_LED_ON_H_REG_ADR        0xFB
#define ALL_LED_OFF_L_REG_ADR       0xFC
#define ALL_LED_OFF_H_REG_ADR       0xFD
#define PRE_SCALE_REG_ADR           0xFE

#define __RESTART                   0x80
#define __SLEEP                     0x10

#define __ALLCALL                   0x01
#define __INVRT                     0x10
#define __OUTDRV                    0x04


extern void mtservo_init( int fd);

extern void mtservo_setPwmFreq( int fd, unsigned int  pwm_freq);

extern void mtservo_setDuty( int fd, int channel, int on, int off);

extern void mtservo_resetAllPWM( int fd, int on, int off);

#endif
