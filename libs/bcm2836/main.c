//#define _XOPEN_SOURCE
#define _POSIX_C_SOURCE 199309L
#define _XOPEN_SOURCE 500

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <poll.h>

#include "bcm2836.h"
#include "common/listlib/list.h"


static bcm2836_Peripheral gpio;
static bcm2836_Peripheral i2c;


void DS1631_StartTempConvert( bcm2836_Peripheral *p_pHandle)
{
	bcm2836_i2cConfigDataLen( p_pHandle, 1);
	bcm2836_i2cFIFODataWrite( p_pHandle, 0x51);
	bcm2836_i2cClearStatusFlags( p_pHandle);
	bcm2836_i2cStartWrite( p_pHandle);

	bcm2836_i2c_BusyWait( p_pHandle);
}

void DS1631_ReadTempRaw( bcm2836_Peripheral *p_pHandle,
	                     int32_t            *p_Res)
{
	*p_Res = 0;

	bcm2836_i2cConfigDataLen( p_pHandle, 1);
	bcm2836_i2cFIFODataWrite( p_pHandle, 0xaa);
	bcm2836_i2cClearStatusFlags( p_pHandle);
	bcm2836_i2cStartWrite( p_pHandle);

	bcm2836_i2c_BusyWait( p_pHandle);

	bcm2836_i2cConfigDataLen( p_pHandle, 2);
	bcm2836_i2cClearStatusFlags( p_pHandle);
	bcm2836_i2cStartRead( p_pHandle);

	bcm2836_i2c_BusyWait( p_pHandle);

	while( bcm2836_i2cFIFOEmpty( p_pHandle) == 0)
	{
		*p_Res <<= 8;
		*p_Res |= bcm2836_regRead2( p_pHandle, BCM2836_I2C_BSCn_FIFO_REG_OFST);
	}
}



void GPIOInit(void)
{
	int fd;

	/* export control of GPIO 24 to user application */
	fd = open("/sys/class/gpio/export", O_WRONLY);
	write(fd, "24", 3);
	close(fd);

	/* set direction of GPIO 24 as input */
	fd = open("/sys/class/gpio/gpio24/direction", O_WRONLY);
	write(fd, "in", 3);
	close(fd);
}


static void * interruptHandler( void *arg)
{
	int oldval = 0, newval = 0, fd;

	fd = open("/sys/class/gpio/gpio24/value", O_RDONLY);


	read( fd, &oldval, sizeof(oldval));

	while(1)
	{
		lseek(fd, 0, SEEK_SET);

		/* add debouncing delay */
		usleep(2000);

		read(fd, &newval, sizeof(newval));

		if (newval != oldval)
		{
			write(STDOUT_FILENO, "Interrupt triggered\n", 21);
		}

		oldval = newval;
	}
}




int main( int argc, char *argv[])
{
	
	pthread_t ISR_thread;
	uint32_t ui32Val;
	int s;

	GPIOInit();

	s = pthread_create( &ISR_thread, NULL, interruptHandler, NULL);

	if (s != 0)
	{
		printf("Error: pthread_create() failed...\n");
		exit(-1);
	}


	while(1)
	{

	}
	


	// while(1)
	// {

	// }

	return 0;




	// while(1) {}

// 	int err;
// 	uint32_t regval;




// 	err = bcm2836_initPeripheral( &gpio, 
// 		BCM2836_GPIO_PERIPH_BYTE_LEN, 
// 		BCM2836_GPIO_PERIPH_BASE_ADR);
// 	if (err == -1)
// 	{
// 		printf("Error: peripheral (GPIO) initialization failed.\n");
// 	}


// 	err = bcm2836_initPeripheral( &i2c,
// 		BCM2836_I2C_BSCn_PERIPH_BYTE_LEN,
// 		BCM2836_I2C_BSC1_PERIPH_BASE_ADR);
// 	if (err == - 1)
// 	{
// 		printf("Error: peripheral (I2C) initialization failed.\n");
// 		goto gpio_deinit;
// 	}


// 	(void) bcm2836_GPIOPinTypeAltFunction( &gpio, 
// 		                                    BCM2836_GPIO_PIN_2, 
// 		                                    BCM2836_GPIO_ALTF_0); // GPIO_2 ---> I2C_SDA1

// 	(void) bcm2836_GPIOPinTypeAltFunction( &gpio, 
// 		                                    BCM2836_GPIO_PIN_3, 
// 		                                    BCM2836_GPIO_ALTF_0); // GPIO_3 ---> I2C_SCL1



// 	/* configure i2c master module */
// 	(void) bcm2836_i2cConfigSlaveAdr( &i2c, 0x48);


// 	DS1631_StartTempConvert( &i2c);

	

// 	while(1)
// 	{
// 		DS1631_ReadTempRaw( &i2c, &regval);
// 		printf("TEMP_RAW = 0x%x\n", regval);

// 		sleep(2);
// 	}
	
// 	return 0;

// gpio_deinit:
// 	bcm2836_deinitPeripheral( &gpio);

// 	return -1;

}








