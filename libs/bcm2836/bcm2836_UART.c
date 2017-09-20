#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "bcm2836.h"

// UART Clock Frequency (in E-3 MHz)
#define FUARTCLK 3000000000

#ifdef BCM2836_CONFIG

/* Name: bcm2836_UARTDisable
 * Description: disable UART
 * Parameters: p_pHandle        - pointer to an initialized UART peripheral object
 *                                which is being disabled
 * Return:     0 IF successful, -1 IF failed
 * Note:
 */
_global int bcm2836_UARTDisable( bcm2836_Peripheral *p_pHandle)
{
	return bcm2836_regResetBits( p_pHandle,
		BCM2836_UART_CR_REG_OFST, BIT(0));
}


/* Name: bcm2836_UARTEnable
 * Description: enable UART
 * Parameters: p_pHandle        - pointer to an initialized UART peripheral object
 *                                which is being enabled
 * Return:     0 IF successful, -1 IF failed
 * Note:
 */
_global int bcm2836_UARTEnable( bcm2836_Peripheral *p_pHandle)
{
	return bcm2836_regSetBits( p_pHandle,
		BCM2836_UART_CR_REG_OFST, BIT(0));
}


/* Name: bcm2836_UART_TXDisable
 * Description: disable UART transmission
 * Parameters: p_pHandle        - pointer to an initialized UART peripheral object
 *                                for which transmission is being disabled
 * Return:     0 IF successful, -1 IF failed
 * Note:
 */
_global int bcm2836_UART_TXDisable( bcm2836_Peripheral *p_pHandle)
{
	return bcm2836_regResetBits( p_pHandle,
		BCM2836_UART_CR_REG_OFST, BIT(8));
}


/* Name: bcm2836_UART_TXEnable
 * Description: enable UART transmission
 * Parameters: p_pHandle        - pointer to an initialized UART peripheral object
 *                                for which transmission is being enabled
 * Return:     0 IF successful, -1 IF failed
 * Note:
 */
_global int bcm2836_UART_TXEnable( bcm2836_Peripheral *p_pHandle)
{
	return bcm2836_regSetBits( p_pHandle, BCM2836_UART_CR_REG_OFST, BIT(8));
}


/* Name: bcm2836_UART_RXDisable
 * Description: disable UART receiving
 * Parameters: p_pHandle        - pointer to an initialized UART peripheral object
 *                                for which receiving is being disabled
 * Return:     0 IF successful, -1 IF failed
 * Note:
 */
_global int bcm2836_UART_RXDisable( bcm2836_Peripheral *p_pHandle)
{
	return bcm2836_regResetBits( p_pHandle, BCM2836_UART_CR_REG_OFST, BIT(9));
}


/* Name: bcm2836_UART_RXEnable
 * Description: enable UART receiving
 * Parameters: p_pHandle        - pointer to an initialized UART peripheral object
 *                                for which receiving is being enabled
 * Return:     0 IF successful, -1 IF failed
 * Note:
 */
_global int bcm2836_UART_RXEnable( bcm2836_Peripheral *p_pHandle)
{
	return bcm2836_regSetBits( p_pHandle, BCM2836_UART_CR_REG_OFST, BIT(9));
}


/* Name: bcm2836_UART_LoopbackEnable
 * Description: enable UART loopback mode (feeds UARTTXD path through to UARTRXD)
 * Parameters: p_pHandle        - pointer to an initialized UART peripheral object
 *                                for which loopback mode is being enabled
 * Return:     0 IF successful, -1 IF failed
 * Note:
 */
_global int bcm2836_UART_LoopbackEnable( bcm2836_Peripheral *p_pHandle)
{
	return bcm2836_regSetBits( p_pHandle, BCM2836_UART_CR_REG_OFST, BIT(7));
}


/* Name: bcm2836_UART_LoopbackDisable
 * Description: disable UART loopback mode
 * Parameters: p_pHandle        - pointer to an initialized UART peripheral object
 *                                for which loopback mode is being enabled
 * Return:     0 IF successful, -1 IF failed
 * Note:
 */
_global int bcm2836_UART_LoopbackDisable( bcm2836_Peripheral *p_pHandle)
{
	return bcm2836_regResetBits( p_pHandle, BCM2836_UART_CR_REG_OFST, BIT(7));
}


/* Name: bcm2836_UART_BusyWait
 * Description: wait (loop) while the UART is busy transmitting data
 * Parameters: p_pHandle        - pointer to an initialized UART peripheral object
 *                                which is being waited on
 * Return:     0 IF successful, -1 IF failed
 * Note:       BUSY flag (bit 3 in FR) is set as soon as transmit FIFO is non-empty
 *             regardless of whether of not UART is enabled
 */
_global void bcm2836_UART_BusyWait( bcm2836_Peripheral *p_pHandle)
{
	if (p_pHandle == NULL)
	{
		pError("bcm2836_UART_BusyWait - passed invalid pointer (null)");
		return;
	}

	while ( *( p_pHandle->m_baseVirtAdr + (BCM2836_UART_FR_REG_OFST / 0x04)) & BIT(3))
	{
		/* loop until FR (flag register) BUSY bit is cleared */
	}
}


/* Name: bcm2836_UARTSetWordLen
 * Description: set the transmit/receive word length in the line control (LCRH)
 *              register; 8bit, 7bit, 6bit or 5bit word length may be set
 * Parameters: p_pHandle        - pointer to an initialized UART peripheral object
 *                                for which TX/RX word lenght is being set
 *             p_wLen           - word length (use 8, 7, 6, or 5)
 * Return:     0 IF successful, -1 IF failed
 * Note:       Using values outside of the range provided above will result in
 *             no change to the WLEN bits in the LCRH register and the function
 *             call will return -1 (error)
 */
_global int bcm2836_UARTSetWordLen( bcm2836_Peripheral *p_pHandle, uint8_t p_wLen)
{
	uint32_t bitmask = 0x00;

	switch(p_wLen)
	{
		case 8:
			bitmask |= (BIT(6) | BIT(5));
			break;
		case 7:
			bitmask |= BIT(6);
			break;
		case 6:
			bitmask |= BIT(5);
			break;
		case 5:
			break;
		default:
			return -1;
			break;
	}

	if ( bcm2836_regResetBits( p_pHandle, BCM2836_UART_LCRH_REG_OFST,
		BIT(6) | BIT(5)) == - 1)
		return -1;

	return bcm2836_regSetBits( p_pHandle, BCM2836_UART_LCRH_REG_OFST, bitmask);
}


/* Name: bcm2836_UARTFIFODisable
 * Description: disable the UART transmit and receive FIFOs (writing to LCRH register)
 * Parameters: p_pHandle        - pointer to an initialized UART peripheral object
 *                                for which TX/RX FIFOs are being disabled
 * Return:     0 IF successful, -1 IF failed
 * Note:
 */
_global int bcm2836_UARTFIFODisable( bcm2836_Peripheral *p_pHandle)
{
	return bcm2836_regResetBits( p_pHandle, BCM2836_UART_LCRH_REG_OFST, BIT(4));
}


/* Name: bcm2836_UARTFIFOEnable
 * Description: enable the UART transmit and receive FIFOs (writing to LCRH register)
 * Parameters: p_pHandle        - pointer to an initialized UART peripheral object
 *                                for which TX/RX FIFOs are being enabled
 * Return:     0 IF successful, -1 IF failed
 * Note:
 */
_global int bcm2836_UARTFIFOEnable( bcm2836_Peripheral *p_pHandle)
{
	return bcm2836_regSetBits( p_pHandle, BCM2836_UART_LCRH_REG_OFST, BIT(4));
}


/* Name: bcm2836_UARTCharPut
 * Description: transmit a character from the UART port
 * Parameters: p_pHandle        - pointer to an initialized UART peripheral object
 *             p_char           - character to be transmitted
 * Return:     0 IF successful, -1 IF failed
 * Note:       This is a BLOCKING call; function will wait (loop) until there is 
 *             free space in the UART transmit FIFO
 */
_global int bcm2836_UARTCharPut( bcm2836_Peripheral *p_pHandle, uint8_t p_char)
{
    int s;

	while( bcm2836_UARTTransmitFIFOFull( p_pHandle))
	{
		// block until transmit FIFO has space
	}

    s = bcm2836_regWrite( p_pHandle, BCM2836_UART_DR_REG_OFST, p_char);
    if (s == -1)
        return -1;

	bcm2836_UART_BusyWait( p_pHandle);
    return 0;
}


/* Name: bcm2836_UARTCharPutNonBlocking
 * Description: transmit a character from the UART port
 * Parameters: p_pHandle        - pointer to an initialized UART peripheral object
 *             p_char           - character to be transmitted
 * Return:     0 IF successful, -1 IF failed
 * Note:       This is a NON-BLOCKING call; function will return immediately (with
 *             a return value of -1) if there is no free space in the UART TX FIFO
 */
_global int bcm2836_UARTCharPutNonBlocking( bcm2836_Peripheral *p_pHandle, uint8_t p_char)
{
    int s;

	if( bcm2836_UARTTransmitFIFOFull( p_pHandle))
		return -1;

	s = bcm2836_regWrite( p_pHandle, BCM2836_UART_DR_REG_OFST, p_char);
    if (s == -1)
        return -1;

    bcm2836_UART_BusyWait( p_pHandle);
    return 0;
}


/* Name: bcm2836_UARTTransmitFIFOFull
 * Description: test if UART transmit FIFO is full
 * Parameters: p_pHandle        - pointer to an initialized UART peripheral object
 *                                for which the TX FIFO Full bit is being tested (
 *                                Flag Register Bit 5)
 * Return:     positive value if TX FIFO is full, 0 otherwise
 * Note:
 */
_global int bcm2836_UARTTransmitFIFOFull( bcm2836_Peripheral *p_pHandle)
{
	return ( bcm2836_regRead2( p_pHandle, BCM2836_UART_FR_REG_OFST) & BIT(5) );
}


/* Name: bcm2836_UARTReceiveFIFOEmpty
 * Description: test if UART receive FIFO is empty
 * Parameters: p_pHandle        - pointer to an initialized UART peripheral object
 *                                for which the RX FIFO Empty bit is being tested (
 *                                Flag Register Bit 4)
 * Return:     positive value if RX FIFO is empty, 0 IF RX FIFO contains data
 * Note:
 */
_global int bcm2836_UARTReceiveFIFOEmpty( bcm2836_Peripheral *p_pHandle)
{
	return ( bcm2836_regRead2( p_pHandle, BCM2836_UART_FR_REG_OFST) & BIT(4));
}


/* Name: bcm2836_UARTCharGet
 * Description: get a character from the receive FIFO
 * Parameters: p_pHandle        - pointer to an initialized UART peripheral object
 *             p_buf            - pointer to buffer in which to store the received
 *                                character
 * Return:     0 IF successful, -1 IF failed
 * Note:       This is a BLOCKING call; function will wait (loop) until there is 
 *             a character in the UART receive FIFO before returning
 */
_global int bcm2836_UARTCharGet( bcm2836_Peripheral *p_pHandle, uint32_t *p_buf)
{
	while( bcm2836_UARTReceiveFIFOEmpty( p_pHandle))
	{
		// block until a character is present in the RX FIFO
	}

	return bcm2836_regRead( p_pHandle, BCM2836_UART_DR_REG_OFST, p_buf);
}


/* Name: bcm2836_UARTCharGetNonBlocking
 * Description: get a character from the receive FIFO
 * Parameters: p_pHandle        - pointer to an initialized UART peripheral object
 *             p_buf            - pointer to buffer in which to store the received
 *                                character
 * Return:     0 IF successful, -1 IF failed
 * Note:       This is a NON-BLOCKING call; function will return immediately (with
 *             a return value of -1) if the UART RX FIFO is empty
 */
_global int bcm2836_UARTCharGetNonBlocking( bcm2836_Peripheral *p_pHandle, uint32_t *p_buf)
{
	if ( bcm2836_UARTReceiveFIFOEmpty( p_pHandle))
		return -1;

	return bcm2836_regRead( p_pHandle, BCM2836_UART_DR_REG_OFST, p_buf);
}


/* Name: bcm2836_UARTBaudRateDivider
 * Description: get the Integral and Fractional components of the baud rate divisor
 *              for a desired baud rate
 * Parameters: p_baudRate       - desired baud rate
 *             p_brd_int        - buffer to store the calculated integer part of BAUDDIV
 *             p_brd_frac       - buffer to store the calculated fractional part of BAUDDIV
 *             p_error          - buffer to store the calculatederror percentage between the
 *                                desired baud rate and the generated baud rate as a result of
 *                                the determined BRDi and BRDf values 
 * Return:     None
 * Note:       
 */
_global void bcm2836_UARTBaudRateDivider( uint32_t  p_baudRate,
                                          uint32_t *p_brd_int,
                                          uint32_t *p_brd_frac,
                                          uint32_t *p_error)
{
    ///////////////////////////////////////////////////////////////////
    // From ARM Manual:
    // - BAUDDIV = (Fuartclock / (16 * Baud Rate))
    // - BAUDDIV is divided into its integral and fractional components
    //   ex( if BAUDDIV = 1.085, then BRDi=1 and BRDf = 0.085)
    // - IBRD = BRDi
    // - FBRD = integer( (BRDf * 64) + 0.5))
    //   ***NOTE: we use 500 instead of 0.5 as we initially scaled the clock
    //            frequency by a factor of 1000 to preserve precision that 
    //            would otherwise be lost during integer division
    ///////////////////////////////////////////////////////////////////

    uint32_t bdiv;
    uint32_t baud_rate_gen, bdiv_gen;

    bdiv = (FUARTCLK / (16 * p_baudRate));

    *p_brd_int = bdiv / 1000;
    *p_brd_frac = (((bdiv % 1000) * 64) + 500) / 1000;

    if (p_error == NULL)
        return;

    /* Calculate error */

    baud_rate_gen = FUARTCLK / (16 * ((*p_brd_int*1000) + ((*p_brd_frac*1000)/64)));

    if (baud_rate_gen >= p_baudRate)
        *p_error = (baud_rate_gen - p_baudRate) * 100 / p_baudRate;
    else
        *p_error = (p_baudRate - baud_rate_gen) * 100 / p_baudRate;

}



/* Name: bcm2836_UARTSetBaudRate
 * Description: set the baud rate divisor value to control the baud rate output of the
 *              UART baud rate generator
 * Parameters: p_pHandle       - pointer to an initialized UART peripheral object
 *             p_baudRate      - baud rate
 * Return:     None
 * Note:       *** NOTE: Optionally modify this function to examine the error % as a result
 *                       of specific IBRD and FBRD values (error % determined by bcm2836_UARTBaudRateDivider).
 *                       If the error % surpasses an acceptable threshold return -1 and do not modify the
 *                       current contents of IBRD_REG and FBRD_REG.
 */
int bcm2836_UARTSetBaudRate( bcm2836_Peripheral *p_pHandle, uint32_t p_baudRate)
{
    int s;
    uint32_t int_brd, frac_brd;

    if (p_baudRate < 0)
        return -1;

    bcm2836_UARTBaudRateDivider( p_baudRate, &int_brd, &frac_brd, NULL);

    // IF BRDi or BRDf are invalid
    if ( int_brd > 0xFFFF || frac_brd > 0x3F)
        return -1; 

    s = bcm2836_regWrite( p_pHandle, BCM2836_UART_IBRD_REG_OFST, int_brd);
    // IF failed to write to UART_IBRD register
    if ( s == -1)
        return -1;

    s = bcm2836_regWrite( p_pHandle, BCM2836_UART_FBRD_REG_OFST, frac_brd);
    // IF failed to write to UART_FBRD register
    if ( s == -1)
        return -1;

    return 0;
}


/* Name: bcm2836_UARTConfig
 * Description: called to configure the UART
 * Parameters: p_pHandle        - pointer to an initialized UART peripheral object
 *             p_uartEn         - BCM2836_UART_CONFIG_UARTENABLE or BCM2836_UART_CONFIG_UARTDISABLE
 *             p_txEn           - BCM2836_UART_CONFIG_TXENABLE or BCM2836_UART_CONFIG_TXDISABLE
 *             p_rxEn           - BCM2836_UART_CONFIG_RXENABLE or BCM2836_UART_CONFIG_RXDISABLE
 *             p_lbEn           - BCM2836_UART_CONFIG_LOOPBACKENABLE or BCM2836_UART_CONFIG_LOOPBACKDISABLE
 *             p_fifoEn         - BCM2836_UART_CONFIG_FIFOENABLE or BCM2836_UART_CONFIG_FIFODISABLE
 *             p_wlen           - BCM2836_UART_CONFIG_WLEN8, BCM2836_UART_CONFIG_WLEN7,
 *                                BCM2836_UART_CONFIG_WLEN6 or BCM2836_UART_CONFIG_WLEN5
 *             p_baudRate       - baud rate to set for the UART baud rate generator
 * Return:     0 IF successful, -1 IF failed
 * Note:       // BCM2836_TBD: more configurations are needed; Buad Rate, Parity Bit etc..
 */
_global int bcm2836_UARTConfig( bcm2836_Peripheral          *p_pHandle,
                                e_bcm2836_Periph_ConfigItem  p_uartEn,
                                e_bcm2836_Periph_ConfigItem  p_txEn,
                                e_bcm2836_Periph_ConfigItem  p_rxEn,
                                e_bcm2836_Periph_ConfigItem  p_lbEn,
                                e_bcm2836_Periph_ConfigItem  p_fifoEn,
                                e_bcm2836_Periph_ConfigItem  p_wlen,
                                uint32_t                     p_baudRate)
{
	///////////////////////////////////////////////////////////////////
	// When configuring LCRH (line control) Register:
	// UART must be disabled

	// When configuring CR (control) Register:
	// 1) disable UART
	// 2) wait for end of transmission/reception of current char
	// 3) flush the transmit FIFO by setting FEN (enable FIFOs)
	//    bit in the LCRH register to 0 (call bcm2836_UARTFIFODisable)
	// 4) reprogram control register, CR
	// 5) enable UART
    ///////////////////////////////////////////////////////////////////

    int s;
    uint8_t wlen;

    s = bcm2836_UARTDisable( p_pHandle);
    if ( s == -1)
    	return -1;

    bcm2836_UART_BusyWait( p_pHandle);

    s = bcm2836_UARTFIFODisable( p_pHandle);
    if ( s == -1)
    	return -1;


    // Enable/Disable Data Transmission
    switch(p_txEn)
    {
    	case BCM2836_UART_CONFIG_TXENABLE:
    		s = bcm2836_UART_TXEnable( p_pHandle);
    		if ( s == -1)
    			return -1;
    		break;
    	case BCM2836_UART_CONFIG_TXDISABLE:
    		s = bcm2836_UART_TXDisable( p_pHandle);
    		if ( s == -1)
    			return -1;
    		break;

    	default:
    		return -1; // Invalid Arg
    		break;
    }


    // Enable/Disable Data Reception
    switch(p_rxEn)
    {
    	case BCM2836_UART_CONFIG_RXENABLE:
    		s = bcm2836_UART_RXEnable( p_pHandle);
    		if ( s == -1)
    			return -1;
    		break;
    	case BCM2836_UART_CONFIG_RXDISABLE:
    		s = bcm2836_UART_RXDisable( p_pHandle);
    		if ( s == -1)
    			return -1;
    		break;

    	default:
    		return -1; // Invalid Arg
    		break;
    }


    // Enable/Disable Data Loopback Mode (for testing)
    switch(p_lbEn)
    {
    	case BCM2836_UART_CONFIG_LOOPBACKENABLE:
    		s = bcm2836_UART_LoopbackEnable( p_pHandle);
    		if ( s == -1)
    			return -1;
    		break;
    	case BCM2836_UART_CONFIG_LOOPBACKDISABLE:
    		s = bcm2836_UART_LoopbackDisable( p_pHandle);
    		if ( s == -1)
    			return -1;
    		break;

    	default:
    		return -1; // Invalid Arg
    		break;
    }


    // Enable/Disable TX/RX FIFOs
    switch(p_fifoEn)
    {
    	case BCM2836_UART_CONFIG_FIFOENABLE:
    		s = bcm2836_UARTFIFOEnable( p_pHandle);
    		if ( s == -1)
    			return -1;
    		break;
    	case BCM2836_UART_CONFIG_FIFODISABLE:
    		s = bcm2836_UARTFIFODisable( p_pHandle);
    		if ( s == -1)
    			return -1;
    		break;

    	default:
    		return -1; // Invalid Arg
    		break;
    }


    // Configure Word Length
    switch(p_wlen)
    {
    	case BCM2836_UART_CONFIG_WLEN8:
    		wlen = 8;
    		break;
    	case BCM2836_UART_CONFIG_WLEN7:
    		wlen = 7;
    		break;
    	case BCM2836_UART_CONFIG_WLEN6:
    		wlen = 6;
    		break;
    	case BCM2836_UART_CONFIG_WLEN5:
    		wlen = 5;
    		break;

    	default:
    		return -1; // Invalid Arg
    		break;
    }
    s = bcm2836_UARTSetWordLen( p_pHandle, wlen);
    if ( s == -1)
    	return -1;


    // Set the baud rate
    s = bcm2836_UARTSetBaudRate( p_pHandle, p_baudRate);
    if ( s == -1)
        return -1;


    // Enable/Disable UART Peripheral (NOTE: This should ALWAYS
    // be done as the last step of configuraiton)
    switch(p_uartEn)
    {
    	case BCM2836_UART_CONFIG_UARTENABLE:
    		s = bcm2836_UARTEnable( p_pHandle);
    		if ( s == -1)
    			return -1;
    		break;
    	case BCM2836_UART_CONFIG_UARTDISABLE:
    		s = bcm2836_UARTDisable( p_pHandle);
    		if ( s == -1)
    			return -1;
    		break;

    	default:
    		return -1; // Invalid Arg
    		break;
    }

    return 0;

}

#endif
