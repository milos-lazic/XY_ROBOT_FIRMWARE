#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "DS1631.h"

#ifdef BCM2836_CONFIG
/* DS1631 interface for BCM2836 */

static struct adc_map_pt DS1631_temp_map_table[] =
{
	/* ADC Value */  /* Temperature (milli-DegC) */
	{32000,          125000},
	{ 6416,          25063},
	{ 2592,          10125},
	{  128,          500},
	{    0,          0},
	{ -128,          -500},
	{-2592,          -10125},
	{-6416,          -25063},
	{-14080,         -55000}
};


/* Function Definitions */


int32_t DS1631_scale_adc_raw_to_temp( struct adc_map_pt *pTable, int32_t adc_code)
{
	// int64_t type needed because 32-bit buffer overflows
	// during the multiplication in the equation below
	int64_t x1, y1, x2, y2, res;

	while(pTable->x >= adc_code)
	{
		pTable++;
	}

	x1 = pTable->x;
	y1 = pTable->y;

	pTable--;

	x2 = pTable->x;
	y2 = pTable->y;


	res = ( ((y2 - y1) * (adc_code - x1)) / (x2 - x1) ) + y1;


	return res;
}


void DS1631_StartTempConvert( bcm2836_Peripheral *p_pHandle)
{
	bcm2836_i2cConfigDataLen( p_pHandle, 1);
	bcm2836_i2cFIFODataWrite( p_pHandle, 0x51);
	bcm2836_i2cClearStatusFlags( p_pHandle);
	bcm2836_i2cStartWrite( p_pHandle);

	bcm2836_i2c_BusyWait( p_pHandle);
}

void DS1631_StopTempConvert( bcm2836_Peripheral *p_pHandle)
{
	bcm2836_i2cConfigDataLen( p_pHandle, 1);
	bcm2836_i2cFIFODataWrite( p_pHandle, 0x22);
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

	while( bcm2836_i2cFIFOEmpty( &bsc1) == 0)
	{
		*p_Res <<= 8;
		*p_Res |= bcm2836_regRead2( p_pHandle, BCM2836_I2C_BSCn_FIFO_REG_OFST);
	}
}


void DS1631_GetTempHighThreshold( bcm2836_Peripheral *p_pHandle,
	                              int32_t            *p_Res)
{
	*p_Res = 0;

	bcm2836_i2cConfigDataLen( p_pHandle, 1);
	( p_pHandle, 0xa1);
	bcm2836_i2cClearStatusFlags( p_pHandle);
	bcm2836_i2cStartWrite( p_pHandle);

	bcm2836_i2c_BusyWait( p_pHandle);

	bcm2836_i2cConfigDataLen( p_pHandle, 2);
	bcm2836_i2cClearStatusFlags( p_pHandle);
	bcm2836_i2cStartRead( p_pHandle);

	bcm2836_i2c_BusyWait( p_pHandle);

	while( bcm2836_i2cFIFOEmpty( &bsc1) == 0)
	{
		*p_Res <<= 8;
		*p_Res |= bcm2836_regRead2( p_pHandle, BCM2836_I2C_BSCn_FIFO_REG_OFST);
	}
}


void DS1631_SetTempHighThreshold( bcm2836_Peripheral *p_pHandle,
	                              int32_t             p_Temp)
{
	DS1631_StopTempConvert( p_pHandle);

/*** DS1631_TBD: incorrect TH clearing method ***/
	/* Clear Temperature_High_Flag in the DS1631 Configuration Register */
	uint32_t cfgRegVal;
	DS1631_ReadConfigRegister( p_pHandle, &cfgRegVal);
	cfgRegVal &= 0xbf;
	DS1631_WriteConfigRegister( p_pHandle, cfgRegVal);
	/* ---------------------------------------------------------------- */
/*** END_DS1631_TBD ***/

	bcm2836_i2cConfigDataLen( p_pHandle, 3);
	bcm2836_i2cFIFODataWrite( p_pHandle, 0xa1);
	bcm2836_i2cFIFODataWrite( p_pHandle, p_Temp >> 8);
	bcm2836_i2cFIFODataWrite( p_pHandle, p_Temp & 0xff);
	bcm2836_i2cClearStatusFlags( p_pHandle);
	bcm2836_i2cStartWrite( p_pHandle);

	bcm2836_i2c_BusyWait( p_pHandle);

	DS1631_StartTempConvert( p_pHandle);
}


void DS1631_GetTempLowThreshold( bcm2836_Peripheral *p_pHandle,
	                              int32_t            *p_Res)
{
	*p_Res = 0;

	bcm2836_i2cConfigDataLen( p_pHandle, 1);
	bcm2836_i2cFIFODataWrite( p_pHandle, 0xa2);
	bcm2836_i2cClearStatusFlags( p_pHandle);
	bcm2836_i2cStartWrite( p_pHandle);

	bcm2836_i2c_BusyWait( p_pHandle);

	bcm2836_i2cConfigDataLen( p_pHandle, 2);
	bcm2836_i2cClearStatusFlags( p_pHandle);
	bcm2836_i2cStartRead( p_pHandle);

	bcm2836_i2c_BusyWait( p_pHandle);

	while( bcm2836_i2cFIFOEmpty( &bsc1) == 0)
	{
		*p_Res <<= 8;
		*p_Res |= bcm2836_regRead2( p_pHandle, BCM2836_I2C_BSCn_FIFO_REG_OFST);
	}
}


void DS1631_SetTempLowThreshold( bcm2836_Peripheral *p_pHandle,
	                             int32_t             p_Temp)
{
	DS1631_StopTempConvert( p_pHandle);

	/* Reset Temperature_Low_Flag in the DS1631 Configuration Register */
	uint32_t cfgRegVal;
	DS1631_ReadConfigRegister( p_pHandle, &cfgRegVal);
	cfgRegVal &= 0xdf;
	DS1631_WriteConfigRegister( p_pHandle, cfgRegVal);
	/* ---------------------------------------------------------------- */

	bcm2836_i2cConfigDataLen( p_pHandle, 3);
	bcm2836_i2cFIFODataWrite( p_pHandle, 0xa2);
	bcm2836_i2cFIFODataWrite( p_pHandle, p_Temp >> 8);
	bcm2836_i2cFIFODataWrite( p_pHandle, p_Temp & 0xff);
	bcm2836_i2cClearStatusFlags( p_pHandle);
	bcm2836_i2cStartWrite( p_pHandle);

	bcm2836_i2c_BusyWait( p_pHandle);

	DS1631_StartTempConvert( p_pHandle);
}


void DS1631_ReadConfigRegister( bcm2836_Peripheral *p_pHandle,
	                            uint32_t           *p_Res)
{
	*p_Res = 0;

	bcm2836_i2cConfigDataLen( p_pHandle, 1);
	bcm2836_i2cFIFODataWrite( p_pHandle, 0xac);
	bcm2836_i2cClearStatusFlags( p_pHandle);
	bcm2836_i2cStartWrite( p_pHandle);

	bcm2836_i2c_BusyWait( p_pHandle);

	bcm2836_i2cConfigDataLen( p_pHandle, 1);
	bcm2836_i2cClearStatusFlags( p_pHandle);
	bcm2836_i2cStartRead( p_pHandle);

	bcm2836_i2c_BusyWait( p_pHandle);

	while( bcm2836_i2cFIFOEmpty( &bsc1) == 0)
	{
		*p_Res <<= 8;
		*p_Res |= bcm2836_regRead2( p_pHandle, BCM2836_I2C_BSCn_FIFO_REG_OFST);
	}
}


void DS1631_WriteConfigRegister( bcm2836_Peripheral *p_pHandle,
	                             uint32_t            p_RegVal)
{
	DS1631_StopTempConvert( p_pHandle);

	bcm2836_i2cConfigDataLen( p_pHandle, 2);
	bcm2836_i2cFIFODataWrite( p_pHandle, 0xac);
	bcm2836_i2cFIFODataWrite( p_pHandle, p_RegVal);
	bcm2836_i2cClearStatusFlags( p_pHandle);
	bcm2836_i2cStartWrite( p_pHandle);

	bcm2836_i2c_BusyWait( p_pHandle);

	DS1631_StartTempConvert( p_pHandle);
}


void DS1631_PowerOnReset( bcm2836_Peripheral *p_pHandle)
{
	bcm2836_i2cConfigDataLen( p_pHandle, 1);
	bcm2836_i2cFIFODataWrite( p_pHandle, 0x54);
	bcm2836_i2cClearStatusFlags( p_pHandle);
	bcm2836_i2cStartWrite( p_pHandle);

	bcm2836_i2c_BusyWait( p_pHandle);
}



#endif