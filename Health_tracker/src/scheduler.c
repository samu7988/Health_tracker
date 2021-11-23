/*
 * @file scheduler.c
 * @brief Implement scheduler for setting and retreiveing temperature event.
 * @Description : 1) Set temperature event
 *                2) Get temperature event
 *                3)implementation of discovery state machine for client
 * @author Sayali Mule
 * @date september 15, 2021
 * @Reference:
 */


//***********************************************************************************
//                              Include files
//***********************************************************************************
#include "scheduler.h"
#include "em_core.h"
#include "app.h"
#include "lcd.h"
#include "cb.h"
#include "src/irq.h"
#include "src/accelerometer.h"
#include <stdbool.h>
#define INCLUDE_LOG_DEBUG 1
#include "src/log.h"

// Include logging for this file
#define STATE_MACHINE_LOGGING (0)
#define ENABLE_BLE (1)
//***********************************************************************************
//                              Global variable
//***********************************************************************************


  // Health Thermometer service UUID defined by Bluetooth SIG
uint8_t thermo_service[2] = { 0x09, 0x18 };

  //Temperature measurement characteristics UUID
uint8_t temp_measure_char[2] = {0x1c, 0x2a};

/*******************************************************************************
 *   Global variables used for state machine(state and event information)
 ***************************************************************************/
static state_e state = STATE_MASTER;


event_e event = EVENT_DEFAULT; //Intiialise the externed variable to default state event
static client_state_e client_state = CLIENT_IDLE;
static bool is_letimer_enabled = false;

/*******************************************************************************
 *   Global variables used for I2c and accelerometer(ADXL 345)
 ***************************************************************************/
static I2C_TransferReturn_TypeDef status = i2cTransferDone;
static uint8_t reg_addr[1] = {ACCEL_REG_INT_SOURCE}; //Device id
static uint8_t read_val[2] = {0};


/*******************************************************************************
 *   Global variables used for pulse sensor
 ***************************************************************************/
uint32_t sampleCounter = 0;          // used to determine pulse timing
uint32_t lastBeatTime = 0;           // used to find IBI
uint32_t thresh = 1950;                // used to find instant moment of heart beat, seeded
uint32_t P = 1950;                      // used to find peak in pulse wave, seeded
uint32_t T = 1950;                     // used to find trough in pulse wave, seeded
int IBI = 600;
int BPM = 0;


bool Pulse = false;     // "True" when User's live heartbeat is detected. "False" when not a "live beat".
bool secondBeat = false;      // used to seed rate array so we startup with reasonable BPM
uint32_t rate[10];                    // array to hold last ten IBI values
bool firstBeat = true;        // used to seed rate array so we startup with reasonable BPM
uint32_t amp = 0;                   // used to hold amplitude of pulse waveform, seeded

/*------------------------------------------------------------------------------------------------------------------------------------*/
/*
@brief: Sets the global event variable to EVENT_I2C_WRITE_DONE
@param: none
@return: none
*/
/*-----------------------------------------------------------------------------------------------------------------------------*/
void set_scheduler_i2c_event(){

  CORE_DECLARE_IRQ_STATE;

   CORE_ENTER_CRITICAL();

  #if ENABLE_BLE == 1
   sl_bt_external_signal(EVENT_I2C_DONE);
  #else
   event |= EVENT_I2C_DONE;
  #endif


  CORE_EXIT_CRITICAL();

}

/*------------------------------------------------------------------------------------------------------------------------------------*/
/*
@brief: Sets the global event variable to EVENT_TIMER_THREE_SEC_EXPIRE
@param: none
@return: none
*/
/*-----------------------------------------------------------------------------------------------------------------------------*/
void set_scheduler_two_ms_event(){

  CORE_DECLARE_IRQ_STATE;

   CORE_ENTER_CRITICAL();

  #if ENABLE_BLE == 1
   sl_bt_external_signal(EVENT_TIMER_TWO_MSEC_EXPIRE);
  #else
   event |= EVENT_TIMER_TWO_MSEC_EXPIRE;
  #endif

  CORE_EXIT_CRITICAL();

}



/*------------------------------------------------------------------------------------------------------------------------------------*/
/*
@brief: Sets the global event variable to EVENT_BUTTON_PRESSED
@param: none
@return: none
*/
/*-----------------------------------------------------------------------------------------------------------------------------*/
void set_scheduler_button_press_event(){

  CORE_DECLARE_IRQ_STATE;

   CORE_ENTER_CRITICAL();

  #if ENABLE_BLE == 1
   sl_bt_external_signal(EVENT_BUTTON_PRESSED);
  #else
   event |= EVENT_BUTTON_PRESSED;
  #endif



  CORE_EXIT_CRITICAL();

}

/*------------------------------------------------------------------------------------------------------------------------------------*/
/*
@brief: Sets the global event variable to EVENT_PB1_BUTTON_PRESSED
@param: none
@return: none
*/
/*-----------------------------------------------------------------------------------------------------------------------------*/
void  set_scheduler_pb1_button_press_event()
{
   CORE_DECLARE_IRQ_STATE;

   CORE_ENTER_CRITICAL();



  #if ENABLE_BLE == 1
   sl_bt_external_signal(EVENT_PB1_BUTTON_PRESSED);
  #else
   event |= EVENT_PB1_BUTTON_PRESSED;
  #endif
   CORE_EXIT_CRITICAL();
}


void set_scheduler_free_fall_event()
{
  CORE_DECLARE_IRQ_STATE;

   CORE_ENTER_CRITICAL();

  #if ENABLE_BLE == 1
   sl_bt_external_signal(EVENT_FREE_FALL);
  #else
   event |= EVENT_FREE_FALL;
  #endif

   CORE_EXIT_CRITICAL();
}


event_e get_scheduler_event()
{
  event_e temp_event = EVENT_DEFAULT;
  if(event & EVENT_FREE_FALL)
  {
      temp_event = EVENT_FREE_FALL;
      event &= ~ EVENT_FREE_FALL;
  }
  else if(event & EVENT_I2C_DONE)
  {
      temp_event = EVENT_I2C_DONE;
      event &= ~EVENT_I2C_DONE;
  }
  else if(event & EVENT_PB1_BUTTON_PRESSED)
  {
      temp_event = EVENT_PB1_BUTTON_PRESSED;
      event &= ~EVENT_PB1_BUTTON_PRESSED;
  }
  else if(event & EVENT_BUTTON_PRESSED)
  {
      temp_event = EVENT_BUTTON_PRESSED;
      event &= ~EVENT_BUTTON_PRESSED;
  }
  else if(event & EVENT_TIMER_TWO_MSEC_EXPIRE)
  {
      temp_event = EVENT_TIMER_TWO_MSEC_EXPIRE;
      event &= ~EVENT_TIMER_TWO_MSEC_EXPIRE;
  }

  return temp_event;
}



/*------------------------------------------------------------------------------------------------------------------------------------*/
/*
@brief: State machine for health thermometer
@param: None
@return: None
*/
/*-----------------------------------------------------------------------------------------------------------------------------*/


void health_tracker_statemachine(sl_bt_msg_t *evt){

//   event_e current_event = get_scheduler_event();

  #if ENABLE_BLE == 1
  // Only run temperature state machine if header is equal to external event signal id(board event)
  //External signal events are basically COMP1_EVENT, UF_EVENT(3sec), and I2C_EVENTS
  if(SL_BT_MSG_ID(evt->header) != sl_bt_evt_system_external_signal_id){
      #if ENABLE_LOGGING && STATE_MACHINE_LOGGING
      LOG_INFO("Not an external signal event:%d\n\r",1);
      #endif
      return;
  }

  event_e current_event = evt->data.evt_system_external_signal.extsignals; //event

  ble_data_struct_t* ble_common_data = get_ble_data(); //get reference to common ble data structure
  #else
     event_e current_event = get_scheduler_event();
  #endif

  switch(state)
  {
    case STATE_MASTER:
    {
      if(current_event & EVENT_FREE_FALL)
      {
          state = STATE_ACCELEROMETER_WRITE_START;
      }
      //if PB1 is pressed, then enable the timer to fire every 2msec
      else if(current_event & EVENT_PB1_BUTTON_PRESSED)
      {
          state = STATE_ENABLE_TIMER_TWO_MS;
      }
      //If 2msec time has expired, start process the ADC data if available
      else if(current_event & EVENT_TIMER_TWO_MSEC_EXPIRE)
      {
          state = STATE_PULSE_SENSOR_READ;
      }

    }

    break;

    case STATE_ACCELEROMETER_WRITE_START:
    {
      status = I2C_write(&reg_addr[0],1, ACCELEROMETER_SENSOR_ADDRESS,1);
      state = STATE_ACCELEROMETER_READ_START;
    }
    break;

    case STATE_ACCELEROMETER_READ_START:
    {
      //once previous I2c transaction/transfer is done.
      if(current_event & EVENT_I2C_DONE)
      {
        status = I2C_read(&read_val[0],1, ACCELEROMETER_SENSOR_ADDRESS,1); //Read values from INT_SOURCE register of accelerometer.
        state = STATE_INTERRUPT_SOURCE_REG_CLEARED;
      }

    }
    break;
    case STATE_INTERRUPT_SOURCE_REG_CLEARED:
    {
      if(current_event & EVENT_I2C_DONE)
      {
          state = STATE_MASTER;
          LOG_INFO("Free fall detected %u\n\r",1);
          //Send indications to bluetooth on next line(TO DO)


      }
    }
    break;
    case STATE_ENABLE_TIMER_TWO_MS:
    {
      if(is_letimer_enabled == false)
      {
      //Enable the LETIMER to fire every 2msec
          LETIMER_Enable(LETIMER0,true);
          is_letimer_enabled = true;
      }
      else
      {
          LETIMER_Enable(LETIMER0,false);
          is_letimer_enabled = false;
      }
      state = STATE_MASTER;


    }
    break;
    case STATE_PULSE_SENSOR_READ:
    {
      if(current_event & EVENT_FREE_FALL)
      {
          state = STATE_ACCELEROMETER_WRITE_START;
      }
      else
      {
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
                 break;                              // IBI value is unreliable so discard it
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
               BPM = BPM/2; //This is hack to divide the BPM by 2 to get correct readings
               LOG_INFO("BPM %u\n\r",BPM);
               //Send indications to client on next line(TO DO)

               // QS FLAG IS NOT CLEARED INSIDE THIS ISR
             }
           }

           if (Signal < thresh && Pulse == true){   // when the values are going down, the beat is over
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

           state = STATE_MASTER;
      }

    }
    break;
  }
}




