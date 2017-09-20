#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "bcm2836.h"

#ifdef BCM2836_CONFIG

/* Name: bcm2836_i2cConfigSlaveAdr
 * Description: set the address of the slave device to communicate with via
 *              the I2C interface
 * Parameters: p_pHandle        - pointer to an initialized I2C BSCn peripheral object
 *                                which is being used to communicate with the addressed
 *                                slave device
 *             p_sAdr           - (7-bit) slave address (check data sheet for 10-bit
 *                                addressing mode)
 * Return:     0 IF successful, -1 IF failed
 * Note:
 */
_global int bcm2836_i2cConfigSlaveAdr( bcm2836_Peripheral *p_pHandle, uint32_t p_sAdr)
{
	return bcm2836_regWrite( p_pHandle, BCM2836_I2C_BSCn_A_REG_OFST, p_sAdr);
}


/* Name: bcm2836_i2cConfigDataLen
 * Description: set the data length (number of command or read bytes) for the next I2C
 *              operation
 * Parameters: p_pHandle        - pointer to an initialized I2C BSCn peripheral object
 *                                which is being used to communicate with the addressed
 *                                slave device
 *             p_dLen           - number of bytes to be written or read
 * Return:     0 IF successful, -1 IF failed
 * Note:
 */
_global int bcm2836_i2cConfigDataLen( bcm2836_Peripheral *p_pHandle, uint32_t p_dLen)
{
	return bcm2836_regWrite( p_pHandle, BCM2836_I2C_BSCn_DLEN_REG_OFST, p_dLen);
}


/* Name: bcm2836_i2cFIFODataWrite
 * Description: place data into the 16-byte FIFO, ready to transmit on the data bus
 * Parameters: p_pHandle        - pointer to an initialized I2C BSCn peripheral object
 *                                which is being used to communicate with the addressed
 *                                slave device
 *             p_Data           - data to write to FIFO
 * Return:     0 IF successful, -1 IF failed
 * Note:
 */
_global int bcm2836_i2cFIFODataWrite( bcm2836_Peripheral *p_pHandle, uint32_t p_Data)
{
	return bcm2836_regWrite( p_pHandle, BCM2836_I2C_BSCn_FIFO_REG_OFST, p_Data);
}


/* Name: bcm2836_i2cFIFODataRead
 * Description: read data from FIFO, received on the data bus (from slave device)
 * Parameters: p_pHandle        - pointer to an initialized I2C BSCn peripheral object
 *                                which is being used to communicate with the addressed
 *                                slave device
 *             p_rBuf           - pointer to buffer in which to store received data
 * Return:     0 IF successful, -1 IF failed
 * Note:
 */
_global int bcm2836_i2cFIFODataRead( bcm2836_Peripheral *p_pHandle, uint32_t *p_rBuf)
{
	return bcm2836_regRead( p_pHandle, BCM2836_I2C_BSCn_FIFO_REG_OFST, p_rBuf);
}


/* Name: bcm2836_i2cGetStatus
 * Description: read the BSC status register (ie. get info on data transfers, errors, etc)
 * Parameters: p_pHandle        - pointer to an initialized I2C BSCn peripheral object
 *                                which is being used to communicate with the addressed
 *                                slave device
 *             p_Res            - pointer to buffer in which to store status register value
 * Return:     0 IF successful, -1 IF failed
 * Note:
 */
_global int bcm2836_i2cGetStatus( bcm2836_Peripheral *p_pHandle, uint32_t *p_Res)
{
	return bcm2836_regRead( p_pHandle, BCM2836_I2C_BSCn_S_REG_OFST, p_Res);
}


/* Name: bcm2836_i2cClearStatusFlags
 * Description: clear status flags that are reset by writing one to the corresponding bits:
 *              CLKT (clock stretch timeout), ERR_ACK (slave acknowledge error), DONE
 *              (transfer done bit)
 * Parameters: p_pHandle        - pointer to an initialized I2C BSCn peripheral object
 *                                which is being used to communicate with the addressed
 *                                slave device
 * Return:     0 IF successful, -1 IF failed
 * Note:
 */
_global int bcm2836_i2cClearStatusFlags( bcm2836_Peripheral *p_pHandle)
{
	uint32_t bmask = BIT(9)|BIT(8)|BIT(1);

	return bcm2836_regWrite( p_pHandle, BCM2836_I2C_BSCn_S_REG_OFST, bmask);
}


/* Name: bcm2836_i2cStartRead
 * Description: start an I2C data read (data from slave will be stored in the FIFO and 
 *              can be retrieved by reading from the FIFO register)
 * Parameters: p_pHandle        - pointer to an initialized I2C BSCn peripheral object
 *                                which is being used to communicate with the addressed
 *                                slave device
 * Return:     0 IF successful, -1 IF failed
 * Note:
 */
_global int bcm2836_i2cStartRead( bcm2836_Peripheral *p_pHandle)
{
	uint32_t bmask = BIT(15)|BIT(7)|BIT(4)|BIT(0);

	return bcm2836_regWrite( p_pHandle, BCM2836_I2C_BSCn_C_REG_OFST, bmask);
}


/* Name: bcm2836_i2cStartWrite
 * Description: start an I2C data write (data is initially placed in FIFO by writing to
 *              the FIFO register)
 * Parameters: p_pHandle        - pointer to an initialized I2C BSCn peripheral object
 *                                which is being used to communicate with the addressed
 *                                slave device
 * Return:     0 IF successful, -1 IF failed
 * Note:
 */
_global int bcm2836_i2cStartWrite( bcm2836_Peripheral *p_pHandle)
{
	uint32_t bmask = BIT(15)|BIT(7);

	return bcm2836_regWrite( p_pHandle, BCM2836_I2C_BSCn_C_REG_OFST, bmask);
}


/* Name: bcm2836_i2c_BusyWait
 * Description: wait until the currenct BSC I2C transfer is complete (polls DONE bit in
 *              status register)
 * Parameters: p_pHandle        - pointer to an initialized I2C BSCn peripheral object
 *                                which is being used to communicate with the addressed
 *                                slave device
 * Return:     0 IF successful, -1 IF failed
 * Note:
 */
_global void bcm2836_i2c_BusyWait( bcm2836_Peripheral *p_pHandle)
{
	if (p_pHandle == NULL)
	{
		pError("bcm2836_i2c_BusyWait - passed invalid pointer (null)");
		return;
	}

	while ( !(*( p_pHandle->m_baseVirtAdr + (BCM2836_I2C_BSCn_S_REG_OFST / 0x04)) & BIT(1)))
	{
		/* loop until S (status register) DONE bit is set */
	}
}


/* Name: bcm2836_i2cClearFIFO
 * Description: clear the FIFO (call on startup)
 * Parameters: p_pHandle        - pointer to an initialized I2C BSCn peripheral object
 *                                which is being used to communicate with the addressed
 *                                slave device
 * Return:     0 IF successful, -1 IF failed
 * Note:
 */
_global int bcm2836_i2cClearFIFO( bcm2836_Peripheral *p_pHandle)
{
	uint32_t bmask = BIT(4);

	return bcm2836_regWrite( p_pHandle, BCM2836_I2C_BSCn_C_REG_OFST, bmask);
}


/* Name: bcm2836_i2cFIFOEmpty
 * Description: test if BSCn I2C FIFO is empty
 * Parameters: p_pHandle        - pointer to an initialized I2C BSCn peripheral object
 *                                which is being used to communicate with the addressed
 *                                slave device
 * Return:     0 IF FIFO contains data, 1 if FIFO is empty, and -1 IF failed
 * Note:
 */
_global int bcm2836_i2cFIFOEmpty( bcm2836_Peripheral *p_pHandle)
{
	return ( bcm2836_regRead2( p_pHandle, BCM2836_I2C_BSCn_S_REG_OFST) & BIT(6));
}


#if 0
/*** BCM2836_TBD: Function Headers Missing, Testing Required ***/
void bcm2836_i2c_BusyWaitTimeOut( bcm2836_Peripheral *p_pHandle, uint32_t p_milliSec)
{
	if (p_pHandle == NULL)
	{
		pError("bcm2836_i2c_BusyWait - passed invalid pointer (null)");
		return;
	}

	while ( !(*( p_pHandle->m_baseVirtAdr + (BCM2836_I2C_BSCn_S_REG_OFST / 0x04)) & BIT(1)) && --p_milliSec)
	{
		/* loop until S (status register) DONE bit is set or timeout occurs*/
		usleep(1000);
	}
}
/*** END_BCM2836_TBD ***/
#endif

#endif
