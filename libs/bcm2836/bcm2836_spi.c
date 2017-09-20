#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include "bcm2836.h"

#ifdef BCM2836_CONFIG

// Mutex needed in case multiple threads are accessing FIFO and,
// therefore attempting to set the TA bit in the control register.
// Not using the mutex will cause deadlock-like issues due to the
// race condition created by allowing multi-threaded access to a
// shared memory location.
static pthread_mutex_t mxSPI = PTHREAD_MUTEX_INITIALIZER;


/* Name: bcm2836_SPI_ConfigChipSelect
 * Description: Configure Chip Select
 * Parameters: p_pHandle        - pointer to an initialized SPI peripheral object
 *             p_csVal          - chip select index (0 - chip select 0)
 *                                                  (1 - chip select 1)
 *                                                  (2 - chip select 3)
 *                                                  (3 - reserved (none))
 * Return:     0 IF successful, -1 IF failed
 * Note:
 */
_global int bcm2836_SPI_ConfigChipSelect( bcm2836_Peripheral *p_pHandle, uint32_t p_csVal)
{
	uint32_t bitmask;

	/* read the current CS register value */
	bitmask = bcm2836_regRead2( p_pHandle, BCM2836_SPI_CS_REG_OFST);

	switch (p_csVal)
	{
		case 0:
			bitmask &= ~(BIT(1)|BIT(0)); // Chip Select 0 (00b)
			break;
		case 1:
			bitmask &= ~(BIT(1));        // Chip Select 1 (01b)
			bitmask |= BIT(0);
			break;
		case 2:
			bitmask |= BIT(1);           // Chip Select 2 (10b)
			bitmask &= ~(BIT(0));
			break;
		case 3:
			bitmask |= (BIT(1)|BIT(0));  // Reserved (11b)
			break;

		default:
			break;
	}

	return bcm2836_regWrite( p_pHandle, BCM2836_SPI_CS_REG_OFST, bitmask);
}


/* Name: bcm2836_SPI_SetTABit
 * Description: Set/Reset SPI Control Transfer Active bit
 *              (Activate of Deactivate Data Transfer)
 * Parameters: p_pHandle        - pointer to an initialized SPI peripheral object
 *             p_sFlag          - true (set TA bit)
 *                                false (clear TA bit)
 * Return:     0 IF successful, -1 IF failed
 * Note:
 */
_global int bcm2836_SPI_SetTABit( bcm2836_Peripheral *p_pHandle, bool p_sFlag)
{
	uint32_t bitmask;

	/* read the current CS register value */
	bitmask = bcm2836_regRead2( p_pHandle, BCM2836_SPI_CS_REG_OFST);

	if (p_sFlag)
		bitmask |= BIT(7);    // TA bit HIGH (set)
	else
		bitmask &= ~(BIT(7)); // TA bit LOW  (clear)

	return bcm2836_regWrite( p_pHandle, BCM2836_SPI_CS_REG_OFST, bitmask);
}


/* Name: bcm2836_SPI_ConfigClkPolarity
 * Description: Configure SPI Clock Rest State (LOW of HIGH)
 * Parameters: p_pHandle        - pointer to an initialized SPI peripheral object
 *             p_sFlag          - true (Rest state of clock = HIGH)
 *                                false (Rest state of clock = LOW)
 * Return:     0 IF successful, -1 IF failed
 * Note:
 */
_global int bcm2836_SPI_ConfigClkPolarity( bcm2836_Peripheral *p_pHandle, bool p_sFlag)
{
	uint32_t bitmask;

	/* read the current CS register value */
	bitmask = bcm2836_regRead2( p_pHandle, BCM2836_SPI_CS_REG_OFST);

	if (p_sFlag)
		bitmask |= BIT(3);    // HIGH Clock rest state
	else
		bitmask &= ~(BIT(3)); // LOW Clock rest state

	return bcm2836_regWrite( p_pHandle, BCM2836_SPI_CS_REG_OFST, bitmask);
}


/* Name: bcm2836_SPI_ConfigClkPhase
 * Description: Configure SPI Clock Phase (first transition at START or MIDDLE of data bit)
 * Parameters: p_pHandle        - pointer to an initialized SPI peripheral object
 *             p_sFlag          - true (First SCLK transition at START of data bit)
 *                                false (First SCLK transition at MIDDLE of data bit)
 * Return:     0 IF successful, -1 IF failed
 * Note:
 */
_global int bcm2836_SPI_ConfigClkPhase( bcm2836_Peripheral *p_pHandle, bool p_sFlag)
{
	uint32_t bitmask;

	/* read the current CS register value */
	bitmask = bcm2836_regRead2( p_pHandle, BCM2836_SPI_CS_REG_OFST);

	if (p_sFlag)
		bitmask |= BIT(2);     // First SCLK transition at START of data bit
	else
		bitmask &= ~(BIT(2));  // First SCLK transition at MIDDLE of data bit

	return bcm2836_regWrite( p_pHandle, BCM2836_SPI_CS_REG_OFST, bitmask);
}


/* Name: bcm2836_SPI_TXFIFOClear
 * Description: Clear SPI TX FIFO
 * Parameters: p_pHandle        - pointer to an initialized SPI peripheral object
 * Return:     0 IF successful, -1 IF failed
 * Note:
 */
_global int bcm2836_SPI_TXFIFOClear( bcm2836_Peripheral *p_pHandle)
{
	uint32_t bitmask;

	/* read the current CS register value */
	bitmask = bcm2836_regRead2( p_pHandle, BCM2836_SPI_CS_REG_OFST);
	bitmask |= BIT(4);
	
	return bcm2836_regWrite( p_pHandle, BCM2836_SPI_CS_REG_OFST, bitmask);
}


/* Name: bcm2836_SPI_RXFIFOClear
 * Description: Clear SPI RX FIFO
 * Parameters: p_pHandle        - pointer to an initialized SPI peripheral object
 * Return:     0 IF successful, -1 IF failed
 * Note:
 */
_global int bcm2836_SPI_RXFIFOClear( bcm2836_Peripheral *p_pHandle)
{
	uint32_t bitmask;

	/* read the current CS register value */
	bitmask = bcm2836_regRead2( p_pHandle, BCM2836_SPI_CS_REG_OFST);
	bitmask |= BIT(5);
	
	return bcm2836_regWrite( p_pHandle, BCM2836_SPI_CS_REG_OFST, bitmask);
}


/* Name: bcm2836_SPI_FIFOWrite
 * Description: Write data to SPI transmit FIFO
 * Parameters: p_pHandle        - pointer to an initialized SPI peripheral object
 *             p_dByte          - TX data (LSB is transferred)
 * Return:     0 IF successful, -1 IF failed
 * Note:
 */
_global int bcm2836_SPI_FIFOWrite( bcm2836_Peripheral *p_pHandle, uint32_t p_dByte)
{
	return bcm2836_regWrite( p_pHandle, BCM2836_SPI_FIFO_REG_OFST, p_dByte);
}


/* Name: bcm2836_SPI_FIFORead
 * Description: Read data from SPI receive FIFO
 * Parameters: p_pHandle        - pointer to an initialized SPI peripheral object
 *             p_Res            - pointer to buffer to store RX data in (LSB is transferred)
 * Return:     0 IF successful, -1 IF failed
 * Note:
 */
_global int bcm2836_SPI_FIFORead( bcm2836_Peripheral *p_pHandle, uint32_t *p_Res)
{
	return bcm2836_regRead( p_pHandle, BCM2836_SPI_FIFO_REG_OFST, p_Res);
}


/* Name: bcm2836_SPI_ConfigClkDiv
 * Description: set the SPI clock divider value (must be a power of 2)
 * Parameters: p_pHandle        - pointer to an initialized SPI peripheral object
 *             p_CDiv           - clock divider value
 * Return:     0 IF successful, -1 IF failed
 * Note:
 */
_global int bcm2836_SPI_ConfigClkDiv( bcm2836_Peripheral *p_pHandle, uint32_t p_CDiv)
{
	return bcm2836_regWrite( p_pHandle, BCM2836_SPI_CLK_REG_OFST, p_CDiv);
}


/* Name: bcm2836_SPI_PutCharNonBlocking
 * Description: Non-blocking SPI TX data write
 * Parameters: p_pHandle        - pointer to an initialized SPI peripheral object
 *             p_Char           - character to write to TX FIFO (LSB is transferred)
 * Return:     0 IF successful, -1 IF failed
 * Note:       returns (with error) immediately if TX FIFO is full prior to the
 *             attempted data write
 */
_global int bcm2836_SPI_PutCharNonBlocking( bcm2836_Peripheral *p_pHandle, uint8_t p_Char)
{
	int s;

	// IF TXD FIFO is full (BIT_18 is 0), OR, RX FIFO is full (BIT_20 is 1)
	// NOTE: For each SPI clock cycle a full duplex data transmission occurs;
	//       the master send one bit on the MOSI line and the slave reads it,
	//       while the slave sends a bit on the MISO line adn the master reads
	//       it. This transfer occurrs simultaneously. Even if only one-way
	//       transfer is intended, the sequence is maitained. (ex. for the 
	//       nokia5110 LCD application, MISO pin is left open and the RX FIFO
	//       will get filled with '0x00' bytes for each tranfer on the MOSI line.
	//       the RX FIFO must be cleared (either by seeting the RX_FIFO_CLR bit
	//       or by reading data from the RX FIFO))
	if ( !(bcm2836_regRead2( p_pHandle, BCM2836_SPI_CS_REG_OFST) & BIT(18)) ||
		   bcm2836_regRead2( p_pHandle, BCM2836_SPI_CS_REG_OFST) & BIT(20))
		return -1;

	s = pthread_mutex_lock(&mxSPI);
	if ( s != 0)
	{
		pError("pthread_mutex_lock ERROR");
		return -1;
	}

	bcm2836_SPI_SetTABit( p_pHandle, true);
	bcm2836_regWrite( p_pHandle, BCM2836_SPI_FIFO_REG_OFST, (uint32_t) p_Char);

	bcm2836_SPI_BusyWait( p_pHandle);

	bcm2836_SPI_SetTABit( p_pHandle, false);

	pthread_mutex_unlock(&mxSPI);

	return 0;	
}


/* Name: bcm2836_SPI_GetCharNonBlocking
 * Description: Non-blocking SPI RX data read
 * Parameters: p_pHandle        - pointer to an initialized SPI peripheral object
 *             p_Char           - buffer to stroe RX data in (LSB is transferred)
 * Return:     0 IF successful, -1 IF failed
 * Note:       returns (with error) immediately if RX FIFO is empty prior to the
 *             attempted data read
 */
_global int bcm2836_SPI_GetCharNonBlocking( bcm2836_Peripheral *p_pHandle, uint32_t *p_Char)
{
	int s;

	// IF RXD FIFO is empty
	if ( !(bcm2836_regRead2( p_pHandle, BCM2836_SPI_CS_REG_OFST) & BIT(17)) )
		return -1;


	s = pthread_mutex_lock(&mxSPI);
	if ( s != 0)
	{
		pError("pthread_mutex_lock ERROR");
		return -1;
	}

	bcm2836_SPI_SetTABit( p_pHandle, true);
	bcm2836_regRead( p_pHandle, BCM2836_SPI_FIFO_REG_OFST, p_Char);

	bcm2836_SPI_BusyWait( p_pHandle);

	bcm2836_SPI_SetTABit( p_pHandle, false);

	pthread_mutex_unlock(&mxSPI);

	return 0;
}


/* Name: bcm2836_SPI_BusyWait
 * Description: Tight loop until SPI transfer (read or write) is complete
 * Parameters: p_pHandle        - pointer to an initialized SPI peripheral object
 * Return:     0 IF successful, -1 IF failed
 * Note:
 */
_global void bcm2836_SPI_BusyWait( bcm2836_Peripheral *p_pHandle)
{
	while ( !( bcm2836_regRead2( p_pHandle, BCM2836_SPI_CS_REG_OFST) & BIT(16)))
	{
		// busy wait until transfer is complete (BIT(16) set to HIGH)
	}
}



/* Name: bcm2836_SPIConfig
 * Description: called to configure the SPI Controller
 * Parameters: p_pHandle        - pointer to an initialized UART peripheral object
 *             p_chipSel        - BCM2836_SPI_CONFIG_CHIPSEL0, BCM2836_SPI_CONFIG_CHIPSEL1
 *                                BCM2836_SPI_CONFIG_CHIPSEL2, or BCM2836_SPI_CONFIG_CHIPSELNONE
 *             p_clkPol         - BCM2836_SPI_CONFIG_CLKPOL_RSTATELOW or BCM2836_SPI_CONFIG_CLKPOL_RSTATEHIGH
 *             p_clkPha         - BCM2836_SPI_CONFIG_CLKPHA_EDGE_AT_START or BCM2836_SPI_CONFIG_CLKPHA_EDGE_AT_MIDDLE
 * Return:     0 IF successful, -1 IF failed
 * Note:       // BCM2836_TBD: more configurations are needed
 */
_global int bcm2836_SPIConfig( bcm2836_Peripheral          *p_pHandle,
                               e_bcm2836_Periph_ConfigItem  p_chipSel,
                               e_bcm2836_Periph_ConfigItem  p_clkPol,
                               e_bcm2836_Periph_ConfigItem  p_clkPha,
                               uint32_t                     p_clkDiv)
{
	int s;
    uint8_t chipSel;

    // Clear TX and RX FIFOs
    s = bcm2836_SPI_TXFIFOClear( p_pHandle);
    if ( s == -1)
    	return -1;

    s = bcm2836_SPI_RXFIFOClear( p_pHandle);
    if ( s == -1)
    	return -1;

    s = bcm2836_SPI_SetTABit( p_pHandle, false);

    // Configure Chip Select
    switch(p_chipSel)
    {
    	case BCM2836_SPI_CONFIG_CHIPSEL0:
    		chipSel = 0;
    		break;
    	case BCM2836_SPI_CONFIG_CHIPSEL1:
    		chipSel = 1;
    		break;
    	case BCM2836_SPI_CONFIG_CHIPSEL2:
    		chipSel = 2;
    		break;
    	case BCM2836_SPI_CONFIG_CHIPSELNONE:
    		chipSel = 3;
    		break;

    	default:
    		return -1; // Invalid Arg
    		break;
    }
    s = bcm2836_SPI_ConfigChipSelect( p_pHandle, chipSel);
    if ( s== -1)
    	return -1;


    // Configure Clock Polarity (Rest State HIGH or LOW)
    switch(p_clkPol)
    {
    	case BCM2836_SPI_CONFIG_CLKPOL_RSTATELOW:
    		s = bcm2836_SPI_ConfigClkPolarity( p_pHandle, false);
    		if ( s == -1)
    			return -1;
    		break;
    	case BCM2836_SPI_CONFIG_CLKPOL_RSTATEHIGH:
    		s = bcm2836_SPI_ConfigClkPolarity( p_pHandle, true);
    		if ( s == -1)
    			return -1;
    		break;

    	default:
    		return -1; // Invalid Arg
    		break;
    }


    // Configure Clock Phase (First SCLK Edge at START of MIDDLE of data bit)
    switch(p_clkPha)
    {
    	case BCM2836_SPI_CONFIG_CLKPHA_EDGE_AT_START:
    		s = bcm2836_SPI_ConfigClkPhase( p_pHandle, true);
    		if ( s == -1)
    			return -1;
    		break;
    	case BCM2836_SPI_CONFIG_CLKPHA_EDGE_AT_MIDDLE:
    		s = bcm2836_SPI_ConfigClkPhase( p_pHandle, false);
    		if ( s == -1)
    			return -1;
    		break;

    	default:
    		return -1; // Invalid Arg
    		break;
    }


    // Set the clock rate (SCLK = Core Clock Speed / CDIV)
    s = bcm2836_SPI_ConfigClkDiv( p_pHandle, p_clkDiv);
    if ( s == -1)
    	return -1;


    return 0;
}

#endif
