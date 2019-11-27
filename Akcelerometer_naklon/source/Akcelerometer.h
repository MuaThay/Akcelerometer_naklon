/*
 * Akcelerometer.h
 *
 *  Created on: 27. 11. 2019
 *      Author: mlojd
 */

#ifndef AKCELEROMETER_H_
#define AKCELEROMETER_H_
// Include

#include <stdio.h>
#include "MMA8451Q/MMA8451Q.h"
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "fsl_i2c.h"
#include "fsl_pit.h"
#include "fsl_smc.h"

//Define

#define PIT_LED_HANDLER PIT_IRQHandler
#define PIT_IRQ_ID PIT_IRQn
#define PIT_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_BusClk)
#define LED_INIT() LED_RED_INIT(LOGIC_LED_ON)
#define LED_TOGGLE() LED_RED_TOGGLE()

//Variables

extern volatile bool pitIsrFlag;

//Function

extern "C" void PIT_LED_HANDLER(void);
//extern "C" void PORTA_IRQHandler(void);
void INIT_BOARD(void);
void INIT_PIT(void);
float dolnoPriepustnyFilter(float os_HPF,float* aktualna_os);
void naklonXYDP(float* suradniceDP, int* naklonXYp);

#endif /* AKCELEROMETER_H_ */
