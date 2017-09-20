#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include "bcm2836.h"


/* Name: bcm2836_initPeripheral
 * Description: initializes a new BCM2836 Peripheral device for a program
 *              to use; opens the peripheral device file (/dev/mem) and creates
 *              a virtual memory mapping of it in main (user space) memory
 * Parameters: p_pHandle        - pointer to the uninitialized peripheral object
 *             p_bLen           - length of mapping in bytes (ie. BCM2836_GPIO_PERIPH_BYTE_LEN)
 *             p_basePhysAdr    - base physical address of peripheral being mapped (ie.
 *                                BCM2836_GPIO_PERIPH_BASE_ADR); found in datasheet
 * Return:     0 IF successful, -1 IF failed (errno set)
 * Note:       Must be called before a peripheral can be used***
 */
int bcm2836_initPeripheral( bcm2836_Peripheral *p_pHandle,
                            uint32_t            p_bLen,
                            uint32_t            p_basePhysAdr)
{
	if (p_pHandle == NULL)
		return -1;

	p_pHandle->m_len = p_bLen;
	p_pHandle->m_basePhysAdr = p_basePhysAdr;
#if defined(TEST_CONFIG)
	p_pHandle->m_fd = open("nv", O_RDWR | O_SYNC); // nv for testing, /dev/mem/ for apps
#elif defined(BCM2836_CONFIG)
	p_pHandle->m_fd = open("/dev/mem", O_RDWR | O_SYNC);
#endif
	if ( p_pHandle->m_fd == -1)
		return -1;

	p_pHandle->m_baseVirtAdr = (uint32_t*)mmap( NULL,
                                                p_pHandle->m_len,
                                                PROT_READ | PROT_WRITE,
                                                MAP_SHARED,
                                                p_pHandle->m_fd,
                                                p_pHandle->m_basePhysAdr);
	
	if ( p_pHandle->m_baseVirtAdr == MAP_FAILED)
	{
		close(p_pHandle->m_fd);
		return -1;
	}

	return 0;
}


/* Name: bcm2836_deinitPeripheral
 * Description: deinitialize (release resources) allocated for a peripheral object;
 *              unmaps a virtual memory mapping and closes the corresponding file descrptor
 * Parameters: p_pHandle       - pointer to the initialized peripheral object that is to
 *                               be deinitialized
 * Return:     0 IF successful, -1 IF failed (errno set)
 * Note:       Call to release peripheral resourses when a mapping is no longer in use
 */
int bcm2836_deinitPeripheral( bcm2836_Peripheral *p_pHandle)
{
	int s;

	if ( p_pHandle == NULL)
		return -1;

	s = munmap( p_pHandle->m_baseVirtAdr, p_pHandle->m_len);
	if ( s == -1)
		return -1;

	s = close( p_pHandle->m_fd);
	if ( s == -1)
		return -1;

	return 0;
}


/* Name: bcm2836_regSetBits
 * Description: set bits in a register
 * Parameters: p_pHandle        - pointer to an initialized peripheral object
 *                                to which the register being written to belongs
 *             p_regOfst        - offset (in bytes) of register from the peripheral
 *                                base address (ie. use BCM2836_GPIO_FSEL_REG_0_OFST);
 *                                found in datasheet
 *             p_bitMask        - bits to be set in the register; can be bitwise OR'd
 *                                (ie. BIT(n1)|BIT(n2)|...); bits 0-31 are valid; undefined
 *                                behaviour if bounds are exceeded
 * Return:     0 IF successful, -1 IF failed (errno set)
 * Note:
 */
int bcm2836_regSetBits( bcm2836_Peripheral *p_pHandle,
						uint32_t            p_regOfst,
						uint32_t            p_bitMask)
{
	int32_t regVal;

	if ( p_pHandle == NULL)
		return -1;

	if ( p_regOfst % 0x04 != 0)
		return -1;

	regVal = bcm2836_regRead2( p_pHandle, p_regOfst);
	if (regVal == -1)
		return -1;

	*( p_pHandle->m_baseVirtAdr + (p_regOfst / 0x04)) = (regVal |=p_bitMask);
	return 0;
}


/* Name: bcm2836_regResetBits
 * Description: reset bits in a register
 * Parameters: p_pHandle        - pointer to an initialized peripheral object
 *                                to which the register being written to belongs
 *             p_regOfst        - offset (in bytes) of register from the peripheral
 *                                base address (ie. use BCM2836_GPIO_FSEL_REG_0_OFST);
 *                                found in datasheet
 *             p_bitMask        - bits to be reset in the register; can be bitwise OR'd
 *                                (ie. BIT(n1)|BIT(n2)|...); bits 0-31 are valid; undefined
 *                                behaviour if bounds are exceeded
 * Return:     0 IF successful, -1 IF failed
 * Note:
 */
int bcm2836_regResetBits( bcm2836_Peripheral *p_pHandle,
						  uint32_t            p_regOfst,
						  uint32_t            p_bitMask)
{
	int32_t regVal;

	if ( p_pHandle == NULL)
		return -1;

	if (p_regOfst % 0x04 != 0)
		return -1;

	regVal = bcm2836_regRead2( p_pHandle, p_regOfst);
	if (regVal == -1)
		return -1;

	// *( p_pHandle->m_baseVirtAdr + (p_regOfst / 0x04)) &= ~p_bitMask;

	*( p_pHandle->m_baseVirtAdr + (p_regOfst / 0x04)) = (regVal &= (~p_bitMask));

	return 0;
}


/* Name: bcm2836_regWrite
 * Description: write to a register
 * Parameters: p_pHandle        - pointer to an initialized peripheral object
 *                                to which the register being written to belongs
 *             p_regOfst        - offset (in bytes) of register from the peripheral
 *                                base address (ie. use BCM2836_GPIO_FSEL_REG_0_OFST);
 *                                found in datasheet
 *             p_regVal         - value to be written to register (32-bit)
 *                                behaviour if bounds are exceeded
 * Return:     0 IF successful, -1 IF failed
 * Note:
 */
int bcm2836_regWrite( bcm2836_Peripheral *p_pHandle,
                      uint32_t            p_regOfst,
                      uint32_t            p_regVal)
{
	if ( p_pHandle == NULL)
		return -1;

	if ( p_regOfst % 0x04 != 0)
		return -1;

	*( p_pHandle->m_baseVirtAdr + (p_regOfst / 0x04)) = p_regVal;

	return 0;
}


/* Name: bcm2836_regRead
 * Description: read a register value
 * Parameters: p_pHandle        - pointer to an initialized peripheral object
 *                                to which the register being read from belongs
 *             p_regOfst        - offset (in bytes) of register from the peripheral
 *                                base address (ie. use )BCM2836_GPIO_FSEL_REG_0_OFST);
 *                                found in datasheet
 *             p_Res            - pointer to buffer in which the register value is stored
 *                                upon a successful read
 * Return:     0 IF successful, -1 IF failed
 * Note:
 */
int bcm2836_regRead( bcm2836_Peripheral *p_pHandle,
                     uint32_t            p_regOfst,
                     uint32_t           *p_Res)
{
	if ( p_pHandle == NULL)
		return -1;

	if ( p_regOfst % 0x04 != 0)
		return -1;

	*p_Res = *(p_pHandle->m_baseVirtAdr + (p_regOfst / 0x04));

	return 0;
}


/* Name: bcm2836_regRead2
 * Description: read a register value
 * Parameters: p_pHandle        - pointer to an initialized peripheral object
 *                                to which the register being read from belongs
 *             p_regOfst        - offset (in bytes) of register from the peripheral
 *                                base address (ie. use )BCM2836_GPIO_FSEL_REG_0_OFST);
 *                                found in datasheet
 * Return:     Value stored in register IF successful, -1 IF failed
 * Note:
 */
int bcm2836_regRead2( bcm2836_Peripheral *p_pHandle,
                      uint32_t            p_regOfst)
{
	if ( p_pHandle == NULL)
		return -1;

	if ( p_regOfst % 0x04 != 0)
		return -1;

	return *(p_pHandle->m_baseVirtAdr + (p_regOfst / 0x04));
}


/* Name: bcm2836_regReset
 * Description: reset all bits in a register ( &= 0x00)
 * Parameters: p_pHandle        - pointer to an initialized peripheral object
 *                                to which the register being reset belongs
 *             p_regOfst        - offset (in bytes) of register from the peripheral
 *                                base address (ie. use )BCM2836_GPIO_FSEL_REG_0_OFST);
 *                                found in datasheet
 * Return:     0 IF successful, -1 IF failed
 * Note:       *** Does not work for all registers in BCM2836; in some cases a 'clear'
 *                 bit in another register must be set (ie. SDA Control reigster clean
 *                 bit is set to clear the FIFO for I2C transfers)
 */
int bcm2836_regReset( bcm2836_Peripheral *p_pHandle,
	                  uint32_t            p_regOfst)
{
	if ( p_pHandle == NULL)
		return -1;

	if ( p_regOfst % 0x04 != 0)
		return -1;

	*( p_pHandle->m_baseVirtAdr + (p_regOfst / 0x04)) &= 0x00000000;
	
	return 0;
}


