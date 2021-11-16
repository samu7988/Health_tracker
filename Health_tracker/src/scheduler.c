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
  state_e state = STATE_IDLE;
  I2C_TransferReturn_TypeDef status = 0;
  uint8_t reg_addr[1] = {0xF3}; //Temperature measurement register address
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
   //event |= EVENT_I2C_DONE;
   sl_bt_external_signal(EVENT_I2C_DONE);

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
/*------------------------------------------------------------------------------------------------------------------------------------*/
/*
@brief: State machine for temperature sensor
@param: None
@return: None
*/
/*-----------------------------------------------------------------------------------------------------------------------------*/


void temperature_statemachine(sl_bt_msg_t *evt){

  // Only run temperature state machine if header is equal to external event signal id(board event)
  //External signal events are basically COMP1_EVENT, UF_EVENT(3sec), and I2C_EVENTS
  if(SL_BT_MSG_ID(evt->header) != sl_bt_evt_system_external_signal_id){
      #if ENABLE_LOGGING && STATE_MACHINE_LOGGING
      LOG_INFO("Not an external signal event:%d\n\r",1);
      #endif
      return;
  }

  current_event = evt->data.evt_system_external_signal.extsignals; //event



  ble_data_struct_t* ble_common_data = get_ble_data(); //get reference to common ble data structure

   switch(state){

     case STATE_IDLE:
       {
         displayPrintf(DISPLAY_ROW_TEMPVALUE, "");
         if(current_event & EVENT_TIMER_THREE_SEC_EXPIRE){
            #if ENABLE_LOGGING && STATE_MACHINE_LOGGING
            LOG_INFO("STATE_IDLE:%d\n\r",0);
            #endif
             turn_on_power(gpioPortD, 15); //Turn on power for temperature sensor for load power management
             timerWaitUs_nonblocking(EIGHTY_MILLISECOND); //wait for 80 msec to stabilise power
             state = STATE_TEMP_SENSOR_ENABLE; //next state
         }
       }
       break;
     case STATE_TEMP_SENSOR_ENABLE:
       {
         //If connection is closed, no point in proceeding in state machine(temperature sensing)
         if(connection_open == 0 || temperature_char == 0){
             state = STATE_IDLE;
         }
         else if(current_event & EVENT_TIMER_USER_REQUESTED_EXPIRE){
              #if ENABLE_LOGGING && STATE_MACHINE_LOGGING
              LOG_INFO("STATE_TEMP_SENSOR_ENABLE:%d\n\r",0);
              #endif
             sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM1); //Put MCU in EM1
             status = I2C_write(&reg_addr[0],1, TEMP_SENSOR_ADDRESS);
             state = STATE_TEMP_WRITE_COMPLETE;
         }

       }
       break;
     case STATE_TEMP_WRITE_COMPLETE:
       {
         //If connection is closed, no point in proceeding in state machine(temperature sensing)
         if(connection_open == 0 || temperature_char == 0){
             state = STATE_IDLE;
         }
         else if(current_event & EVENT_I2C_DONE){
              #if ENABLE_LOGGING && STATE_MACHINE_LOGGING
              LOG_INFO("STATE_TEMP_WRITE_COMPLETE:%d\n\r",0);
              #endif
             sl_power_manager_remove_em_requirement(SL_POWER_MANAGER_EM1); //Remove EM1
             timerWaitUs_nonblocking(TEN_POINT_EIGHTMS); //Start wait for 10.8 msec
             state = STATE_TEMP_READ_START;
         }
       }
       break;
     case STATE_TEMP_READ_START:
       {
         //If connection is closed, no point in proceeding in state machine(temperature sensing)
         if(connection_open == 0 || temperature_char == 0){
             state = STATE_IDLE;
         }
         else if(current_event & EVENT_TIMER_USER_REQUESTED_EXPIRE){
            #if ENABLE_LOGGING && STATE_MACHINE_LOGGING
            LOG_INFO("STATE_TEMP_READ_START:%d\n\r",0);
            #endif
             sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM1); //Put MCU in EM1
             status = I2C_read(&read_val[0],2, TEMP_SENSOR_ADDRESS); //Read values
             state = STATE_TEMP_READ_COMPLETE;
         }
       }
       break;
     case STATE_TEMP_READ_COMPLETE:
       {
         //If connection is closed, no point in proceeding in state machine(temperature sensing)
         if(connection_open == 0 || temperature_char == 0){
             state = STATE_IDLE;
         }
         else if(current_event & EVENT_I2C_DONE){ //when I2c read is complete , power down the sensor
              #if ENABLE_LOGGING && STATE_MACHINE_LOGGING
              LOG_INFO("STATE_TEMP_READ_COMPLETE:%d\n\r",0);
              #endif
             sl_power_manager_remove_em_requirement(SL_POWER_MANAGER_EM1); //Bring MCU out of EM1

             uint32_t raw_temp_value = (read_val[0] << 8) | (read_val[1] &0xfc);

             //int32_t d_val = (((175.72 * raw_temp_value)/65536) - 46.85) ; //Datasheet formula

             //Formula obtained from the thermometer example
             int32_t d_val = (((raw_temp_value) * 21965L) >> 13) - 46850; //Thermometer eg. formula

             displayPrintf(DISPLAY_ROW_TEMPVALUE, "Temp=%f",(float)d_val/1000);
            // turn_off_power(gpioPortD, 15); //Turn off power for temperature sensor for load power management


             LOG_INFO("Temp:%f\n\r",(float)d_val/1000);

             //Reference: Professor's slide
             uint8_t htm_temperature_buffer[5] = {0};
             uint8_t *p = &htm_temperature_buffer[1]; //byte[0] is used to indicate farenheit or celsius(keep 0 for C)
             uint32_t htm_temperature_flt;

             htm_temperature_flt = UINT32_TO_FLOAT(d_val, -3); //Dont divide it by 1000

             UINT32_TO_BITSTREAM(p, htm_temperature_flt);

             sl_status_t status = 0;

             //Send to client only if indications are enabled.
             if(temperature_char == 1){
                 //Send temperature value to client
                 //if there is an inflight indication then enqueue

                 if(inflight_indication == 0 && is_empty(cbuffer) == CB_EMPTY)
                 {
                      #if ENABLE_LOGGING
                     LOG_INFO("##No inflight indication and cb is empty so send the temperature event indication directly%f\n\r",(float)d_val/1000);
                     #endif
                     status |= sl_bt_gatt_server_send_indication(ble_common_data->char_connection_handle,gattdb_temperature_measurement, sizeof(htm_temperature_buffer), htm_temperature_buffer);
                     inflight_indication = 1;

                 }
                 else if(inflight_indication == 0 && is_empty(cbuffer) == CB_NO_ERROR)
                 {
                     indication_t dqued_val;
                     cb_dequeue(cbuffer, &dqued_val);
                     if(dqued_val.charHandle == gattdb_temperature_measurement){
                         #if ENABLE_LOGGING
                         LOG_INFO("==soft timer event: dequeued temperature event %f\n\r",gattFloat32ToInt(&dqued_val.buffer[0]));
                         #endif
                     }
                     else if(dqued_val.charHandle == gattdb_button_state){
                         #if ENABLE_LOGGING
                         LOG_INFO("==soft timer event: dequeued button event %d\n\r",dqued_val.buffer[0]);
                         #endif
                     }
                     status |= sl_bt_gatt_server_send_indication(ble_common_data->char_connection_handle,dqued_val.charHandle, dqued_val.bufferLength, &dqued_val.buffer[0]);
                     inflight_indication = 1;
                 }
                 else{
                     #if ENABLE_LOGGING
                     LOG_INFO("***Indication inflight or  cb is not empty, Enqueue temperature event into CB %f\n\r",(float)d_val/1000);
                     #endif
                     indication_t temp;
                     temp.charHandle = gattdb_temperature_measurement;
                     temp.bufferLength = 5;
                     memcpy(&temp.buffer[0], &htm_temperature_buffer[0],sizeof(htm_temperature_buffer));
                     if(cb_enqueue(cbuffer, &temp) == CB_ERROR)
                     {
                         #if ENABLE_LOGGING
                         LOG_INFO("Error while enqueueing in temperature event %d\n\r",0);
                         #endif
                     }
                     else{
                         #if ENABLE_LOGGING
                         LOG_INFO("enqueued in temperature event %d\n\r",cb_get_length(cbuffer));
                         #endif
                     }
                 }

               //Update local gatt database
               sl_bt_gatt_server_write_attribute_value(gattdb_temperature_type,0, sizeof(htm_temperature_buffer), p); //Update local gatt database

//                 #if ENABLE_LOGGING
//                if(status == 0){
//                  LOG_INFO("Send indication with Temp %f \n\r",(float)d_val/1000);
//                }
//                else{
//                  LOG_INFO("Send indication failed %d\n\r",1);
//                }
//                #endif

                (void)status; //suppress warning for this variable

               }

             state = STATE_IDLE; //return back to first state
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
