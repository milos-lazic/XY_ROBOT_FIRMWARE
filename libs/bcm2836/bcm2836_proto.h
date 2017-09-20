#ifndef _BCM2836_PROTO_H_
#define _BCM2836_PROTO_H_

#include <stdbool.h>

/* Function Prototypes */

/************************************************/
/***** BCM2836 Core Rigister Operations API *****/
/************************************************/

int bcm2836_initPeripheral( bcm2836_Peripheral *p_pHandle,
                            uint32_t            p_bLen,
                            uint32_t            p_basePhysAdr);

int bcm2836_deinitPeripheral( bcm2836_Peripheral *p_pHandle);

int bcm2836_regSetBits( bcm2836_Peripheral *p_pHandle,
                        uint32_t            p_regOfst,
                        uint32_t            p_bitMask);

int bcm2836_regResetBits( bcm2836_Peripheral *p_pHandle,
                          uint32_t            p_regOfst,
                          uint32_t            p_bitMask);

int bcm2836_regWrite( bcm2836_Peripheral *p_pHandle,
                      uint32_t            p_regOfst,
                      uint32_t            p_regVal);

int bcm2836_regRead( bcm2836_Peripheral *p_pHandle,
                     uint32_t            p_regOfst,
                     uint32_t           *p_Res);

int bcm2836_regRead2( bcm2836_Peripheral *p_pHandle,
                      uint32_t            p_regOfst);

int bcm2836_regReset( bcm2836_Peripheral *p_pHandle,
                      uint32_t            p_regOfst);



/*************************************/
/***** GPIO Peripheral Interface *****/
/*************************************/

int bcm2836_GPIOPinTypeInput( bcm2836_Peripheral *p_pHandle,
                              e_bcm2836_GPIO_Pin  p_nPin);

int bcm2836_GPIOPinTypeOutput( bcm2836_Peripheral *p_pHandle,
                               e_bcm2836_GPIO_Pin  p_nPin);

int bcm2836_GPIOPinTypeAltFunction( bcm2836_Peripheral  *p_pHandle,
                                    e_bcm2836_GPIO_Pin   p_nPin,
                                    e_bcm2836_GPIO_AltF  p_nAltFun);

#if 0
int bcm2836_GPIOPinSet( bcm2836_Peripheral *p_pHandle,
                        e_bcm2836_GPIO_Pin  p_nPin);

int bcm2836_GPIOPinClear( bcm2836_Peripheral *p_pHandle,
                          e_bcm2836_GPIO_Pin  p_nPin);
#endif

void bcm2836_GPIOPinMasterConfig( bcm2836_Peripheral  *p_pHandle);

int bcm2836_GPIOSetPinLevel( bcm2836_Peripheral      *p_pHandle,
                             e_bcm2836_GPIO_Pin       p_nPin,
                             e_bcm2836_GPIO_PinLevel  p_pLevel);



/*************************************/
/***** UART Peripheral Interface *****/
/*************************************/

int bcm2836_UARTDisable( bcm2836_Peripheral *p_pHandle);

int bcm2836_UARTEnable( bcm2836_Peripheral *p_pHandle);

int bcm2836_UART_TXDisable( bcm2836_Peripheral *p_pHandle);

int bcm2836_UART_TXEnable( bcm2836_Peripheral *p_pHandle);

int bcm2836_UART_RXDisable( bcm2836_Peripheral *p_pHandle);

int bcm2836_UART_RXEnable( bcm2836_Peripheral *p_pHandle);

int bcm2836_UART_LoopbackEnable( bcm2836_Peripheral *p_pHandle);

int bcm2836_UART_LoopbackDisable( bcm2836_Peripheral *p_pHandle);

int bcm2836_UARTSetWordLen( bcm2836_Peripheral *p_pHandle, uint8_t p_wLen);

int bcm2836_UARTFIFODisable( bcm2836_Peripheral *p_pHandle);

int bcm2836_UARTFIFOEnable( bcm2836_Peripheral *p_pHandle);

void bcm2836_UART_BusyWait( bcm2836_Peripheral *p_pHandle);

int bcm2836_UARTCharPut( bcm2836_Peripheral *p_pHandle, uint8_t p_char);

int bcm2836_UARTCharPutNonBlocking( bcm2836_Peripheral *p_pHandle, uint8_t p_char);

int bcm2836_UARTCharGet( bcm2836_Peripheral *p_pHandle, uint32_t *p_buf);

int bcm2836_UARTCharGetNonBlocking( bcm2836_Peripheral *p_pHandle, uint32_t *p_buf);

int bcm2836_UARTTransmitFIFOFull( bcm2836_Peripheral *p_pHandle);

int bcm2836_UARTReceiveFIFOEmpty( bcm2836_Peripheral *p_pHandle);

void bcm2836_UARTBaudRateDivider( uint32_t  p_baudRate,
                                  uint32_t *p_brd_int,
                                  uint32_t *p_brd_frac,
                                  uint32_t *p_error);

int  bcm2836_UARTSetBaudRate( bcm2836_Peripheral *p_pHandle, uint32_t p_baudRate);

int bcm2836_UARTConfig( bcm2836_Peripheral          *p_pHandle,
                        e_bcm2836_Periph_ConfigItem  p_uartEn,
                        e_bcm2836_Periph_ConfigItem  p_txEn,
                        e_bcm2836_Periph_ConfigItem  p_rxEn,
                        e_bcm2836_Periph_ConfigItem  p_lbEn,
                        e_bcm2836_Periph_ConfigItem  p_fifoEn,
                        e_bcm2836_Periph_ConfigItem  p_wlen,
                        uint32_t                     p_baudRate);


/****************************************/
/***** BSC I2C Peripheral Interface *****/
/****************************************/

int bcm2836_i2cConfigSlaveAdr( bcm2836_Peripheral *p_pHandle, uint32_t p_sAdr);

int bcm2836_i2cConfigDataLen( bcm2836_Peripheral *p_pHandle, uint32_t p_dLen);

int bcm2836_i2cFIFODataWrite( bcm2836_Peripheral *p_pHandle, uint32_t p_Data);

int bcm2836_i2cFIFODataRead( bcm2836_Peripheral *p_pHandle, uint32_t *p_rBuf);

int bcm2836_i2cGetStatus( bcm2836_Peripheral *p_pHandle, uint32_t *p_Res);

int bcm2836_i2cClearStatusFlags( bcm2836_Peripheral *p_pHandle);

int bcm2836_i2cStartRead( bcm2836_Peripheral *p_pHandle);

int bcm2836_i2cStartWrite( bcm2836_Peripheral *p_pHandle);

void bcm2836_i2c_BusyWait( bcm2836_Peripheral *p_pHandle);

int bcm2836_i2cClearFIFO( bcm2836_Peripheral *p_pHandle);

int bcm2836_i2cFIFOEmpty( bcm2836_Peripheral *p_pHandle);

/*** BCM2836_TBD ***/
#if 0
void bcm2836_i2c_BusyWaitTimeOut( bcm2836_Peripheral *p_pHandle, uint32_t p_milliSec);
#endif
/*** END_BCM2836_TBD ***/



/************************************/
/***** SPI Peripheral Interface *****/
/************************************/

int bcm2836_SPI_ConfigChipSelect( bcm2836_Peripheral *p_pHandle, uint32_t p_csVal);

int bcm2836_SPI_SetTABit( bcm2836_Peripheral *p_pHandle, bool p_sFlag);

int bcm2836_SPI_ConfigClkPolarity( bcm2836_Peripheral *p_pHandle, bool p_sFlag);

int bcm2836_SPI_ConfigClkPhase( bcm2836_Peripheral *p_pHandle, bool p_sFlag);

int bcm2836_SPI_FIFOWrite( bcm2836_Peripheral *p_pHandle, uint32_t p_dByte);

int bcm2836_SPI_FIFORead( bcm2836_Peripheral *p_pHandle, uint32_t *p_Res);

int bcm2836_SPI_PutCharNonBlocking( bcm2836_Peripheral *p_pHandle, uint8_t p_Char);

int bcm2836_SPI_GetCharNonBlocking( bcm2836_Peripheral *p_pHandle, uint32_t *p_Char);

int bcm2836_SPI_ConfigClkDiv( bcm2836_Peripheral *p_pHandle, uint32_t p_CDiv);

void bcm2836_SPI_BusyWait( bcm2836_Peripheral *p_pHandle);

int bcm2836_SPI_TXFIFOClear( bcm2836_Peripheral *p_pHandle);

int bcm2836_SPI_RXFIFOClear( bcm2836_Peripheral *p_pHandle);

int bcm2836_SPIConfig( bcm2836_Peripheral          *p_pHandle,
                               e_bcm2836_Periph_ConfigItem  p_chipSel,
                               e_bcm2836_Periph_ConfigItem  p_clkPol,
                               e_bcm2836_Periph_ConfigItem  p_clkPha,
                               uint32_t                     p_clkDiv);

#endif
