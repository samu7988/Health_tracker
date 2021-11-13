/***********************************************************************************
* @file timer.h
 * @brief Contains the function to set to control CMU peripheral
 * @Description : Controls various aspects of CMU like setting on period for LETIMER, prescalar, etc
 * @author Sayali Mule
 * @date September 8, 2021
 * @Reference:
 *
 *****************************************************************************/

#ifndef SRC_I2C_H_
#define SRC_I2C_H_

//***********************************************************************************
//                              Include files
//***********************************************************************************
#include "em_i2c.h"
#include "em_gpio.h"

//***********************************************************************************
//                              Macros

//***********************************************************************************
#define TEMP_SENSOR_ADDRESS (0x40U)



//***********************************************************************************
//                              Global variables

//***********************************************************************************

//***********************************************************************************
//                              Function prototype

//***********************************************************************************

void I2C_init();
I2C_TransferReturn_TypeDef I2C_read(uint8_t* read_val , uint8_t len, uint8_t slave_addr);
I2C_TransferReturn_TypeDef I2C_write(uint8_t* read_val , uint8_t len, uint8_t slave_addr);
I2C_TransferReturn_TypeDef I2C_write_read(uint8_t* write_val, uint8_t* read_val , uint8_t wlen,uint8_t rlen, uint8_t slave_add);






#endif /* SRC_I2C_H_ */
