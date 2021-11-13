/**
 * @file adc.c
 * @brief This file is to be used for the setting the ADC of the microcontroller.
 * @author Sayali mule
 * @date November 12, 2021
 * @Reference/Credits:
 *
 *
 */

//***********************************************************************************
// Include files
//***********************************************************************************

#include "adc.h"
// Include logging for this file
#define INCLUDE_LOG_DEBUG 1
#include "src/log.h"
//***********************************************************************************
// Macros
//***********************************************************************************
#define adcFreq   16000000
#define NUM_INPUTS (1)

//***********************************************************************************
// Global variables
//***********************************************************************************

volatile uint32_t sample;
volatile uint32_t millivolts;

//***********************************************************************************
// Function implementation
//***********************************************************************************
/*------------------------------------------------------------------------------------------------------------------------------------*/
/*
  @brief: Initialise ADC peripheral

 Set the Reference voltage, Acquisition time, ADC channel input selection and enabling the ADC interrupt

 @param:None
 @param:None
PF7 adcPosSelAPORT2XCH23, PD13 APORT4XCH5
 @return: None
 */
/*-----------------------------------------------------------------------------------------------------------------------------*/
void ADC_init(){

    // Declare init structs
    ADC_Init_TypeDef init = ADC_INIT_DEFAULT;
    ADC_InitSingle_TypeDef  initScan = ADC_INITSCAN_DEFAULT;

    // Modify init structs
    init.prescale   = ADC_PrescaleCalc(adcFreq, 0);
    init.timebase = ADC_TimebaseCalc(0);


    initScan.diff       = 0;            // single ended
    initScan.reference  = adcRef2V5;    // internal 5V reference
    initScan.resolution = adcRes12Bit;  // 12-bit resolution
    initScan.acqTime    = adcAcqTime4;  // set acquisition time to meet minimum requirement
    initScan.fifoOverwrite = true;      // FIFO overflow overwrites old data

    // Select ADC input.
    // Add VDD to scan for demonstration purposes
    initScan.posSel = adcPosSelAPORT2XCH9;


    // Initialize ADC and Scan
    ADC_Init(ADC0, &init);
    ADC_InitSingle(ADC0, &initScan);

    // Enable Scan interrupts
    ADC_IntEnable(ADC0, ADC_IEN_SINGLE);

    // Enable ADC interrupts
    NVIC_ClearPendingIRQ(ADC0_IRQn);
    NVIC_EnableIRQ(ADC0_IRQn);

    //Start with first conversion
    ADC_Start(ADC0, adcStartSingle);

}
/*------------------------------------------------------------------------------------------------------------------------------------*/
/*
  @brief:ADC interrupt handler

 Perform the required function when ADC conversion gets completed

 @param:None
 @param:None

 @return: None
 */
/*-----------------------------------------------------------------------------------------------------------------------------*/
void ADC0_IRQHandler(void)
{

    uint32_t adc_reading;


      // Read data from ADC
     adc_reading = ADC_DataSingleGet(ADC0);

      // Convert data to mV and store into array
     //analog_val = (adc_reading) * (SYSTEM_VOLTAGE)/(ADC_RESOLUTION)

      millivolts = adc_reading * 2500 / 4096;
      LOG_INFO("ADC voltage: %u\n\r",millivolts);

    // Start next ADC conversion
    //ADC_Start(ADC0, adcStartSingle);

}
