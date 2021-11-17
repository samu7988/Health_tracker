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
#include "app.h"

//***********************************************************************************
//                              Global variable
//***********************************************************************************
int rollover_count = 0;
bool button_pressed = 0;

volatile uint32_t Signal = 0;
volatile uint32_t sampleCounter = 0;          // used to determine pulse timing
volatile uint32_t lastBeatTime = 0;           // used to find IBI
volatile uint32_t thresh = 1950;                // used to find instant moment of heart beat, seeded
volatile uint32_t P = 1950;                      // used to find peak in pulse wave, seeded
volatile uint32_t T = 1950;                     // used to find trough in pulse wave, seeded


volatile bool Pulse = false;     // "True" when User's live heartbeat is detected. "False" when not a "live beat".
volatile bool secondBeat = false;      // used to seed rate array so we startup with reasonable BPM
volatile uint32_t rate[10];                    // array to hold last ten IBI values
volatile bool firstBeat = true;        // used to seed rate array so we startup with reasonable BPM
volatile uint32_t amp = 0;                   // used to hold amplitude of pulse waveform, seeded



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
//      set_scheduler_three_second_event();
      gpioLed0Toggle();

      //Heart beat algorithm
      sampleCounter += 2;                         // keep track of the time in mS with this variable
      int N = sampleCounter - lastBeatTime;       // monitor the time since the last beat to avoid noise
      //  find the peak and trough of the pulse wave
      if(Signal < thresh && N > (IBI/5)*3){       // avoid dichrotic noise by waiting 3/5 of last IBI
        if (Signal < T){                        // T is the trough
          T = Signal;                         // keep track of lowest point in pulse wave
        }
      }

      if(Signal > thresh && Signal > P){          // thresh condition helps avoid noise
        P = Signal;                             // P is the peak
      }

      //  NOW IT'S TIME TO LOOK FOR THE HEART BEAT
      // signal surges up in value every time there is a pulse
      if (N > 250){                                   // avoid high frequency noise
        if ( (Signal > thresh) && (Pulse == false) && (N > (IBI/5)*3) ){
          Pulse = true;                               // set the Pulse flag when we think there is a pulse
//          digitalWrite(blinkPin,HIGH);                // turn on pin 13 LED
          IBI = sampleCounter - lastBeatTime;         // measure time between beats in mS
          lastBeatTime = sampleCounter;               // keep track of time for next pulse

          if(secondBeat){                        // if this is the second beat, if secondBeat == TRUE
            secondBeat = false;                  // clear secondBeat flag
            for(int i=0; i<=9; i++){             // seed the running total to get a realisitic BPM at startup
              rate[i] = IBI;
            }
          }

          if(firstBeat){                         // if it's the first time we found a beat, if firstBeat == TRUE
            firstBeat = false;                   // clear firstBeat flag
            secondBeat = true;                   // set the second beat flag
            CORE_EXIT_CRITICAL();             // enable interrupts again
            return;                              // IBI value is unreliable so discard it
          }

          uint32_t runningTotal = 0;                  // clear the runningTotal variable

          for(int i=0; i<=8; i++){                // shift data in the rate array
            rate[i] = rate[i+1];                  // and drop the oldest IBI value
            runningTotal += rate[i];              // add up the 9 oldest IBI values
          }

          rate[9] = IBI;                          // add the latest IBI to the rate array
          runningTotal += rate[9];                // add the latest IBI to runningTotal
          runningTotal /= 10;                     // average the last 10 IBI values
          BPM = 60000/runningTotal;               // how many beats can fit into a minute? that's BPM!
          QS = true;                              // set Quantified Self flag
          // QS FLAG IS NOT CLEARED INSIDE THIS ISR
        }
      }

      if (Signal < thresh && Pulse == true){   // when the values are going down, the beat is over
//        digitalWrite(blinkPin,LOW);            // turn off pin 13 LED
        Pulse = false;                         // reset the Pulse flag so we can do it again
        amp = P - T;                           // get amplitude of the pulse wave
        thresh = amp/2 + T;                    // set thresh at 50% of the amplitude
        P = thresh;                            // reset these for next time
        T = thresh;
      }

      if (N > 2500){                           // if 2.5 seconds go by without a beat
        thresh = 1910;                          // set thresh default
        P = 1900;                               // set P default
        T = 1900;                               // set T default
        lastBeatTime = sampleCounter;          // bring the lastBeatTime up to date
        firstBeat = true;                      // set these to avoid noise
        secondBeat = false;                    // when we get the heartbeat back
      }

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

   I2C_TransferReturn_TypeDef status = i2cTransferInProgress;

   status = I2C_Transfer(I2C0);

   if(status == i2cTransferDone){
       NVIC_DisableIRQ(I2C0_IRQn);
       set_scheduler_i2c_event();
   }
   if(status < i2cTransferDone){
       NVIC_DisableIRQ(I2C0_IRQn);
   }

}


void GPIO_EVEN_IRQHandler(void)
{
    CORE_DECLARE_IRQ_STATE;
    CORE_ENTER_CRITICAL();
  // Get and clear all pending GPIO interrupts
  uint32_t interruptMask = GPIO_IntGet();

  // Check if PB0 was pressed
  if (GPIO->IF & (1 << PB0_pin))
  {
      button_pressed = !button_pressed;
      if(button_pressed){

          set_scheduler_button_press_event();
      }
      else{

          set_scheduler_button_release_event();
      }
  }
  GPIO_IntClear(interruptMask);
  CORE_EXIT_CRITICAL();

}

//void GPIO_ODD_IRQHandler(void)
//{
//  // Get and clear all pending GPIO interrupts
//  uint32_t interruptMask = GPIO_IntGet();
//
//  // Check if PB1 was pressed
//  if (GPIO->IF & (1 << PB1_pin))
//  {
//      GPIO_IntClear(interruptMask);
//
//  //    gpioLed0Toggle();
//  }
//}
