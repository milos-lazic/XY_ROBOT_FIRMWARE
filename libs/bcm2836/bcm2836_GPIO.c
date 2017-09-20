#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include "bcm2836.h"

#ifdef BCM2836_CONFIG


/* Name: bcm2836_GPIOPinTypeInput
 * Description: configure a physical GPIO pin as a digital input
 * Parameters: p_pHandle        - pointer to an initialized peripheral object
 *             p_nPin           - GPIO pin number (check Raspberry PI 2 GPIO pin
 *                                diagram)
 * Return:     0 IF successful, -1 IF failed
 * Note:       *** Does not perform bounds-checking. It is the developer's 
 *                 responsibility to ensure that the parameter passed in as
 *                 the pin number (p_nPin) is valid. Failure to do so results
 *                 in undefined behaviour (ex. overwriting adjacent registers).
 */
_global int bcm2836_GPIOPinTypeInput( bcm2836_Peripheral *p_pHandle,
                                      e_bcm2836_GPIO_Pin  p_nPin)
{
	int s;
	uint32_t regOfst = BCM2836_GPIO_GPFSEL0_REG_OFST + ((p_nPin/10)*0x04);

	/* reset the FSEL bits for the corresponding pin first */
	s = bcm2836_regResetBits( p_pHandle, regOfst, 0x07 << ((p_nPin % 10) * 3));
	if ( s == -1)
		return -1;

	/* no further action is needed; for a pin to become and input
	   its corresponding (3) FSEL bits should be clear (000b) */

	return 0;
}


/* Name: bcm2836_GPIOPinTypeOutput
 * Description: configure a physical GPIO pin as a digital output
 * Parameters: p_pHandle        - pointer to an initialized peripheral object
 *             p_nPin           - GPIO pin number (check Raspberry PI 2 GPIO pin
 *                                diagram)
 * Return:     0 IF successful, -1 IF failed
 * Note:       *** Does not perform bounds-checking. It is the developer's 
 *                 responsibility to ensure that the parameter passed in as
 *                 the pin number (p_nPin) is valid. Failure to do so results
 *                 in undefined behaviour (ex. overwriting adjacent registers).
 */
_global int bcm2836_GPIOPinTypeOutput( bcm2836_Peripheral *p_pHandle,
                                       e_bcm2836_GPIO_Pin  p_nPin)
{
	int s;
	uint32_t regOfst = BCM2836_GPIO_GPFSEL0_REG_OFST + ((p_nPin/10)*0x04);

	/* reset the FSEL bits for the corresponding pin first */
	s = bcm2836_regResetBits( p_pHandle, regOfst, 0x07 << ((p_nPin % 10) * 3));
	if ( s == -1)
		return -1;

	s = bcm2836_regSetBits( p_pHandle, regOfst, 0x01 << ((p_nPin % 10) * 3));
	if ( s == -1)
		return -1;

	return 0;
}


/* Name: bcm2836_GPIOPinTypeAltFunction
 * Description: configure alternative function on a physical GPIO pin
 * Parameters: p_pHandle        - pointer to an initialized peripheral object
 *             p_nPin           - GPIO pin number (check Raspberry PI 2 GPIO pin
 *                                diagram)
 *             p_nAltFun        - alternate function ID (check table 6-31 in BCM2835
 *                                datasheet)
 * Return:     0 IF successful, -1 IF failed
 * Note:       *** Does not perform bounds-checking. It is the developer's 
 *                 responsibility to ensure that the parameter passed in as
 *                 the pin number (p_nPin) is valid. Failure to do so results
 *                 in undefined behaviour (ex. overwriting adjacent registers).
 */
_global int bcm2836_GPIOPinTypeAltFunction( bcm2836_Peripheral  *p_pHandle,
                                            e_bcm2836_GPIO_Pin   p_nPin,
                                            e_bcm2836_GPIO_AltF  p_nAltFun)
{
	int s;
	uint32_t regOfst = BCM2836_GPIO_GPFSEL0_REG_OFST + ((p_nPin/10)*0x04);
	uint32_t altFuncMask;

	/* reset the FSEL bits for the corresponding pin first */
	s = bcm2836_regResetBits( p_pHandle, regOfst, 0x07 << ((p_nPin % 10) * 3));
	if ( s == -1)
		return -1;

	switch( p_nAltFun)
	{
		case BCM2836_GPIO_ALTF_0:
			altFuncMask = 0x04;
			break;
		case BCM2836_GPIO_ALTF_1:
			altFuncMask = 0x05;
			break;
		case BCM2836_GPIO_ALTF_2:
			altFuncMask = 0x06;
			break;
		case BCM2836_GPIO_ALTF_3:
			altFuncMask = 0x07;
			break;
		case BCM2836_GPIO_ALTF_4:
			altFuncMask = 0x03;
			break;
		case BCM2836_GPIO_ALTF_5:
			altFuncMask = 0x02;
			break;
		default:
			return -1;
			break;
	}

	s = bcm2836_regSetBits( p_pHandle, regOfst, altFuncMask << ((p_nPin % 10) * 3));
	if (s == -1)
		return -1;

	return 0;
}


/* Name: bcm2836_GPIOPinSet
 * Description: set a physical GPIO pin output to HIGH logic level
 * Parameters: p_pHandle        - pointer to an initialized peripheral object
 *             p_nPin           - GPIO pin number (check Raspberry PI 2 GPIO pin
 *                                diagram)
 * Return:     0 IF successful, -1 IF failed
 * Note:       *** Does not perform bounds-checking. It is the developer's 
 *                 responsibility to ensure that the parameter passed in as
 *                 the pin number (p_nPin) is valid. Failure to do so results
 *                 in undefined behaviour (ex. overwriting adjacent registers).
 */
_local int bcm2836_GPIOPinSet( bcm2836_Peripheral *p_pHandle,
                               e_bcm2836_GPIO_Pin  p_nPin)
{
	int s;
	uint32_t regOfst = BCM2836_GPIO_GPSET0_REG_OFST + ((p_nPin/32)*0x04);

	s = bcm2836_regSetBits( p_pHandle, regOfst, BIT(p_nPin % 32));
	if (s == -1)
		return -1;

	return 0;
}


/* Name: bcm2836_GPIOPinClear
 * Description: set a physical GPIO pin output to LOW logic level
 * Parameters: p_pHandle        - pointer to an initialized peripheral object
 *             p_nPin           - GPIO pin number (check Raspberry PI 2 GPIO pin
 *                                diagram)
 * Return:     0 IF successful, -1 IF failed
 * Note:       *** Does not perform bounds-checking. It is the developer's 
 *                 responsibility to ensure that the parameter passed in as
 *                 the pin number (p_nPin) is valid. Failure to do so results
 *                 in undefined behaviour (ex. overwriting adjacent registers).
 */
_local int bcm2836_GPIOPinClear( bcm2836_Peripheral *p_pHandle,
                                 e_bcm2836_GPIO_Pin  p_nPin)
{
	int s;
	uint32_t regOfst = BCM2836_GPIO_GPCLR0_REG_OFST + ((p_nPin/32)*0x04);

	s = bcm2836_regSetBits( p_pHandle, regOfst, BIT(p_nPin % 32));
	if (s == -1)
		return -1;

	return 0;
}


/* Name: bcm2836_GPIOSetPinLevel
 * Description: set the logical state (HIGH/LOW) of a physical GPIO pin
 * Parameters: p_pHandle        - pointer to an initialized peripheral object
 *             p_nPin           - GPIO pin number (check Raspberry PI 2 GPIO pin
 *                                diagram)
 *             p_pLevel         - HIGH (BCM2836_GPIO_PIN_LEVEL_HIGH) or
 *                                LOW (BCM2836_GPIO_PIN_LEVEL_LOW)
 * Return:     0 IF pin level setting is succesful, -1 IF operation failed
 * Note:       *** Does not perform bounds-checking. It is the developer's 
 *                 responsibility to ensure that the parameter passed in as
 *                 the pin number (p_nPin) is valid. Failure to do so results
 *                 in undefined behaviour (ex. overwriting adjacent registers).
 */
_global int bcm2836_GPIOSetPinLevel( bcm2836_Peripheral      *p_pHandle,
                                     e_bcm2836_GPIO_Pin       p_nPin,
                                     e_bcm2836_GPIO_PinLevel  p_pLevel)
{
	switch(p_pLevel)
	{
		case BCM2836_GPIO_PIN_LEVEL_LOW:
			return bcm2836_GPIOPinClear( p_pHandle, p_nPin);
			break;
		case BCM2836_GPIO_PIN_LEVEL_HIGH:
			return bcm2836_GPIOPinSet( p_pHandle, p_nPin);
			break;

		default:
			/* do nothing */
			break;
	}
}


/* Name: bcm2836_GPIOPinLevel
 * Description: get the logical state (HIGH/LOW) of a physical GPIO pin
 * Parameters: p_pHandle        - pointer to an initialized peripheral object
 *             p_nPin           - GPIO pin number (check Raspberry PI 2 GPIO pin
 *                                diagram)
 * Return:     0 IF pin is LOW, 1 IF pin is HIGH, -1 IF failed to get pin level
 * Note:       *** Does not perform bounds-checking. It is the developer's 
 *                 responsibility to ensure that the parameter passed in as
 *                 the pin number (p_nPin) is valid. Failure to do so results
 *                 in undefined behaviour (ex. overwriting adjacent registers).
 */
_global int bcm2836_GPIOPinLevel( bcm2836_Peripheral *p_pHandle,
                                  e_bcm2836_GPIO_Pin  p_nPin)
{
	int s;
	uint32_t regOfst = BCM2836_GPIO_GPLEV0_REG_OFST + ((p_nPin/32)*0x04);
	uint32_t regVal;

	s = bcm2836_regRead( p_pHandle, regOfst, &regVal);
	if (s == -1)
		return -1;

	if (regVal & BIT(p_nPin % 32))
		return 1;
	else
		return 0;
}


#endif
