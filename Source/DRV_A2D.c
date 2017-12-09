/*********************************************************************************************************************
	Project		: 	Single Section Digital Axle Counter
	Version		: 	2.0
	Revision	:	1
	Filename	: 	drv_a2d.c
	Target MCU	: 	PIC24FJ256GB210
    Compiler	: 	XC16 V1.21
	Author		:	Sudheer Herle
	Date		:	25/12/2005
	Company Name: 	Insys
	Modification History:
					Rev No			Date		Description
					 --				 --				--
	Functions	: 	void Initialise_A2D_Driver(void)
					void Start_A2D_Driver(void)
					void Update_Vdd_Mon_State(void)
					void Update_A2D_State(void)
					void Decrement_A2D_10msTmr(void)
*********************************************************************************************************************/
#include <xc.h>

#include "COMMON.H"
#include "DRV_A2D.H"

a2d_data_t A2D_Data;
vdd_mon_info_t Vdd_Mon_Info;

void Update_A2D_State(void);

/*********************************************************************************
File name 			:drv_a2d.c
Function Name		:void Initialise_A2D_Driver(void)
Created By			:Sudheer Herle
Date Created		:25/12/2005
Modification History:
					Rev No			Date		Description
					 --				 --				--
Tracability:
		SRS()    	:

Abstract			:
Algorithm			:
Description			:
Input Element		:None
Output Element		:void

**********************************************************************************/
void Initialise_A2D_Driver(void)
{

	AD1CON1 = ADCON1_CONFIG;
	AD1CON2 = ADCON2_CONFIG;
        AD1CON3 = ADCON3_CONFIG;
        AD1CHS  = A2D_CS_DEFAULT;
        AD1CSSL = A2D_SC_DEFAULT;
        AD1CSSH = A2D_SC_DEFAULT;
	AD1CON1bits.ADON   = SET_LOW;		/* Switch-off A/D module */

	A2D_Data.State = A2D_NOT_STARTED;
	A2D_Data.Result.Word = 0;
	A2D_Data.Iteration = 0;
	A2D_Data.Summation = 0;
	A2D_Data.Timeout_10ms = 0;
	Vdd_Mon_Info.State = VDD_MON_NOT_STARTED;
	Vdd_Mon_Info.Vdd_Value = VDD_DEFAULT_VALUE;
}
/*********************************************************************************
File name 			:drv_a2d.c
Function Name		:void Start_A2D_Driver(void)
Created By			:Sudheer Herle
Date Created		:25/12/2005
Modification History:
					Rev No			Date		Description
					 --				 --				--
Tracability:
		SRS()    	:

Abstract			:
Algorithm			:
Description			:
Input Element		:None
Output Element		:void

**********************************************************************************/
void Start_A2D_Driver(void)
{
	A2D_Data.State = A2D_IDLE;
	Vdd_Mon_Info.State = VDD_OK;
	Add_SM_Event_to_Queue(EVENT_VDD_VOLTAGE_NORMAL);	/* EVENT_VDD_VOLTAGE_NORMAL event is added to Event Queue */
}
/*********************************************************************************
File name 			:drv_a2d.c
Function Name		:void Update_Vdd_Mon_State(void)
Created By			:Sudheer Herle
Date Created		:25/12/2005
Modification History:
					Rev No			Date		Description
					 --				 --				--
Tracability:
		SRS()    	:

Abstract			:
Algorithm			:
Description			:
Input Element		:None
Output Element		:void

**********************************************************************************/
void Update_Vdd_Mon_State(void)
{
	Update_A2D_State();

	switch (Vdd_Mon_Info.State)
	{
                case VDD_MON_NOT_STARTED:
                    break;
		case VDD_OK:
			if (Vdd_Mon_Info.Vdd_Value > VDD_HIGH_SET_VALUE)
				{
				Add_SM_Event_to_Queue(EVENT_VDD_VOLTAGE_HIGH);	/* EVENT_VDD_VOLTAGE_HIGH event is added to Event Queue */
				Vdd_Mon_Info.State = VDD_HIGH;
				break;
				}
			if (Vdd_Mon_Info.Vdd_Value < VDD_LOW_SET_VALUE)
				{
				Add_SM_Event_to_Queue(EVENT_VDD_VOLTAGE_LOW);	/* EVENT_VDD_VOLTAGE_LOW event is added to Event Queue */
				Vdd_Mon_Info.State = VDD_LOW;
				}
			break;
		case VDD_HIGH:
			if (Vdd_Mon_Info.Vdd_Value <= VDD_HIGH_CLEAR_VALUE)
				{
				Add_SM_Event_to_Queue(EVENT_VDD_VOLTAGE_NORMAL);	/* EVENT_VDD_VOLTAGE_NORMAL event is added to Event Queue */
				Vdd_Mon_Info.State = VDD_OK;
				}
			break;
		case VDD_LOW:
			if (Vdd_Mon_Info.Vdd_Value >= VDD_LOW_CLEAR_VALUE)
				{
				Add_SM_Event_to_Queue(EVENT_VDD_VOLTAGE_NORMAL);	/* EVENT_VDD_VOLTAGE_NORMAL event is added to Event Queue */
				Vdd_Mon_Info.State = VDD_OK;
				}
			break;
	}
}
/*********************************************************************************
File name 			:drv_a2d.c
Function Name		:void Update_A2D_State(void)
Created By			:Sudheer Herle
Date Created		:25/12/2005
Modification History:
					Rev No			Date		Description
					 --				 --				--
Tracability:
		SRS()    	:

Abstract			:
Algorithm			:
Description			:
Input Element		:None
Output Element		:void

**********************************************************************************/
void Update_A2D_State(void)
{
	switch (A2D_Data.State)
		{
                case A2D_NOT_STARTED:
                    break;
		case A2D_IDLE:
			if (A2D_Data.Timeout_10ms == TIMEOUT_EVENT)
				{
				A2D_Data.State = A2D_WAIT_FOR_CONVERSION;
				//AD1CSSL = CHANNEL4_SELECT;
				AD1CON1 = ADCON1_CONFIG;
				AD1CON2 = ADCON2_CONFIG;
                                AD1CON3 = ADCON3_CONFIG;
                                AD1CSSL = A2D_SC_DEFAULT;
                                AD1CSSH = A2D_SC_DEFAULT;
                                AD1CON2bits.SMPI = 0x1F;
                                AD1CHS  = 0x0003; //chanel for 3_3 Mon
				/*
				 * Configure ADCON1 & ADCON2 first and then only load
				 * ADCON0 with CHANNEL4_SELECT.
				 */
				A2D_Data.Result.Word = 0;
				AD1CON1bits.ADON = SET_HIGH;		/* Start conversion */
				}
			break;
		case A2D_WAIT_FOR_CONVERSION:
			if (IFS0bits.AD1IF == 1)
				{
                                    A2D_Data.Result.Word = (ADC1BUF0 + ADC1BUF1 + ADC1BUF2 + ADC1BUF3 +
                                                            ADC1BUF4 + ADC1BUF5 + ADC1BUF6 + ADC1BUF7 +
                                                            ADC1BUF8 + ADC1BUF9 + ADC1BUFA + ADC1BUFB +
                                                            ADC1BUFC + ADC1BUFD + ADC1BUFE + ADC1BUFF +
                                                            ADC1BUF10 + ADC1BUF11 + ADC1BUF12 + ADC1BUF13 +
                                                            ADC1BUF14 + ADC1BUF15 + ADC1BUF16 + ADC1BUF17 +
                                                            ADC1BUF18 + ADC1BUF19 + ADC1BUF1A + ADC1BUF1B +
                                                            ADC1BUF1C + ADC1BUF1D + ADC1BUF1E + ADC1BUF1F)/16;
                                    AD1CON1bits.ADON = SET_LOW;		/* Switch-off A/D module */
                                    A2D_Data.State = A2D_SAVE_RESULT;
				}
			break;
		case A2D_SAVE_RESULT:
			A2D_Data.State = A2D_IDLE;
			A2D_Data.Iteration = A2D_Data.Iteration + 1;
			A2D_Data.Summation = A2D_Data.Summation + A2D_Data.Result.Word;
			A2D_Data.Timeout_10ms = 1;
			if (A2D_Data.Iteration >= A2D_MAX_ITERATIONS)
				{
                                Vdd_Mon_Info.Vdd_Value = VDD_DEFAULT_VALUE;
				//Vdd_Mon_Info.Vdd_Value = (A2D_Data.Summation / A2D_Data.Iteration);
				A2D_Data.Iteration = 0;
				A2D_Data.Summation = 0;
				}
			break;
		}
}

/*********************************************************************************
File name 			:drv_a2d.c
Function Name		:void Decrement_A2D_10msTmr(void)
Created By			:Sudheer Herle
Date Created		:25/12/2005
Modification History:
					Rev No			Date		Description
					 --				 --				--
Tracability:
		SRS()    	:

Abstract			:
Algorithm			:
Description			:
Input Element		:None
Output Element		:void

**********************************************************************************/
void Decrement_A2D_10msTmr(void)
{
	if (A2D_Data.Timeout_10ms > 0)
		{
		A2D_Data.Timeout_10ms = A2D_Data.Timeout_10ms - 1;
		}
}

