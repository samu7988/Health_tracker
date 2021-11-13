/***********************************************************************************
* @file cb.c
 * @brief Contains the function to create and perform operation on circular buffer
 * @Description : Create circular buffer, enqueue, dequeue data into circular buffer
 * @author Sayali Mule
 * @date October 22, 2021
 * @Reference:
 *
 *****************************************************************************/
//***********************************************************************************
//                              Include files
//***********************************************************************************
#include "cb.h"
#include <stdlib.h>
#include <string.h>
// Include logging for this file
#define INCLUDE_LOG_DEBUG 1
#include "src/log.h"

//***********************************************************************************
//                              Global variable
//***********************************************************************************
cb_t* cbuffer = NULL;

 //***********************************************************************************
 //                              Macros
 //***********************************************************************************



 /*------------------------------------------------------------------------------------------------------------------------------------*/
 /*
 @brief: Create circular buffer
 @param: length : length of circular buffer
 @return: cb_t*: Returns pointer to circular buffer handle
 */
 /*-----------------------------------------------------------------------------------------------------------------------------*/
void create_cb(size_t length){
  cb_t* cb_handle =(cb_t*)malloc(sizeof(cb_t));
  cb_handle->buffer =(indication_t*)malloc(sizeof(indication_t) * length);
  cb_handle->write = cb_handle->buffer;
  cb_handle->read = cb_handle->buffer;
  cb_handle->count = 0;
  cb_handle->length = length;

  cbuffer = cb_handle; //Store the cb handle in global variable

}


/*------------------------------------------------------------------------------*/
/*
@brief: Check whether circular buffer is full
@param: length : length of circular buffer
@return: cb_t*: Returns pointer to circular buffer handle
*/
/*------------------------------------------------------------------------------*/
cb_status_e is_full(cb_t* cb_handle){
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();

  cb_status_e status = CB_NO_ERROR;

  if(cb_handle == NULL){
      #if ENABLE_LOGGING
      LOG_INFO("is_full() cb_handle is NULL %d\n\r",0);
      #endif
      status =  CB_ERROR;
  }
  else{

      if(cb_handle->count == cb_handle->length){
          #if ENABLE_LOGGING
          LOG_INFO("is_full() cb is full %d\n\r",0);
          #endif
          status = CB_FULL;
      }
      else{
          status = CB_NO_ERROR;
      }
  }
  CORE_EXIT_CRITICAL();
  return status;
}


/*------------------------------------------------------------------------------*/
/*
@brief: Check whether circular buffer is empty
@param: cb_handle: pointer to circular buffer
@return: cb_status_e: Status code of circular buffer: FULL, EMPTY, ERROR, NO_ERROR
*/
/*------------------------------------------------------------------------------*/
cb_status_e is_empty(cb_t* cb_handle){

  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();

  cb_status_e status = CB_NO_ERROR;
  if(cb_handle == NULL){
      #if ENABLE_LOGGING
      LOG_INFO("is_empty() cb_handle is NULL %d\n\r",0);
      #endif
      status =  CB_ERROR;
  }
  else{

      if(cb_handle->count == 0){
          #if ENABLE_LOGGING
          LOG_INFO("is_empty() cb is empty %d\n\r",0);
          #endif
          status =  CB_EMPTY;
      }
      else{
          status =  CB_NO_ERROR;
      }
  }
 CORE_EXIT_CRITICAL();

  return status;
}

/*------------------------------------------------------------------------------*/
/*
@brief: Get length of circular buffer
@param: cb_handle: pointer to circular buffer

@return: cb_status_e: Status code of circular buffer: FULL, EMPTY, ERROR, NO_ERROR
*/
/*------------------------------------------------------------------------------*/
size_t cb_get_length(cb_t* cb_handle){
  if(cb_handle == NULL){
      return -1;
  }
  else{
      return cb_handle->count;
  }
}
/*------------------------------------------------------------------------------*/
/*
@brief: Enqueues value into circular buffer
@param: cb_handle: pointer to circular buffer
        val: data that is to be enqueued
@return: cb_status_e: Status code of circular buffer: FULL, EMPTY, ERROR, NO_ERROR
*/
/*------------------------------------------------------------------------------*/
cb_status_e cb_enqueue(cb_t* cb_handle, indication_t* val){

    CORE_DECLARE_IRQ_STATE;
    CORE_ENTER_CRITICAL();

    cb_status_e status = CB_NO_ERROR;
  if(cb_handle == NULL){
      #if ENABLE_LOGGING
      LOG_INFO("cb_enqueue() cb_handle is NULL %d\n\r",0);
      #endif
      status = CB_ERROR;
  }
  else{
      //if cb is full or not
      if(is_full(cb_handle) == CB_FULL){
          #if ENABLE_LOGGING
          LOG_INFO("cb_enqueue() cb is full %d\n\r",0);
          #endif
          status =  CB_FULL;
      }
      else{
//          *(cb_handle->write) = val;
          #if ENABLE_LOGGING
          LOG_INFO("cb_enqueue() cb enqueue %d\n\r",0);
          #endif
          memcpy(cb_handle->write,val,sizeof(indication_t));
          //wrap around write pointer
          if(cb_handle->write == cb_handle->buffer + cb_handle->length -1){
              cb_handle->write = cb_handle->buffer;
          }
          else{
              cb_handle->write = cb_handle->write + 1;
          }
          cb_handle->count += 1;
      }

      status =  CB_NO_ERROR;
  }
  CORE_EXIT_CRITICAL();

  return status;
}

/*------------------------------------------------------------------------------*/
/*
@brief: Dequeues value from circular buffer
@param: cb_handle: pointer to circular buffer
        val: Pointer to data that is dequeued
@return: cb_status_e: Status code of circular buffer: FULL, EMPTY, ERROR, NO_ERROR
*/
/*------------------------------------------------------------------------------*/
cb_status_e cb_dequeue(cb_t* cb_handle, indication_t* val){
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();

  cb_status_e status = CB_NO_ERROR;
  if(cb_handle == NULL){
      #if ENABLE_LOGGING
      LOG_INFO("cb_dequeue() cb_handle is NULL %d\n\r",0);
      #endif
      status =  CB_ERROR;
  }
  else{
      //if cb is full or not
      if(is_empty(cb_handle) == CB_EMPTY){
          #if ENABLE_LOGGING
          LOG_INFO("cb_dequeue() cb is empty %d\n\r",0);
          #endif
          status =  CB_EMPTY;
      }
      else{
          #if ENABLE_LOGGING
          LOG_INFO("cb_dequeue() cb dequeue %d\n\r",0);
          #endif
          //*val = *(cb_handle->read);
          memcpy(val, cb_handle->read, sizeof(indication_t));
          //wrap around read pointer
          if(cb_handle->read == cb_handle->buffer + cb_handle->length -1){
              cb_handle->read = cb_handle->buffer;
          }
          else{
              cb_handle->read = cb_handle->read + 1;
          }

          cb_handle->count -= 1;
      }

      status =  CB_NO_ERROR;
  }
  CORE_EXIT_CRITICAL();

  return status;
}


