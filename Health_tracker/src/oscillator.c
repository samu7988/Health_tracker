/***********************************************************************************
* @file oscillator.c
 * @brief Contains the function to set to control CMU peripheral
 * @Description : Controls various aspects of CMU like setting on period for LETIMER, prescalar, etc
 * @author Sayali Mule
 * @date September 8, 2021
 * @Reference:
 *
 *****************************************************************************/
//***********************************************************************************
//                              Include files
//***********************************************************************************
#include <src/oscillator.h>

/*------------------------------------------------------------------------------------------------------------------------------------*/
/*
@brief:Initialises the clock peripheral
 @parameter: energy_mode
 @return:none
 */
/*-----------------------------------------------------------------------------------------------------------------------------*/
void Clock_init()
{

  /**************************************************************************
   *                    Oscillator selection based on energy mode
   *******************************************************************************/
    // Select LFXO for the LETIMER(enabling the peripheral clock)
    if(LOWEST_ENERGY_MODE <= EM2){
      CMU_OscillatorEnable(cmuOsc_LFXO ,true,true);
      CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);
    }
    else if(LOWEST_ENERGY_MODE == EM3){
        CMU_OscillatorEnable(cmuOsc_ULFRCO ,true,true);
        CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_ULFRCO);

    }

  /**************************************************************************
   *                   LETIMER clock initialisation
   *******************************************************************************/
  CMU_ClockDivSet (cmuClock_LETIMER0, PRESCALAR); //to set the prescaler value

  CMU_ClockEnable(cmuClock_LETIMER0, true);

  /**************************************************************************
   *                   I2C clock initialisation
   *******************************************************************************/
  CMU_ClockEnable(cmuClock_I2C0, true); //Enable I2c clock.

  /**************************************************************************
   *                   ADC clock initialisation
   *******************************************************************************/
  CMU_ClockEnable(cmuClock_ADC0, true);

}

