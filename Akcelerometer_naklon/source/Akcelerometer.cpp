/*
 * Akcelerometer.cpp
 *
 *  Created on: 27. 11. 2019
 *      Author: mlojd
 */

#include "Akcelerometer.h"
#include "math.h"

pit_config_t pitConfig;

float pi = 3.14159265;
float fc = 1;
float T = 0.1;

extern "C" void PIT_LED_HANDLER(void)
{
	//Clear interrupt flag
	PIT_ClearStatusFlags(PIT, kPIT_Chnl_0,kPIT_TimerFlag);
	pitIsrFlag = true;

}

void INIT_PIT(void){ //Periodic interrupt timer

    PIT_GetDefaultConfig(&pitConfig);
    PIT_Init(PIT, &pitConfig);
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(100000, PIT_SOURCE_CLOCK));
    PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);
    EnableIRQ(PIT_IRQ_ID);

    PIT_StartTimer(PIT, kPIT_Chnl_0);
}

void INIT_BOARD(void){
	/* Init board hardware. */
		BOARD_InitBootPins();
		BOARD_InitBootClocks();
		BOARD_InitBootPeripherals();

		/* Init FSL debug console. */
		BOARD_InitDebugConsole();

		LED_BLUE_INIT(kGPIO_DigitalOutput);
		LED_RED_INIT(kGPIO_DigitalOutput);
		LED_GREEN_INIT(kGPIO_DigitalOutput);

}

float dolnoPriepustnyFilter(float os_DPF,float* aktualna_os)
{

	float alfa = (2*pi*T*fc)/(2*pi*T*fc + 1);

	os_DPF = os_DPF + (alfa*((*aktualna_os) - os_DPF));

	return os_DPF;
}

void naklonXYDP(float* suradniceDP, int* naklonXYp) {
	int naklonX, naklonY = 0;
	naklonX = round( atan(suradniceDP[0] / pow( pow(suradniceDP[1],2) + pow(suradniceDP[2],2), 0.5)) * (180/3.14));
	naklonY = round( atan(suradniceDP[1] / pow( pow(suradniceDP[0],2) + pow(suradniceDP[2],2), 0.5)) * (180/3.14));
	naklonXYp[0] = naklonX;
	naklonXYp[1] = naklonY;

}







