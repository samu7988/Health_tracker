/***********************************************************************************
* @file accelerometer.h
 * @brief Contains the function to set to control and setup the accelerometer
 * @Description : Controls various aspects of accelerometer
 * @author Sayali Mule
 * @date November 17, 2021
 * @Reference:
 *
 *****************************************************************************/

#ifndef SRC_ACCELEROMETER_H_
#define SRC_ACCELEROMETER_H_

//***********************************************************************************
//                              Include files
//***********************************************************************************


//***********************************************************************************
//                              Macros
//***********************************************************************************
#define ACCEL_DEVID                (0x00)
#define ACCEL_REG_POWER_CTL        (0x2D)
#define ACCEL_REG_DATA_FORMAT      (0x31)
#define ACCEL_REG_THRESH_TAP       (0x1D)
#define ACCEL_REG_DUR              (0x21)
#define ACCEL_REG_LATENT           (0x22)
#define ACCEL_REG_WINDOW           (0x23)
#define ACCEL_REG_THRESH_ACT       (0x24)
#define ACCEL_REG_THRESH_INACT     (0x25)
#define ACCEL_REG_TIME_INACT       (0x26)
#define ACCEL_REG_ACT_INACT_CTL    (0x27)
#define ACCEL_REG_THRESH_FF        (0x28)
#define ACCEL_REG_TIME_FF          (0x29)

#define ACCEL_REG_TAP_AXES         (0x2A)
#define ACCEL_REG_ACT_TAP_STATUS   (0x2B)
#define ACCEL_REG_BW_RATE          (0x2C)
#define ACCEL_REG_POWER_CTL        (0x2D)
#define ACCEL_REG_INT_ENABLE       (0x2E)
#define ACCEL_REG_INT_MAP          (0x2F)
#define ACCEL_REG_INT_SOURCE       (0x30)
#define ACCEL_REG_DATA_FORMAT      (0x31)
#define ACCEL_REG_DATAX0           (0x32)
#define ACCEL_REG_DATAX1           (0x33)
#define ACCEL_REG_DATAY0           (0x34)
#define ACCEL_REG_DATAY1           (0x35)
#define ACCEL_REG_DATAZ0           (0x36)
#define ACCEL_REG_DATAZ1           (0x37)
#define ACCEL_REG_FIFO_CTL         (0x38)
#define ACCEL_REG_FIFO_STATUS      (0x39)


#define ACCEL_GRAVITY_EARTH        9.80665f
#define WAKEUP_MODE                  (0x00) //0
#define AUTOSLEEP_MODE               (0x10) //16
#define MEASUREMENT_MODE             (0x08) //8
#define ENABLE_FREE_FALL_INT         (0x04)


//***********************************************************************************
//                              Enum

//***********************************************************************************
typedef enum
{
    RANGE_16G          = 0x3,
    RANGE_8G           = 0x2,
    RANGE_4G           = 0x1,
    RANGE_2G           = 0x0
} accel_range_t;


typedef enum
{
    ACCEL_INT2 = 0x01,
    ACCEL_INT1 = 0x00
} accel_int_t;


//***********************************************************************************
//                              Global variables

//***********************************************************************************


//***********************************************************************************
//                              Function prototype

void setup_accelerometer();


#endif /* SRC_ACCELEROMETER_H_ */
