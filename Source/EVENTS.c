/*********************************************************************************************************************
	Project		: 	Single Section Digital Axle Counter
	Version		: 	2.0 
	Revision	:	1	
	Filename	: 	events.c
	Target MCU	: 	PIC24FJ256GB210   
    Compiler	: 	XC16 Compiler V1.21  
	Author		:	Sudheer Herle
	Date		:	
	Company Name: 	Insys Digital Systems
	Modification History:
					Rev No			Date		Description
					 --				 --				--    
	Functions	:   UINT16 Get_Current_Event_Number(void)
					BYTE Get_Events_Counter_Value(void)
					void Clear_Events_Counter(void)
					BOOL Setup_Events_Erasure(void)
					void Initialise_Events_Sch(void)
					void Start_Events_Sch(void)
					events_sch_state_t Inspect_First_Event(void)
					void Update_Events_Sch_State(void)
					void Decrement_Events_Sch_10msTmr(void)
					void Add_SM_Event_to_Queue(event_id_t uchEventId)
					void Build_Event_Queue_Record(BYTE uchCPU, event_id_t uchEventId)
					void Add_to_Event_Queue(BYTE uchCPU, event_id_t uchEventId, UINT16 uiUSFwdCount, UINT16 uiUSRevCount, UINT16 uiDSFwdCount, UINT16 uiDSRevCount)
					void Detect_DAC_Events(BYTE uchCPU, event_register_t *New_Event)
					BOOL Save_DAC_Event(void)
					BOOL Modify_Token_of_old_Events(void)
					BYTE Compute_LRC(BYTE *puchMsg, BYTE uchMsgLen)
					void Synchronise_Events_Register(void)
			
*********************************************************************************************************************/
#include <xc.h>
#include <time.h>

#include "COMMON.H"
#include "DRV_MEM.H"
#include "command_proc.h"
#include "comm_host.h"
#include "EVENTS.H"
#include "EEPROM.H"

extern sm_status_t Status;							/* from cpu_sm.c */
extern time_t SystemClock;							/* from cpu_sm.c */
extern BYTE CPU1_Address;							/* from cpu_sm.c */
extern BYTE CPU2_Address;							/* from cpu_sm.c */
extern BYTE CPU1_data[SPI_MESSAGE_LENGTH];								/* from comm_dac.c */
extern BYTE CPU2_data[SPI_MESSAGE_LENGTH];								/* from comm_dac.c */

char CPU1_System_error, CPU2_System_error;

/* Event Detection Table - when Status Bit changed from Low to High*/
const event_id_t Event_When_High[NO_OF_EVENT_REGISTERS][EVENTS_PER_REGISTER] = {
		{EVENT_US_BLOCK_CLEAR, EVENT_DS_BLOCK_CLEAR, EVENT_DAC_DS_RESET_APPLIED, EVENT_SYSTEM_NORMAL,
		 EVENT_DIRECT_OUT_COUNT, EVENT_NOT_OCCURRED, EVENT_NOT_OCCURRED, EVENT_DAC_US_RESET_APPLIED},
		{EVENT_ALL_BOARDS_FOUND, EVENT_PD1_BOARD_FOUND, EVENT_PD2_BOARD_FOUND, EVENT_MODEM_BOARD_A_FOUND,
		 EVENT_MODEM_BOARD_B_FOUND, EVENT_RELAY_BOARD_A_FOUND, EVENT_RELAY_BOARD_B_FOUND, EVENT_PEER_CPU_FOUND},
		{EVENT_LU1_US1_COMM_RESTORED, EVENT_LU1_US2_COMM_RESTORED, EVENT_LU1_DS1_COMM_RESTORED, EVENT_LU1_DS2_COMM_RESTORED,
		 EVENT_LU2_US1_COMM_RESTORED, EVENT_LU2_US2_COMM_RESTORED, EVENT_LU2_DS1_COMM_RESTORED, EVENT_LU2_DS2_COMM_RESTORED},
		{EVENT_US1_LU1_COMM_RESTORED, EVENT_US2_LU1_COMM_RESTORED, EVENT_DS1_LU1_COMM_RESTORED, EVENT_DS2_LU1_COMM_RESTORED,
		 EVENT_US1_LU2_COMM_RESTORED, EVENT_US2_LU2_COMM_RESTORED, EVENT_DS1_LU2_COMM_RESTORED, EVENT_DS2_LU2_COMM_RESTORED},
		{EVENT_PD1_NORMAL, EVENT_PD2_NORMAL, EVENT_PEER_CPU_NORMAL, EVENT_PEER_CPU_COMM_RESTORED,
		 EVENT_VITAL_RELAY_A_NORMAL, EVENT_VITAL_RELAY_B_NORMAL, EVENT_PREP_RELAY_A_NORMAL, EVENT_PREP_RELAY_B_NORMAL},
		{EVENT_MODEM_A_NORMAL, EVENT_MODEM_B_NORMAL, EVENT_US_DAC_NORMAL, EVENT_DS_DAC_NORMAL,
		 EVENT_US1_POWER_RESTORED, EVENT_US2_POWER_RESTORED, EVENT_DS1_POWER_RESTORED, EVENT_DS2_POWER_RESTORED}
		};

/* Event Detection Table - when Status Bit changed from High to Low*/
const event_id_t Event_When_Low[NO_OF_EVENT_REGISTERS][EVENTS_PER_REGISTER] = {
		{EVENT_US_BLOCK_OCCUPIED, EVENT_DS_BLOCK_OCCUPIED, EVENT_NOT_OCCURRED, EVENT_SYSTEM_DEFECTIVE,
		 EVENT_NOT_OCCURRED, EVENT_NOT_OCCURRED, EVENT_NOT_OCCURRED, EVENT_NOT_OCCURRED},
		{EVENT_BOARDS_MISSING, EVENT_PD1_BOARD_MISSING, EVENT_PD2_BOARD_MISSING, EVENT_MODEM_BOARD_A_MISSING,
		 EVENT_MODEM_BOARD_B_MISSING, EVENT_RELAY_BOARD_A_MISSING, EVENT_RELAY_BOARD_B_MISSING, EVENT_PEER_CPU_MISSING},
		{EVENT_LU1_US1_COMM_FAILURE, EVENT_LU1_US2_COMM_FAILURE, EVENT_LU1_DS1_COMM_FAILURE, EVENT_LU1_DS2_COMM_FAILURE,
		 EVENT_LU2_US1_COMM_FAILURE, EVENT_LU2_US2_COMM_FAILURE, EVENT_LU2_DS1_COMM_FAILURE, EVENT_LU2_DS2_COMM_FAILURE},
		{EVENT_US1_LU1_COMM_FAILURE, EVENT_US2_LU1_COMM_FAILURE, EVENT_DS1_LU1_COMM_FAILURE, EVENT_DS2_LU1_COMM_FAILURE,
		 EVENT_US1_LU2_COMM_FAILURE, EVENT_US2_LU2_COMM_FAILURE, EVENT_DS1_LU2_COMM_FAILURE, EVENT_DS2_LU2_COMM_FAILURE},
		{EVENT_PD1_FAILED, EVENT_PD2_FAILED, EVENT_PEER_CPU_FAILED, EVENT_PEER_CPU_COMM_FAILURE,
		 EVENT_VITAL_RELAY_A_FAILED, EVENT_VITAL_RELAY_B_FAILED, EVENT_PREP_RELAY_A_FAILED, EVENT_PREP_RELAY_B_FAILED},
		{EVENT_MODEM_A_ERROR, EVENT_MODEM_B_ERROR, EVENT_US_DAC_FAILED, EVENT_DS_DAC_FAILED,
		 EVENT_US1_POWER_FAILED, EVENT_US2_POWER_FAILED, EVENT_DS1_POWER_FAILED, EVENT_DS2_POWER_FAILED}
		};

extern host_sch_info_t   Host_Sch_Info;				/* from comm_host.c */
events_sch_info_t Events_Sch_Info;		/* Events Scheduler Information */	

/* All detected new events is stored in Event Queue. Then Event Scheduler reads events from Queue 
   and store into serial EEPROM	*/
events_queue_t    Events_Queue;			
										
/* Ring Buffer maintains Pointers (Tail and Head pointer)on Events EEPROM Memory.
   whenever new event is written into EEPROM Tail is Incremented.
   whenever previous event Token is modified with "OLD_EVENT_TOKEN" Head is Incremented.*/
ring_buffer_t     Events_Ring_Buffer;

event_register_t  Events[MAXIMUM_NO_OF_CPU]; /* it holds the Previous Status of Both Cpu's */



events_sch_state_t Inspect_First_Event(void);
void Build_Event_Queue_Record(BYTE, event_id_t);
void Add_to_Event_Queue(BYTE, event_id_t, UINT16, UINT16, UINT16 , UINT16);
BOOL Save_DAC_Event(void);
void Synchronise_Events_Register(void);
extern BOOL Send_Event_to_TAB;
extern event_record_t Latest_event;

/*********************************************************************************
File name 			:events.c
Function Name		:UINT16 Get_Current_Event_Number(void)
Created By			:Sudheer Herle
Date Created		:
Modification History:
					Rev No			Date		Description
					 --				 --				--
Tracability:
		SRS()    	:

Abstract			: Returns the Last updated Event Number
Algorithm			:
Description			: 
Input Element		:None
Output Element		:return Events_Ring_Buffer.Tail number.

**********************************************************************************/
UINT16 Get_Current_Event_Number(void)
{
	return (Events_Ring_Buffer.Tail);
}

/*********************************************************************************
File name 			:events.c
Function Name		:BYTE Get_Events_Counter_Value(void)
Created By			:Sudheer Herle
Date Created		:
Modification History:
					Rev No			Date		Description
					 --				 --				--
Tracability:
		SRS()    	:

Abstract			:Returns the Number of Events Occured
Algorithm			:
Description			: 
Input Element		:None
Output Element		:return Events_Ring_Buffer.Count value

**********************************************************************************/

BYTE Get_Events_Counter_Value(void)
{
	return (Events_Ring_Buffer.Count);
}

/*********************************************************************************
File name 			:events.c
Function Name		:void Clear_Events_Counter(void)
Created By			:Sudheer Herle
Date Created		:
Modification History:
					Rev No			Date		Description
					 --				 --				--
Tracability:
		SRS()    	:

Abstract			: Clears the Event Counter Variable. (Events_Ring_Buffer.Count)
Algorithm			:
Description			: 
Input Element		:None
Output Element		:void

**********************************************************************************/
void Clear_Events_Counter(void)
{
	Events_Ring_Buffer.Count = 0;
}

/*********************************************************************************
File name 			:events.c
Function Name		:BOOL Setup_Events_Erasure(void)
Created By			:Sudheer Herle
Date Created		:
Modification History:
					Rev No			Date		Description
					 --				 --				--
Tracability:
		SRS()    	:

Abstract			:It setup the Event scheduler and Event Queue ready, to Erase all the  
					 Events in serial EEPROM.  
Algorithm			:
Description			: 
Input Element		:None
Output Element		:returns the status of Setup_Events_Erasure function

**********************************************************************************/
BOOL Setup_Events_Erasure(void)
{
	BOOL bReturnValue = FALSE;
	if (Events_Sch_Info.State < EVENTS_SCHEDULER_IDLE)
	{
		/* SM Cpu Initialisation itself not completed, so it can't accept ERASE Command 
		   i.e still Events scheduler is searching the Head and Tail Pointer in serial EEPROM */ 
		return (bReturnValue);
	}
	if (Status.Flags.Erasure_in_Progress_P != SET_LOW ||
		Status.Flags.Erasure_in_Progress_N != SET_HIGH)
		{
		/* Erasure already in progress */
		return (bReturnValue);
		}
	bReturnValue = TRUE;	
	
	/* Stop Event Logging during Erase Time */
	Status.Flags.Inhibit_Logging_P = SET_HIGH;
	Status.Flags.Inhibit_Logging_N = SET_LOW;
	Status.Flags.Erasure_in_Progress_P = SET_HIGH;
	Status.Flags.Erasure_in_Progress_N = SET_LOW;

	Events_Sch_Info.Block_Number = 0;                /* Start Erase from Block No 0 */
	Events_Sch_Info.Next_State = ERASE_EEPROM_BLOCK; /* Set Next state of Events Scheduler to ERASE_EEPROM_BLOCK */

	/*
	 * Clear the event queue
	 */
	Events_Queue.FrontIndex = 0;
	Events_Queue.RearIndex  = 0;

	/*
	 * Clear the circular buffer
	 */
	Events_Ring_Buffer.Head  = 0;
	Events_Ring_Buffer.Tail  = 0;
	Events_Ring_Buffer.Count = 0;

	/*
	 * Initialise Event registers to default values
	 */
	Events[0].Byte[0] = (BYTE) EVENT_REGISTER1_DEFAULT;
	Events[0].Byte[1] = (BYTE) EVENT_REGISTER2_DEFAULT;
	Events[0].Byte[2] = (BYTE) EVENT_REGISTER3_DEFAULT;
	Events[0].Byte[3] = (BYTE) EVENT_REGISTER4_DEFAULT;
	Events[0].Byte[4] = (BYTE) EVENT_REGISTER5_DEFAULT;
	Events[0].Byte[5] = (BYTE) EVENT_REGISTER6_DEFAULT;
	Events[1].Byte[0] = (BYTE) EVENT_REGISTER1_DEFAULT;
	Events[1].Byte[1] = (BYTE) EVENT_REGISTER2_DEFAULT;
	Events[1].Byte[2] = (BYTE) EVENT_REGISTER3_DEFAULT;
	Events[1].Byte[3] = (BYTE) EVENT_REGISTER4_DEFAULT;
	Events[1].Byte[4] = (BYTE) EVENT_REGISTER5_DEFAULT;
	Events[1].Byte[5] = (BYTE) EVENT_REGISTER6_DEFAULT;
	/* Save the Event Registers of Both Cpu on EEPROM */  
	Save_Event_Register_on_EEPROM(CPU1_ID, (event_register_t *)&Events[0]);
	Save_Event_Register_on_EEPROM(CPU2_ID, (event_register_t *)&Events[1]);

	return (bReturnValue);
}

/*********************************************************************************
File name 			:events.c
Function Name		:void Initialise_Events_Sch(void)
Created By			:Sudheer Herle
Date Created		:
Modification History:
					Rev No			Date		Description
					 --				 --				--
Tracability:
		SRS()    	:

Abstract			:Initialises the Event Scheduler
Algorithm			:
Description			: 
Input Element		:None
Output Element		:void

**********************************************************************************/
void Initialise_Events_Sch(void)
{
	/*
	 * Initialise the Scheduler
	 */
	Events_Sch_Info.State = INSPECT_FIRST_EVENT;
	Events_Sch_Info.Timeout_10ms = 0;
	Events_Sch_Info.Record_No = 1;

	/*
	 * Initialise the event queues
	 */
	Events_Queue.FrontIndex = 0;
	Events_Queue.RearIndex = 0;

	/*
	 * Initialise the circular buffer
	 */
	Events_Ring_Buffer.Head  = 0;
	Events_Ring_Buffer.Tail  = 0;
	Events_Ring_Buffer.Count = 0;	
}

/*********************************************************************************
File name 			:events.c
Function Name		:void Start_Events_Sch(void)
Created By			:Sudheer Herle
Date Created		:
Modification History:
					Rev No			Date		Description
					 --				 --				--
Tracability:
		SRS()    	:

Abstract			:This function starts Events scheduler and Enable Data Logging.
Algorithm			:
Description			: 
Input Element		:None
Output Element		:void

**********************************************************************************/
void Start_Events_Sch(void)
{
	Events_Sch_Info.State = EVENTS_SCHEDULER_IDLE; /* Set Events Scheduler info state to EVENTS_SCHEDULER_IDLE */
	Events_Sch_Info.Timeout_10ms = 0;			   /* Set Events Scheduler  info timeout to zero */
	Status.Flags.Inhibit_Logging_P = SET_LOW;
	Status.Flags.Inhibit_Logging_N = SET_HIGH;
	Status.Flags.Erasure_in_Progress_P = SET_LOW;
	Status.Flags.Erasure_in_Progress_N = SET_HIGH;
}

/*********************************************************************************
File name 			:events.c
Function Name		:events_sch_state_t Inspect_First_Event(void)
Created By			:Sudheer Herle
Date Created		:
Modification History:
					Rev No			Date		Description
					 --				 --				--
Tracability:
		SRS()    	:

Abstract			: It Reads the First Event at 0th Location of Serial EEPROM and sets the
					  Events Scheduler next State. 	
Algorithm			:
Description			: 
Input Element		:None
Output Element		:events_sch_state_t

**********************************************************************************/
events_sch_state_t Inspect_First_Event(void)
{
	event_record_t Event_Record;
	BYTE uchBuf=0;
	events_sch_state_t Next_State;

	for (uchBuf = 0; uchBuf <	EVENT_RECORD_SIZE; uchBuf++)
	{
		Event_Record.Byte[uchBuf] = 0; /* Clears the Event_Record Bytes*/
	}

	Next_State = SYNCHRONISE_EVENT_REGISTERS;		/* Default state */

	Read_Event_from_Serial_EEProm(0, (event_record_t *) &Event_Record);
//	Read_Event_from_Serial_EEProm(1, (event_record_t *) &Event_Record);

	if (Event_Record.Field.Token != LATEST_EVENT_TOKEN && Event_Record.Field.Token != OLD_EVENT_TOKEN)
	{
		/* No Events Recorded in Serial EEPROM */
		return (Next_State);
	}

	uchBuf = Compute_LRC(Event_Record.Byte, 15);
	if (Event_Record.Field.LRC != uchBuf)
	{
		Status.Flags.MEM_Corrupted = TRUE;		/* Checksum does not match */
		return (Next_State);
	}

	if (Event_Record.Field.Token == LATEST_EVENT_TOKEN)
	{
		Next_State = LOCATE_LAST_TOKEN_IN_CW_DIR;  /* Search Last Token from Up to Down Direction */ 
	}
	else
	{
		Next_State = LOCATE_FIRST_TOKEN_IN_CCW_DIR;	/* Search First Token from Down to Up Direction */
	}
	return (Next_State);
}

/*********************************************************************************
File name 			:events.c
Function Name		:void Update_Events_Sch_State(void)
Created By			:Sudheer Herle
Date Created		:
Modification History:
					Rev No			Date		Description
					 --				 --				--
Tracability:
		SRS()    	:

Abstract			:This Function handles Event Scheduler. 

Algorithm			: 
					1.Event Scheduler will be in "INSPECT_FIRST_EVENT" state at Power ON Condtion.
					2.It will read First Record from Serial EEPROM.
					3.Deponds on the Token Avilable in First Record, Scheduler will select the Search Path.
					  *.If Token Found is LATEST_EVENT_TOKEN,  						
						and State changed to "LOCATE_LAST_TOKEN_IN_CW_DIR" State. 
					  *.If Token Found is not LATEST_EVENT_TOKEN,  
						and State changed to "LOCATE_FIRST_TOKEN_IN_CCW_DIR" State. 
					4.In "LOCATE_LAST_TOKEN_IN_CW_DIR" State Records are read from Top to Down.
					  Each Record Token is Checked for Not Equal to "Latest Event Token"	
					  If "Old Event Token" found , Record No of that Token is Set to Tail Pointer.
					  and State changed to "LOCATE_LAST_TOKEN_IN_CCW_DIR" state.
					5.In "LOCATE_LAST_TOKEN_IN_CCW_DIR" State Records are read Down to Up.
					  Each Record Token is Checked for Not Equal to "Latest Event Token"	
					  If "old Event Token" found, Record No of that Token is Set to Head Pointer.
					  and State changed to "SYNCHRONISE_EVENT_REGISTERS" state.		
					6.In "LOCATE_FIRST_TOKEN_IN_CCW_DIR" State Records are read from Down to up.
					  Each Record Token is Checked for Equal to "Latest Event Token"	
					  If "Latest Event Token" found ,Record No of that Token is Set to Tail Pointer.
					  and State changed to "LOCATE_FIRST_TOKEN_IN_CW_DIR" state.
					7.In "LOCATE_FIRST_TOKEN_IN_CW_DIR" State Records are read Up to Down.
					  Each Record Token is Checked for Equal to "Latest Event Token"	
					  If "Latest Event Token" found, Record No of that Token is Set to Head Pointer.
					  and State changed to "SYNCHRONISE_EVENT_REGISTERS" state.
					8.In "SYNCHRONISE_EVENT_REGISTERS" ,Events Register of Both Cpus are assigned to
					  the CPU_Event Register value Read from EEPROM.Then Set Scheduler to Idle State.
					9.In Idle State, it check for any new event available in Queue.If new events are 
					  available then  save into Serial EEPROM , scheduler goto "SAVE_EVENTS_ON_EEPROM".
					  If Scheduler next state is "ERASE_EEPROM_BLOCK", then Scheduler state changed 
					  to "ERASE_EEPROM_BLOCK" State.
					10.In "SAVE_EVENTS_ON_EEPROM" State Record by Record all events with "Latest Event Token"
					   are Stored into serial EEPROM with regular intervals. Once all Records are Saved 
					   into EEPROM, Scheduler State changed to "MODIFY_OLD_EVENTS_TOKEN" State.
					11.In this "MODIFY_OLD_EVENTS_TOKEN" State Each Record is read from Head pointer and 
					   its Token is changed "Old Event Token". Then Save back the Events in the Same Location.
					   Like this All records except final one are changed to "Old Event Token" 
					   This process get completed, when Head Pointer will less by one compared with Tail Pointer.
					   Then Scheduler goto Idle State.
					12.In "ERASE_EEPROM_BLOCK" State, EEPROM Block is Erased Block by Block with Regular interval.
						Once All Blocks are Erased, Scheduler goto Idle State with Event Logging Enabled.			
Description			: 
Input Element		:None
Output Element		:void

**********************************************************************************/
extern volatile char inspect_event_done;
extern UINT16 Page;
extern wordtype_t Erase_address;
extern EEPROM_Erase_State Block_erase_status;
unsigned char Erase_Start;
void Update_Events_Sch_State(void)
{
	event_record_t Event_Record;

	switch (Events_Sch_Info.State)
	{
            case EVENTS_SCHEDULER_NOT_STARTED:
                break;
		case INSPECT_FIRST_EVENT:
			Events_Sch_Info.State = Inspect_First_Event();
                        
			if (Events_Sch_Info.State == LOCATE_LAST_TOKEN_IN_CW_DIR)
			{
				Events_Sch_Info.Record_No = 0;
			}
			else
			{
				if (Events_Sch_Info.State == LOCATE_FIRST_TOKEN_IN_CCW_DIR)
				{
					Events_Sch_Info.Record_No = (MAXIMUM_EVENTS - 1);
				}
			}
			break;
		case LOCATE_LAST_TOKEN_IN_CW_DIR:
			Read_Event_from_Serial_EEProm((UINT16) Events_Sch_Info.Record_No, (event_record_t *) &Event_Record);
			Events_Ring_Buffer.Tail = Events_Sch_Info.Record_No;
			Events_Sch_Info.Record_No = Events_Sch_Info.Record_No + 1;
			if (Event_Record.Field.Token != LATEST_EVENT_TOKEN || Events_Sch_Info.Record_No >= MAXIMUM_EVENTS)
			{
				Events_Sch_Info.State = LOCATE_LAST_TOKEN_IN_CCW_DIR;
				Events_Sch_Info.Record_No = (MAXIMUM_EVENTS - 1);
			}
			break;
		case LOCATE_LAST_TOKEN_IN_CCW_DIR:
			Read_Event_from_Serial_EEProm((UINT16) Events_Sch_Info.Record_No, (event_record_t *) &Event_Record);
			Events_Ring_Buffer.Head = Events_Sch_Info.Record_No;
			Events_Sch_Info.Record_No = Events_Sch_Info.Record_No - 1;
			if (Event_Record.Field.Token != LATEST_EVENT_TOKEN || Events_Sch_Info.Record_No < 0)
			{
				Events_Sch_Info.State = SYNCHRONISE_EVENT_REGISTERS;
			}
			break;
		case LOCATE_FIRST_TOKEN_IN_CCW_DIR:
			Read_Event_from_Serial_EEProm((UINT16) Events_Sch_Info.Record_No, (event_record_t *) &Event_Record);
			Events_Ring_Buffer.Tail = Events_Sch_Info.Record_No;
			Events_Sch_Info.Record_No = Events_Sch_Info.Record_No - 1;
			if (Event_Record.Field.Token == LATEST_EVENT_TOKEN)
			{
				Events_Sch_Info.State = LOCATE_FIRST_TOKEN_IN_CW_DIR;
				Events_Sch_Info.Record_No = 1;
				break;
			}
			if (Events_Sch_Info.Record_No < 0)
			{
				Events_Sch_Info.State = LOCATE_FIRST_TOKEN_IN_CW_DIR;
				Events_Sch_Info.Record_No = 1;
				Events_Ring_Buffer.Tail = 0;
			}
			break;
		case LOCATE_FIRST_TOKEN_IN_CW_DIR:
			Read_Event_from_Serial_EEProm((UINT16) Events_Sch_Info.Record_No, (event_record_t *) &Event_Record);
			Events_Ring_Buffer.Head = Events_Sch_Info.Record_No;
			Events_Sch_Info.Record_No = Events_Sch_Info.Record_No + 1;
			if (Event_Record.Field.Token == LATEST_EVENT_TOKEN)
			{
				Events_Ring_Buffer.Tail = (Events_Ring_Buffer.Tail + 1) % MAXIMUM_EVENTS;
				Events_Sch_Info.State = SYNCHRONISE_EVENT_REGISTERS;

				break;
			}
			if (Events_Sch_Info.Record_No >= MAXIMUM_EVENTS)
			{
				Events_Ring_Buffer.Tail = (Events_Ring_Buffer.Tail + 1) % MAXIMUM_EVENTS;
				Events_Sch_Info.State = SYNCHRONISE_EVENT_REGISTERS;
				Events_Ring_Buffer.Head = 0;
			}
			break;
		case SYNCHRONISE_EVENT_REGISTERS:
			Synchronise_Events_Register();
			Start_Events_Sch();
                        inspect_event_done = 1;
			break;
		case EVENTS_SCHEDULER_IDLE:
			if (Events_Sch_Info.Next_State == ERASE_EEPROM_BLOCK)
			{
				Build_End_EEPROM_Erase_Reply_Command();
				Host_Sch_Info.State      = TRANSMIT_SINGLE_RECORD_FOR_HOST;
		//		Host_Sch_Info.State = HOST_COMM_SCHEDULER_IDLE;
				Events_Sch_Info.State = Events_Sch_Info.Next_State;
				break;
			}
			if (Events_Queue.FrontIndex != Events_Queue.RearIndex)
			{
				/*
				 * DAC event queue is not empty
				 */
				Events_Sch_Info.State = SAVE_EVENTS_ON_EEPROM;
			}

			break;
		case SAVE_EVENTS_ON_EEPROM:
			if (Save_DAC_Event() == TRUE)
			{
				/* ONE Event saved */
				Events_Sch_Info.State = SAVE_EVENTS_ON_EEPROM_WAIT;
				Events_Sch_Info.Timeout_10ms = SERIAL_EEPROM_TWC_TIMEOUT;
			}
			else
			{
				/* No more events to save */
				Events_Sch_Info.State = MODIFY_OLD_EVENTS_TOKEN;
			}
			break;
		case SAVE_EVENTS_ON_EEPROM_WAIT:
			if (Is_Serial_EEPROM_Write_Complete())
			{
				Events_Sch_Info.State = SAVE_EVENTS_ON_EEPROM;
				break;
			}
			if (Events_Sch_Info.Timeout_10ms == TIMEOUT_EVENT)
			{
				Events_Sch_Info.State = SAVE_EVENTS_ON_EEPROM;
			}
			break;
		case MODIFY_OLD_EVENTS_TOKEN:
			if (Modify_Token_of_old_Events())
			{
				/*
				 * Token of oldest Event modified, probably more to follow
				 */
				Events_Sch_Info.State = MODIFY_OLD_EVENTS_TOKEN_WAIT;
				Events_Sch_Info.Timeout_10ms = SERIAL_EEPROM_TWC_TIMEOUT;
			}
			else
			{
				/*
				 * All tokens of old Events modified
				 */
				Events_Sch_Info.State = EVENTS_SCHEDULER_IDLE;
				Events_Sch_Info.Timeout_10ms = 0;
			}
			break;
		case MODIFY_OLD_EVENTS_TOKEN_WAIT:
			if (Is_Serial_EEPROM_Write_Complete())
			{
				Events_Sch_Info.State = MODIFY_OLD_EVENTS_TOKEN;
				break;
			}
			if (Events_Sch_Info.Timeout_10ms == TIMEOUT_EVENT)
			{
				Events_Sch_Info.State = MODIFY_OLD_EVENTS_TOKEN;
			}
			break;
		case ERASE_EEPROM_BLOCK:
                    Erase_Start = 1;
			Erase_Serial_EEProm_Block(Events_Sch_Info.Block_Number);
                        if(Block_erase_status == START)
                        {
        			Events_Sch_Info.Block_Number+=1;
                        }
			Events_Sch_Info.State = ERASE_EEPROM_BLOCK_WAIT;
			Events_Sch_Info.Timeout_10ms = SERIAL_EEPROM_TWC_TIMEOUT;
			break;
		case ERASE_EEPROM_BLOCK_WAIT:
			if (Is_Serial_EEPROM_Write_Complete() || 
				Events_Sch_Info.Timeout_10ms == TIMEOUT_EVENT)
			{
				if (Events_Sch_Info.Block_Number < MAXIMUM_BLOCKS)
				{
					Events_Sch_Info.State = ERASE_EEPROM_BLOCK;
				}
				else
				{
					Status.Flags.Inhibit_Logging_P = SET_LOW;
					Status.Flags.Inhibit_Logging_N = SET_HIGH;
					Status.Flags.Erasure_in_Progress_P = SET_LOW;
					Status.Flags.Erasure_in_Progress_N = SET_HIGH;
					Build_End_EEPROM_Erase_Reply_Command(); 
					Host_Sch_Info.State      = TRANSMIT_SINGLE_RECORD_FOR_HOST;
					Host_Sch_Info.Next_State = HOST_COMM_SCHEDULER_IDLE;
					Events_Sch_Info.State = EVENTS_SCHEDULER_IDLE;
					Events_Sch_Info.Timeout_10ms = 0;
					Events_Sch_Info.Next_State = EVENTS_SCHEDULER_NOT_STARTED;
				}
			}
			break;
	}
}

/*********************************************************************************
File name 			:events.c
Function Name		:void Decrement_Events_Sch_10msTmr(void)
Created By			:Sudheer Herle
Date Created		:
Modification History:
					Rev No			Date		Description
					 --				 --				--
Tracability:
		SRS()    	:

Abstract			:For every 10ms,Events_Sch_Info.Timeout variable decremented.
Algorithm			:
Description			: 
Input Element		:None
Output Element		:void

**********************************************************************************/

void Decrement_Events_Sch_10msTmr(void)
{
	if (Events_Sch_Info.Timeout_10ms > 0)
	{
		Events_Sch_Info.Timeout_10ms = Events_Sch_Info.Timeout_10ms - 1;
	}
}

/*********************************************************************************
File name 			:events.c
Function Name		:void Add_SM_Event_to_Queue(event_id_t uchEventId)
Created By			:Sudheer Herle
Date Created		:
Modification History:
					Rev No			Date		Description
					 --				 --				--
Tracability:
		SRS()    	:

Abstract			:This function add sm event to event queue
Algorithm			:
Description			: 
Input Element		:uchEventId
Output Element		:void

**********************************************************************************/
void Add_SM_Event_to_Queue(event_id_t uchEventId)
{
	Events_Queue.RearIndex = (Events_Queue.RearIndex + 1) % EVENTS_QUEUE_SIZE;
	/* SM event. Not related to any DAC CPU */
	Events_Queue.Data[Events_Queue.RearIndex].Source_Id = 0xFF;			/* SM ID - 0xFF */
	Events_Queue.Data[Events_Queue.RearIndex].Event_Id = uchEventId;
	Events_Queue.Data[Events_Queue.RearIndex].US_Fwd_AxleCount = 0;
	Events_Queue.Data[Events_Queue.RearIndex].US_Rev_AxleCount = 0;
	Events_Queue.Data[Events_Queue.RearIndex].DS_Fwd_AxleCount = 0;
	Events_Queue.Data[Events_Queue.RearIndex].DS_Rev_AxleCount = 0;
}

/*********************************************************************************
File name 			:events.c
Function Name		:void Build_Event_Queue_Record(BYTE uchCPU, event_id_t uchEventId)
Created By			:Sudheer Herle
Date Created		:
Modification History:
					Rev No			Date		Description
					 --				 --				--
Tracability:
		SRS()    	:

Abstract			: It Builds the Event Reco
 * 
 * rd to store into Queue
Algorithm			:
Description			: 
Input Element		:uchCPU,uchEventId
Output Element		:void

**********************************************************************************/
void Build_Event_Queue_Record(BYTE uchCPU, event_id_t uchEventId)
{
	wordtype_t US_Fwd_Count;
	wordtype_t US_Rev_Count;
	wordtype_t DS_Fwd_Count;
	wordtype_t DS_Rev_Count;
	BYTE Cpu_Address;
	

	if (uchCPU != CPU1_ID && uchCPU != CPU2_ID)
	{
		return;		/* out of range */
	}
	/* Initialise all the counts to ZEROs */
	US_Fwd_Count.Word = 0;
	US_Rev_Count.Word = 0;
	DS_Fwd_Count.Word = 0;
	DS_Rev_Count.Word = 0;

	if (uchCPU == CPU1_ID)
		{
		if (CPU1_data[MESSAGE_TYPE_OFFSET] == READ_AXLE_COUNT)
			{
			/* Collect Cpu1 All counts to Store into Event Record */
			US_Fwd_Count.Byte.Lo = CPU1_data[US_FWD_COUNT_OFFSET];
			US_Fwd_Count.Byte.Hi = CPU1_data[US_FWD_COUNT_OFFSET + 1];
			US_Rev_Count.Byte.Lo = CPU1_data[US_REV_COUNT_OFFSET];
			US_Rev_Count.Byte.Hi = CPU1_data[US_REV_COUNT_OFFSET + 1];
			DS_Fwd_Count.Byte.Lo = CPU1_data[DS_FWD_COUNT_OFFSET];
			DS_Fwd_Count.Byte.Hi = CPU1_data[DS_FWD_COUNT_OFFSET + 1];
			DS_Rev_Count.Byte.Lo = CPU1_data[DS_REV_COUNT_OFFSET];
			DS_Rev_Count.Byte.Hi = CPU1_data[DS_REV_COUNT_OFFSET + 1];
			Cpu_Address = CPU1_Address;
			}
		if (CPU1_data[MESSAGE_TYPE_OFFSET] == READ_RESET_INFO)
			{
			 Cpu_Address = CPU1_Address;
			}
		}
	else
		{
		if (CPU2_data[MESSAGE_TYPE_OFFSET] == READ_AXLE_COUNT)
			{
			/* Collect Cpu1 All counts to Store into Event Record */
			US_Fwd_Count.Byte.Lo = CPU2_data[US_FWD_COUNT_OFFSET];
			US_Fwd_Count.Byte.Hi = CPU2_data[US_FWD_COUNT_OFFSET + 1];
			US_Rev_Count.Byte.Lo = CPU2_data[US_REV_COUNT_OFFSET];
			US_Rev_Count.Byte.Hi = CPU2_data[US_REV_COUNT_OFFSET + 1];
			DS_Fwd_Count.Byte.Lo = CPU2_data[DS_FWD_COUNT_OFFSET];
			DS_Fwd_Count.Byte.Hi = CPU2_data[DS_FWD_COUNT_OFFSET + 1];
			DS_Rev_Count.Byte.Lo = CPU2_data[DS_REV_COUNT_OFFSET];
			DS_Rev_Count.Byte.Hi = CPU2_data[DS_REV_COUNT_OFFSET + 1];
			Cpu_Address = CPU2_Address;
			}
		if (CPU2_data[MESSAGE_TYPE_OFFSET] == READ_RESET_INFO)
			{
			 Cpu_Address = CPU2_Address;
			}
		}
	/* This Function adds the Event to Event Queue */
	Add_to_Event_Queue(Cpu_Address, uchEventId, US_Fwd_Count.Word, US_Rev_Count.Word, DS_Fwd_Count.Word, DS_Rev_Count.Word);
}

/*********************************************************************************
File name 			:events.c
Function Name		:void Add_to_Event_Queue(BYTE uchCPU, event_id_t uchEventId, UINT16 uiUSFwdCount, UINT16 uiUSRevCount, UINT16 uiDSFwdCount, UINT16 uiDSRevCount)
Created By			:Sudheer Herle
Date Created		:
Modification History:
					Rev No			Date		Description
					 --				 --				--
Tracability:
		SRS()    	:

Abstract			:This Function adds the Event to Event Queue
Algorithm			:
Description			: 
Input Element		:uchCPU,uchEventId,uiUSFwdCount,uiUSRevCount,uiDSFwdCount,uiDSRevCount
Output Element		:void

**********************************************************************************/
void Add_to_Event_Queue(BYTE Cpu_Address, event_id_t uchEventId, UINT16 uiUSFwdCount, UINT16 uiUSRevCount, UINT16 uiDSFwdCount, UINT16 uiDSRevCount)
{
	if (Cpu_Address != CPU1_Address && Cpu_Address != CPU2_Address)
	{
		return;		/* out of range */
	}

	Events_Queue.RearIndex = (Events_Queue.RearIndex + 1) % EVENTS_QUEUE_SIZE;
	Events_Queue.Data[Events_Queue.RearIndex].Source_Id = Cpu_Address;				/* Source Id */
	Events_Queue.Data[Events_Queue.RearIndex].Event_Id  = uchEventId;			/* Event Id */
	Events_Queue.Data[Events_Queue.RearIndex].US_Fwd_AxleCount = uiUSFwdCount;	/* US-Fwd Count */
	Events_Queue.Data[Events_Queue.RearIndex].US_Rev_AxleCount = uiUSRevCount;	/* US-Rev Count */
	Events_Queue.Data[Events_Queue.RearIndex].DS_Fwd_AxleCount = uiDSFwdCount;	/* DS-Fwd Count */
	Events_Queue.Data[Events_Queue.RearIndex].DS_Rev_AxleCount = uiDSRevCount;  /* DS-Rev Count */
}

/*********************************************************************************
File name 			:events.c
Function Name		:void Detect_DAC_Events(BYTE uchCPU, event_register_t *New_Event)
Created By			:Sudheer Herle
Date Created		:
Modification History:
					Rev No			Date		Description
					 --				 --				--
Tracability:
		SRS()    	:

Abstract			: Detects the New Events Occured by Comparing Event Register with Shadow Register.
Algorithm			:
Description			: 
Input Element		:uchCPU,New_Event
Output Element		:void

**********************************************************************************/
extern unsigned char log_event,System_error,System_error_code;
void Detect_DAC_Events(BYTE uchCPU, event_register_t *New_Event)
{
	BYTE uchCpuId, i, j;
	bitadrb_t OldData, NewData;
	event_id_t Event_Id;
	BYTE uchCnt = 0;

	if (uchCPU != CPU1_ID && uchCPU != CPU2_ID)
	{
		return;		/* out of range */
	}
	uchCpuId = (uchCPU - 1);

	if (Status.Flags.Inhibit_Logging_P == SET_HIGH && Status.Flags.Inhibit_Logging_N == SET_LOW)
	{
		return;		/* Data logging is INHIBITED, no point in detecting events */
	}
	/* Event detection */
	for (i = 0; i < 6; i++)
	{
		OldData.Byte = Events[uchCpuId].Byte[i];	/* Read one Byte From Events Register */
		NewData.Byte = New_Event->Byte[i];			/* Read one Byte From New Events Register */
		for (j = 0; j < 8; j++)
		{
			if (OldData.Bit.b0 != NewData.Bit.b0)
			{
				/* New Event */
				if (NewData.Bit.b0 == SET_HIGH)
				{
					/* New Bit Changed from Low to High */
					Event_Id = Event_When_High[i][j];
				}
				else
				{
					/* New Bit Changed from High to Low */
					Event_Id = Event_When_Low[i][j];
				}
                                log_event = 1;
                                if(Event_Id==EVENT_NOT_OCCURRED || Event_Id==EVENT_SYSTEM_DEFECTIVE || Event_Id==EVENT_BOARDS_MISSING || Event_Id==EVENT_ALL_BOARDS_FOUND || Event_Id==EVENT_PEER_CPU_MISSING || Event_Id == EVENT_US_DAC_FAILED || Event_Id==EVENT_US_DAC_NORMAL || Event_Id == EVENT_DS_DAC_FAILED || Event_Id==EVENT_DS_DAC_NORMAL)
                                {
                                    log_event = 0;
                                }
                                if(Event_Id==EVENT_US_DAC_FAILED || Event_Id==EVENT_DS_DAC_FAILED)
                                {
                                    log_event = 1;
                                }
                                if(Event_Id == EVENT_PD1_FAILED || Event_Id == EVENT_PD2_FAILED)
                                {
                                    switch(System_error_code)
                                    {
                                        case 28:
                                            Event_Id = EVENT_PD_PULSE_MISMATCH;
                                            break;
                                        case 29:
                                            Event_Id = EVENT_BOOTUP_PD_FAILED;
                                            break;
                                        case 31:
                                            Event_Id = EVENT_PD1_AND_PD2_PULSATING;
                                            break;
                                        case 32:
                                            Event_Id = EVENT_PD1_SUP_LOW;
                                            break;
                                        case 33:
                                            Event_Id = EVENT_PD2_SUP_LOW;
                                            break;
                                        case 34:
                                            Event_Id = EVENT_PD1_PULSATING;
                                            break;
                                        case 35:
                                            Event_Id = EVENT_PD2_PULSATING;
                                            break;
                                        case 36:
                                            Event_Id = EVENT_PD_STATE_MISSING;
                                            break;
                                        case 37:
                                            Event_Id = EVENT_PD_SUP_PULSATING;
                                            break;
                                        case 38:
                                            Event_Id = EVENT_STATE_FAIL;
                                            break;
                                        case 39:
                                            Event_Id = EVENT_PD_NOT_DETECTING;
                                            break;
                                        case 45:
                                            Event_Id = EVENT_PD_SUP_MISSING;
                                            break;
                                        default:
                                            break;
                                    }
                                }
				if (log_event)
				{
					uchCnt = uchCnt + 1;
					Build_Event_Queue_Record(uchCPU, Event_Id);
				}
			}
			OldData.Byte = (OldData.Byte >> 1);
			NewData.Byte = (NewData.Byte >> 1);
			}
		/* Update Event register with Shadow register */
		Events[uchCpuId].Byte[i] = New_Event->Byte[i];
	}
	if (uchCnt > 0)
	{
		/* Save the Event register to on chip EEPROM */
		Save_Event_Register_on_EEPROM(uchCPU, (event_register_t *)&Events[uchCpuId]);
	}
}

/*********************************************************************************
File name 			:events.c
Function Name		:BOOL Save_DAC_Event(void)
Created By			:Sudheer Herle
Date Created		:
Modification History:
					Rev No			Date		Description
					 --				 --				--
Tracability:
		SRS()    	:

Abstract			: This Function reads events from Events Queue and Store into Serial EEPROM 
Algorithm			:
Description			: 
Input Element		:None
Output Element		:BOOL - Status of Save_DAC_Events()

**********************************************************************************/
BOOL Save_DAC_Event(void)
{
	event_record_t Event_Record;
	static int count = 0;	

	if (Events_Queue.FrontIndex == Events_Queue.RearIndex)
	{
		/*
		 * DAC event queue is empty or all events written on to Serial EEPROM		 */
		
		return (FALSE);
	}
	Events_Queue.FrontIndex = (Events_Queue.FrontIndex + 1) % EVENTS_QUEUE_SIZE;
	Event_Record.Field.Token = LATEST_EVENT_TOKEN;       /* Token is Updated */
	Event_Record.Field.Date_Time = SystemClock;			 /* System Clock is updated */
	Event_Record.Field.Source_Id = Events_Queue.Data[Events_Queue.FrontIndex].Source_Id; /* Source Id */ 
	Event_Record.Field.Event_Id = Events_Queue.Data[Events_Queue.FrontIndex].Event_Id;	 /* Event Id */
	Event_Record.Field.US_Fwd_AxleCount = Events_Queue.Data[Events_Queue.FrontIndex].US_Fwd_AxleCount;	/* US-Fwd Count */
	Event_Record.Field.US_Rev_AxleCount = Events_Queue.Data[Events_Queue.FrontIndex].US_Rev_AxleCount;	/* US-Rev Count */
	Event_Record.Field.DS_Fwd_AxleCount = Events_Queue.Data[Events_Queue.FrontIndex].DS_Fwd_AxleCount;	/* DS-Fwd Count */
	Event_Record.Field.DS_Rev_AxleCount = Events_Queue.Data[Events_Queue.FrontIndex].DS_Rev_AxleCount;	/* DS-Rev Count */	
	Event_Record.Field.LRC = Compute_LRC(Event_Record.Byte, 15);	/* computed LRc Value */
        Send_Event_to_TAB = TRUE;
        Latest_event = Event_Record;
        /*
	 * Write the event on to Serial EEPROM
	 */
	if (Write_Event_on_Serial_EEProm(Events_Ring_Buffer.Tail, (event_record_t *) &Event_Record) == TRUE)
	{
		/*
		 * Increment the tail pointer of Ring Buffer
		 */	
		if(Event_Record.Field.Event_Id == 0x43)
		{
		count++;
		}			
		Events_Ring_Buffer.Tail = (Events_Ring_Buffer.Tail + 1) % MAXIMUM_EVENTS;
		Events_Ring_Buffer.Count = Events_Ring_Buffer.Count + 1;
		return (TRUE);
	}
	else
	{
		return (FALSE); /* Write failed */
	}
}

/*********************************************************************************
File name 			:events.c
Function Name		:BOOL Modify_Token_of_old_Events(void)
Created By			:Sudheer Herle
Date Created		:
Modification History:
					Rev No			Date		Description
					 --				 --				--
Tracability:
		SRS()    	:

Abstract			:It modifies the token of previous events to "OLD EVENT" TOKEN
Algorithm			:
Description			: 
Input Element		:None
Output Element		:Returns the status of modify token of old events

**********************************************************************************/

BOOL Modify_Token_of_old_Events(void)
{
	event_record_t Event_Record;

	/*
	 * We have to remove LATEST_EVENT_TOKEN and replace with OLD_EVENT_TOKEN
	 * for all records except the most recent one. Write cycle time Twc is 
	 * 5 milli-seconds, which means we have to space our writes by atleast 5 ms.
	 * Hence, we remove token of only one record per function call allowing
	 * the calling function to space the delay between writes.
	 */

	if (Events_Ring_Buffer.Head == Events_Ring_Buffer.Tail)
	{
		/*
		 * Queue is empty		
		 */		
		return (BOOL) FALSE;
	}
	if (Events_Ring_Buffer.Tail > Events_Ring_Buffer.Head)
	{
		if ((Events_Ring_Buffer.Tail - Events_Ring_Buffer.Head) > 1)
		{
			if (Read_Event_from_Serial_EEProm(Events_Ring_Buffer.Head, (event_record_t *) &Event_Record) == FALSE)
			{
				return (BOOL) FALSE;	/* Read failed */
			}
			Event_Record.Field.Token = OLD_EVENT_TOKEN;  /* Change the Token to "OLD_EVENT_TOKEN" */
			Event_Record.Field.LRC = Compute_LRC(Event_Record.Byte, 15);
			if (Write_Event_on_Serial_EEProm(Events_Ring_Buffer.Head, (event_record_t *) &Event_Record) == FALSE)
			{
				return (BOOL) FALSE;	/* Write failed */
			}		
			Events_Ring_Buffer.Head = (Events_Ring_Buffer.Head + 1) % MAXIMUM_EVENTS;
		}
		else
		{
			/*
			 * no more tokens to replace
			 */
			return (BOOL) FALSE;
		}
	}
	else
	{
		/*
		 * Events_Ring_Buffer.Tail has rolled over
		 */
		if ((Events_Ring_Buffer.Tail + (MAXIMUM_EVENTS - Events_Ring_Buffer.Head)) > 1)
		{
			if (Read_Event_from_Serial_EEProm(Events_Ring_Buffer.Head, (event_record_t *) &Event_Record) == FALSE)
			{
				return (BOOL) FALSE;	/* Read failed */
			}
			Event_Record.Field.Token = OLD_EVENT_TOKEN;	  /* Change the Token to "OLD_EVENT_TOKEN" */
			Event_Record.Field.LRC = Compute_LRC(Event_Record.Byte, 15);
			if (Write_Event_on_Serial_EEProm(Events_Ring_Buffer.Head, (event_record_t *) &Event_Record) == FALSE)
			{
				return (BOOL) FALSE;	/* Write failed */
			}
			Events_Ring_Buffer.Head = (Events_Ring_Buffer.Head + 1) % MAXIMUM_EVENTS;
		}
		else
		{
			/*
			 * no more tokens to replace
			 */
			return (BOOL) FALSE;
		}
	}
	/*
	 * 1 token replaced.
	 */
	return (BOOL) TRUE;
}


/*********************************************************************************
File name 			:events.c
Function Name		:BYTE Compute_LRC(BYTE *puchMsg, BYTE uchMsgLen)
Created By			:Sudheer Herle
Date Created		:
Modification History:
					Rev No			Date		Description
					 --				 --				--
Tracability:
		SRS()    	:

Abstract			:compute LRC value for the Message Buffer.
Algorithm			:
Description			: 
Input Element		:*puchMsg,uchMsgLen
Output Element		:BYTE - Computed LRC Value 

**********************************************************************************/

BYTE Compute_LRC(BYTE *puchMsg, BYTE uchMsgLen)
{
	BYTE uchLRC = 0;
	BYTE uchCheck = 0;

	while (uchMsgLen)					/* pass through message buffer */
	{
	   uchMsgLen = uchMsgLen - 1;
	   uchCheck = *puchMsg ;	
	   uchLRC += ++uchCheck;				/* add without carry */
	   puchMsg = puchMsg + 1;	
	}
	return ((BYTE)(-((SCHAR)uchLRC)));	/* return two's complement */
}

/*********************************************************************************
File name 			:events.c
Function Name		:void Synchronise_Events_Register(void)
Created By			:Sudheer Herle
Date Created		:
Modification History:
					Rev No			Date		Description
					 --				 --				--
Tracability:
		SRS()    	:

Abstract			: Read the Event Register Values From On chip EEPROM and Copies to Event Register.
Algorithm			:
Description			: 
Input Element		:None
Output Element		:void

**********************************************************************************/
void Synchronise_Events_Register(void)
{
	BYTE i=0;
	BYTE *pEEPROM_data;

	if (Is_CPU1_EEPROM_Record_Valid())/* Check whether CPU1 Event Register Stored is Vaild or Not */
	{
		/* Load value from on-chip EEPROM */
		pEEPROM_data = Get_CPU1_EEPROM_Record(); 
		for (i = 0; i < NO_OF_EVENT_REGISTERS; i++)
		{
			/* Copies the values read from on chip EEPROM to Event Register  */
			Events[0].Byte[i] = *pEEPROM_data; 
			pEEPROM_data = pEEPROM_data + 1;
		}
	}
	else
	{
		/*CPU1 Event Register Stored on On-chip EEPROM is Not Vaild,
		  So copy the Default values to Event Register */
		Events[0].Byte[0] = (BYTE) EVENT_REGISTER1_DEFAULT;
		Events[0].Byte[1] = (BYTE) EVENT_REGISTER2_DEFAULT;
		Events[0].Byte[2] = (BYTE) EVENT_REGISTER3_DEFAULT;
		Events[0].Byte[3] = (BYTE) EVENT_REGISTER4_DEFAULT;
		Events[0].Byte[4] = (BYTE) EVENT_REGISTER5_DEFAULT;
		Events[0].Byte[5] = (BYTE) EVENT_REGISTER6_DEFAULT;
	}
	if (Is_CPU2_EEPROM_Record_Valid())/* Check whether CPU2 Event Register Stored is Vaild or Not */
	{
		/* Load value from on-chip EEPROM */
		pEEPROM_data = Get_CPU2_EEPROM_Record();
		for (i = 0; i < NO_OF_EVENT_REGISTERS; i++)
		{
			/* Copies the values read from on chip EEPROM to Event Register  */
			Events[1].Byte[i] = *pEEPROM_data;
			pEEPROM_data = pEEPROM_data + 1;
		}
	}
	else
	{   
		/*CPU2 Event Register Stored on On-chip EEPROM is Not Vaild,
		  So copy the Default values to Event Register */
		Events[1].Byte[0] = (BYTE) EVENT_REGISTER1_DEFAULT;
		Events[1].Byte[1] = (BYTE) EVENT_REGISTER2_DEFAULT;
		Events[1].Byte[2] = (BYTE) EVENT_REGISTER3_DEFAULT;
		Events[1].Byte[3] = (BYTE) EVENT_REGISTER4_DEFAULT;
		Events[1].Byte[4] = (BYTE) EVENT_REGISTER5_DEFAULT;
		Events[1].Byte[5] = (BYTE) EVENT_REGISTER6_DEFAULT;
	}
}
