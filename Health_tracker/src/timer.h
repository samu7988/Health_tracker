/***********************************************************************************
* @file timer.h
 * @brief Contains the function to set to control CMU peripheral
 * @Description : Controls various aspects of CMU like setting on period for LETIMER, prescalar, etc
 * @author Sayali Mule
 * @date September 8, 2021
 * @Reference:
 *
 *****************************************************************************/



#ifndef SRC_TIMER_H_
#define SRC_TIMER_H_

//***********************************************************************************
//                              Include files
//***********************************************************************************
#include "em_letimer.h"
#include "em_cmu.h"
#include "../app.h"
#include "irq.h"
//***********************************************************************************
//                              Macros

//***********************************************************************************
//Letimer period and ON time

#define PRESCALAR          (8)
#define CLK_FREQ (CMU_ClockFreqGet(cmuClock_LETIMER0)) //to get peripherals clock frequency exp .LETIMER0
#define ON_TIME_LOAD_VAL ((((LETIMER_ON_TIME_MS) * (CLK_FREQ))/(1000))) //Comp0 load value

//#define TOTAL_PERIOD_LOAD_VAL (((LETIMER_PERIOD_MS) * (CLK_FREQ))/(1000)) //Comp0 load value

//***********************************************************************************
//                              Global variables

//***********************************************************************************


//***********************************************************************************
//                              Function prototype

//***********************************************************************************
 void le_timer_init();
 uint8_t timerWaitUs_blocking(uint32_t);
 uint8_t timerWaitUs_nonblocking(uint32_t);
 uint32_t letimerMilliseconds();

#endif /* SRC_TIMER_H_ */
