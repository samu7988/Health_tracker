/*
 * @file i2c.c
 * @brief
 * @Description : 1) I2c read and write operation
 *                2)
 *
 * @author Sayali Mule
 * @date september 15, 2021
 * @Reference:
 */

//***********************************************************************************
//                              Include files
//***********************************************************************************
#include "i2c.h"




//***********************************************************************************
//                              Global variable
//***********************************************************************************
 I2C_TransferSeq_TypeDef transfer;


 //***********************************************************************************
 //                              Macros
 //***********************************************************************************
#define I2C_INT_ENABLED (1)



 /*------------------------------------------------------------------------------------------------------------------------------------*/
 /*
 @brief: Initialises I2C
 @param: none
 @return: none
 */
 /*-----------------------------------------------------------------------------------------------------------------------------*/
 void I2C_init(){

   I2C_Init_TypeDef i2cInit = I2C_INIT_DEFAULT;

   // Using PC10 (SCL) and PC11 (SDA)
   GPIO_PinModeSet(gpioPortC, 10, gpioModeWiredAndPullUpFilter, 1); //SCL
   GPIO_PinModeSet(gpioPortC, 11, gpioModeWiredAndPullUpFilter, 1); //SDA



   // Enable pins at location 4 as specified in datasheet
    I2C0->ROUTEPEN = I2C_ROUTEPEN_SDAPEN | I2C_ROUTEPEN_SCLPEN;
    I2C0->ROUTELOC0 = (I2C0->ROUTELOC0 & (~_I2C_ROUTELOC0_SDALOC_MASK)) | I2C_ROUTELOC0_SDALOC_LOC16; //see pin mapping in datasheet
    I2C0->ROUTELOC0 = (I2C0->ROUTELOC0 & (~_I2C_ROUTELOC0_SCLLOC_MASK)) | I2C_ROUTELOC0_SCLLOC_LOC14; //see pin mapping in datasheet

    //Initialising the I2C
    I2C_Init(I2C0, &i2cInit);

 }

/*-------------------------------------------------------------------------------*
@brief: Write bytes from I2C
@param: write_val: Pointer to write val to be written
        len: length of write val array
@return: none
*/
/*-----------------------------------------------------------------------------------------------------------------------------*/
I2C_TransferReturn_TypeDef I2C_write(uint8_t* write_val, uint8_t len, uint8_t slave_addr){


  I2C_TransferReturn_TypeDef ret;
  NVIC_EnableIRQ(I2C0_IRQn);
  // Write the measure command.
  transfer.addr = slave_addr << 1;
  transfer.flags = I2C_FLAG_WRITE;
  transfer.buf[0].data = write_val;
  transfer.buf[0].len = len;



  //Start I2C transfer(read operation)
  ret = I2C_TransferInit(I2C0, &transfer);

  return ret;

}

/*------------------------------------------------------------------------------------------------------------------------------------*/
/*
@brief: Read bytes from I2C
@param: read_val: values read from sensor are put in this array
        len: length of input buffer
@return: I2C_TransferReturn_TypeDef: Returns whether there is any error with I2c
*/
/*-----------------------------------------------------------------------------------------------------------------------------*/
I2C_TransferReturn_TypeDef I2C_read(uint8_t* read_val , uint8_t len, uint8_t slave_add){

  // Transfer structure
   I2C_TransferReturn_TypeDef result;
   NVIC_EnableIRQ(I2C0_IRQn);
   // Initializing I2C transfer
   transfer.addr          = slave_add<<1;
   transfer.flags         = I2C_FLAG_READ;
   transfer.buf[0].data   = read_val;
   transfer.buf[0].len    = len;

   //Start I2C transfer(read operation)
   result = I2C_TransferInit(I2C0, &transfer);


   return result;

}


