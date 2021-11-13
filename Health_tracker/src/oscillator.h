/***********************************************************************************
* @file oscillator.h
 * @brief Contains the function to set to control CMU peripheral
 * @Description : Controls various aspects of CMU like setting on period for LETIMER, prescalar, etc
 * @author Sayali Mule
 * @date September 8, 2021
 * @Reference:
 *
 *****************************************************************************/

#ifndef SRC_OSCILLATOR_H_
#define SRC_OSCILLATOR_H_
//***********************************************************************************
//                              Include files
//***********************************************************************************
#include <stdint.h>
#include "em_cmu.h"
#include "timer.h"
//***********************************************************************************
//                             Function prototype
//***********************************************************************************
void Clock_init();
#endif /* SRC_OSCILLATOR_H_ */
