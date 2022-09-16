#include <xc.h>

#include "COMMON.h"
#include "DRV_CNT.H"

Reset_Counter_t Reset_Counter;


void Initialise_Cnt_Driver(void)
{
	Reset_Counter.State = CNT_IDLE;
	Reset_Counter.Timeout_10ms = 0;
}

void Update_Cnt_State(void)
{
	switch (Reset_Counter.State)
		{
		case CNT_IDLE:
			RESET_COUNTER_PORT = SET_LOW;
			break;
		case CNT_SET_HIGH:
			RESET_COUNTER_PORT = SET_HIGH;
			Reset_Counter.State = CNT_HOLD_HIGH;	
			Reset_Counter.Timeout_10ms = CNT_PLUSE_HIGH_TIMEOUT;
			break;
		case CNT_HOLD_HIGH:
			if (Reset_Counter.Timeout_10ms == TIMEOUT_EVENT)
				{
				Reset_Counter.State = CNT_SET_LOW;
				}
			
			break;
		case CNT_SET_LOW:
			RESET_COUNTER_PORT = SET_LOW;
			Reset_Counter.State = CNT_IDLE;
			break;

		}
}

void Decrement_Cnt_10msTmr(void)
{

	if (Reset_Counter.Timeout_10ms > 0)
		{
		Reset_Counter.Timeout_10ms--;
		}
}

void Increment_Reset_Counter(void)
{
		Reset_Counter.State = CNT_SET_HIGH;
		Reset_Counter.Timeout_10ms = 0;
}

