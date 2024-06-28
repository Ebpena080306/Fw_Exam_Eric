
#include <stdio.h>
#include <unistd.h>

#include "adc_measurement_task.h"
#include "mcu_adc.h"
#include "spi_adc.h"
#include "voltage_sender.h"

static int counterPer100ms = 0;

void AdcMeasurementTask_Init(void)
{
    // For Internal ADC
    // Initialize Internal MCU Clock tree for ADC
    // Need to check datasheet for more important initializations

    // For External ADC
    // Initialize SPI Clock
    // Initialize pin Maps for external ADC Reading.
    // Initialize External ADC's Register
    // Need to check datasheet for more important initializations like sampling rate and more
}

uint32_t AdcMeasurementTask_ReadRawData(uint8_t channel)
{
    uint32_t AdcData_Channel_Read[4] = {0};
    uint32_t AdcProcessedRawtomV = 0;
    uint32_t Adc_Vref = 0;
    uint32_t Adc_Bits = 0;

    switch((int)channel)
    {
        case (int)ADC_CHAN_0: // ADC CHANNEL 0
        {
            //Read raw ADC Data from channel 0
            #ifdef ADC_VLOW_COST    
                AdcData_Channel_Read[ADC_CHAN_0] = (uint32_t)MCU_ADC_GetAdcData(ADC_CHAN_0); // return raw from a 12 bit mcu adc
            #else
                AdcData_Channel_Read[ADC_CHAN_0] = SPI_ADC_GetAdcData(ADC_CHAN_0); // return raw from a 32 bit external adc
            #endif
        }
        break;
        case (int)ADC_CHAN_1: // ADC CHANNEL 1
        {
            //Read raw ADC Data from channel 1
            #ifdef ADC_VLOW_COST    
                AdcData_Channel_Read[ADC_CHAN_1] = (uint16_t)MCU_ADC_GetAdcData(ADC_CHAN_1); // return raw from a 12 bit mcu adc
            #else
                AdcData_Channel_Read[ADC_CHAN_1] = SPI_ADC_GetAdcData(ADC_CHAN_1); // return raw from a 32 bit external adc
            #endif
        }
        break;
        case (int)ADC_CHAN_2: // ADC CHANNEL 2
        {
            //Read raw ADC Data from channel 2
            #ifdef ADC_VLOW_COST    
                AdcData_Channel_Read[ADC_CHAN_2] = (uint16_t)MCU_ADC_GetAdcData(ADC_CHAN_2); // return raw from a 12 bit mcu adc
            #else
                AdcData_Channel_Read[ADC_CHAN_2] = SPI_ADC_GetAdcData(ADC_CHAN_2); // return raw from a 32 bit external adc
            #endif
        }
        break;
        case (int)ADC_CHAN_3: // ADC CHANNEL 3
        {
            //Read raw ADC Data from channel 3
            #ifdef ADC_VLOW_COST    
                AdcData_Channel_Read[ADC_CHAN_3] = (uint16_t)MCU_ADC_GetAdcData(ADC_CHAN_3); // return raw from a 12 bit mcu adc
            #else
                AdcData_Channel_Read[ADC_CHAN_3] = SPI_ADC_GetAdcData(ADC_CHAN_3); // return raw from a 32 bit external adc
            #endif
        }
        break;
        default: // default
        {
            // Error Catching area
        }
        break;
    }

    // Compute to mV
    #ifdef ADC_VLOW_COST
        // get vref then if 3.3 we need to multiply by 1000 to remove decimal
        Adc_Vref = (ADC_VREF_VLOW_COST*1000);
        Adc_Bits = ADC_RESOLUTION_12B;
        AdcProcessedRawtomV = (((Adc_Vref / 2^Adc_Bits) / 1000) * AdcData_Channel_Read[channel]) / 1000; // voltage needs to be in mV, Apply basic Voltage formula conversion
    #else
        Adc_Vref = ADC_VREF_VHIGH_COST;
        Adc_Bits = ADC_RESOLUTION_32B;
        AdcProcessedRawtomV = ((Adc_Vref / 2^Adc_Bits) * AdcData_Channel_Read[channel]) / 1000; // voltage needs to be in mV, Apply basic Voltage formula conversion
    #endif

    return AdcProcessedRawtomV;
}

void AdcMeasurementTask(void)
{   
    uint32_t Get_AdcData_Channel_rawData = 0;
    
    counterPer100ms++; // increment up every entrance to the function

    switch(counterPer100ms)
    {
        case (1): // function was initially called to send VA, meaning outside timer was flagged by 100ms.
        {
            Get_AdcData_Channel_rawData = AdcMeasurementTask_ReadRawData(VOLTAGE_A);
            SendVoltage(VOLTAGE_A, Get_AdcData_Channel_rawData);
        }
        break;
        case (5): // function was called to send VB, meaning outside timer was flagged by 500ms.
        {
            Get_AdcData_Channel_rawData[VOLTAGE_B] = AdcMeasurementTask_ReadRawData(VOLTAGE_B);
            SendVoltage(VOLTAGE_B, Get_AdcData_Channel_rawData);
        }
        break;
        case (22): // function was called to send VC, meaning outside timer was flagged by 2200ms.
        {
            Get_AdcData_Channel_rawData[VOLTAGE_C] = AdcMeasurementTask_ReadRawData(VOLTAGE_C);
            SendVoltage(VOLTAGE_C, Get_AdcData_Channel_rawData);
        }
        break;
        case (37): // function was called to send VD, meaning outside timer was flagged by 3700ms.
        {
            Get_AdcData_Channel_rawData[VOLTAGE_D] = AdcMeasurementTask_ReadRawData(VOLTAGE_D);
            SendVoltage(VOLTAGE_C, Get_AdcData_Channel_rawData);
        }
        break;
        default:
        {
            if(counterPer100ms > 37)
            {
                counterPer100ms = 0;
            }
        }
        break;
    }
}

