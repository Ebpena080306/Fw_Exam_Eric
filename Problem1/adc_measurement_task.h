#ifndef __ADC_MEASUREMENT_TASK_H__
#define __ADC_MEASUREMENT_TASK_H__

/*****************************************************************************/
/****************************** Include Files ********************************/
/*****************************************************************************/



/*****************************************************************************/
/*************************** Macro Definitions *******************************/
/*****************************************************************************/
// Define VARIANTS in this area
#define ADC_VLOW_COST       1                   // MCU 12-bit ADC with 3.3v as vref
//#define ADC_VHIGH_COST      1                   // SPI 32-bit ADC with 5v as vref 

// Define MACRO in this area
#define ADC_VREF_VLOW_COST  3.3
#define ADC_VREF_VHIGH_COST 5
#define ADC_RESOLUTION_12B  12
#define ADC_RESOLUTION_32B  32

/*****************************************************************************/
/*************************** EXT Function APIS *******************************/
/*****************************************************************************/

void AdcMeasurementTask_Init(void);
void AdcMeasurementTask(void);

#endif // END OF FILE __ADC_MEASUREMENT_TASK_H__ //
