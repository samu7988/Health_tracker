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
  STATE_IDLE,  //default state
  STATE_ACCEL_INIT,
  STATE_ACCEL_ENABLE_POWER_MEAS,
  STATE_ACCELEROMETER_SENSOR_ENABLE, //ACCELEROMETER sensor is stable, can read it now
  STATE_ACCELEROMETER_WRITE_COMPLETE,
  STATE_ACCELEROMETER_READ_START, //Start reading ACCELEROMETER
  STATE_ACCELEROMETER_READ_COMPLETE, //ACCELEROMETER reading is complete
  STATE_ERROR //Error state
}state_e;



typedef enum{
  CLIENT_IDLE,     //DEFAULT
  SCANNING, //BOOT ID
  OPENING,  //SCAN REPORT
  DISCOVER_SERVICE, // OPEN CONNECTION
  DISCOVER_CHAR, // DISCOVER_SERVICE/PROCESS_COMPLETE
  INDICATION_ENABLED,
}client_state_e;

//***********************************************************************************
//                              Global variables

//***********************************************************************************
extern event_e event; //Declare global variable to extend its visibility
//***********************************************************************************
//                              Function prototype

//***********************************************************************************
void set_scheduler_temp_event();
event_e get_scheduler_event();
void set_scheduler_i2c_event();
void set_scheduler_three_second_event();
void set_scheduler_user_requested_timer_expire_event();
void set_scheduler_button_press_event();
void set_scheduler_button_release_event();
void accelerometer_statemachine(sl_bt_msg_t *evt);
void discovery_statemachine(sl_bt_msg_t *evt);

#endif /* SRC_SCHEDULER_H_ */
