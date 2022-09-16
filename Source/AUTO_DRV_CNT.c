#include <xc.h>

#include "COMMON.h"
#include "AUTO_DRV_CNT.h"

Auto_Reset_Counter_t Auto_Reset_Counter;


void Initialise_Auto_Cnt_Driver(void)
{
	Auto_Reset_Counter.State = AUTO_RESET_CNT_IDLE;
	Auto_Reset_Counter.Timeout_10ms = 0;
}

void Update_Auto_Cnt_State(void)
{
	switch (Auto_Reset_Counter.State)
		{
		case AUTO_RESET_CNT_IDLE:
			AUTO_RESET_COUNTER_PORT = SET_LOW;
			break;
		case AUTO_RESET_CNT_SET_HIGH:
			AUTO_RESET_COUNTER_PORT = SET_HIGH;
			Auto_Reset_Counter.State = AUTO_RESET_CNT_HOLD_HIGH;	
			Auto_Reset_Counter.Timeout_10ms = CNT_PLUSE_HIGH_TIMEOUT;
			break;
		case AUTO_RESET_CNT_HOLD_HIGH:
			if (Auto_Reset_Counter.Timeout_10ms == TIMEOUT_EVENT)
				{
				Auto_Reset_Counter.State = AUTO_RESET_CNT_SET_LOW;
				}
			
			break;
		case AUTO_RESET_CNT_SET_LOW:
			AUTO_RESET_COUNTER_PORT = SET_LOW;
			Auto_Reset_Counter.State = AUTO_RESET_CNT_IDLE;
			break;

		}
}

void Decrement_Auto_Cnt_10msTmr(void)
{

	if (Auto_Reset_Counter.Timeout_10ms > 0)
		{
		Auto_Reset_Counter.Timeout_10ms--;
		}
}

void Increment_Auto_Reset_Counter(void)
{
		Auto_Reset_Counter.State = AUTO_RESET_CNT_SET_HIGH;
		Auto_Reset_Counter.Timeout_10ms = 0;
}

