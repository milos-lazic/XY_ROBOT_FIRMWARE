#ifndef _DS1631_H_
#define _DS1631_H_

#ifdef BCM2836_CONFIG
/* DS1631 interface for BCM2836 */

#include <stdint.h>
#include "../../bcm2836.h"

/* DS1631 Interface Defines */
struct adc_map_pt
{
	int32_t x;
	int32_t y;
};

/* DS1631 Interface Function Prototypes */
int32_t DS1631_scale_adc_raw_to_temp( struct adc_map_pt *pTable, int32_t adc_code);
void DS1631_StartTempConvert( bcm2836_Peripheral *p_pHandle);
void DS1631_StopTempConvert( bcm2836_Peripheral *p_pHandle);
void DS1631_ReadTempRaw( bcm2836_Peripheral *p_pHandle, int32_t *p_Res);
void DS1631_GetTempHighThreshold( bcm2836_Peripheral *p_pHandle, int32_t *p_Res);
void DS1631_SetTempHighThreshold( bcm2836_Peripheral *p_pHandle, int32_t p_Temp);
void DS1631_GetTempLowThreshold( bcm2836_Peripheral *p_pHandle, int32_t *p_Res);
void DS1631_SetTempLowThreshold( bcm2836_Peripheral *p_pHandle, int32_t p_Temp);
void DS1631_ReadConfigRegister( bcm2836_Peripheral *p_pHandle, uint32_t *p_Res);
void DS1631_WriteConfigRegister( bcm2836_Peripheral *p_pHandle, uint32_t p_RegVal);
void DS1631_PowerOnReset( bcm2836_Peripheral *p_pHandle);

#endif


#endif