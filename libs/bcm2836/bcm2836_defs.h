#ifndef _BCM2836_DEFS_H_
#define _BCM2836_DEFS_H_

#include <stdint.h>
#include <stdio.h>

/* Macros */
#define pError(str) \
	printf("%s:LINE%4d Error: %s\n", __FILE__, __LINE__, str)

#define BIT(x) \
	(0x00000001 << x)

/* Definitions*/

#define _local static
#define _global

#ifdef BCM2836_CONFIG

/*****************************/
/***** BCM2836 REGISTERS *****/
/*****************************/

/* ===== GPIO Peripheral ===== */
#define BCM2836_GPIO_PERIPH_BASE_ADR        0x3f200000     /* GPIO Peripheral Base Address */
#define BCM2836_GPIO_GPFSEL0_REG_OFST       0x00           /* GPIO Function Select 0 */
#define BCM2836_GPIO_GPFSEL1_REG_OFST       0x04           /* GPIO Function Select 1 */
#define BCM2836_GPIO_GPFSEL2_REG_OFST       0x08           /* GPIO Function Select 2 */
#define BCM2836_GPIO_GPFSEL3_REG_OFST       0x0c           /* GPIO Function Select 3 */
#define BCM2836_GPIO_GPFSEL4_REG_OFST       0x10           /* GPIO Function Select 4 */
#define BCM2836_GPIO_GPFSEL5_REG_OFST       0x14           /* GPIO Function Select 5 */
#define BCM2836_GPIO_GPSET0_REG_OFST        0x1c           /* GPIO Pin Output Set 0 */
#define BCM2836_GPIO_GPSET1_REG_OFST        0x20           /* GPIO Pin Output Set 1 */
#define BCM2836_GPIO_GPCLR0_REG_OFST        0x28           /* GPIO Pin Output Clear 0 */
#define BCM2836_GPIO_GPCLR1_REG_OFST        0x2c           /* GPIO Pin Output Clear 1 */
#define BCM2836_GPIO_GPLEV0_REG_OFST        0x34           /* GPIO Pin Level 0 */
#define BCM2836_GPIO_GPLEV1_REG_OFST        0x38           /* GPIO Pin Level 1 */
#define BCM2836_GPIO_GPEDS0_REG_OFST        0x40           /* GPIO Pin Event Detect Status 0 */
#define BCM2836_GPIO_GPEDS1_REG_OFST        0x44           /* GPIO Pin Event Detect Status 1 */
#define BCM2836_GPIO_GPREN0_REG_OFST        0x4c           /* GPIO Pin Rising Edge Detect Enable 0 */
#define BCM2836_GPIO_GPREN1_REG_OFST        0x50           /* GPIO Pin Rising Edge Detect Enable 1 */
#define BCM2836_GPIO_GPFEN0_REG_OFST        0x58           /* GPIO Pin Falling Edge Detect Enable 0 */
#define BCM2836_GPIO_GPFEN1_REG_OFST        0x5c           /* GPIO Pin Falling Edge Detect Enable 1 */
#define BCM2836_GPIO_GPHEN0_REG_OFST        0x64           /* GPIO Pin High Detect Enable 0 */
#define BCM2836_GPIO_GPHEN1_REG_OFST        0x68           /* GPIO Pin High Detect Enable 1 */
#define BCM2836_GPIO_GPLEN0_REG_OFST        0x70           /* GPIO Pin Low Detect Enable 0 */
#define BCM2836_GPIO_GPLEN1_REG_OFST        0x74           /* GPIO Pin Low Detect Enable 1 */
#define BCM2836_GPIO_GPAREN0_REG_OFST       0x7c           /* GPIO Pin Async. Rising Edge Detect 0 */
#define BCM2836_GPIO_GPAREN1_REG_OFST       0x80           /* GPIO Pin Async. Rising Edge Detect 1 */
#define BCM2836_GPIO_GPAFEN0_REG_OFST       0x88           /* GPIO Pin Async. Falling Edge Detect 0 */
#define BCM2836_GPIO_GPAFEN1_REG_OFST       0x8c           /* GPIO Pin Async. Falling Edge Detect 1 */
#define BCM2836_GPIO_GPPUD_REG_OFST         0x94           /* GPIO Pin Pull-up/down Enable */
#define BCM2836_GPIO_GPPUDCLK0_REG_OFST     0x98           /* GPIO Pin Pull-up/down Enable Clock 0 */
#define BCM2836_GPIO_GPPUDCLK1_REG_OFST     0x9c           /* GPIO Pin Pull-up/down Enable Clock 1 */
#define BCM2836_GPIO_PERIPH_BYTE_LEN        0xb4           /* GPIO Peripheral Memory Block Length */

/* ===== UART Peripheral ===== */
#define BCM2836_UART_PERIPH_BASE_ADR        0x3f201000     /* UART Peripheral Base Address */
#define BCM2836_UART_DR_REG_OFST            0x00           /* UART Data Register */
#define BCM2836_UART_RSRECR_REG_OFST        0x04           /* UART Receive Status/Error Clear Register */
#define BCM2836_UART_FR_REG_OFST            0x18           /* UART Flag Register */
#define BCM2836_UART_IBRD_REG_OFST          0x24           /* UART Integer Baud Rate Divisor */
#define BCM2836_UART_FBRD_REG_OFST          0x28           /* UART Fractional Baud Rate Divisor */
#define BCM2836_UART_LCRH_REG_OFST          0x2c           /* UART Line Control Register */
#define BCM2836_UART_CR_REG_OFST            0x30           /* UART Control Register */
#define BCM2836_UART_IFLS_REG_OFST          0x34           /* UART Interupt FIFO Level Select Register */
#define BCM2836_UART_IMSC_REG_OFST          0x38           /* UART Interupt Mask Set Clear Register */
#define BCM2836_UART_RIS_REG_OFST           0x3c           /* UART Raw Interupt Status Register */
#define BCM2836_UART_MIS_REG_OFST           0x40           /* UART Masked Interupt Status Register */
#define BCM2836_UART_ICR_REG_OFST           0x44           /* UART Interupt Clear Register */
#define BCM2836_UART_DMACR_REG_OFST         0x48           /* UART DMA Control Register */
#define BCM2836_UART_ITCR_REG_OFST          0x80           /* UART Test Control Register */
#define BCM2836_UART_ITIP_REG_OFST          0x84           /* UART Integration Test Input Register */
#define BCM2836_UART_ITOP_REG_OFST          0x88           /* UART Integration Test Output Register */
#define BCM2836_UART_TDR_REG_OFST           0x8c           /* UART Test Data Register */
#define BCM2836_UART_PERIPH_BYTE_LEN        0x1000         /* UART Peripheral Memory Block Length */

/* ===== I2C Peripheral ===== */
/* Broadcom Serial Controller (BSC) 0 */
#define BCM2836_I2C_BSC0_PERIPH_BASE_ADR    0x3f205000     /* I2C BSC0 Base Address */
/* Broadcom Serial Controller (BSC) 1 */
#define BCM2836_I2C_BSC1_PERIPH_BASE_ADR    0x3f804000     /* I2C BSC1 Base Address */
/* Broadcom Serial Controller (BSC) 2 */
#define BCM2836_I2C_BSC2_PERIPH_BASE_ADR    0x3f805000     /* I2C BSC2 Base Address */

#define BCM2836_I2C_BSCn_C_REG_OFST         0x00           /* I2C BSCn Control Register */
#define BCM2836_I2C_BSCn_S_REG_OFST         0x04           /* I2C BSCn Status Register */
#define BCM2836_I2C_BSCn_DLEN_REG_OFST      0x08           /* I2C BSCn Data Length Register */
#define BCM2836_I2C_BSCn_A_REG_OFST         0x0c           /* I2C BSCn Slave Address Register */
#define BCM2836_I2C_BSCn_FIFO_REG_OFST      0x10           /* I2C BSCn Data FIFO Register */
#define BCM2836_I2C_BSCn_DIV_REG_OFST       0x14           /* I2C BSCn Clock Divider Register */
#define BCM2836_I2C_BSCn_DEL_REG_OFST       0x18           /* I2C BSCn Data Delay Register */
#define BCM2835_I2C_BSCn_CLKT_REG_OFST      0x1c           /* I2C BSCn Clock Stretch Timeout Register */
#define BCM2836_I2C_BSCn_PERIPH_BYTE_LEN    0x20           /* I2C BSCn Peripheral Memory Block Length */

/* ===== SPI (Serial peripheral interface) ===== */
#define BCM2836_SPI_PERIPH_BASE_ADR         0x3f204000     /* SPI Peripheral Base Address */
#define BCM2836_SPI_CS_REG_OFST             0x00           /* SPI Master Control and Status Register */
#define BCM2836_SPI_FIFO_REG_OFST           0x04           /* SPI Master TX and RX FIFOs Register */
#define BCM2836_SPI_CLK_REG_OFST            0x08           /* SPI Master Clock Divider Register */
#define BCM2836_SPI_DLEN_REG_OFST           0x0c           /* SPI Master Data Length Register */
#define BCM2836_SPI_LTOH_REG_OFST           0x10           /* SPI LOSSI Mode TOH Register */
#define BCM2836_SPI_DC_REG_OFST             0x14           /* SPI DMA DREQ Controls Register */
#define BCM2836_SPI_PERIPH_BYTE_LEN         0x1000         /* SPI Peripheral Memory Block Length */

#endif


#ifdef TEST_CONFIG
	/* fake peripheral for testing purposes - RDWR ops done on "nv" file */
#define BCM2836_TEST_PERIPH_BASE_ADR        0x00000000
#define BCM2836_TEST_PERIPH_BYTE_LEN        0x1000
#define BCM2836_TEST_PERIPH_TEST_REG_OFST   0x00
#endif



/* Name: bcm2836_Peripheral
 * Description: main bcm2836 peripheral structure
 * Members: m_baseVirtAdr     - address in virtual memory at which
 *                              the physical mapping begins; writing
 *                              to a mapped memory area is done by
 *                              *(m_baseVirtAdr + <OFST>); this value
 *                              will be set by mmap() in most cases
 *          m_fd              - file descriptor referring to the
 *                              physical file being mapped (usually
 *                              this will be /dev/mem)
 *          m_len             - length of mapping in bytes
 *          m_basePhysAdr     - unsigned integer the physical address
 *                              at which the mapping should start (ie.
 *                              for GPIO this is 0x3f200000)
 */
typedef struct bcm2836_Peripheral {
	uint32_t     *m_baseVirtAdr;     // virtual memory base address (set by call to mmap)
	int           m_fd;              // file descriptor for physical memory file being mapped (usually /dev/mem)
	uint32_t      m_len;             // length (in bytes) of memory mapping
	uint32_t      m_basePhysAdr;     // physical address from which to map (offset from start of file) in hex (check datasheet)
} bcm2836_Peripheral;




typedef enum bcm2836_ErrCode {

	/* BCM2836 Core Errors */
	errSuccess = 0,                  // Success
	errGeneric,                      // Generic Error (Unspecified)
	errFileOpenFail,                 // Error opening a file - call to open() returned -1
	errFileCloseFail,                // Error closing a file - call to close() returned -1
	errMemMapFail,                   // Error mapping an area of memory - call to mmap() returned MAP_FAILED
	errMemUnmapFail,                 // Error unmapping an area of memroy - calle to munmap() returned -1
	errInvPeriphHandle,              // Invalid (usually uninitialized) peripheral handle
	errInvRegOffset,                 // Invalid register offset




} bcm2836_ErrCode;




/********************************************/
/***** BCM2836 GPIO Perhipheral Defines *****/
/********************************************/
typedef enum e_bcm2836_GPIO_Pin
{
	BCM2836_GPIO_PIN_MIN = 1,
	BCM2836_GPIO_PIN_2,
	BCM2836_GPIO_PIN_3,
	BCM2836_GPIO_PIN_4,
	BCM2836_GPIO_PIN_5,
	BCM2836_GPIO_PIN_6,
	BCM2836_GPIO_PIN_7,
	BCM2836_GPIO_PIN_8,
	BCM2836_GPIO_PIN_9,
	BCM2836_GPIO_PIN_10,
	BCM2836_GPIO_PIN_11,
	BCM2836_GPIO_PIN_12,
	BCM2836_GPIO_PIN_13,
	BCM2836_GPIO_PIN_14,
	BCM2836_GPIO_PIN_15,
	BCM2836_GPIO_PIN_16,
	BCM2836_GPIO_PIN_17,
	BCM2836_GPIO_PIN_18,
	BCM2836_GPIO_PIN_19,
	BCM2836_GPIO_PIN_20,
	BCM2836_GPIO_PIN_21,
	BCM2836_GPIO_PIN_22,
	BCM2836_GPIO_PIN_23,
	BCM2836_GPIO_PIN_24,
	BCM2836_GPIO_PIN_25,
	BCM2836_GPIO_PIN_26,
	BCM2836_GPIO_PIN_27,
	BCM2836_GPIO_PIN_MAX

} e_bcm2836_GPIO_Pin;


typedef enum e_bcm2836_GPIO_AltF
{
	BCM2836_GPIO_ALTF_0 = 0,
	BCM2836_GPIO_ALTF_1,
	BCM2836_GPIO_ALTF_2,
	BCM2836_GPIO_ALTF_3,
	BCM2836_GPIO_ALTF_4,
	BCM2836_GPIO_ALTF_5,
	BCM2836_GPIO_ALTF_MAX

} e_bcm2836_GPIO_AltF;


typedef enum e_bcm2836_GPIO_PinDir
{
	BCM2836_GPIO_DIR_INPUT = 0,
	BCM2836_GPIO_DIR_OUTPUT,
	BCM2836_GPIO_DIR_MAX

} e_bcm2836_GPIO_PinDir;


typedef enum e_bcm2836_GPIO_PinLevel
{
	BCM2836_GPIO_PIN_LEVEL_LOW = 0,
	BCM2836_GPIO_PIN_LEVEL_HIGH,
	BCM2836_GPIO_PIN_LEVEL_MAX

} e_bcm2836_GPIO_PinLevel;



typedef enum e_bcm2836_UART_ConfigItem
{
/********************************************/
/***** BCM2836 UART Configuration Items *****/
/********************************************/
	BCM2836_UART_CONFIG_UARTENABLE = 0,
	BCM2836_UART_CONFIG_UARTDISABLE,
	BCM2836_UART_CONFIG_TXENABLE,
	BCM2836_UART_CONFIG_TXDISABLE,
	BCM2836_UART_CONFIG_RXENABLE,
	BCM2836_UART_CONFIG_RXDISABLE,
	BCM2836_UART_CONFIG_LOOPBACKENABLE,
	BCM2836_UART_CONFIG_LOOPBACKDISABLE,
	BCM2836_UART_CONFIG_FIFOENABLE,
	BCM2836_UART_CONFIG_FIFODISABLE,
	BCM2836_UART_CONFIG_WLEN8,
	BCM2836_UART_CONFIG_WLEN7,
	BCM2836_UART_CONFIG_WLEN6,
	BCM2836_UART_CONFIG_WLEN5,

/*******************************************/
/***** BCM2836 SPI Configuration Items *****/
/*******************************************/
	BCM2836_SPI_CONFIG_CHIPSEL0 = 0,
	BCM2836_SPI_CONFIG_CHIPSEL1,
	BCM2836_SPI_CONFIG_CHIPSEL2,
	BCM2836_SPI_CONFIG_CHIPSELNONE,
	BCM2836_SPI_CONFIG_CLKPOL_RSTATELOW,
	BCM2836_SPI_CONFIG_CLKPOL_RSTATEHIGH,
	BCM2836_SPI_CONFIG_CLKPHA_EDGE_AT_START,
	BCM2836_SPI_CONFIG_CLKPHA_EDGE_AT_MIDDLE,

/***********************************************/
/***** BCM2836 BSC I2C Configuration Items *****/
/***********************************************/

/*******************************************/
/***** BCM2836 SPI Configuration Items *****/
/*******************************************/

} e_bcm2836_Periph_ConfigItem;


#endif
