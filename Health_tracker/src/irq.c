/*
 * @file irq.c
 * @brief Sets up LETIMER IRQ Handler
 * @Description : 1)Sets up Interrupt handler
 *
 * @author Sayali Mule
 * @date september 8, 2021
 * @Reference:  https://github.com/SiliconLabs/peripheral_examples/blob/master/series1/letimer/one_shot_output_compare/src/main_tg11.c
 *              https://github.com/SiliconLabs/peripheral_examples/blob/master/series2/gpio/switch_led_interrupt/src/main_switch_led_int_s2_xg21.c
 */


//***********************************************************************************
//                              Include files
//***********************************************************************************
#include "irq.h"
#include <stdbool.h>
#define INCLUDE_LOG_DEBUG 1
#include "src/log.h"
#include "src/adc.h"
#include "src/accelerometer.h"
#include "app.h"

//***********************************************************************************
//                              Global variable
//***********************************************************************************
int rollover_count = 0;
bool button_pressed = 0;

volatile uint32_t Signal = 0; //Define




/*------------------------------------------------------------------------------------------------------------------------------------*/
/*
@brief: LETIMER0 IRQ Handler
@param: none
@return: none
*/
/*-----------------------------------------------------------------------------------------------------------------------------*/
void LETIMER0_IRQHandler(void){
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();

  //if COMP0 match occurs, then first clear the interrupt and then turn OFF the LED
  if(LETIMER0->IF & LETIMER_IF_UF)  //checking if the underflow bit is set or not
  {
      LETIMER_IntClear(LETIMER0, LETIMER_IF_UF);
      rollover_count++;
      set_scheduler_two_ms_event();


//      ADC_Start(ADC0, adcStartSingle);

  }

//  //if COMP1 match occurs,
//  if(LETIMER0->IF & LETIMER_IF_COMP1){ //This interrupt is set when user requested time is completed from timerUs_nonblocking
//      LETIMER_IntClear(LETIMER0, LETIMER_IF_COMP1); //clear the interrupt
//      LETIMER_IntDisable(LETIMER0, LETIMER_IF_COMP1); //Disable Comp1 interrupt
//      set_scheduler_user_requested_timer_expire_event(); //set event to indicate timer expire
//  }
  CORE_EXIT_CRITICAL();
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
    CORE_DECLARE_IRQ_STATE;
    CORE_ENTER_CRITICAL();

    // Read data from ADC(0 - 4096)
    Signal = ADC_DataSingleGet(ADC0);

//      LOG_INFO("ADC voltage: %u\n\r",Signal);
    ADC_Start(ADC0, adcStartSingle); //start next adc conversion

    CORE_EXIT_CRITICAL();



}

/*------------------------------------------------------------------------------------------------------------------------------------*/
/*
@brief: I2C interrupt handler
@param: none
@return: none
*/
/*-----------------------------------------------------------------------------------------------------------------------------*/
void I2C0_IRQHandler(void){

  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();
   I2C_TransferReturn_TypeDef status = i2cTransferInProgress;

   status = I2C_Transfer(I2C0);

   if(status == i2cTransferDone){
//       NVIC_DisableIRQ(I2C0_IRQn);
       set_scheduler_i2c_event();
   }
   if(status < i2cTransferDone){
//       NVIC_DisableIRQ(I2C0_IRQn);
   }
   CORE_EXIT_CRITICAL();

}


void GPIO_EVEN_IRQHandler(void)
{
    CORE_DECLARE_IRQ_STATE;
    CORE_ENTER_CRITICAL();
  // Get and clear all pending GPIO interrupts
  uint32_t interruptMask = GPIO_IntGet();
  // if Accelerometer sensor generates an interrupt during free fall

  if (interruptMask & (1 << ACCELEROMETER_pin))
   {
      LOG_INFO("Scheduling free fall event\n\r");
       set_scheduler_free_fall_event();
       GPIO_IntClear(1 << ACCELEROMETER_pin);

   }

  // Check if PB0 was pressed(used to confirm bonding)
  if (interruptMask & (1 << PB0_pin))
  {
      set_scheduler_button_press_event();
      GPIO_IntClear(1 << PB0_pin);


  }

//  uint8_t data = read_accelerometer_register(ACCEL_REG_INT_SOURCE);
  CORE_EXIT_CRITICAL();

}

void GPIO_ODD_IRQHandler(void)
{
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();
  // Get and clear all pending GPIO interrupts
  uint32_t interruptMask = GPIO_IntGet();


  // Check if PB1 was pressed
  if (GPIO->IF & (1 << PB1_pin))
  {
      button_pressed = !button_pressed; //Indicate whether button is pressed or not
      if(button_pressed == 0)
      {
          LOG_INFO("PB1 state changed\n\r");
          set_scheduler_pb1_button_press_event();

      }
      GPIO_IntClear(interruptMask);

  }
  CORE_EXIT_CRITICAL();

}
