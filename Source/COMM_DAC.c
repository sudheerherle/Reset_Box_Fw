/*********************************************************************************************************************
	Project		: 	Single Section Digital Axle Counter
	Version		: 	2.0
	Revision	:	1
	Filename	: 	comm_dac.c
	Target MCU	: 	PIC24FJ256GB210
    Compiler	: 	XC16 V1.21
	Author		:	Tilak BG
	Date		:
	Company Name: 	Insys
	Modification History:
					Rev No			Date		Description
					 --				 --				--
	Functions	:   void Initialise_SPI(void)
					void Update_SPI_State(void)
					void Set_SPI_Sch_Idle(void)
					void Decrement_SPI_Sch_msTmr(void)
					void Process_SPI_Message(BYTE uchCPU_ID)
					void Build_Message(BYTE uchCPU_ID)
					void Update_Shadow_Register(BYTE uchCPU)
					void Synchronise_Shadow_Register(void)
					void Update_Comm_Err_Counter(BYTE uchCPU_ID)

*********************************************************************************************************************/
#include <xc.h>

#include "COMMON.H"
#include "COMM_DAC.H"
#include "CRC16.H"
#include "EEPROM.H"
#include "EVENTS.H"

extern sm_status_t Status;				/* from cpu_sm.c */
extern dac_comm_error Dac_Comm_Err;			/* from cpu_sm.c */
extern  BYTE CPU1_Address;				/* from cpu_sm.c */
extern  BYTE CPU2_Address;				/* from cpu_sm.c */
extern dac_sysinfo_t DAC_sysinfo;			/* from cpu_sm.c */

extern BYTE Latest_DAC_data[SPI_MESSAGE_LENGTH];


extern BYTE CPU1_data[SPI_MESSAGE_LENGTH];			/* Buffer to store data recived from Cpu1 */
extern BYTE CPU2_data[SPI_MESSAGE_LENGTH];			/* Buffer to store data recived from Cpu2 */

BYTE CPU1_data_GLCD[GCPU_SPI_MESSAGE_LENGTH];			/* Buffer to store data recived from Cpu1 */
BYTE CPU2_data_GLCD[GCPU_SPI_MESSAGE_LENGTH];			/* Buffer to store data recived from Cpu2 */

BYTE Shadow_CPU1_data[SPI_MESSAGE_LENGTH];			/* Buffer to store data recived from Cpu1 */
BYTE Shadow_CPU2_data[SPI_MESSAGE_LENGTH];			/* Buffer to store data recived from Cpu2 */

BYTE Shadow_CPU1_data_GLCD[GCPU_SPI_MESSAGE_LENGTH];			/* Buffer to store data recived from Cpu1 */
BYTE Shadow_CPU2_data_GLCD[GCPU_SPI_MESSAGE_LENGTH];			/* Buffer to store data recived from Cpu2 */

BYTE Transmit_Queue[SPI_MESSAGE_LENGTH];	/* Buffer which holds the data to be transmitted to cpu */
 comm_sch_info_t SPI_Sch_Info;			/* structure to hold inforamtion of SPI scheduler */
spirecvobject SPIRecvObject;					/* SPI Message Receiving Buffer */
event_register_t Shadow[MAXIMUM_NO_OF_CPU]; /* shadow will have present status of Both Cpus */


void Set_SPI_Sch_Idle(void);
void Process_SPI_Message(BYTE);
void Build_Message(BYTE);
void Update_Shadow_Register(BYTE);
void Synchronise_Shadow_Register(void);
void Update_Comm_Err_Counter(BYTE);

extern unsigned char Erase_Start;
unsigned char Erase_track;
/* The Transmission from CPU to the SM_CPU is through the SPI. The software routiene shall
   initilaize the ports either as output or input based on the usage of the ports. The Software
   shall also intialize the schedulers state, timeout and scan rate. */
/*********************************************************************************
File name 			:comm_dac.c
Function Name		:void Initialise_SPI(void)
Created By			:Tilak BG
Date Created		:
Modification History:
					Rev No			Date		Description
					 --				 --				--
Tracability:
		SRS()    	:

Abstract			:SPI configuration and SPI scheduler Initilaistion will be done.
Algorithm			:
Description			:
Input Element		:None
Output Element		:void

**********************************************************************************/

void setup_SPI (void);
void Initialise_SPI(void)
{

//	TRISC5 = OUTPUT_PORT;						     /* configure SDO is Output */
//	TRISC3  = OUTPUT_PORT;						     /* configure SCK is Output */
	SS_CPU1_PORT_DIR = OUTPUT_PORT;				     /* configure SS1 - Slave Select CPU1 is Output */
	SS_CPU2_PORT_DIR = OUTPUT_PORT;				     /* configure SS2 - Slave Select CPU2 is Output */
	SS_CPU1_PORT = SPI_DESELECT_SLAVE_DEVICE;        /* Deselect SPI Slave -CPU1 */
	SS_CPU2_PORT = SPI_DESELECT_SLAVE_DEVICE;		 /* Deselect SPI Slave -CPU2 */

	SPI_Sch_Info.State = SPI_SCHEDULER_IDLE;		 /* set SPI Scheduler to Idle State */
	SPI_Sch_Info.Timeout_ms = 0;					 /* set SPI Scheduler timeout to zero */
	SPI_Sch_Info.ScanRate = SPI_SCHEDULER_SCAN_RATE; /* set SPI Scheduler Scan rate to SPI_SCHEDULER_SCAN_RATE */

        /* Configure Remappable pins */
        //*************************************************************
        // Unlock Registers
        //*************************************************************
        __builtin_write_OSCCONL(OSCCON & 0xbf); //clear the bit 6 of OSCCONL to unlock Pin Re-map

        RPINR22bits.SDI2R = 26;
        RPOR10bits.RP21R  = 11;
        RPOR9bits.RP19R   = 10;

        //************************************************************
        // Lock Registers
        //************************************************************
        __builtin_write_OSCCONL(OSCCON | 0x40); //set the bit 6 of OSCCONL to lock Pin Re-map
        //************************************************************

        SPI2CON2bits.SPIBEN = 1;
        IFS2bits.SPI2IF = 0;
        IEC2bits.SPI2IE = 0;
        SPI2CON1 = SPI_MASTER;  // select mode
        SPI2STAT = SPI_ENABLE;  // enable the peripheral
        //setup_SPI();
//	Synchronise_Shadow_Register();					 /* copy Shadow_Register with EEPROM Stored Value at Boot Time */
        Erase_Start = 0;
        Erase_track = 0;
}

/*********************************************************************************
File name 			:comm_dac.c
Function Name		:void Update_SPI_State(void)
Created By			:Tilak BG
Date Created		:
Modification History:
					Rev No			Date		Description
					 --				 --				--
Tracability:
		SRS()    	:

Abstract			: SPI Scheduler which implements SPI protocol to exchange data with both Cpus
Algorithm			:
						1.Initially SPI scheduler will be in Idle State.
						2.After Spi_Scan time completed, it will select Appropriate Cpu to which it wants to
						   Exchange the data.
						3.After Pre_Transmission delay, it will goto step 4.
						4.Transmit One Byte and Receive One Byte.
						5.Check for All Bytes are Transmitted. If Not it will repeat Step 4.
						6.It will check CRC for recived Bytes. If Crc matches it will process the message.
						7.It will deselect the selected cpu and Select the other Cpu for Communication.
						8.It will goto Idle State with calculated Scan time.
Description			:
Input Element		:None
Output Element		:void

**********************************************************************************/
                    UINT16 temp_crc_SPI;
                    extern volatile char inspect_event_done;
void Update_SPI_State(void)
{
	static BYTE uchSelectedCPU = 1,index;                    /* uchSelectedCPU holds selected Cpu Id to which SM cpu is communicating, index will indicate the Byte No */
	static BYTE uchBytes_to_Transmit,FirstByte_Received=0;   /* uchBytes_to_Transmit holds No of Bytes to be exchangedng, FirstByte_Received indicates that recived Byte is First one or Not*/

        if(Erase_Start == 1)
        {
            LATDbits.LATD12 = 1;
        }
        if(inspect_event_done == 0)
            return;
	switch (SPI_Sch_Info.State)
		{
		case SPI_SCHEDULER_IDLE:
			if (SPI_Sch_Info.Timeout_ms == TIMEOUT_EVENT)	/* SPI- scheduler scan time completed, So, Select the appropriate cpu */
				{
				if (uchSelectedCPU == 1)
					{
					/* Select  CPU1 */
					SS_CPU1_PORT = SPI_SELECT_SLAVE_DEVICE;			/* CPU1 is Selceted */
					SS_CPU2_PORT = SPI_DESELECT_SLAVE_DEVICE;		/* CPU2 is De-Selceted */
					}
				else
					{
					/* Select CPU2 */
					SS_CPU2_PORT = SPI_SELECT_SLAVE_DEVICE;			/* CPU2 is Selceted */
					SS_CPU1_PORT = SPI_DESELECT_SLAVE_DEVICE;		/* CPU1 is De-Selceted */
					}
				index =0;
//				TMR2ON = 1;											  /* Switch ON Timer2 - SPI Clock Source */
				Build_Message(uchSelectedCPU);						  /* Bulid the message to transmit */
				SPI_Sch_Info.Timeout_ms = SPI_PRE_TRANSMISSION_DELAY; /* Set pre transmission delay */
				SPI_Sch_Info.State = SPI_BREATHING_TIME_FOR_SLAVE;
				}
			break;
		case SPI_BREATHING_TIME_FOR_SLAVE:
			if (SPI_Sch_Info.Timeout_ms == TIMEOUT_EVENT)
				{
				SPI_Sch_Info.State = SPI_WAIT_FOR_XMIT_TO_FINISH;	  /* pre transmission delay completed, so state is changed to transmission */
				uchBytes_to_Transmit = (BYTE) SPI_MESSAGE_LENGTH + 1; /* No of Bytes to be exchanged is assigned.one Byte is added to Actual Length.
  																	   * Becuse First Byte received from Cpu is Dummy Byte */
				SPIRecvObject.Index = 0;							  /* Receive Buffer index is assigned to 0*/
				}
			break;
		case SPI_WAIT_FOR_XMIT_TO_FINISH:
                    
                    if(Erase_Start == 1)
                    {
                        Erase_track = 1;
                        Erase_Start = 0;
                    }
			if (SPI_Sch_Info.Timeout_ms == TIMEOUT_EVENT)
			{
					if (uchBytes_to_Transmit > 1)
					{
						while(SPI2STATbits.SPITBF == 0)//if (BF == 0)					   /* Still all Bytes are not completely transmitted */
						{
                                                    /* SSPBUF empty - Transmit Status, So we can Put the Next Byte */
                                                    SPI2BUF = Transmit_Queue[index];
                                                    index = index + 1;
                                                    uchBytes_to_Transmit = uchBytes_to_Transmit - 1;
                                                }
						SPI_Sch_Info.State = SPI_WAIT_FOR_RECEIVE_TO_COMPLETE;  /* Go to Receiving State */
						
					}
					else
					{
					SPI_Sch_Info.State = SPI_CHECK_CRC;	/* All Bytes transmission and Reception Over, So go to CRC Check to validate Received Data */
					FirstByte_Received = 0;
					}
			}
			break;
		case SPI_WAIT_FOR_RECEIVE_TO_COMPLETE:
                        if(SPI2STATbits.SPIRBF)
                        {
                                while(!SPI2STATbits.SRXMPT)
				{
                                    /* SSPBUF full - Received data from Slave */
                                    SPIRecvObject.Msg_Buffer[SPIRecvObject.Index] = SPI2BUF;
                                    SPIRecvObject.Index = SPIRecvObject.Index + 1;
                                }
				SPI_Sch_Info.State = SPI_WAIT_FOR_XMIT_TO_FINISH;             /* go to Transmitting State */
				SPI_Sch_Info.Timeout_ms = SPI_DELAY_BETWEEN_BYTES;			  /* Introduce A time delay Between Bytes */
			}
			break;
		case SPI_CHECK_CRC:

                    temp_crc_SPI = Crc16((const BYTE *)&SPIRecvObject.Msg_Buffer[0], SPI_MESSAGE_LENGTH-2);
			if (temp_crc_SPI == ((UINT16)(SPIRecvObject.Msg_Buffer[SPI_MESSAGE_LENGTH-1]<<8)+(SPIRecvObject.Msg_Buffer[SPI_MESSAGE_LENGTH-2])))
			{
				/* valid CRC-16 Checksum */
		 	  Process_SPI_Message(uchSelectedCPU);						     /* Received  Data is Valid, Process the Data */
			}
			if (uchSelectedCPU == 1)
			{
				/* CPU1 over, now select CPU2 */
				uchSelectedCPU = 2;											/* Select Cpu 2 */
			}
			else
			{
				/* CPU2 over, now select CPU1 */
				uchSelectedCPU = 1;											/* Select Cpu 1 */
			}
			SPI_Sch_Info.Timeout_ms = SPI_HOLD_SS_LOW_TIMEOUT;
			SPI_Sch_Info.State = SPI_HOLD_SS_LOW;
			break;
		case SPI_HOLD_SS_LOW:
			if (SPI_Sch_Info.Timeout_ms == TIMEOUT_EVENT)
			{
				Set_SPI_Sch_Idle();											/* this function is called to Set the time gap for Next Transmission */
			}
			break;
		}
}

/*********************************************************************************
File name 			:comm_dac.c
Function Name		:void Build_Message(BYTE uchCPU_ID)
Created By			:Tilak BG
Date Created		:
Modification History:
					Rev No			Date		Description
					 --				 --				--
Tracability:
		SRS()    	:

Abstract			: Builds The Transmit Queue with Cpu Info to Transmit to CPu
Algorithm			:
Description			:
Input Element		:uchCPU_ID - represent the CPU No (CPU1 or Cpu2)
Output Element		:void

**********************************************************************************/

void Build_Message(BYTE uchCPU_ID)
{
	BYTE i=0;

	if (uchCPU_ID == CPU1_ID)
	{
		/* Build CPU2 information to transmit to CPU1  */
		for(i =0;i < SPI_MESSAGE_LENGTH;i++)
		{
	   	  Transmit_Queue[i] = CPU2_data[i];
		  /* After Building Cpu2 data to sent to Cpu1, Cpu2 data has to be cleaned.
		   * otherwise the same message will get retransmitted in case of Cpu2 failed to send New Data */
		  CPU2_data[i] = (BYTE) 0;
		}
	}
	if (uchCPU_ID == CPU2_ID)
	{
		/* Build CPU1 information to transmit to CPU2  */
		for(i =0;i < SPI_MESSAGE_LENGTH;i++)
		{
		  Transmit_Queue[i] = CPU1_data[i];
                  /* After Building Cpu1 data to sent to Cpu2, Cpu1 data has to cleaned
	       * otherwise the same message will get retransmitted in case of Cpu1 failed to send New Data */
		  CPU1_data[i] = (BYTE) 0;
		}
	}
}

/*********************************************************************************
File name 			:comm_dac.c
Function Name		:void Synchronise_Shadow_Register(void)
Created By			:Tilak BG
Date Created		:
Modification History:
					Rev No			Date		Description
					 --				 --				--
Tracability:
		SRS()    	:

Abstract			: Shadow Register will be updated by Status information Stored On onchip EEPROM
Algorithm			:
Description			:
Input Element		:None
Output Element		:void

**********************************************************************************/

void Synchronise_Shadow_Register(void)
{
	BYTE i=0;
	BYTE *pEEPROM_data;

	if (Is_CPU1_EEPROM_Record_Valid())  /* Check the CPU1 Data stored on EEPROM is Valid or Not */
	{
		/* The stored data is Valid, So Load value from on-chip EEPROM */
		pEEPROM_data = Get_CPU1_EEPROM_Record();
		for (i = 0; i < NO_OF_EVENT_REGISTERS; i++)
		{
			Shadow[0].Byte[i] = *pEEPROM_data;
			pEEPROM_data = pEEPROM_data + 1;
		}
	}
	else
	{
		/* The stored data is not Valid, So Load with Default Values */
		Shadow[0].Byte[0] = (BYTE) EVENT_REGISTER1_DEFAULT;
		Shadow[0].Byte[1] = (BYTE) EVENT_REGISTER2_DEFAULT;
		Shadow[0].Byte[2] = (BYTE) EVENT_REGISTER3_DEFAULT;
		Shadow[0].Byte[3] = (BYTE) EVENT_REGISTER4_DEFAULT;
		Shadow[0].Byte[4] = (BYTE) EVENT_REGISTER5_DEFAULT;
		Shadow[0].Byte[5] = (BYTE) EVENT_REGISTER6_DEFAULT;
	}
	if (Is_CPU2_EEPROM_Record_Valid()) /* Check the CPU1 Data stored on EEPROM is Valid or Not */
	{
		/* The stored data is Valid, So Load value from on-chip EEPROM */
		pEEPROM_data = Get_CPU2_EEPROM_Record();
		for (i = 0; i < NO_OF_EVENT_REGISTERS; i++)
		{
			Shadow[1].Byte[i] = *pEEPROM_data;
			pEEPROM_data = pEEPROM_data + 1;
		}
	}
	else
	{
		/* The stored data is not Valid, So Load with Default Values */
		Shadow[1].Byte[0] = (BYTE) EVENT_REGISTER1_DEFAULT;
		Shadow[1].Byte[1] = (BYTE) EVENT_REGISTER2_DEFAULT;
		Shadow[1].Byte[2] = (BYTE) EVENT_REGISTER3_DEFAULT;
		Shadow[1].Byte[3] = (BYTE) EVENT_REGISTER4_DEFAULT;
		Shadow[1].Byte[4] = (BYTE) EVENT_REGISTER5_DEFAULT;
		Shadow[1].Byte[5] = (BYTE) EVENT_REGISTER6_DEFAULT;
	}
}

/*********************************************************************************
File name 			:comm_dac.c
Function Name		:void Update_Comm_Err_Counter(BYTE uchCPU_ID)
Created By			:Tilak BG
Date Created		:
Modification History:
					Rev No			Date		Description
					 --				 --				--
Tracability:
		SRS()    	:

Abstract			: Update communication Crc error counts of Both CPU
Algorithm			:
					 1.For Every Crc error Dac cpu will toggle error Bit.
					 2.each CPU will have two error Bits, one For Modem A and Modem B
					 3.For Every State Change of these bits, this fuction will increment the error Count.

Description			:
Input Element		:uchCPU_ID - represent the CPU No (CPU1 or Cpu2)
Output Element		:void

**********************************************************************************/

void Update_Comm_Err_Counter(BYTE uchCPU_ID)
{
	bitadrb_t Buffer;

	if (uchCPU_ID != CPU1_ID && uchCPU_ID != CPU2_ID)
	{
		return;		/* Invalid range */
	}

	if(uchCPU_ID == CPU1_ID)
	{
		Buffer.Byte = CPU1_data[7];
		/* Update New state of CPU1 Comm A and Comm B Error Bit*/
		Dac_Comm_Err.Flags.New_Cpu1_CommA_Err_State = Buffer.Bit.b2;
		Dac_Comm_Err.Flags.New_Cpu1_CommB_Err_State = Buffer.Bit.b3;

		if( Dac_Comm_Err.Flags.New_Cpu1_CommA_Err_State != Dac_Comm_Err.Flags.Old_Cpu1_CommA_Err_State)
		{
			 /* New state is not same as old State, Update the counter.*/
			  if(Dac_Comm_Err.CPU1_CommA_Error_Count > LARGEST_CONVERTABLE_INTEGER)
			  {
              	Dac_Comm_Err.CPU1_CommA_Error_Count =0;
			  }
			  else
			  {
				Dac_Comm_Err.CPU1_CommA_Error_Count = Dac_Comm_Err.CPU1_CommA_Error_Count + 1;
			  }
		}
		if( Dac_Comm_Err.Flags.New_Cpu1_CommB_Err_State != Dac_Comm_Err.Flags.Old_Cpu1_CommB_Err_State)
		{
			 /* New state is not same as old State, Update the counter.*/
			  if(Dac_Comm_Err.CPU1_CommB_Error_Count > LARGEST_CONVERTABLE_INTEGER)
			  {
              	Dac_Comm_Err.CPU1_CommB_Error_Count =0;
			  }
			  else
			  {
				Dac_Comm_Err.CPU1_CommB_Error_Count = Dac_Comm_Err.CPU1_CommB_Error_Count + 1;
			  }
		 }
		/* Update old state with New state*/
		Dac_Comm_Err.Flags.Old_Cpu1_CommA_Err_State = Dac_Comm_Err.Flags.New_Cpu1_CommA_Err_State;
		Dac_Comm_Err.Flags.Old_Cpu1_CommB_Err_State = Dac_Comm_Err.Flags.New_Cpu1_CommB_Err_State;
	}
	else
	{
		Buffer.Byte = CPU2_data[7];
		/* Update New state of CPU1 Comm A and Comm B Error Bit*/
		Dac_Comm_Err.Flags.New_Cpu2_CommA_Err_State = Buffer.Bit.b2;
		Dac_Comm_Err.Flags.New_Cpu2_CommB_Err_State = Buffer.Bit.b3;

		if( Dac_Comm_Err.Flags.New_Cpu2_CommA_Err_State !=  Dac_Comm_Err.Flags.Old_Cpu2_CommA_Err_State)
		  {
			 /* New state is not same as old State, Update the counter.*/
			  if(Dac_Comm_Err.CPU2_CommA_Error_Count > LARGEST_CONVERTABLE_INTEGER)
			    {
              	Dac_Comm_Err.CPU2_CommA_Error_Count =0;
				}
			  else
				{
				Dac_Comm_Err.CPU2_CommA_Error_Count = Dac_Comm_Err.CPU2_CommA_Error_Count + 1;
				}
		  }
		if( Dac_Comm_Err.Flags.New_Cpu2_CommB_Err_State != Dac_Comm_Err.Flags.Old_Cpu2_CommB_Err_State)
		  {
		 	/* New state is not same as old State, Update the counter.*/
			  if(Dac_Comm_Err.CPU2_CommB_Error_Count > LARGEST_CONVERTABLE_INTEGER)
			    {
              	Dac_Comm_Err.CPU2_CommB_Error_Count =0;
				}
			  else
				{
				Dac_Comm_Err.CPU2_CommB_Error_Count = Dac_Comm_Err.CPU2_CommB_Error_Count + 1;
				}
		  }
		/* Update old state with New state*/
		Dac_Comm_Err.Flags.Old_Cpu2_CommA_Err_State = Dac_Comm_Err.Flags.New_Cpu2_CommA_Err_State;
		Dac_Comm_Err.Flags.Old_Cpu2_CommB_Err_State = Dac_Comm_Err.Flags.New_Cpu2_CommB_Err_State;
	}
}

