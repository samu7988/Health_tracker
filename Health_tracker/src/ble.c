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
int power(int num, int exponent){

  int result = 1;
  while(exponent){
      result *= num;
      exponent--;
  }
  return result;
}

// Original code from Dan Walkes. I (Sluiter) fixed a sign extension bug with the mantissa.
// convert IEEE-11073 32-bit float to integer
float gattFloat32ToInt(const uint8_t *value_start_little_endian)
{
  uint8_t signByte = 0;
  int32_t mantissa;
  // data format pointed at by value_start_little_endian is:
  // [0] = contains the flags byte
  // [3][2][1] = mantissa (24-bit 2’s complement)
  // [4] = exponent (8-bit 2’s complement)
  int8_t exponent = (int8_t)value_start_little_endian[4];
  // sign extend the mantissa value if the mantissa is negative
  if (value_start_little_endian[3] & 0x80) { // msb of [3] is the sign of the mantissa
  signByte = 0xFF;
  }
  mantissa = (int32_t) (value_start_little_endian[1] << 0) |
  (value_start_little_endian[2] << 8) |
  (value_start_little_endian[3] << 16) |
  (signByte << 24) ;

  float temperature = 0;
  if(exponent < 0){
      exponent = -1 * exponent;
//      LOG_INFO("Exponent value %d\n", power(10,exponent));
      temperature = (((float)1/power(10, exponent)) * mantissa);
  }
//   value = 10^exponent * mantissa, pow() returns a double type
  else{
      temperature = (power(10, exponent) * mantissa);
  }
  //temperature = (float)mantissa/1000;
  LOG_INFO("Tempearture: %f\n\r",temperature);

  return temperature;
} // gattFloat32ToInt



//***********************************************************************************
//                               Global variables
//***********************************************************************************
// BLE private data
ble_data_struct_t ble_data;

#if DEVICE_IS_BLE_SERVER == 0
static const uint8_t server_address[1][6] = SERVER_BT_ADDRESS;
#endif

bool temperature_char = 0; //Global boolean variable(initially temperature characteristics is OFF)
bool button_char = 0;
bool connection_open = 0; //Initially set connection as closed
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

//  if(SL_BT_MSG_ID(evt->header) != sl_bt_evt_system_soft_timer_id)
//    LOG_INFO("Event is %x\n\r",SL_BT_MSG_ID(evt->header));

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
      #else
      status |= sl_bt_system_get_identity_address(&ble_common_data->myAddress, &ble_common_data->type);
      //Print client address
      displayPrintf(DISPLAY_ROW_BTADDR, "%x:%x:%x:%x:%x:%x",ble_common_data->myAddress.addr[0],ble_common_data->myAddress.addr[1],
                    ble_common_data->myAddress.addr[2], ble_common_data->myAddress.addr[3],
                    ble_common_data->myAddress.addr[4], ble_common_data->myAddress.addr[5]);

      //Set Passive scanning
      status |= sl_bt_scanner_set_mode(sl_bt_gap_1m_phy, 0); //Passive scanning

      // Set scan interval and scan window
      status |= sl_bt_scanner_set_timing(sl_bt_gap_1m_phy, SCAN_INTERVAL, SCAN_WINDOW);

      // Set the default connection parameters for subsequent connections
      status |= sl_bt_connection_set_default_parameters(CONN_MIN,CONN_MAX,0,CONN_TIMEOUT,0,0xFFFF);

      // Start scanning - looking for thermometer devices
      status |= sl_bt_scanner_start(sl_bt_gap_1m_phy, sl_bt_scanner_discover_generic);

      //Print harcoded server address
      displayPrintf(DISPLAY_ROW_BTADDR2, "%x:%x:%x:%x:%x:%x",server_address[0][0],server_address[0][1],
                    server_address[0][2], server_address[0][3],
                    server_address[0][4], server_address[0][5]);

      displayPrintf(DISPLAY_ROW_CONNECTION,"Discovering");


      #if ENABLE_LOGGING
      if(status == SL_STATUS_OK){
          LOG_INFO("CLIENT:sl_bt_evt_system_boot_id success %d\n\r",status);
      }
      else{
          LOG_INFO("CLIENT:sl_bt_evt_system_boot_id fail %d\n\r",status);
      }
      #endif//ENABLE_LOGGING

      #endif// DEVICE_IS_BLE_SERVER
      break;


    /*********************************************
     *        CONNECTION OPEN EVENT (BOTH)
     ***********************************************/
    case sl_bt_evt_connection_opened_id:
     #if DEVICE_IS_BLE_SERVER
      status = 0;
      connection_open = 1; //Indicates that the connection is opened

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
      #else
      //Handle open connection for client
      displayPrintf(DISPLAY_ROW_CONNECTION,"Connected");

      #endif //DEVICE_IS_BLE_SERVER
      break;

    /*********************************************
     *        CONNECTION CLOSED EVENT(BOTH)
     ***********************************************/
    case sl_bt_evt_connection_closed_id:
      #if DEVICE_IS_BLE_SERVER == 1
      status = 0;
      connection_open = 0; //Indicates that the connection is closed


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
      #else
      // This event is generated when a connection is dropped
      // remove connection from active connections
      // start scanning again to find new devices
      status |= sl_bt_scanner_start(sl_bt_gap_1m_phy, sl_bt_scanner_discover_generic);
      displayPrintf(DISPLAY_ROW_CONNECTION,"Discovering");

      #if ENABLE_LOGGING
      if(status == SL_STATUS_OK){
          LOG_INFO("CLIENT:sl_bt_evt_connection_closed_id success %d\n\r",status);
      }
      else{
          LOG_INFO("CLIENT:sl_bt_evt_connection_closed_id fail %d\n\r",status);
      }
      #endif //ENABLE_LOGGING
      #endif//DEVICE_BLE_SERVER
      break;

    /*#####################################################################################
         EVENTS FOR SERVER
    ######################################################################################*/
  #if DEVICE_IS_BLE_SERVER == 1
  /*********************************************
   *      CHARACTERISTICS STATUS EVENT(SERVER)
   ***********************************************/
    //When user presses indicate button on client(temperature or button state) or confirmation has been received from client to server
    case sl_bt_evt_gatt_server_characteristic_status_id:


      //If temperature indications are enabled
      if(evt->data.evt_gatt_server_characteristic_status.characteristic == gattdb_temperature_measurement &&
          evt->data.evt_gatt_server_characteristic_status.status_flags == sl_bt_gatt_server_client_config)
      {

            //Indications are enabled
            if(evt->data.evt_gatt_server_characteristic_status.client_config_flags ==  sl_bt_gatt_server_indication){

            temperature_char = 1; //Enable indication

            //Turn on LED0
            gpioLed0SetOn();

             ble_common_data->char_connection_handle =  evt->data.evt_gatt_server_characteristic_status.connection;

            #if ENABLE_LOGGING
            LOG_INFO("Temperature Indications are enabled %d\n\r",0);
            #endif
          }
          else
          {
            //Indications are disabled
            //Turn off the LED0
            temperature_char = 0;
            gpioLed0SetOff();
          }
      }
      //if button indications are enabled
      else if(evt->data.evt_gatt_server_characteristic_status.characteristic == gattdb_button_state &&
          evt->data.evt_gatt_server_characteristic_status.status_flags == sl_bt_gatt_server_client_config)
      {
//          status |= sl_bt_gatt_server_write_attribute_value(gattdb_button_state,0, sizeof(int), 1);

          #if ENABLE_LOGGING
          LOG_INFO("BUTTON Characteristics changed %d\n\r",0);
          #endif

          //Indications are enabled
          if(evt->data.evt_gatt_server_characteristic_status.client_config_flags ==  sl_bt_gatt_server_indication){

              //Store handle for future reference
              ble_common_data->char_connection_handle =  evt->data.evt_gatt_server_characteristic_status.connection;

              //Set this variable to indicate that indications are enabled
              button_char = 1;

              //Turn on the LED1
              gpioLed1SetOn();

          #if ENABLE_LOGGING
          if(status == SL_STATUS_OK)
            LOG_INFO("Button state Indications are enabled%d\n\r",ble_common_data->button_pressed);
          else
            LOG_INFO("Button state indications failed %d\n\r",status);
          #endif

        }
        else
        {
              //Indications are disabled
              //Turn off the LED1
              button_char = 0;
              gpioLed1SetOff();
        }
      }

      else if(evt->data.evt_gatt_server_characteristic_status.status_flags == sl_bt_gatt_server_confirmation){
//          #if ENABLE_LOGGING
//          LOG_INFO("Confirmation for indications has been received %d\n\r",0);
//          #endif
          inflight_indication = 0; //turn off inflight indication when acknowledgement received from client
      }
      else{
          temperature_char = 0;
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

      if(inflight_indication == 0 && is_empty(cbuffer) == CB_NO_ERROR)
      {
          indication_t dqued_val;
          cb_dequeue(cbuffer, &dqued_val);
          #if ENABLE_LOGGING
          if(dqued_val.charHandle == gattdb_temperature_measurement){
              LOG_INFO("==soft timer event: dequeued temperature event %f\n\r",gattFloat32ToInt(&dqued_val.buffer[0]));
          }
          else if(dqued_val.charHandle == gattdb_button_state){
              LOG_INFO("==soft timer event: dequeued button event %d\n\r",dqued_val.buffer[0]);
          }
          #endif

          status |= sl_bt_gatt_server_send_indication(ble_common_data->char_connection_handle,dqued_val.charHandle, dqued_val.bufferLength, &dqued_val.buffer[0]);
          inflight_indication = 1;


        #if ENABLE_LOGGING
        if(status == SL_STATUS_OK)
        {

            LOG_INFO("soft timer success %u\n\r",status);
        }
        else
        {
            LOG_INFO("soft timer fail %u\n\r",status);
        }
        #endif
      }

      //Prevent charge build up on LCD, occurs every 1 second

      displayUpdate();
      break;

      /*********************************************
       *        CONNECTION PARAMETERS EVENT(SERVER)
       ***********************************************/
      case sl_bt_evt_connection_parameters_id:


        #if ENABLE_LOGGING
//        if(evt->data.evt_connection_parameters.security_mode == connection_mode1_level4){
//            LOG_INFO("Authenticated Secure Connections pairing with encryption:%d\n\r",0);
//        }

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
      if(evt->data.evt_system_external_signal.extsignals & EVENT_BUTTON_PRESSED)
      {

         // ble_common_data->button_pressed = 1;//Indicates that PB0 is pressed
          displayPrintf(DISPLAY_ROW_9, "Button Pressed");

          //If bonding has not happened yet then call passkeyconfirm function once user presses PB0
          if(ble_common_data->bonding == 0)
          {
            //Accept the pass key confirm value on server to make sure about encryption
            status |= sl_bt_sm_passkey_confirm(ble_common_data->passkey_handle,1);

          }
          //send button_state value to client if indications are enabled and bonding is complete
          else if(ble_common_data->bonding == 1 && button_char == 1)
          {
              //Update local gatt database
              uint8_t button_pressed = 1;
              status |= sl_bt_gatt_server_write_attribute_value(gattdb_button_state,0, sizeof(button_pressed), &button_pressed); //Update local gatt database


              if(inflight_indication == 0 && is_empty(cbuffer) == CB_EMPTY)
              {
                  uint8_t button_pressed = 1;
//                  LOG_INFO("##No inflight indication so send the button press indication directly%d\n\r",0);
                status |= sl_bt_gatt_server_send_indication(ble_common_data->char_connection_handle,gattdb_button_state, sizeof(button_pressed),&button_pressed);
                inflight_indication = 1;

                #if ENABLE_LOGGING
                if(status == SL_STATUS_OK)
                {

                    LOG_INFO("Sending button press indication success %u\n\r",status);
                }
                else
                {
                    LOG_INFO("Sending button press indication fail %u\n\r",status);
                }
                #endif

              }
              else if(inflight_indication == 0 && is_empty(cbuffer) == CB_NO_ERROR)
              {
                  indication_t dqued_val;
                  cb_dequeue(cbuffer, &dqued_val);

                  #if ENABLE_LOGGING
                  if(dqued_val.charHandle == gattdb_temperature_measurement){
                      LOG_INFO("==soft timer event: dequeued temperature event %f\n\r",gattFloat32ToInt(&dqued_val.buffer[0]));
                  }
                  else if(dqued_val.charHandle == gattdb_button_state){
                      LOG_INFO("==soft timer event: dequeued button event %d\n\r",dqued_val.buffer[0]);
                  }
                  #endif

                  status |= sl_bt_gatt_server_send_indication(ble_common_data->char_connection_handle,dqued_val.charHandle, dqued_val.bufferLength, &dqued_val.buffer[0]);
                  inflight_indication = 1;
              }
              else
              {
//                  LOG_INFO("**Indication in flight or cb not empty, Enqueue button press event in cb %d\n\r",0);
                  indication_t temp;
                  temp.charHandle = gattdb_button_state;
                  temp.bufferLength = 1;
                  temp.buffer[0] = 0x01;
                  if(cb_enqueue(cbuffer, &temp) == CB_ERROR)
                  {
                      #if ENABLE_LOGGING
                      LOG_INFO("Error while enqueueing in button press %d\n\r",-1);
                      #endif
                  }
                  else{
                      #if ENABLE_LOGGING
                      LOG_INFO("enqueued in button press %d\n\r",cb_get_length(cbuffer));
                      #endif
                  }
              }





          }
          //if indications are not enabled and bonding is successfull
          else if(ble_common_data->bonding == 1 && button_char == 0)
          {
              uint8_t button_pressed = 1;

              //Update local gatt database
              status |= sl_bt_gatt_server_write_attribute_value(gattdb_button_state,0, sizeof(button_pressed), &button_pressed); //Update local gatt database
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
      else if(evt->data.evt_system_external_signal.extsignals & EVENT_BUTTON_RELEASE)
      {
          displayPrintf(DISPLAY_ROW_9, "Button Released");


          //send button_state value to client if indications are enabled and bonding is complete
          if(ble_common_data->bonding == 1 && button_char == 1)
          {
              uint8_t button_pressed = 0;
              status |= sl_bt_gatt_server_write_attribute_value(gattdb_button_state,0, sizeof(button_pressed), &button_pressed); //Update local gatt database


              if(inflight_indication == 0 && is_empty(cbuffer) == CB_EMPTY)
              {
                uint8_t button_pressed = 0;
                #if ENABLE_LOGGING
                LOG_INFO("##No inflight indication so send the button press indication directly%d\n\r",0);
                #endif

                status |= sl_bt_gatt_server_send_indication(ble_common_data->char_connection_handle,gattdb_button_state, sizeof(button_pressed), &button_pressed);
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
              else {
                  #if ENABLE_LOGGING
                  LOG_INFO("**Indication in flight or cb not empty, Enqueue button release event into CB %d\n\r",0);
                  #endif
                  indication_t temp;
                  temp.charHandle = gattdb_button_state;
                  temp.bufferLength = 1;
                  temp.buffer[0] = 0x00;
                  if(cb_enqueue(cbuffer, &temp) == CB_ERROR)
                  {
                      #if ENABLE_LOGGING
                      LOG_INFO("Error while enqueueing in button release");
                      #endif
                  }
                  else{
                      #if ENABLE_LOGGING
                      LOG_INFO("enqueued in button release %d\n\r",cb_get_length(cbuffer));
                      #endif
                  }
              }


              #if ENABLE_LOGGING
              if(status == SL_STATUS_OK){

                  LOG_INFO("Sending button release indication success %u\n\r",status);
              }
              else{
                  LOG_INFO("Sending button release indication fail %u\n\r",status);
              }
              #endif

          }
          //if indications are not enabled and bonding is successfull
          else if(ble_common_data->bonding == 1 && button_char == 0)
          {
              uint8_t button_pressed = 0;
              status |= sl_bt_gatt_server_write_attribute_value(gattdb_button_state,0, sizeof(button_pressed), &button_pressed); //Update local gatt database
          }


        #if ENABLE_LOGGING
        if(status == SL_STATUS_OK)
        {
            LOG_INFO("Release button  %d\n\r",0);
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

  /*#####################################################################################
       EVENTS FOR CLIENT
  ######################################################################################*/
    // This event is generated when an advertisement packet or a scan response
    // is received from a responder
    #if DEVICE_IS_BLE_SERVER == 0

  /*********************************************
   *      SCAN REPORT EVENT(CLIENT)
   ***********************************************/
    // This event is generated when an advertisement packet or a scan response
    // is received from a server
    case sl_bt_evt_scanner_scan_report_id:



      //Check if the address stored(hardcoded) matches with the address in the adverstisement packet
      if((server_address[0][0] == evt->data.evt_scanner_scan_report.address.addr[0]) &&
                           (server_address[0][1] == evt->data.evt_scanner_scan_report.address.addr[1]) &&
                           (server_address[0][2] == evt->data.evt_scanner_scan_report.address.addr[2]) &&
                           (server_address[0][3] == evt->data.evt_scanner_scan_report.address.addr[3]) &&
                           (server_address[0][4] == evt->data.evt_scanner_scan_report.address.addr[4]) &&
                           (server_address[0][5] == evt->data.evt_scanner_scan_report.address.addr[5]))
        {
            //Connect after scan response
          status |= sl_bt_scanner_stop();
          status |= sl_bt_connection_open(evt->data.evt_scanner_scan_report.address, evt->data.evt_scanner_scan_report.address_type,sl_bt_gap_1m_phy,&ble_common_data->scan_connection_handle);
        }

      displayPrintf(DISPLAY_ROW_CONNECTION,"Discovering");

      #if ENABLE_LOGGING
      if(status == SL_STATUS_OK){
          LOG_INFO("sl_bt_evt_scanner_scan_report_id success %d\n\r",status);
      }
      else{
          LOG_INFO("sl_bt_evt_scanner_scan_report_id fail %d\n\r",status);
      }
      #endif//ENABLE LOGGING
      break;

    /*********************************************
     *      GATT SERVICE EVENT(CLIENT)
     ***********************************************/
    // This event is generated when a new service is discovered
    case sl_bt_evt_gatt_service_id:
      //When thermometer service is discovered by client
      if(!(memcmp(evt->data.evt_gatt_service.uuid.data,thermo_service,2))){

          // Save service handle for future reference
          ble_common_data->thermometer_service_handle = evt->data.evt_gatt_service.service;

          #if ENABLE_LOGGING
          LOG_INFO("sl_bt_evt_gatt_service_id success %d\n\r",0);
          #endif

      }
      else{
        #if ENABLE_LOGGING
        LOG_INFO("sl_bt_evt_gatt_service_id fail %d\n\r",1);
        #endif
      }


      break;

    /*********************************************
     *      GATT CHARACTERISTICS EVENT (CLIENT)
     ***********************************************/
    // This event is generated when a new characteristic is discovered
    case sl_bt_evt_gatt_characteristic_id:
      if(!(memcmp(evt->data.evt_gatt_characteristic.uuid.data,temp_measure_char,2)))
      {

          // Save characteristic handle for future reference
          ble_common_data->thermometer_characteristic_handle= evt->data.evt_gatt_characteristic.characteristic;

          #if ENABLE_LOGGING
          LOG_INFO("sl_bt_evt_gatt_characteristic_id success %d\n\r",0);
          #endif

      }
      else{
        #if ENABLE_LOGGING
        LOG_INFO("sl_bt_evt_gatt_characteristic_id fail %d\n\r",1);
        #endif
      }
      break;

    /*********************************************
     *      GATT CHARACTERISTICS VALUE(TEMP VALUE) EVENT(CLIENT)
     ***********************************************/
    // This event is generated when a characteristic value was received e.g. an indication
    case sl_bt_evt_gatt_characteristic_value_id:
      //Handled in the discovery state machine
      displayPrintf(DISPLAY_ROW_CONNECTION, "Handling Indications");

      break;

    /*********************************************
     *      GATT PROCEDURE COMPLETE EVENT(CLIENT)
     ***********************************************/
    // This event is generated for various procedure completions, e.g. when a
    // write procedure is completed, or service discovery is completed
    case sl_bt_evt_gatt_procedure_completed_id:
      //This is handled in discovery state machine
      break;

      #endif //DEVICE_BLE_SERVER

  // more case statements to handle other BT events
  } // end - switch
}





