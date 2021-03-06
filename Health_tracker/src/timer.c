/*
 * @file timer.c
 * @brief Sets up LETimer Peripherals to run in free mode
 * @Description : 1)Sets up Interrupt handler
 *                2)Sets up the top value
 *
 * @author Sayali Mule
 * @date september 8, 2021
 * @Reference:  https://github.com/SiliconLabs/peripheral_examples/blob/master/series1/letimer/one_shot_output_compare/src/main_tg11.c
 */


//***********************************************************************************
//                              Include files
//***********************************************************************************
#include "timer.h"
#include "gpio.h"

//***********************************************************************************
//                              Global variable
//***********************************************************************************
uint32_t enable_timer_count = 0;


/*------------------------------------------------------------------------------------------------------------------------------------*/
/*
@brief: Initialises LETIMER0
@param: none
@return: none
*/
/*-----------------------------------------------------------------------------------------------------------------------------*/
void le_timer_init(){


  LETIMER_Init_TypeDef letimerInit = LETIMER_INIT_DEFAULT;


  /* Reload COMP0 on underflow, set output, and run in one-shot mode
  top value get loaded in the COMP0..COMP0 value goes in count reg
  (whenever counter underflows it takes the top value from COMP0 all the time)
  */
  letimerInit.comp0Top  = true;
  letimerInit.ufoa0     = letimerUFOANone;
  letimerInit.repMode   = letimerRepeatFree;
  letimerInit.enable = false;
  letimerInit.topValue = ON_TIME_LOAD_VAL;

  // Initialize and enable LETIMER
  LETIMER_Init(LETIMER0, &letimerInit);


  //Enable Interrupt for LETIMER0
  /* Enable LETIMER0 interrupt vector in NVIC*/
  NVIC_EnableIRQ(LETIMER0_IRQn);

  /* Enable Comp0 match interrupt */
  LETIMER_IntEnable(LETIMER0, LETIMER_IF_UF);

   // Compare on wake-up interval count
  LETIMER_Enable(LETIMER0,false);




}


/*-------------------------------------------------------------------*/
/*
@brief: Returns the time in miliseconds since system is powered on
@param: none
@return: none
*/
/*----------------------------------------------------------------------*/

uint32_t letimerMilliseconds(){
  uint32_t millisecond = (rollover_count * 3000) + ((1000)*(ON_TIME_LOAD_VAL - LETIMER_CounterGet(LETIMER0)))/(CMU_ClockFreqGet(cmuClock_LETIMER0));
  return millisecond;
}

/*----------------------------------------------*/
/*
@brief: Non Blocking wait using LETIMER0
@param: none
@return: none
*/
/*---------------------------------------*/
uint8_t timerWaitUs_nonblocking(uint32_t us_wait){

  //Input validation
  if(us_wait == 0)
    return 255;


  //Input parameter is in microsecond, convert it first into seconds
  double time_in_seconds = 0;
  time_in_seconds = (float)us_wait/(1000000); //converted microsecond to seconds, since our calculation requires value in seconds


  uint32_t clock_freq = CMU_ClockFreqGet(cmuClock_LETIMER0);
  uint32_t curr_count = LETIMER_CounterGet(LETIMER0);
  uint32_t destination_cnt = 0;
  uint32_t num_ticks = time_in_seconds * clock_freq; //Num ticks = Total period/Time for 1 tick

  //If num ticks calculated exceeds the maximum , then return 0
  if((0xFFFFU/(float)clock_freq) < time_in_seconds){
      return 255;
  }
//  uint32_t num_reload = (num_ticks)/(ON_TIME_LOAD_VAL); //If num ticks is greater than top value
//  uint32_t modulo = (num_ticks)%ON_TIME_LOAD_VAL;

  if(curr_count > (num_ticks)){
      destination_cnt = curr_count - (num_ticks);
  }
  else{

      destination_cnt = ON_TIME_LOAD_VAL - (num_ticks - curr_count);
  }

  LETIMER_CompareSet(LETIMER0, 1, destination_cnt); //Set COMP1 to destination cnt
  LETIMER_IntEnable(LETIMER0, LETIMER_IF_COMP1); //Enable Comp1 interrupt

  return 0;
}

