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
volatile uint32_t adc_reading = 0;

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
    initScan.reference  = adcRef5V;    // internal 5V reference
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
//    NVIC_EnableIRQ(ADC0_IRQn);

    //Start with first conversion
    ADC_Start(ADC0, adcStartSingle);

}

