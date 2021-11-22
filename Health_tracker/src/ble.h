/***********************************************************************************
* @file ble.h
 * @brief
 * @Description :
 * @author Sayali Mule
 * @date September 8, 2021
 * @Reference:
 *
 *****************************************************************************/


#ifndef SRC_BLE_H_
#define SRC_BLE_H_


//***********************************************************************************
//                              Include files
//***********************************************************************************
#include <stdint.h>
#include "sl_bluetooth.h"
#include <stdbool.h>




//***********************************************************************************
//                               Macros
//***********************************************************************************
#define ADVERTISE_MIN       400     // Minimum advertising period, for 250 ms, 250/0.625 = 400
#define ADVERTISE_MAX       400     // Minimum advertising period, for 250 ms, 250/0.625 = 400

#define CONN_INT   (75)    //75ms
#define CONN_MIN   60      // Time = Value x 1.25 ms, for 75 ms, value = 60
#define CONN_MAX   60      // Time = Value x 1.25 ms, for 75 ms, value = 60
#define CONN_TIMEOUT      100  //1000ms

#define SLAVE_LATENCY_MS (300) //300
#define SLAVE_LATENCY ((SLAVE_LATENCY_MS/CONN_INT) - (1) )  //(300/75) - 1 = 3
#define SUPERVISION_TIMEOUT    ((1) + (SLAVE_LATENCY)) * ((CONN_MIN) * (2)) //From professor's slides ((1 + 3) * (60 * 2))

#define SCAN_INTERVAL                 80   //50ms
#define SCAN_WINDOW                   40   //25ms

//REference: Professor's slides
#define UINT8_TO_BITSTREAM(p, n) { *(p)++ = (uint8_t)(n); }
#define UINT32_TO_BITSTREAM(p, n) { *(p)++ = (uint8_t)(n); *(p)++ = (uint8_t)((n) >> 8); \
*(p)++ = (uint8_t)((n) >> 16); *(p)++ = (uint8_t)((n) >> 24); }
#define UINT32_TO_FLOAT(m, e) (((uint32_t)(m) & 0x00FFFFFFU) | (uint32_t)((int32_t)(e) << 24))


  // Health Thermometer service UUID defined by Bluetooth SIG
extern uint8_t thermo_service[2] ;

  //Temperature measurement characteristics UUID
extern uint8_t temp_measure_char[2] ;

//Enum for setting type of event i.e
typedef enum{
  EVENT_I2C_DONE = 1, //Set when i2c write or i2c read operation is completed
  EVENT_TIMER_TWO_MSEC_EXPIRE = 2, //event is set every 3 seconds(read temp sensor)
  EVENT_FREE_FALL = 4,
  EVENT_TIMER_USER_REQUESTED_EXPIRE = 8, //event is set when user requested time period is expired
  EVENT_BUTTON_PRESSED = 16,
  EVENT_PB1_BUTTON_PRESSED = 32,
  EVENT_BUTTON_RELEASE = 64,
  EVENT_DEFAULT = 0
}event_e;
//***********************************************************************************
//                               Structure
//***********************************************************************************
//Reference: Professor's slide
// BLE Data Structure, save all of our private BT data in here.
// Modern C (circa 2021 does it this way)
// typedef ble_data_struct_t is referred to as an anonymous struct definition
typedef struct {


  // values for client
  bd_addr myAddress;
  uint8_t type;
  uint64_t event;


// values unique for server
// The advertising set handle allocated from Bluetooth stack.
  uint8_t advertisingSetHandle;
  uint8_t char_connection_handle;
  uint8_t bonding_handle;
  uint8_t passkey_handle;
  uint8_t button_pressed;
  bool bonding;

// values unique for client
  uint8_t scan_connection_handle;
  uint32_t thermometer_service_handle;
  uint16_t thermometer_characteristic_handle;

} ble_data_struct_t;



//***********************************************************************************
//                               Global variables
//***********************************************************************************
extern bool temperature_char;
extern bool connection_open;
extern bool inflight_indication;


//***********************************************************************************
//                              Function prototype
//***********************************************************************************

ble_data_struct_t* get_ble_data();
void handle_ble_event(sl_bt_msg_t *evt);
float gattFloat32ToInt(const uint8_t *value_start_little_endian);









































#endif /* SRC_BLE_H_ */
