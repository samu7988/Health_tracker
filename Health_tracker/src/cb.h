/***********************************************************************************
* @file cb.h
 * @brief Contains the function to create and perform operation on circular buffer
 * @Description : Create circular buffer, enqueue, dequeue data into circular buffer
 * @author Sayali Mule
 * @date October 22, 2021
 * @Reference:
 *
 *****************************************************************************/

#ifndef CB_H_
#define CB_H_

//***********************************************************************************
//                              Include files
//***********************************************************************************
#include <stdint.h>
#include <stddef.h>


//***********************************************************************************
//                              Macros and structure

//***********************************************************************************
typedef struct{
  uint16_t charHandle; // char handle from gatt_db.h
  size_t bufferLength; // length of buffer in bytes to send
  uint8_t buffer[5]; // the actual data buffer for the indication,  need space for HTM [5] and button_state [2] indications
}indication_t;

typedef struct{
  indication_t* buffer;
  indication_t* write;
  indication_t* read;
  size_t count;
  size_t length;
}cb_t;

typedef enum{
  CB_FULL,
  CB_EMPTY,
  CB_ERROR,
  CB_NO_ERROR
}cb_status_e;

//***********************************************************************************
//                              Global variables

//***********************************************************************************
extern cb_t* cbuffer;

//***********************************************************************************
//                              Function prototype

//***********************************************************************************
void create_cb(size_t length);
cb_status_e is_full(cb_t* cb_handle);
cb_status_e is_empty(cb_t* cb_handle);
cb_status_e cb_enqueue(cb_t* cb_handle, indication_t* val);
cb_status_e cb_dequeue(cb_t* cb_handle, indication_t* val);
size_t cb_get_length(cb_t* cb_handle);





#endif /* CB_H_ */
