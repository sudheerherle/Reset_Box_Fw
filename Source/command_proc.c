/*********************************************************************************************************************
	Project		: 	Single Section Digital Axle Counter
	Version		: 	2.0 
	Revision	:	1	
	Filename	: 	comm_dac.c
	Target MCU	: 	PIC24FJ256GB210   
    Compiler	: 	XC16 Compiler V1.21  
	Author		:	Sudheer Herle
	Date		:	
	Company Name: 	Insys Digital Systems
	Modification History:
					Rev No			Date		Description
					 --				 --				--    
	Functions	:   void Process_Host_Command(host_recv_msg_info_t *Received_data,BYTE Selected_Port)
					void Setup_Get_Logged_Events_Params(void)
					void Build_Next_Logged_Event_Reply(void)
					void Build_Time_Record_Message(void)
					void Build_Event_Record_Message(void)
					void Notify_End_of_Events_Reply(void)
					void Process_Set_Date_And_Time_Command(void)
					void Build_Reply_To_Erase_Command(void)
					void Build_Reply_To_Status_Command(void)
			
*********************************************************************************************************************/
#include <xc.h>
#include <time.h>
#include <limits.h>

#include "COMMON.H"
#include "command_proc.h"
#include "comm_host.h"
#include "COMM_SMC.H"
#include "DRV_MEM.H"
#include "EVENTS.H"
#include "CRC16.H"

extern dac_sysinfo_t DAC_sysinfo;
extern time_t SystemClock;						/* from cpu_sm.c */
extern sm_status_t Status;						/* from cpu_sm.c */
extern event_register_t Shadow[MAXIMUM_NO_OF_CPU];	/* comm_dac.c	 */
extern BYTE CPU1_Address;						/* from cpu_sm.c */
extern BYTE CPU2_Address;						/* from cpu_sm.c */

										 /* Message Length Table for Different Commands from PC/SMC */	 
												   /* D     T	  G	    E     S     R */		
const BYTE uchCommand_Length_Table[8][2] = {
    {0,6},{1,11},{2,4},{3,4},{4,4},{5,5},{6,4},{7,4}
										   };    
extern host_sch_info_t   Host_Sch_Info;				/* from comm_host.c */			
query_param_t     		 Query_Param;				/* this structure holds query param information */
event_record_t    	     Event_Record;				/* this structure holds event record information */
xmit_queue_info_t 		 Xmit_Queue;				/* this structure holds Xmit queue information */
BYTE  Command_Info[12];								/* received data from host command info */ 
extern UINT16 Page;
extern wordtype_t Erase_address;
BOOL Send_Event_to_TAB;
event_record_t Latest_event;
extern EEPROM_Erase_State Block_erase_status;
smc_info_t Smc1XmitObject;
smc_info_t Correct_data;
void Setup_Get_Logged_Events_Params(void);
void Process_Set_Date_And_Time_Command(void);
void Build_Time_Record_Message(void);
void Build_Event_Record_Message(void);
void Build_Reply_To_Erase_Command(void);
void Build_Reply_To_Status_Command(void);
void Build_Reply_To_Get_80bytes_Command(void);
void Build_Reply_To_Send_event_Command(void);

/*********************************************************************************
File name 			:comm_dac.c
Function Name		:void Process_Host_Command(host_recv_msg_info_t *Received_data,BYTE Selected_Port)
Created By			:Sudheer Herle
Date Created		:
Modification History:
					Rev No			Date		Description
					 --				 --				--
Tracability:
		SRS()    	:

Abstract			: Process the command from  Host and Station Master Unit.
Algorithm			:
					 *.SM external Interface is designed to Service one command at a time either
					   from Host or Smc. When SM-CPU  is servicing one, the command comes from 
					   other side is rejected.	  					
Description			: 
Input Element		:Received_data - Received from host or smc 
					 Selected_Port - Either comm1(FSK-smc) or comm2(Host)
Output Element		:void

**********************************************************************************/
void Process_Host_Command(host_recv_msg_info_t *Received_data,BYTE Selected_Port)
{

	Command_Info[HOST_ADDR_OFFSET]      = Received_data->Msg_Buffer[HOST_ADDR_OFFSET];	
	if( Command_Info[HOST_ADDR_OFFSET] != CPU1_Address &&
		Command_Info[HOST_ADDR_OFFSET] != CPU2_Address)
	{
		/* The command received is not concern to this unit */
		//return;
	}
	Command_Info[HOST_COMMAND_OFFSET] = Received_data->Msg_Buffer[HOST_COMMAND_OFFSET];

	if (Status.Flags.Query_Pending == TRUE && Command_Info[HOST_COMMAND_OFFSET] != REPLY_TO_RECORDS )
	{	
	  /* Pervious Query is not Completed. So it won't take New Commands */	
      return;		
	}	

	switch (Command_Info[HOST_COMMAND_OFFSET])
	{	
		case SET_RTC_DATE_AND_TIME:			
			   /* Collect the Date and Time Information send by Host / SMC to Command Buffer*/	
			   Command_Info[HOST_DATA_BYTE1_OFFSET] = Received_data->Msg_Buffer[HOST_DATA_BYTE1_OFFSET];	
			   Command_Info[HOST_DATA_BYTE2_OFFSET] = Received_data->Msg_Buffer[HOST_DATA_BYTE2_OFFSET];	
			   Command_Info[HOST_DATA_BYTE3_OFFSET] = Received_data->Msg_Buffer[HOST_DATA_BYTE3_OFFSET];	
			   Command_Info[HOST_DATA_BYTE4_OFFSET] = Received_data->Msg_Buffer[HOST_DATA_BYTE4_OFFSET];	
			   Command_Info[HOST_DATA_BYTE5_OFFSET] = Received_data->Msg_Buffer[HOST_DATA_BYTE5_OFFSET];	
			   Command_Info[HOST_DATA_BYTE6_OFFSET] = Received_data->Msg_Buffer[HOST_DATA_BYTE6_OFFSET];	
			   Command_Info[HOST_DATA_BYTE7_OFFSET] = Received_data->Msg_Buffer[HOST_DATA_BYTE7_OFFSET];					 
			   Process_Set_Date_And_Time_Command();   /* This Function executes the Date and Time Updation */  
 			   Build_Time_Record_Message();			  /* Build the Date and Time Reply to unit who has send the Query */ 		
			  /* Reply from Host */	
			  Host_Sch_Info.State      = TRANSMIT_SINGLE_RECORD_FOR_HOST;
			  Host_Sch_Info.Next_State = HOST_COMM_SCHEDULER_IDLE;		
			break;		
		case GET_RTC_DATE_TIME:
			 Build_Time_Record_Message();	/* Build the Date and Time Reply to unit who has send the Query */ 		
			  /* Reply from Host */		
		   Host_Sch_Info.State      = TRANSMIT_SINGLE_RECORD_FOR_HOST;
		   Host_Sch_Info.Next_State = HOST_COMM_SCHEDULER_IDLE;		
			break;
		case GET_LOGGED_EVENTS:
			 Command_Info[HOST_DATA_BYTE1_OFFSET] = Received_data->Msg_Buffer[HOST_DATA_BYTE1_OFFSET]; /* No of Days Events Required  */
			 Command_Info[HOST_DATA_BYTE2_OFFSET] = Received_data->Msg_Buffer[HOST_DATA_BYTE2_OFFSET]; /* Mode Selection Async / Sync */	
			 Setup_Get_Logged_Events_Params();  /*it determines no of Events to be Transmitted */							  
			/* Reply to Host */	
			   Xmit_Queue.Record_Count = 0;	 
			   Xmit_Queue.Record_Index = 0;	
			   Host_Sch_Info.Tx_USB_Logged_events = 1;
               Host_Sch_Info.State      = BUILD_RECORDS_FOR_HOST;
			   Host_Sch_Info.Next_State = HOST_COMM_SCHEDULER_IDLE;							   
			break;
		case REPLY_TO_RECORDS:				
			/* This command is received when CRC Corruption Occured at Receiving End - Station Master Unit */
			 Xmit_Queue.Record_Rx_Status = Received_data->Msg_Buffer[HOST_DATA_BYTE1_OFFSET];
			/* Reply from Host */	
			if(Xmit_Queue.Record_Rx_Status == ALL_RECORDS_SUCCESSFULL)
		 	{
			   /* All records are received at Host Successfully 
				  So, go for Building of Next Batch of Records */		
			   Xmit_Queue.Record_Count = 0;	 
			   Xmit_Queue.Record_Index = 0;
			   Host_Sch_Info.State      = BUILD_RECORDS_FOR_HOST;
		       Host_Sch_Info.Next_State = HOST_COMM_SCHEDULER_IDLE;		
		 	}
			else
			{
			  /* Some Records received at Host corrupted, So for Retransmission of those Records */	
			   Host_Sch_Info.Timeout_ms = 0;
			   Host_Sch_Info.State      = PROCESS_HOST_REPLY_COMMAND;
		       Host_Sch_Info.Next_State = HOST_COMM_SCHEDULER_IDLE;			
			}			   
			 break;				
		case ERASE_EVENTS_EEPROM:		
			if(Setup_Events_Erasure())  /* setup the Event Logger for ERASE Operation */
			{
                            Page = 0;
                            Erase_address.Word = 0;
                            Block_erase_status = START;
				/* Erase Command Accepted, So Send a Reply to Command Sender */
			    Build_Reply_To_Erase_Command();
				/* Reply to Host */	
			  Host_Sch_Info.State      = TRANSMIT_SINGLE_RECORD_FOR_HOST;
			  Host_Sch_Info.Next_State = HOST_COMM_SCHEDULER_IDLE;		
			 }									
            break;
		case GET_DAC_STATUS:
			Build_Reply_To_Status_Command();	/* Build Dac Status for Reply to Get_Dac_status Command */
			/* Reply to Host */	
			  Host_Sch_Info.State      = TRANSMIT_SINGLE_RECORD_FOR_HOST;
			  Host_Sch_Info.Next_State = HOST_COMM_SCHEDULER_IDLE;		
			break;
                case GET_EVENT_COUNTS:
			Build_Reply_To_Get_Event_Command();	/* Build Dac Status for Reply to Get_Dac_status Command */
//                        Build_Reply_To_Get_80bytes_Command();
			/* Reply to Host */
			  Host_Sch_Info.State      = TRANSMIT_SINGLE_RECORD_FOR_HOST;
			  Host_Sch_Info.Next_State = HOST_COMM_SCHEDULER_IDLE;
			break;
                case GET_80_BYTES:
                    if(Send_Event_to_TAB == TRUE){
                        Build_Reply_To_Send_event_Command();
                        Send_Event_to_TAB = FALSE;
                    }else{
                      Build_Reply_To_Get_80bytes_Command();	/* Build Dac Status for Reply to Get_Dac_status Command */
			/* Reply to Host */
                      Host_Sch_Info.State      = TRANSMIT_SINGLE_RECORD_FOR_HOST;
                      Host_Sch_Info.Next_State = HOST_COMM_SCHEDULER_IDLE;
                    }
                    break;
	}
}

/*********************************************************************************
File name 			:comm_dac.c
Function Name		:void Setup_Get_Logged_Events_Params(void)
Created By			:Sudheer Herle
Date Created		:
Modification History:
					Rev No			Date		Description
					 --				 --				--
Tracability:
		SRS()    	:

Abstract			: Sets The Parameter reading Events From Serial EEPROM
					  Parameters :No of Events to be Retrived, Read Pointer,Read Count,Record Count  	
Algorithm			:
Description			: 
Input Element		:None
Output Element		:void

**********************************************************************************/
void Setup_Get_Logged_Events_Params(void)
{
	time_t time_tmp;
	struct tm *tp;
	
	/* By making query pending status to True,
	 other Commands are rejected unless this status is set to False */ 								
	Status.Flags.Query_Pending = TRUE;

	Query_Param.Max_Permitted_Reads = MAXIMUM_EVENTS;
	Query_Param.Read_Count   = 0;
	
	if ((Get_Current_Event_Number()-1)>MAXIMUM_EVENTS)
		{
			Query_Param.Read_Pointer=0;
		}
	else
		{
			Query_Param.Read_Pointer = Get_Current_Event_Number()-1;
		}	
	Query_Param.uchIs_Next_Available = (BOOL) TRUE;
	Xmit_Queue.Mode = 0xFF;     /* Transmission Mode Async / Sync */
    Xmit_Queue.Record_Count = 0;
    Xmit_Queue.Record_Index = 0;						
	Clear_Events_Counter();										/* from events.c */

	/* first data byte contains number of days of data to be retreived */
	if (Command_Info[HOST_DATA_BYTE1_OFFSET] != 0 &&
        Command_Info[HOST_DATA_BYTE1_OFFSET] != ALL_EVENTS )
	{
		tp = localtime(&SystemClock);                          /* Get the Current Time from  System Clock */
		tp->tm_hour = 0;
		tp->tm_min  = 0;
		tp->tm_sec  = 0;
		/*Make time to 00:00:00. From this time No of Days are Substracted*/	
		time_tmp = mktime(tp);	              /* Get the Time in Seconds Format */          
		/* TimeStamp_SetPoint = time_tmp - (No of Days * Seconds per Day) */	
		Query_Param.TimeStamp_SetPoint = ( time_tmp - 
								(Command_Info[HOST_DATA_BYTE1_OFFSET]* SECONDS_PER_DAY));
	}
	else
	{
		/* All the events has to be transferred. Set maximum possible date */
		Query_Param.TimeStamp_SetPoint = (long) 0 ;
	}
}

/*********************************************************************************
File name 			:comm_dac.c
Function Name		:void Build_Next_Logged_Event_Reply(void)
Created By			:Sudheer Herle
Date Created		:
Modification History:
					Rev No			Date		Description
					 --				 --				--
Tracability:
		SRS()    	:

Abstract			: Read the Events from Serial EEPROM and Builds the Xmit Queue
Algorithm			:
Description			: 
Input Element		:None
Output Element		:void

**********************************************************************************/
void Build_Next_Logged_Event_Reply(void)
{
	BYTE uchBuf=0;


	/* Ignore any new event which has occured after GET_LOGGED_EVENTS command */
	uchBuf = Get_Events_Counter_Value();        /* returns No of events Occured */
	Query_Param.Max_Permitted_Reads -= uchBuf;  
	Clear_Events_Counter();						/* from events.c */

	if(Query_Param.Read_Count >= Query_Param.Max_Permitted_Reads) /* Check Whether All events are read from Memory */
	{
  	  Notify_End_of_Events_Reply();		/* Build End of Events Reply */					
	  Query_Param.uchIs_Next_Available = (BOOL) FALSE;	
	  return;
	}
	/* Fetch the data */
	if(Read_Event_from_Serial_EEProm(Query_Param.Read_Pointer, (event_record_t *) &Event_Record) == FALSE)
	{
		return;	/* Read failed */
	}
	uchBuf = Compute_LRC(Event_Record.Byte, 15);

	/* Update Read Pointer */
	if(Query_Param.Read_Pointer <= 0)
	{
       Query_Param.Read_Pointer = MAXIMUM_EVENTS - 1;
	}
	else
	{		
	   Query_Param.Read_Pointer = Query_Param.Read_Pointer - 1;	
	}

	Query_Param.Read_Count = Query_Param.Read_Count + 1;	   /* Increment Read Count */

	if (Event_Record.Field.LRC != uchBuf)
	{
		/* Checksum does not match. Record corrupted */
			/* Record read was out of Time Stamp, So No need to Transmit the record */
		 	 Notify_End_of_Events_Reply();		/* Build End of Events Reply */
			 Query_Param.uchIs_Next_Available = FALSE;
                         return;
	}
	if (Event_Record.Field.Token == LATEST_EVENT_TOKEN || 
		Event_Record.Field.Token == OLD_EVENT_TOKEN)
	{		
		if (Event_Record.Field.Date_Time >= Query_Param.TimeStamp_SetPoint)
		{				 
			 Build_Event_Record_Message();	/* Build the Record in Xmit_Queue for Transmission */			 	
		}
		else
		{
			/* Record read was out of Time Stamp, So No need to Transmit the record */		
		 	 Notify_End_of_Events_Reply();		/* Build End of Events Reply */		
			 Query_Param.uchIs_Next_Available = FALSE;			
		}
	}

}

/*********************************************************************************
File name 			:comm_dac.c
Function Name		:void Build_Time_Record_Message(void)
Created By			:Sudheer Herle
Date Created		:
Modification History:
					Rev No			Date		Description
					 --				 --				--
Tracability:
		SRS()    	:

Abstract			: Build Date and Time Message Record 
Algorithm			:
Description			: 
Input Element		:None
Output Element		:void

**********************************************************************************/
void Build_Time_Record_Message(void)
{
	wordtype_t CheckSum;
	long_t L_Time;
	BYTE *puchMsg;
 
	L_Time.LWord = SystemClock;                    /* Read the System Clock */
	puchMsg  = & Xmit_Queue.Record[0].Msg_Buffer[0];
	*(puchMsg)    = CPU1_Address;				   /* Put the Source Id - Address of CPU1 */	
	*(puchMsg+1)  = DATE_TIME_REPLY;			   /* Command Id */	
	*(puchMsg+2)  = L_Time.Byte.Byte1; 			   /* System Time in sec -4 Bytes */	
	*(puchMsg+3)  = L_Time.Byte.Byte2;
	*(puchMsg+4)  = L_Time.Byte.Byte3;
	*(puchMsg+5)  = L_Time.Byte.Byte4;
	CheckSum.Word = Crc16(puchMsg,6);				/* Calculate CRC 16 for Xmit Buffer */
	*(puchMsg+6)  = CheckSum.Byte.Lo;				/* CRC low Byte */ 
	*(puchMsg+7)  = CheckSum.Byte.Hi;				/* CRC High Byte */
	*(puchMsg+8)  = 0x0D;  							/* Add CR */
	*(puchMsg+9)  = 0x0A;  							/* Add LF */
	Xmit_Queue.Record[0].Host_Msg_Length = 10;  
	Xmit_Queue.Record[0].Smc_Msg_Length = 8;  
	Xmit_Queue.Index = 0;
	Xmit_Queue.Record_Index =0;
    Xmit_Queue.Record_Count =1;	
	Status.Flags.Query_Pending = FALSE;
}

/*********************************************************************************
File name 			:comm_dac.c
Function Name		:void Build_Event_Record_Message(void)
Created By			:Sudheer Herle
Date Created		:
Modification History:
					Rev No			Date		Description
					 --				 --				--
Tracability:
		SRS()    	:

Abstract			:Build Event  Message Record 
Algorithm			:
Description			: 
Input Element		:None
Output Element		:void

**********************************************************************************/
void Build_Event_Record_Message(void)
{
	wordtype_t CheckSum;
	BYTE *puchMsg;


	if(Xmit_Queue.Record_Index >= MAX_XMIT_QUEUE_SIZE)
	{
	   Xmit_Queue.Record_Index = 0;
	}
	puchMsg = & Xmit_Queue.Record[Xmit_Queue.Record_Index].Msg_Buffer[0];
	*(puchMsg)      = Event_Record.Byte[5];		/* Source Id   */
	*(puchMsg+1)   	= LOGGED_EVENTS_REPLY;		/* Command  */	
	*(puchMsg+2)    = Event_Record.Byte[6];		/* Event Id	   */												
	*(puchMsg+3)    = Event_Record.Byte[1];		/* System Clock 4 Bytes */
	*(puchMsg+4)    = Event_Record.Byte[2];
	*(puchMsg+5)    = Event_Record.Byte[3];
	*(puchMsg+6)    = Event_Record.Byte[4];		
	*(puchMsg+7)    = Event_Record.Byte[7];		/* US FWD Axle Count 2 Bytes */
	*(puchMsg+8)    = Event_Record.Byte[8];	
	*(puchMsg+9)   = Event_Record.Byte[9];		/* US REV Axle Count 2 Bytes */
	*(puchMsg+10)   = Event_Record.Byte[10];
	*(puchMsg+11)   = Event_Record.Byte[11];	/* DS FWD Axle Count 2 Bytes */
	*(puchMsg+12)   = Event_Record.Byte[12];
	*(puchMsg+13)   = Event_Record.Byte[13];	/* DS REV Axle Count 2 Bytes */
	*(puchMsg+14)   = Event_Record.Byte[14];
	CheckSum.Word   = Crc16(puchMsg,15);		/* Calculate CRC 16 for Xmit Buffer */
	*(puchMsg+15)   = CheckSum.Byte.Lo;			/* CRC low Byte */ 
	*(puchMsg+16)   = CheckSum.Byte.Hi;			/* CRC high Byte */ 
	*(puchMsg+17)   = 0x0D;  /* Add CR */
	*(puchMsg+18)   = 0x0A;  /* Add LF */
	Xmit_Queue.Record[Xmit_Queue.Record_Index].Host_Msg_Length = 19;  
	Xmit_Queue.Record[Xmit_Queue.Record_Index].Smc_Msg_Length  = 17;  
	Xmit_Queue.Index  = 0;
	if(Xmit_Queue.Record_Count >= MAX_XMIT_QUEUE_SIZE)
	{
	  Xmit_Queue.Record_Index = 0;
	}
	else
	{
	  Xmit_Queue.Record_Index = Xmit_Queue.Record_Index + 1;
	}
    Xmit_Queue.Record_Count = Xmit_Queue.Record_Count + 1;	
}

/*********************************************************************************
File name 			:comm_dac.c
Function Name		:void Notify_End_of_Events_Reply(void)
Created By			:Sudheer Herle
Date Created		:
Modification History:
					Rev No			Date		Description
					 --				 --				--
Tracability:
		SRS()    	:

Abstract			:Build End of Events Reply
Algorithm			:
Description			: 
Input Element		:None
Output Element		:void

**********************************************************************************/
void Notify_End_of_Events_Reply(void)
{
	wordtype_t CheckSum;
	BYTE *puchMsg;
 
	if(Xmit_Queue.Record_Index >= MAX_XMIT_QUEUE_SIZE)
	{
	   Xmit_Queue.Record_Index = 0;
	}
	puchMsg = & Xmit_Queue.Record[Xmit_Queue.Record_Index].Msg_Buffer[0];	
	*(puchMsg)    = CPU1_Address;				/* Source Id   */
	*(puchMsg+1)  = END_OF_EVENT_REPLY;			/* Command  */
	CheckSum.Word = Crc16(puchMsg, 2);			/* Calculate CRC 16 for Xmit Buffer */
	*(puchMsg+2)  = CheckSum.Byte.Lo;			/* CRC low Byte */ 
	*(puchMsg+3)  = CheckSum.Byte.Hi;			/* CRC high Byte */ 
	*(puchMsg+4)  = 0x0D;  						/* Add CR */
	*(puchMsg+5)  = 0x0A;  						/* Add LF */
	Xmit_Queue.Record[Xmit_Queue.Record_Index].Host_Msg_Length = 6;  
	Xmit_Queue.Record[Xmit_Queue.Record_Index].Smc_Msg_Length = 4;  
	Xmit_Queue.Index = 0;
	if( Xmit_Queue.Record_Count >= MAX_XMIT_QUEUE_SIZE)
	{
		Xmit_Queue.Record_Index =0;
	}
	else
	{
		Xmit_Queue.Record_Index = Xmit_Queue.Record_Index + 1;
	}
    Xmit_Queue.Record_Count = Xmit_Queue.Record_Count + 1;
}

/*********************************************************************************
File name 			:comm_dac.c
Function Name		:void Process_Set_Date_And_Time_Command(void)
Created By			:Sudheer Herle
Date Created		:
Modification History:
					Rev No			Date		Description
					 --				 --				--
Tracability:
		SRS()    	:

Abstract			:This Function executes the Date and Time Updation
Algorithm			:
Description			: 
Input Element		:None
Output Element		:void

**********************************************************************************/
void Process_Set_Date_And_Time_Command(void)
{
	wordtype_t year;
	BYTE month,date;
	BYTE hour,minute,sec;

	year.Byte.Lo = 	Command_Info[HOST_DATA_BYTE1_OFFSET];
	year.Byte.Hi = 	Command_Info[HOST_DATA_BYTE2_OFFSET];
    month 		 =  Command_Info[HOST_DATA_BYTE3_OFFSET];
    date 		 =  Command_Info[HOST_DATA_BYTE4_OFFSET];
    hour 		 =  Command_Info[HOST_DATA_BYTE5_OFFSET];
    minute 		 =  Command_Info[HOST_DATA_BYTE6_OFFSET];
    sec 		 =  Command_Info[HOST_DATA_BYTE7_OFFSET];

	if(Set_RTC_Date(date,month,year.Word))
	  {
	    /*This function is used to change day, Month and Year of Real Time Clock IC */
	    if(Set_RTC_Time(hour,minute,sec))
		   { 		/*This function is used to change hour, Minute and Sec of Real Time Clock IC */
			Add_SM_Event_to_Queue((event_id_t) EVENT_DATE_TIME_CHANGED); /* DATE_TIME_Changed event is added to Event Queue */
		   }
	  }
}

/*********************************************************************************
File name 			:comm_dac.c
Function Name		:void Build_Reply_To_Erase_Command(void)
Created By			:Sudheer Herle
Date Created		:
Modification History:
					Rev No			Date		Description
					 --				 --				--
Tracability:
		SRS()    	:

Abstract			:Build erase command reply 
Algorithm			:
Description			: 
Input Element		:None
Output Element		:void

**********************************************************************************/
void Build_Reply_To_Erase_Command(void)
{
	wordtype_t CheckSum;
	BYTE *puchMsg;
  

	puchMsg = & Xmit_Queue.Record[0].Msg_Buffer[0];
	*(puchMsg)    = CPU1_Address;				/* Source Id   */
	*(puchMsg+1)  = EEPROM_ERASED_REPLY;		/* Command  */
	CheckSum.Word = Crc16(puchMsg, 2);			/* Calculate CRC 16 for Xmit Buffer */
	*(puchMsg+2)  = CheckSum.Byte.Lo;			/* CRC low Byte */ 
	*(puchMsg+3)  = CheckSum.Byte.Hi;			/* CRC high Byte */ 
	*(puchMsg+4)  = 0x0D;  						/* Add CR */
	*(puchMsg+5)  = 0x0A;  						/* Add LF */
	Xmit_Queue.Record[0].Host_Msg_Length = 6;  
	Xmit_Queue.Record[0].Smc_Msg_Length = 4;  
	Xmit_Queue.Index = 0;
    Xmit_Queue.Record_Count =1;
	Xmit_Queue.Record_Index =0;	
//	Status.Flags.Query_Pending = FALSE;
}

/*********************************************************************************
File name 			:comm_dac.c
Function Name		:Build_End_EEPROM_Erase_Reply_Command(void)
Created By			:Sudheer Herle
Date Created		:
Modification History:
					Rev No			Date		Description
					 --				 --				--
Tracability:
		SRS()    	:

Abstract			:Build erase command reply 
Algorithm			:
Description			: 
Input Element		:None
Output Element		:void

**********************************************************************************/
void Build_End_EEPROM_Erase_Reply_Command(void)
{
	wordtype_t CheckSum;
	BYTE *puchMsg;
  

	puchMsg = & Xmit_Queue.Record[0].Msg_Buffer[0];
	*(puchMsg)    = CPU1_Address;						/* Source Id   */
	*(puchMsg+1)  = END_EEPROM_ERASED_REPLY;			/* Command  */
	CheckSum.Word = Crc16(puchMsg, 2);					/* Calculate CRC 16 for Xmit Buffer */
	*(puchMsg+2)  = CheckSum.Byte.Lo;					/* CRC low Byte */ 
	*(puchMsg+3)  = CheckSum.Byte.Hi;					/* CRC high Byte */ 
	*(puchMsg+4)  = 0x0D;  								/* Add CR */
	*(puchMsg+5)  = 0x0A;  								/* Add LF */
	Xmit_Queue.Record[0].Host_Msg_Length = 6;  
	Xmit_Queue.Record[0].Smc_Msg_Length = 4;  
	Xmit_Queue.Index = 0;
    Xmit_Queue.Record_Count =1;
	Xmit_Queue.Record_Index =0;	
	Status.Flags.Query_Pending = FALSE;
}

/*********************************************************************************
File name 			:comm_dac.c
Function Name		:void Build_Reply_To_Status_Command(void)
Created By			:Sudheer Herle
Date Created		:
Modification History:
					Rev No			Date		Description
					 --				 --				--
Tracability:
		SRS()    	:

Abstract			:Build status command reply
Algorithm			:
Description			: 
Input Element		:None
Output Element		:void

**********************************************************************************/
void Build_Reply_To_Status_Command(void)
{
	wordtype_t CheckSum;
	BYTE *puchMsg;
  

	puchMsg = & Xmit_Queue.Record[0].Msg_Buffer[0];
		
	*(puchMsg)    = CPU1_Address;		/* Source Id   */
	*(puchMsg+1)  = DAC_STATUS_REPLY;					/* Command  */
        *(puchMsg+2)  = DAC_sysinfo.Unit_Type;
	if(Command_Info[HOST_ADDR_OFFSET] == CPU1_Address)
	{	
		*(puchMsg+3)  = (BYTE) Shadow[0].Id.US_DAC;		/* US unit status */
		*(puchMsg+4)  = (BYTE) Shadow[0].Id.DS_DAC;		/* DS unit status */
		*(puchMsg+5)  = (BYTE) Shadow[0].Id.US_Block;   /* US Track_status */
		*(puchMsg+6)  = (BYTE) Shadow[0].Id.DS_Block;	/* DS Track_status */
	}
	else
	{
		*(puchMsg+3)  = (BYTE) Shadow[1].Id.US_DAC;		/* US unit status */
		*(puchMsg+4)  = (BYTE) Shadow[1].Id.DS_DAC;		/* DS unit status */
		*(puchMsg+5)  = (BYTE) Shadow[1].Id.US_Block;	/* US Track_status */
		*(puchMsg+6)  = (BYTE) Shadow[1].Id.DS_Block;	/* DS Track_status */
	}				
	CheckSum.Word = Crc16(puchMsg, 7);					/* Calculate CRC 16 for Xmit Buffer */
	*(puchMsg+7)  = CheckSum.Byte.Lo;					/* CRC low Byte */
	*(puchMsg+8)  = CheckSum.Byte.Hi;					/* CRC high Byte */
	*(puchMsg+9)  = 0x0D;  								/* Add CR */
	*(puchMsg+10)  = 0x0A;  								/* Add LF */
	Xmit_Queue.Record[0].Host_Msg_Length = 11;
	Xmit_Queue.Record[0].Smc_Msg_Length = 9;
	Xmit_Queue.Index = 0;
    Xmit_Queue.Record_Count =1;
	Xmit_Queue.Record_Index =0;	
	Status.Flags.Query_Pending = FALSE;
}

/*********************************************************************************
File name 			:comm_dac.c
Function Name		:void Build_Reply_To_Status_Command(void)
Created By			:Sudheer Herle
Date Created		:
Modification History:
					Rev No			Date		Description
					 --				 --				--
Tracability:
		SRS()    	:

Abstract			:void Build_Reply_To_Get_Event_Command(void)
Algorithm			:
Description			:
Input Element		:None
Output Element		:void

**********************************************************************************/
extern ring_buffer_t     Events_Ring_Buffer;
void Build_Reply_To_Send_event_Command(void)
{
        BYTE *puchMsg;
	puchMsg = & Xmit_Queue.Record[0].Msg_Buffer[0];
        int p=0;
	for(p=0;p<16;p++){
             *(puchMsg+p)  = Latest_event.Byte[p];
        }
        Xmit_Queue.Record[0].Msg_Buffer[78] = 0xAA;
        Xmit_Queue.Record[0].Msg_Buffer[79] = 0x55;
	Xmit_Queue.Record[0].Host_Msg_Length = 80;
	Xmit_Queue.Record[0].Smc_Msg_Length = 11;
	Xmit_Queue.Index = 0;
        Xmit_Queue.Record_Count =1;
	Xmit_Queue.Record_Index =0;
	Status.Flags.Query_Pending = FALSE;
}
void Build_Reply_To_Get_80bytes_Command(void)
{
//        wordtype_t CheckSum;
	BYTE *puchMsg;


	puchMsg = & Xmit_Queue.Record[0].Msg_Buffer[0];
        int p=0;
	for(p=0;p<80;p++){
             *(puchMsg+p)  = Correct_data.Msg_Buffer[p];
        }
	Xmit_Queue.Record[0].Host_Msg_Length = 80;
	Xmit_Queue.Record[0].Smc_Msg_Length = 11;
	Xmit_Queue.Index = 0;
        Xmit_Queue.Record_Count =1;
	Xmit_Queue.Record_Index =0;
	Status.Flags.Query_Pending = FALSE;
        
}
void Build_Reply_To_Get_Event_Command(void)
{
        event_record_t count_record;
    	long_t S_Time, E_Time, Event_count;
	wordtype_t CheckSum;
	BYTE *puchMsg;

	puchMsg = & Xmit_Queue.Record[0].Msg_Buffer[0];

	*(puchMsg)    = CPU1_Address;		/* Source Id   */
	*(puchMsg+1)  = REPLY_EVENTS_COUNT;					/* Command  */
        Read_Event_from_Serial_EEProm(Events_Ring_Buffer.Head, (event_record_t *)&count_record);
        if(count_record.Field.Token == LATEST_EVENT_TOKEN)
        {
            // This is the latest recorded event
            E_Time.LWord = count_record.Field.Date_Time;
        }
        else
        {   // 0 if there is any error
            E_Time.LWord = 0x0000;
        }
        Read_Event_from_Serial_EEProm(Events_Ring_Buffer.Head + 1, (event_record_t *)&count_record);
        if(count_record.Field.Token != LATEST_EVENT_TOKEN || count_record.Field.Token != OLD_EVENT_TOKEN)
        {
            // record 0 is the 1st event.
            Read_Event_from_Serial_EEProm(0, (event_record_t *)&count_record);
            S_Time.LWord = count_record.Field.Date_Time;
            Event_count.LWord = Events_Ring_Buffer.Tail;
        }
        else if(count_record.Field.Token == OLD_EVENT_TOKEN)
        {
            // Tail record or next of head record is the 1st event. Memory has been rouded to use from the starting.
            S_Time.LWord = count_record.Field.Date_Time;
            Event_count.LWord = MAXIMUM_EVENTS;
        }
        else
        {
            // 0 if there is any error
            S_Time.LWord = 0x0000;
            Event_count.LWord = 0;
        }
        *(puchMsg+2)  = Event_count.Byte.Byte1; 			   /* Send nuber of events */
	*(puchMsg+3)  = Event_count.Byte.Byte2;
	*(puchMsg+4)  = Event_count.Byte.Byte3;
	*(puchMsg+5)  = Event_count.Byte.Byte4;
        *(puchMsg+6)  = S_Time.Byte.Byte1; 			   /* First event time */
	*(puchMsg+7)  = S_Time.Byte.Byte2;
	*(puchMsg+8)  = S_Time.Byte.Byte3;
	*(puchMsg+9)  = S_Time.Byte.Byte4;
        *(puchMsg+10)  = E_Time.Byte.Byte1; 			   /* End event time */
	*(puchMsg+11)  = E_Time.Byte.Byte2;
	*(puchMsg+12)  = E_Time.Byte.Byte3;
	*(puchMsg+13)  = E_Time.Byte.Byte4;
        CheckSum.Word = Crc16(puchMsg,14);				/* Calculate CRC 16 for Xmit Buffer */
	*(puchMsg+14)  = CheckSum.Byte.Lo;				/* CRC low Byte */
	*(puchMsg+15)  = CheckSum.Byte.Hi;				/* CRC High Byte */
	*(puchMsg+16)  = 0x0D;  								/* Add CR */
	*(puchMsg+17)  = 0x0A;  								/* Add LF */
	Xmit_Queue.Record[0].Host_Msg_Length = 18;
	Xmit_Queue.Record[0].Smc_Msg_Length = 16;
	Xmit_Queue.Index = 0;
        Xmit_Queue.Record_Count =1;
	Xmit_Queue.Record_Index =0;
	Status.Flags.Query_Pending = FALSE;
}

