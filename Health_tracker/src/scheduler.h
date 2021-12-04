/***********************************************************************************
* @file Schedular.h
 * @brief
 * @Description :
 * @author Sayali Mule
 * @date September 15, 2021
 * @Reference:
 *
 *****************************************************************************/

#ifndef SRC_SCHEDULER_H_
#define SRC_SCHEDULER_H_



//***********************************************************************************
//                              Include files
//***********************************************************************************
#include "ble.h"





//***********************************************************************************
//                              Macros and Enums

//***********************************************************************************



//Different states
typedef enum{
  STATE_MASTER,  //default state
  STATE_PULSE_SENSOR_READ,
  STATE_ACCELEROMETER_READ_START, //Start reading ACCELEROMETER
  STATE_INTERRUPT_SOURCE_REG_CLEARED,
}state_e;




//***********************************************************************************
//                              Global variables

//***********************************************************************************
extern event_e event; //Declare global variable to extend its visibility
//***********************************************************************************
//                              Function prototype

//***********************************************************************************
void set_scheduler_i2c_event();
void set_scheduler_two_ms_event();
void set_scheduler_button_press_event();
void accelerometer_statemachine(sl_bt_msg_t *evt);
void set_scheduler_free_fall_event();
void health_tracker_statemachine(sl_bt_msg_t *evt);
#endif /* SRC_SCHEDULER_H_ */
