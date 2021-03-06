/*
  gpio.c
 
   Created on: Dec 12, 2018
       Author: Dan Walkes
   Updated by Dave Sluiter Dec 31, 2020. Minor edits with #defines.

   March 17
   Dave Sluiter: Use this file to define functions that set up or control GPIOs.

 */




#include "gpio.h"





// Set GPIO drive strengths and modes of operation
void gpioInit()
{


  //LED0(PF4)
	GPIO_DriveStrengthSet(LED0_port, gpioDriveStrengthWeakAlternateWeak);
	GPIO_PinModeSet(LED0_port, LED0_pin, gpioModePushPull, false);

  //LED1(PF5)
  GPIO_DriveStrengthSet(LED1_port, gpioDriveStrengthWeakAlternateWeak);
  GPIO_PinModeSet(LED1_port, LED1_pin, gpioModePushPull, false);

  //accelerometer INT1 pin(PC8)
  GPIO_PinModeSet(ACCELEROMETER_port, ACCELEROMETER_pin, gpioModeInput, false);
  GPIO_ExtIntConfig(ACCELEROMETER_port,ACCELEROMETER_pin,ACCELEROMETER_pin,true,false,true); //Rising edge interrupt for INT1


  //Initialise PB0 in input mode and set to rising edge interrupt(PF6)
  GPIO_PinModeSet(PB0_port, PB0_pin, gpioModeInputPull, true);
  GPIO_ExtIntConfig (PB0_port, PB0_pin, PB0_pin, true, true, true);


  //Initialise PB1 in input mode and set to rising edge interrupt(PF7)
  GPIO_PinModeSet(PB1_port, PB1_pin, gpioModeInputPull, true);
  GPIO_ExtIntConfig (PB1_port, PB1_pin, PB1_pin, true, true, true);


  //Clear all pending interrupts
  NVIC_ClearPendingIRQ(GPIO_EVEN_IRQn);
  NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);

  //Enable the odd and even interrupt
  NVIC_EnableIRQ(GPIO_ODD_IRQn);
  NVIC_EnableIRQ(GPIO_EVEN_IRQn);




} // gpioInit()


void gpioLed0SetOn()
{
	GPIO_PinOutSet(LED0_port,LED0_pin);
}


void gpioLed0SetOff()
{
	GPIO_PinOutClear(LED0_port,LED0_pin);
}


void gpioLed1SetOn()
{
	GPIO_PinOutSet(LED1_port,LED1_pin);
}


void gpioLed1SetOff()
{
	GPIO_PinOutClear(LED1_port,LED1_pin);
}

void gpioLed0Toggle()
{
  GPIO_PinOutToggle(LED0_port,LED0_pin);
}

/*------------------------------------------------------------------------------------------------------------------------------------*/
/*
@brief: Turn on power(Use to turn on power(load power management)
@param: port: Pass the port
        pin: Pass the pin number
@return: none
*/
/*-----------------------------------------------------------------------------------------------------------------------------*/
void turn_on_power(GPIO_Port_TypeDef port, unsigned int pin){

  GPIO_PinOutSet(port, pin);
}

/*------------------------------------------------------------------------------------------------------------------------------------*/
/*
@brief: Turn off power(Use to turn on power(load power management)
@param: port: Pass the port
        pin: Pass the pin number
@return: none
*/
/*-----------------------------------------------------------------------------------------------------------------------------*/
void turn_off_power(GPIO_Port_TypeDef port, unsigned int pin){

  GPIO_PinOutClear(port, pin);
}

/*------------------------------------------------------------------------------------------------------------------------------------*/
/*
@brief: Toggle the EXTCOMIN input to the LCD.
@param: port: Pass the port
        pin: Pass the pin number
@return: none
*/
/*-----------------------------------------------------------------------*/

void gpioSetDisplayExtcomin(bool extcomin){
  if(extcomin == 0){
     GPIO_PinOutClear(gpioPortD, 13);
  }
  else{
      GPIO_PinOutSet(gpioPortD, 13);
  }
}


