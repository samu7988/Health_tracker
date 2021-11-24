/***********************************************************************************
* @file ble.c
 * @brief Handles ble events
 * @Description :Handles various events related to server and client.
 * @author Sayali Mule
 * @date September 8, 2021
 * @Reference:https://github.com/SiliconLabs/bluetooth_stack_features/tree/master/security/pairing_processes_example
 *
 *****************************************************************************/
//***********************************************************************************
//                              Include files
//***********************************************************************************
#include "ble.h"
#include "gatt_db.h"
#include "lcd.h"
#include "cb.h"

// Include logging for this file
#define INCLUDE_LOG_DEBUG 1
#include "src/log.h"


//***********************************************************************************
//                               Macros
//***********************************************************************************



//***********************************************************************************
//                               Global variables
//***********************************************************************************
// BLE private data
ble_data_struct_t ble_data;

#if DEVICE_IS_BLE_SERVER == 0
static const uint8_t server_address[1][6] = SERVER_BT_ADDRESS;
#endif

bool pulse_sensor_char = 0; //Global boolean variable(initially temperature characteristics is OFF)
bool accelerometer_char = 0;
//bool connection_open = 0; //Initially set connection as closed
bool inflight_indication = 0; //No inflight

//***********************************************************************************
//                              Function definition
//***********************************************************************************
/*------------------------------------------------------------------------------------------------------------------------------------*/
/*
@brief: Returns reference(address) to global variable ble_data
@param: None
@return:ble_data_struct_t*: Pointer to structure
*/
/*------------------------------------------------------------------*/
ble_data_struct_t* get_ble_data(){
  return &ble_data;
}


/*------------------------------------------------------------------------------------------------------------------------------------*/
/*
@brief: Handle ble event for any client and server event
@param: None
@return:evt: Event pointer that points to structure and data
*/
/*------------------------------------------------------------------*/
void handle_ble_event(sl_bt_msg_t *evt){

  ble_data_struct_t* ble_common_data = get_ble_data();
  sl_status_t status = 0;

  switch (SL_BT_MSG_ID(evt->header)) {

/*#####################################################################################
     EVENTS COMMON TO SERVER AND CLIENTS
######################################################################################*/


    /*********************************************
     *        BOOT_ID EVENT (BOTH)
     ***********************************************/
    case sl_bt_evt_system_boot_id:
      #if DEVICE_IS_BLE_SERVER
      //Get bluetooth address
      status |= sl_bt_system_get_identity_address(&ble_common_data->myAddress, &ble_common_data->type);

      //Configure the security manager with MITM protection, Encryption requires bonding, Secure connections only
      status |= sl_bt_sm_configure(0x0F, sl_bt_sm_io_capability_displayyesno);

      //Delete all existing bondings
      status |= sl_bt_sm_delete_bondings();
      ble_common_data->bonding = 0;

      status |= sl_bt_advertiser_create_set(&ble_common_data->advertisingSetHandle);


      status |= sl_bt_advertiser_set_timing(ble_common_data->advertisingSetHandle, ADVERTISE_MIN, ADVERTISE_MAX, 0, 0);


      status |= sl_bt_advertiser_start(ble_common_data->advertisingSetHandle, sl_bt_advertiser_general_discoverable, sl_bt_advertiser_connectable_scannable);
      displayPrintf(DISPLAY_ROW_CONNECTION,"Advertising");

      displayPrintf(DISPLAY_ROW_BTADDR, "%x:%x:%x:%x:%x:%x",ble_common_data->myAddress.addr[0],ble_common_data->myAddress.addr[1],
                    ble_common_data->myAddress.addr[2], ble_common_data->myAddress.addr[3],
                    ble_common_data->myAddress.addr[4], ble_common_data->myAddress.addr[5]);



      #if ENABLE_LOGGING
      if(status == SL_STATUS_OK){
          LOG_INFO("SERVER:sl_bt_evt_system_boot_id success %d\n\r",status);
      }
      else{
          LOG_INFO("SERVER:sl_bt_evt_system_boot_id fail %d\n\r",status);
      }
      #endif
      #endif
      break;


    /*********************************************
     *        CONNECTION OPEN EVENT (BOTH)
     ***********************************************/
    case sl_bt_evt_connection_opened_id:
     #if DEVICE_IS_BLE_SERVER
      status = 0;
//      connection_open = 1; //Indicates that the connection is opened

      status |= sl_bt_advertiser_stop(ble_common_data->advertisingSetHandle);

      status |= sl_bt_connection_set_parameters(evt->data.evt_connection_opened.connection, CONN_MIN, CONN_MAX, SLAVE_LATENCY, SUPERVISION_TIMEOUT, 0, 0xFFFF);

      displayPrintf(DISPLAY_ROW_CONNECTION,"Connected");

      #if ENABLE_LOGGING
      if(status == SL_STATUS_OK){
          LOG_INFO("SERVER:sl_bt_evt_connection_opened_id success with timeout %d\n\r",SUPERVISION_TIMEOUT);
      }
      else{
          LOG_INFO("SERVER:sl_bt_evt_connection_opened_id fail %d\n\r",status);
      }
      #endif
      #endif
      break;

    /*********************************************
     *        CONNECTION CLOSED EVENT(BOTH)
     ***********************************************/
    case sl_bt_evt_connection_closed_id:
      #if DEVICE_IS_BLE_SERVER == 1
      status = 0;
//      connection_open = 0; //Indicates that the connection is closed


      //Delete any existing bondings
      status |= sl_bt_sm_delete_bondings();
      ble_common_data->bonding = 0;

      //Start advertisement
      status |= sl_bt_advertiser_start(ble_common_data->advertisingSetHandle, sl_bt_advertiser_general_discoverable, sl_bt_advertiser_connectable_scannable);

      #if ENABLE_LOGGING
      if(status == SL_STATUS_OK){
          LOG_INFO("SERVER:sl_bt_evt_connection_closed_id success %d\n\r",status);
      }
      else{
          LOG_INFO("SERVER:sl_bt_evt_connection_closed_id fail %d\n\r",status);
      }
      #endif
      displayPrintf(DISPLAY_ROW_CONNECTION,"Advertising");
      displayPrintf(DISPLAY_ROW_TEMPVALUE,"");

      break;
      #endif
    /*#####################################################################################
         EVENTS FOR SERVER
    ######################################################################################*/
  #if DEVICE_IS_BLE_SERVER == 1
  /*********************************************
   *      CHARACTERISTICS STATUS EVENT(SERVER)
   ***********************************************/
    //When user presses indicate button on client(temperature or button state) or confirmation has been received from client to server
    case sl_bt_evt_gatt_server_characteristic_status_id:


      //if Client configuration for temperature characteristics are changed(either temperature indications enabled/disabled)
      //if temperature indications are enabled, make the temperature_char = True and turn on LED0
      //elif temperature indications are disabled, make the temperature_char = False and turn off LED0
      if(evt->data.evt_gatt_server_characteristic_status.characteristic == gattdb_BPM &&
          evt->data.evt_gatt_server_characteristic_status.status_flags == sl_bt_gatt_server_client_config)
      {

            //If Indications are enabled
            if(evt->data.evt_gatt_server_characteristic_status.client_config_flags ==  sl_bt_gatt_server_indication){

                pulse_sensor_char = 1; //Enable indication

              //Turn on LED0
              gpioLed0SetOn();

               ble_common_data->char_connection_handle =  evt->data.evt_gatt_server_characteristic_status.connection;

              #if ENABLE_LOGGING
              LOG_INFO("Pulse sensor Indications are enabled %d\n\r",0);
              #endif
          }
          else
          {
            //Indications are disabled
            //Turn off the LED0
            pulse_sensor_char = 0;
            gpioLed0SetOff();
          }
      }
      //if Client configuration for button characteristics are changed(either button indications enabled/disabled)
      //if button indications are enabled, make the button_char = True and turn on LED1
      //elif button indicatiosn are disabled, make the button_char = False and turn off LED1
      else if(evt->data.evt_gatt_server_characteristic_status.characteristic == gattdb_Free_fall  &&
          evt->data.evt_gatt_server_characteristic_status.status_flags == sl_bt_gatt_server_client_config)
      {

          #if ENABLE_LOGGING
          LOG_INFO("BUTTON Characteristics changed %d\n\r",0);
          #endif

          //If button indications are enabled
          if(evt->data.evt_gatt_server_characteristic_status.client_config_flags ==  sl_bt_gatt_server_indication){

              //Store handle for future reference
              ble_common_data->char_connection_handle =  evt->data.evt_gatt_server_characteristic_status.connection;

              //Set this variable to indicate that indications are enabled
              accelerometer_char = 1;

              //Turn on the LED1
              gpioLed1SetOn();

          #if ENABLE_LOGGING
          if(status == SL_STATUS_OK)
            LOG_INFO("Accelerometer Indications are enabled%d\n\r",ble_common_data->button_pressed);
          else
            LOG_INFO("Accelerometer indications failed %d\n\r",status);
          #endif

        }
        else //button indications are disabled
        {
              //Indications are disabled
              //Turn off the LED1
            accelerometer_char = 0;
              gpioLed1SetOff();
        }
      }
      //Acknowledgement for sent data to client has been received.
      else if(evt->data.evt_gatt_server_characteristic_status.status_flags == sl_bt_gatt_server_confirmation){

          inflight_indication = 0; //turn off inflight indication when acknowledgement received from client
      }
      else
      {
          pulse_sensor_char = 0;
          accelerometer_char = 0;
          #if ENABLE_LOGGING
          LOG_INFO("Indications are disabled %d\n\r",0);
          #endif
      }
      break;



      /*********************************************
       *      SERVER_PASSKEY_DISPLAY EVENT(SERVER)
       ***********************************************/
    //Indicates a request to display the passkey to the user
    case sl_bt_evt_sm_passkey_display_id:
      displayPrintf(DISPLAY_ROW_PASSKEY,"Passkey = %u",evt->data.evt_sm_passkey_display.passkey);
      #if ENABLE_LOGGING
      LOG_INFO("SERVER:sl_bt_evt_sm_passkey_display_id  with passkey %u\n\r",evt->data.evt_sm_passkey_display.passkey);
      #endif
      break;


      /*********************************************
       *      SERVER_PASSKEY_CONFIRM EVENT(SERVER)
       ***********************************************/
      //Indicates a request for passkey display and confirmation by the user
    case sl_bt_evt_sm_confirm_passkey_id:
      //store the passkey connection handle for future reference: Used in confirm_pass key function in external signal id event
      ble_common_data->passkey_handle = evt->data.evt_sm_confirm_passkey.connection;
      displayPrintf(DISPLAY_ROW_PASSKEY,"%u",evt->data.evt_sm_confirm_passkey.passkey);
      displayPrintf(DISPLAY_ROW_ACTION,"Confirm with PB0");

      #if ENABLE_LOGGING
      LOG_INFO("SERVER:sl_bt_evt_sm_confirm_passkey_id success with passkey %u\n\r",evt->data.evt_sm_confirm_passkey.passkey);
      #endif
      break;


      /*********************************************
       *      SERVER_BONDED EVENT(SERVER)
       ***********************************************/
    //Triggered after the pairing or bonding procedure is successfully completed
    case sl_bt_evt_sm_bonded_id:
      displayPrintf(DISPLAY_ROW_CONNECTION, "Bonded");
      displayPrintf(DISPLAY_ROW_ACTION, "");
      displayPrintf(DISPLAY_ROW_PASSKEY, "");
      ble_common_data->bonding = 1; //Indicates that bonding is successfull

     #if ENABLE_LOGGING
     LOG_INFO("SERVER:sl_bt_evt_sm_bonded_id success %d\n\r",0);
     #endif
      break;

    /*********************************************
     *      SERVER_CONFIRM BONDED EVENT(SERVER)
     ***********************************************/
    //Indicates a user request to display that the new bonding request is received and for the user to confirm the request
    case sl_bt_evt_sm_confirm_bonding_id:

      status |= sl_bt_sm_bonding_confirm(evt->data.evt_sm_confirm_bonding.connection, 1);

      #if ENABLE_LOGGING
      if(status == SL_STATUS_OK){
          LOG_INFO("SERVER:sl_bt_evt_sm_confirm_bonding_id success, User pressed read button on App %d\n\r",status);
      }
      else{
          LOG_INFO("SERVER:sl_bt_evt_sm_confirm_bonding_id fail %d\n\r",status);
      }
      #endif

      break;

      /*********************************************
       *      SERVER BONDING FAILED EVENT(SERVER)
       ***********************************************/
    //This event is triggered if the pairing or bonding procedure fails
    case sl_bt_evt_sm_bonding_failed_id:
      #if ENABLE_LOGGING
      LOG_INFO("SERVER:sl_bt_evt_sm_bonding_failed_id Bonding failed %d\n\r",0);
      #endif

      status |= sl_bt_connection_close(evt->data.evt_sm_bonding_failed.connection);

      #if ENABLE_LOGGING
      if(status == SL_STATUS_OK){
          LOG_INFO("SERVER:sl_bt_evt_sm_bonding_failed_id Connection closed success %d\n\r",status);
      }
      else{
          LOG_INFO("SERVER:sl_bt_evt_sm_bonding_failed_id Connection closed fail %d\n\r",status);
      }
      #endif

      break;
      /*********************************************
       *      SERVER_INDICATION TIMEOUT EVENT(SERVER)
       ***********************************************/
    case sl_bt_evt_gatt_server_indication_timeout_id:
      #if ENABLE_LOGGING
      LOG_INFO("Confirmation not received from client%d\n\r",1);
      #endif
      break;

      /*********************************************
       *      SERVER SOFT TIMER EVENT(SERVER)
       ***********************************************/
    case sl_bt_evt_system_soft_timer_id:

//      if(inflight_indication == 0 && is_empty(cbuffer) == CB_NO_ERROR)
//      {
//          indication_t dqued_val;
//          cb_dequeue(cbuffer, &dqued_val);
//          #if ENABLE_LOGGING
//          if(dqued_val.charHandle == gattdb_temperature_measurement){
//              LOG_INFO("==soft timer event: dequeued temperature event %f\n\r",gattFloat32ToInt(&dqued_val.buffer[0]));
//          }
////          else if(dqued_val.charHandle == gattdb_button_state){
////              LOG_INFO("==soft timer event: dequeued button event %d\n\r",dqued_val.buffer[0]);
////          }
//          #endif
//
//          status |= sl_bt_gatt_server_send_indication(ble_common_data->char_connection_handle,dqued_val.charHandle, dqued_val.bufferLength, &dqued_val.buffer[0]);
//          inflight_indication = 1;
//
//
//        #if ENABLE_LOGGING
//        if(status == SL_STATUS_OK)
//        {
//
//            LOG_INFO("soft timer success %u\n\r",status);
//        }
//        else
//        {
//            LOG_INFO("soft timer fail %u\n\r",status);
//        }
//        #endif
//      }

      //Prevent charge build up on LCD, occurs every 1 second

      displayUpdate();
      break;

      /*********************************************
       *        CONNECTION PARAMETERS EVENT(SERVER)
       ***********************************************/
      case sl_bt_evt_connection_parameters_id:


        #if ENABLE_LOGGING


        LOG_INFO("Connection Parameters: interval: %f Latency: %f Timeout: %f\n\r",
            ((float)evt->data.evt_connection_parameters.interval * 1.25),
            (((float)evt->data.evt_connection_parameters.latency + 1) * 75),
            (float)evt->data.evt_connection_parameters.timeout);
        #endif
        break;

  /*********************************************
   *        EXTERNAL SIGNAL EVENT(SERVER)
   ***********************************************/
    //This event is generated when peripheral events are set(I2c, 3 second, button press, button release, etc)
    case sl_bt_evt_system_external_signal_id:

      ble_common_data->event = sl_bt_evt_system_external_signal_id;
      //if PB0 is pressed:
      //and If Bonding has not happened, bonding process will be confirmed
      //else if bonding has happened and button indications are enabled, enqueue into CB or send indications directly
      //else if bonding has happened and butoon indications are disabled, then only update the local gatt database.
      if(evt->data.evt_system_external_signal.extsignals & EVENT_BUTTON_PRESSED)
      {

         // ble_common_data->button_pressed = 1;//Indicates that PB0 is pressed
          displayPrintf(DISPLAY_ROW_9, "Button Pressed");

          //If bonding has not happened yet then call passkey confirm function once user presses PB0
          if(ble_common_data->bonding == 0)
          {
            //Accept the pass key confirm value on server to make sure about encryption
            status |= sl_bt_sm_passkey_confirm(ble_common_data->passkey_handle,1);

          }


          #if ENABLE_LOGGING
          if(status == SL_STATUS_OK)
          {
              LOG_INFO("button Press  %d\n\r",0);
              LOG_INFO("sl_bt_evt_system_external_signal_id success %u\n\r",status);
          }
          else
          {
              LOG_INFO("sl_bt_evt_system_external_signal_id fail %u\n\r",status);
          }
          #endif
      }


      break;
  #endif//DEVICE_IS_BLE_SERVER



  // more case statements to handle other BT events
  } // end - switch
}





