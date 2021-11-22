/*
 * @file irq.h
 * @brief Sets up LETIMER IRQ Handler
 * @Description : 1)Sets up Interrupt handler
 *
 * @author Sayali Mule
 * @date september 8, 2021
 * @Reference:  https://github.com/SiliconLabs/peripheral_examples/blob/master/series1/letimer/one_shot_output_compare/src/main_tg11.c
 */




#ifndef SRC_IRQ_H_
#define SRC_IRQ_H_
//***********************************************************************************
//                              Include files
//***********************************************************************************
#include "em_letimer.h"
#include "timer.h"
#include "em_core.h"
#include "scheduler.h"
#include "em_i2c.h"

//***********************************************************************************
//                              Global variables
//***********************************************************************************
extern int rollover_count;
extern int milli_count;
extern uint32_t enable_timer_count;
extern volatile uint32_t Signal; //declare
#endif /* SRC_IRQ_H_ */
