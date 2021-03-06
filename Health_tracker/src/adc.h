/**
 * @file adc.h
 * @brief Header file for ADC peripheral of EFR32.
 * @author Sayali Mule
 * @date November 12, 2021
 *
 */
#ifndef ADC_H_
#define ADC_H_

//***********************************************************************************
// Include files
//***********************************************************************************

#include "em_device.h"
#include "em_chip.h"
#include "em_core.h"
#include "em_emu.h"
#include "em_gpio.h"
#include "em_adc.h"
#include<stdlib.h>
#include<stdint.h>
//***********************************************************************************
// global variables
//***********************************************************************************


//***********************************************************************************
// function prototypes
//***********************************************************************************
/*------------------------------------------------------------------------------------------------------------------------------------*/
/*
  @brief: Initialise ADC peripheral

 Set the Reference voltage, Acquisition time, ADC channel input selection and enabling the ADC interrupt

 @param:None
 @param:None

 @return: None
 */
/*-----------------------------------------------------------------------------------------------------------------------------*/
void ADC_init();
/*------------------------------------------------------------------------------------------------------------------------------------*/
/*
  @brief:ADC interrupt handler

 Perform the required function when ADC conversion gets completed

 @param:None
 @param:None

 @return: None
 */
/*-----------------------------------------------------------------------------------------------------------------------------*/
void ADC0_IRQHandler(void);
#endif /* ADC_H_ */
