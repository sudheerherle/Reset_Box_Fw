#include <xc.h>

#include "COMMON.H"
#include "DRV_LED.H"

Preparatory_Reset_LED_t Preparatory_Reset_LED;

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
			PR_LED_PORT = SET_LOW;
			break;
		case LED_STEADY_ON:
			PR_LED_PORT = SET_HIGH;
			break;
		case LED_FLASHING_OFF:
			PR_LED_PORT = SET_LOW;
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
			PR_LED_PORT = SET_HIGH;
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

void Decrement_LED_10msTmr(void)
{

	if (Preparatory_Reset_LED.Timeout_10ms > 0)
		{
		Preparatory_Reset_LED.Timeout_10ms--;
		}
}

void Set_Preparatory_LED_Off(void)
{
	Preparatory_Reset_LED.State = LED_STEADY_OFF;
}

void Set_Preparatory_LED_On(void)
{
	Preparatory_Reset_LED.State = LED_STEADY_ON;
}

void Set_Preparatory_LED_Flashing(void)
{
	Preparatory_Reset_LED.State = LED_FLASHING_OFF;
}
