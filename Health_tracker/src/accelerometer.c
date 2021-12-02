/********************************************************************
 * @file Accelerometer.c
 * @brief 1)setup accelerometer, enable free fall detection
 * @Description : Enable accelerometer for i2c mode of operation and enable free fall detection
 *
 * SDA : PC10
 * SCL : PC11
 * INT1: PC8
 * @author Sayali Mule
 * @date  November 17, 2021
 * @Reference:
 **********************************************************************/

//***********************************************************************************
//                              Include files
//***********************************************************************************
#include "i2c.h"
#include "accelerometer.h"
// Include logging for this file
#define INCLUDE_LOG_DEBUG 1
#include "src/log.h"
//***********************************************************************************
//                              Global variable
//***********************************************************************************



 //***********************************************************************************
 //                              Macros
 //***********************************************************************************
static void set_range(accel_range_t range);
static void set_free_fall_threshold(uint8_t threshold);
static void set_free_fall_duration(uint8_t duration);
static void setup_interrupt(accel_int_t interrupt);

uint8_t read_accelerometer_register(uint8_t reg_address)
{

  uint8_t read_data = 0;
  I2C_write(&reg_address, 1, ACCELEROMETER_SENSOR_ADDRESS,0); //send register address first

  I2C_read(&read_data,1, ACCELEROMETER_SENSOR_ADDRESS,0); //Read the value

  return read_data;
}

void write_accelerometer_register(uint8_t reg_address, uint8_t write_val)
{


  uint8_t val[2] = {reg_address, write_val};

  I2C_write(&val[0], 2, ACCELEROMETER_SENSOR_ADDRESS,0); //send the register address first


}

 /*------------------------------------------------------------------------------------------------------------------------------------*/
 /*
 @brief:Setup accelerometer
 @param: none
 @return: none
 */
 /*-----------------------------------------------------------------------------------------------------------------------------*/
void setup_accelerometer()
{
  /*GPIO pin setup.
    SDA : PC10
    SCL : PC11
    INT1: PD12
  */

  uint8_t val = 0;

  //Read the Device ID register
  val = read_accelerometer_register(ACCEL_DEVID);

  if(val != 0xE5)
  {
      LOG_INFO("Device ID register reading incorrect\n\r");
  }
  //Enable wakeup, autosleep, and measurement mode

  write_accelerometer_register(ACCEL_REG_POWER_CTL, MEASUREMENT_MODE);

  // Clear settings
//  clear_settings();
  set_range(RANGE_2G);


  //Free fall detection
  set_free_fall_threshold(5);
  set_free_fall_duration(20);

  // Select INT 1 for get activities
  setup_interrupt(ACCEL_INT1);
    uint8_t data = read_accelerometer_register(ACCEL_REG_INT_SOURCE);
    (void)data;

}





//*****************************************************************************
/*
@brief:Set data range
@param: range: data range
@return: none
*/
//
//****************************************************************************/
static void set_range(accel_range_t range)
{
    // Get actual value register
    uint8_t value = read_accelerometer_register(ACCEL_REG_DATA_FORMAT);

    // Update the data rate
    // (&) 0b11110000 (0xF0 - Leave HSB)
    // (|) 0b0000xx?? (range - Set range)
    // (|) 0b00001000 (0x08 - Set Full Res)
    value &= 0xF0;
    value |= range;
    value |= (1 << 3);

    write_accelerometer_register(ACCEL_REG_DATA_FORMAT, value);
}


//*****************************************************************************
/*
@brief:Constrain the values between a and b
@param:x, a, b
@return: Constrained value

*/
//
//****************************************************************************/

static uint8_t constrain(uint8_t x, uint8_t a, uint8_t b)
{
    if ((x>=a) && (x<=b)) return (uint8_t)x;
    if (x<a) return a;
    if (x>b) return b;
    return 0;
}

//*****************************************************************************
/*
@brief:Function to set free fall threshold.
@param:Free fall threshold
@return: none
Note : Threshhold - 65.5mg / LSB
*/
//
//****************************************************************************/
static void set_free_fall_threshold(uint8_t threshold)
{
    uint8_t scaled = constrain(threshold, 0, 255);
    write_accelerometer_register(ACCEL_REG_THRESH_FF, scaled);
}

//*****************************************************************************
/*
@brief: Function to set free fall duration .
@param: threshold - Free fall duration
@return: none
 Note:   Duration - 5ms / LSB
*/
//
//****************************************************************************/
static void set_free_fall_duration(uint8_t duration)
{
    uint8_t scaled = constrain(duration, 0, 255);
    write_accelerometer_register(ACCEL_REG_TIME_FF, scaled);
}

/*------------------------------------------------------------------------------------------------------------------------------------*/
/*

@brief:Function to setup interrupt
@param: interrupt
@return: none
*/
/*-----------------------------------------------------------------------------------------------------------------------------*/

static void setup_interrupt(accel_int_t interrupt)
{
    /* INT_ENABLE Register bit fields,
     * D7: DATA_READY
     * D6: SINGLE_TAP
     * D5: DOUBLE_TAP
     * D4: Activity
     * D3: Inactivity
     * D2: FREE_FALL
     * D1: Watermark
     * D0: Overun
     * Here, only enable FREE_FALL Interrupt */
    write_accelerometer_register(ACCEL_REG_INT_ENABLE, ENABLE_FREE_FALL_INT);

    if (interrupt == 0)  //selecting INT1 for free fall interupt
    {
        write_accelerometer_register(ACCEL_REG_INT_MAP, 0x00);
    }
    else
    {
        write_accelerometer_register(ACCEL_REG_INT_MAP, 0xFF);
    }
}

