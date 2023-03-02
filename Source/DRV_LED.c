#include <xc.h>

#include "COMMON.h"
#include "DRV_LED.H"

Preparatory_Reset_LED_t Preparatory_Reset_LED;
Preparatory_Reset_LED_t Preparatory_Reset_LED2;

extern BYTE HA_config;

void Initialise_LED_Driver(void)
{
	Preparatory_Reset_LED.State = LED_STEADY_OFF;
	Preparatory_Reset_LED.Timeout_10ms = 0;
}

void Update_Preparatory_LED_State(void)
{
	switch (Preparatory_Reset_LED.State)
		{
		case LED_STEADY_OFF:
			PR1_LED_PORT = SET_LOW;
//            if(HA_config){
//                PR2_LED_PORT = SET_LOW;
//            }
			break;
            
		case LED_STEADY_ON:
            Preparatory_Reset_LED.Timeout_10ms = 0;
			PR1_LED_PORT = SET_HIGH;
			break;
            
            
		case LED_FLASHING_OFF:
			PR1_LED_PORT = SET_LOW;
			Preparatory_Reset_LED.State = LED_FLASHING_OFF_WAIT;
			Preparatory_Reset_LED.Timeout_10ms = LED_FLASHING_OFF_TIMEOUT;
			break;
		case LED_FLASHING_OFF_WAIT:
			if (Preparatory_Reset_LED.Timeout_10ms == TIMEOUT_EVENT)
				{
				Preparatory_Reset_LED.State = LED_FLASHING_ON;
				Preparatory_Reset_LED.Timeout_10ms = 0;
				}
			break;
		case LED_FLASHING_ON:
			PR1_LED_PORT = SET_HIGH;
			Preparatory_Reset_LED.State = LED_FLASHING_ON_WAIT;
			Preparatory_Reset_LED.Timeout_10ms = LED_FLASHING_ON_TIMEOUT;
			break;
		case LED_FLASHING_ON_WAIT:
			if (Preparatory_Reset_LED.Timeout_10ms == TIMEOUT_EVENT)
				{
				Preparatory_Reset_LED.State = LED_FLASHING_OFF;
				Preparatory_Reset_LED.Timeout_10ms = 0;
				}
			break;
		}
}

void Update_Preparatory_LED2_State(void)
{
	switch (Preparatory_Reset_LED2.State)
		{
		case LED_STEADY_OFF:
			PR2_LED_PORT = SET_LOW;
			break;
           
		case LED_STEADY_ON:
			PR2_LED_PORT = SET_HIGH;
			break;
            
		case LED_FLASHING_OFF:
            PR2_LED_PORT = SET_LOW;
			Preparatory_Reset_LED2.State = LED_FLASHING_OFF_WAIT;
			Preparatory_Reset_LED2.Timeout_10ms = LED_FLASHING_OFF_TIMEOUT;
			break;
		case LED_FLASHING_OFF_WAIT:
			if (Preparatory_Reset_LED2.Timeout_10ms == TIMEOUT_EVENT)
				{
				Preparatory_Reset_LED2.State = LED_FLASHING_ON;
				Preparatory_Reset_LED2.Timeout_10ms = 0;
				}
			break;
		case LED_FLASHING_ON:
			PR2_LED_PORT = SET_HIGH;
			Preparatory_Reset_LED2.State = LED_FLASHING_ON_WAIT;
			Preparatory_Reset_LED2.Timeout_10ms = LED_FLASHING_ON_TIMEOUT;
			break;
		case LED_FLASHING_ON_WAIT:
			if (Preparatory_Reset_LED2.Timeout_10ms == TIMEOUT_EVENT)
				{
				Preparatory_Reset_LED2.State = LED_FLASHING_OFF;
				Preparatory_Reset_LED2.Timeout_10ms = 0;
				}
			break;
		}
}


void Decrement_LED_10msTmr(void)
{

	if (Preparatory_Reset_LED.Timeout_10ms > 0)
		{
		Preparatory_Reset_LED.Timeout_10ms--;
		}
    if (Preparatory_Reset_LED2.Timeout_10ms > 0)
		{
		Preparatory_Reset_LED2.Timeout_10ms--;
		}
}

void Set_Preparatory_LED_Off(void)
{
	Preparatory_Reset_LED.State = LED_STEADY_OFF;
}

void Set_Preparatory_LED2_Off(void)
{
	Preparatory_Reset_LED2.State = LED_STEADY_OFF;
}

void Set_Preparatory_LED_On(void)
{
    PR1_LED_PORT = SET_HIGH;
	Preparatory_Reset_LED.State = LED_STEADY_ON;
}

void Set_Preparatory_LED2_On(void)
{
    PR2_LED_PORT = SET_HIGH;
	Preparatory_Reset_LED2.State = LED_STEADY_ON;
}

void Set_Preparatory_LED_Flashing(void)
{
	Preparatory_Reset_LED.State = LED_FLASHING_ON;
}

void Set_Preparatory_LED2_Flashing(void)
{
	Preparatory_Reset_LED2.State = LED_FLASHING_ON;
}