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
#define INCLUDE_LOG_DEBUG 1
#include "src/log.h"

// Include logging for this file
#define STATE_MACHINE_LOGGING (0)

//***********************************************************************************
//                              Global variable
//***********************************************************************************
  state_e state = STATE_ACCELEROMETER_SENSOR_ENABLE;
  I2C_TransferReturn_TypeDef status = 0;
  uint8_t reg_addr[1] = {0x00}; //Device id
  uint8_t read_val[2] = {0};
  event_e current_event = EVENT_DEFAULT;

  event_e event = EVENT_DEFAULT; //Intiialise the externed variable to default state event
  client_state_e client_state = CLIENT_IDLE;

  // Health Thermometer service UUID defined by Bluetooth SIG
uint8_t thermo_service[2] = { 0x09, 0x18 };

  //Temperature measurement characteristics UUID
uint8_t temp_measure_char[2] = {0x1c, 0x2a};

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
   event |= EVENT_I2C_DONE;
//   sl_bt_external_signal(EVENT_I2C_DONE);

  CORE_EXIT_CRITICAL();

}

/*------------------------------------------------------------------------------------------------------------------------------------*/
/*
@brief: Sets the global event variable to EVENT_TIMER_THREE_SEC_EXPIRE
@param: none
@return: none
*/
/*-----------------------------------------------------------------------------------------------------------------------------*/
void set_scheduler_three_second_event(){

  CORE_DECLARE_IRQ_STATE;

   CORE_ENTER_CRITICAL();

   sl_bt_external_signal(EVENT_TIMER_THREE_SEC_EXPIRE);

  CORE_EXIT_CRITICAL();

}

/*------------------------------------------------------------------------------------------------------------------------------------*/
/*
@brief: Sets the global event variable to EVENT_TIMER_POWER_ON
@param: none
@return: none
*/
/*-----------------------------------------------------------------------------------------------------------------------------*/
void set_scheduler_user_requested_timer_expire_event(){

  CORE_DECLARE_IRQ_STATE;

   CORE_ENTER_CRITICAL();

   sl_bt_external_signal(EVENT_TIMER_USER_REQUESTED_EXPIRE);

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

   sl_bt_external_signal(EVENT_BUTTON_PRESSED);

  CORE_EXIT_CRITICAL();

}
/*------------------------------------------------------------------------------------------------------------------------------------*/
/*
@brief: Sets the global event variable to EVENT_BUTTON_RELEASE
@param: none
@return: none
*/
/*-----------------------------------------------------------------------------------------------------------------------------*/
void set_scheduler_button_release_event(){

  CORE_DECLARE_IRQ_STATE;

   CORE_ENTER_CRITICAL();

   sl_bt_external_signal(EVENT_BUTTON_RELEASE); //ext_signal |= EVENT_BUTTON_RELEASE

   CORE_EXIT_CRITICAL();

}

event_e get_scheduler_event()
{
  event_e temp_event = EVENT_DEFAULT;
  if(event & EVENT_I2C_DONE)
  {
      temp_event = EVENT_I2C_DONE;
      event &= ~EVENT_I2C_DONE;
  }

  return temp_event;
}
/*------------------------------------------------------------------------------------------------------------------------------------*/
/*
@brief: State machine for accelerometer sensor
@param: None
@return: None
*/
/*-----------------------------------------------------------------------------------------------------------------------------*/


void accelerometer_statemachine(sl_bt_msg_t *evt){

  current_event = get_scheduler_event();

   switch(state){

     case STATE_ACCELEROMETER_SENSOR_ENABLE:
       {
         status = I2C_write(&reg_addr[0],1, ACCELEROMETER_SENSOR_ADDRESS);
         state = STATE_ACCELEROMETER_WRITE_COMPLETE;

       }
       break;
     case STATE_ACCELEROMETER_WRITE_COMPLETE:
       {

         if(current_event & EVENT_I2C_DONE){
              #if ENABLE_LOGGING && STATE_MACHINE_LOGGING
              LOG_INFO("STATE_ACCELEROMETER_WRITE_COMPLETE:%d\n\r",0);
              #endif
             state = STATE_ACCELEROMETER_READ_START;
         }
       }
       break;
     case STATE_ACCELEROMETER_READ_START:
       {

            #if ENABLE_LOGGING && STATE_MACHINE_LOGGING
            LOG_INFO("STATE_ACCELEROMETER_READ_START:%d\n\r",0);
            #endif
             status = I2C_read(&read_val[0],1, ACCELEROMETER_SENSOR_ADDRESS); //Read values
             state = STATE_ACCELEROMETER_READ_COMPLETE;

       }
       break;
     case STATE_ACCELEROMETER_READ_COMPLETE:
       {
           if(current_event & EVENT_I2C_DONE){ //when I2c read is complete , power down the sensor
              #if ENABLE_LOGGING && STATE_MACHINE_LOGGING
              LOG_INFO("STATE_ACCELEROMETER_READ_COMPLETE:%d\n\r",0);
              #endif

              LOG_INFO("Read value : %x\n\r",read_val[0]);
             state = STATE_ACCELEROMETER_SENSOR_ENABLE; //return back to first state
         }
     }
       break;

     default:
       break;

   }


}


/*------------------------------------------------------------------------------------------------------------------------------------*/
/*
@brief: State machine for Discovery(client)
@param: None
@return: None
*/
/*-----------------------------------------------------------------------------------------------------------------------------*/


void discovery_statemachine(sl_bt_msg_t *evt){

  uint32_t client_event = SL_BT_MSG_ID(evt->header); //Read client specific events
  sl_status_t status = 0;
  ble_data_struct_t* ble_common_data = get_ble_data();


  switch (client_state){
    case CLIENT_IDLE:
      if(client_event == sl_bt_evt_system_boot_id){
          client_state = SCANNING;
      }
      break;
    case SCANNING:
      if(client_event == sl_bt_evt_scanner_scan_report_id){
          client_state = OPENING;
      }
      else if(client_event == sl_bt_evt_connection_closed_id){
          client_state = SCANNING;
      }
      break;
    case OPENING:
      //Connection is established, start with service discovery
      if(client_event == sl_bt_evt_connection_opened_id){
          // Discover Health Thermometer service on the responder device
          status |= sl_bt_gatt_discover_primary_services_by_uuid(evt->data.evt_connection_opened.connection,
                                                            sizeof(thermo_service),
                                                            (const uint8_t*)thermo_service);


        #if ENABLE_LOGGING
        if(status == SL_STATUS_OK){
            LOG_INFO("CLIENT:sl_bt_evt_connection_opened_id success %d\n\r",status);
        }
        else{
            LOG_INFO("CLIENT:sl_bt_evt_connection_opened_id fail %d\n\r",status);
        }
        #endif //ENABLE LOGGING
          client_state = DISCOVER_SERVICE;
      }
      else if(client_event == sl_bt_evt_connection_closed_id){
          client_state = SCANNING;
      }
      break;

    case DISCOVER_SERVICE:
      if(client_event == sl_bt_evt_gatt_procedure_completed_id){
          //start with characteristics discovery
          status |= sl_bt_gatt_discover_characteristics_by_uuid(evt->data.evt_gatt_procedure_completed.connection,
                                                                   ble_common_data->thermometer_service_handle,
                                                                   sizeof(temp_measure_char),
                                                                   (const uint8_t*)temp_measure_char);

          #if ENABLE_LOGGING
          if(status == SL_STATUS_OK){
              LOG_INFO("sl_bt_evt_gatt_procedure_completed_id success, discover services %d\n\r",status);
          }
          else{
              LOG_INFO("sl_bt_evt_gatt_procedure_completed_id fail, discover services %d\n\r",status);
          }
          #endif
          client_state = DISCOVER_CHAR;
      }
      else if(client_event == sl_bt_evt_connection_closed_id){
          client_state = SCANNING;
      }
      break;

    case DISCOVER_CHAR:
      if(client_event == sl_bt_evt_gatt_procedure_completed_id){
          sl_bt_scanner_stop(); //Might be optional

          //Enable indication to server to start sending temperature value
          status |= sl_bt_gatt_set_characteristic_notification(evt->data.evt_gatt_procedure_completed.connection,
                                                               ble_common_data->thermometer_characteristic_handle,
                                                               sl_bt_gatt_indication);
          #if ENABLE_LOGGING
          if(status == SL_STATUS_OK){
              LOG_INFO("sl_bt_evt_gatt_procedure_completed_id success, discover characteristics %d\n\r",status);
          }
          else{
              LOG_INFO("sl_bt_evt_gatt_procedure_completed_id fail, discover characteristics %d\n\r",status);
          }
          #endif
          client_state = INDICATION_ENABLED;
      }
      else if(client_event == sl_bt_evt_connection_closed_id){
          client_state = SCANNING;
      }
      break;

    case INDICATION_ENABLED:
      if(client_event == sl_bt_evt_gatt_characteristic_value_id){
          if(evt->data.evt_gatt_characteristic.characteristic == gattdb_temperature_measurement)
          {
              status |= sl_bt_gatt_send_characteristic_confirmation(evt->data.evt_gatt_characteristic_value.connection);
             uint8_t* temp_val = evt->data.evt_gatt_characteristic_value.value.data;
             LOG_INFO("Receive data %x\n\r",temp_val[4]);

             float temperature = gattFloat32ToInt(temp_val);
             displayPrintf(DISPLAY_ROW_TEMPVALUE,"Temp val=%f",temperature);


            #if ENABLE_LOGGING
               LOG_INFO("sl_bt_evt_gatt_characteristic_value_id success %f\n\r",temperature);
            #endif
          }
          else{
            #if ENABLE_LOGGING
              LOG_INFO("sl_bt_evt_gatt_characteristic_value_id fail %d\n\r",0);
            #endif
          }
      }
      else if(client_event == sl_bt_evt_connection_closed_id){
          client_state = SCANNING;
      }
      break;


  }

}

