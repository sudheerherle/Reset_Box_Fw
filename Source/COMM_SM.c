#include <xc.h>
#include <time.h>
#include <limits.h>

#include "COMMON.H"
#include "COMM_SM.H"
#include "CRC16.H"


extern near rb_status_t Status;					/* Reset Box Status information */
extern near time_t SystemClock;					/* from resetbox.c   */
extern near rb_info_t RB_Info;					/* from ACC_RB.C */

sm_sch_info_t			Sm_Sch_Info;			/* structure holds station master interface communication scheduler */
sm_info_t				Com1RecvObject;

const unsigned char uchStatusLookup[13][2] = 
	{{ 0, 0}, { 1, 1}, { 1, 2}, { 2, 1}, { 2, 2}, 
	 { 3, 3}, { 4, 4}, { 5, 5}, { 6, 6}, { 6, 2}, 
	 { 2, 6}, { 6, 1}, { 1, 6}
	};


void SetupCOM1BaudRate(BYTE);
void Clear_COM1_Receive_Buffer(void);
void Process_SM_Message(void);
void Set_Sm_Sch_Idle(void);
void Save_DS_Data(void);
void Save_US_Data(void);
void Save_3DP1S_A_Data(void);
void Save_3DP1S_B_Data(void);
void Save_3DP1S_C_Data(void);

void Initialise_Sm_CommSch(void)
{
	Sm_Sch_Info.State      = SM_SCHEDULER_NOT_STARTED;		 /*  set smc comm scheduler to "SM_SCHEDULER_NOT_STARTED" state  */ 
	Sm_Sch_Info.BytePeriod = BYTE_PERIOD_9MS;				     /* set byte PERIOD to 1ms for 1200 Baud Rate* */ 
	SetupCOM1BaudRate(BAUDRATE_1200);						     /*  set smc comm baudrate to 1200bps */ 
}

void SetupCOM1BaudRate(BYTE uchBaudRate)
{
	switch(uchBaudRate)
	{
		case BAUDRATE_1200:
			BRGH   = BRGH_FOR_1200;
			BRG161 = BRGH16_FOR_1200;
			SPBRG1 = (SPBRG_FOR_1200 & 0xFF);
			SPBRGH1= ((SPBRG_FOR_1200 >> 8)& 0xFF);		
			break;
		case BAUDRATE_2400:
			BRGH   = BRGH_FOR_2400;
			BRG161 = BRGH16_FOR_2400;
			SPBRG1 = (SPBRG_FOR_2400 & 0xFF);
			SPBRGH1= ((SPBRG_FOR_2400 >> 8)& 0xFF);	
			break;
		case BAUDRATE_9600:
			BRGH   = BRGH_FOR_9600;
			BRG161 = BRGH16_FOR_9600;
			SPBRG1 = (SPBRG_FOR_9600 & 0xFF);
			SPBRGH1= ((SPBRG_FOR_9600 >> 8)& 0xFF);	
			break;
		case BAUDRATE_19200:
			BRGH   = BRGH_FOR_19200;
			BRG161 = BRGH16_FOR_19200;
			SPBRG1 = (SPBRG_FOR_19200 & 0xFF);
			SPBRGH1= ((SPBRG_FOR_19200 >> 8)& 0xFF);	
			break;			
	}
		
	SYNC  = SET_LOW;	/* Asynchronous mode */
	SPEN  = SET_HIGH;	/* Enable serial port pins */
	TX9   = SET_LOW;	/* 8-Bit Transmission */
	RX9   = SET_LOW;	/* 8-Bit Reception */
	TX1IE = SET_LOW;	/* Disable Transmit Interrupt */
	RC1IE = SET_LOW;	/* Disable Receive Interrupt */
	TXEN  = SET_LOW;	/* Disable Transmitter */
	SREN  = SET_LOW;    /* Single receive enable bit */  
	CREN  = SET_LOW;	/* Disable Continous Receive */
	WUE1  = SET_LOW;    /* Disable Wakeup enable bit  */
	ABDEN1= SET_LOW;    /* Disable Auto BUAD RATE detection*/
}

void Start_Sm_Communication(void)
{
	/*
	 * We use this channel only to receive data periodically
	 * from SM CPU.
	 */
	TXEN = SET_LOW;		/* Disable Transmitter */
	CREN = SET_HIGH;	/* Enable Receiver */	
    Sm_Sch_Info.State =  SM_NO_DATA_RECEIVED;
	Sm_Sch_Info.Timeout_ms = SM_COMM_LINK_FAILURE_TIMEOUT;
	MODEM_RS = SET_HIGH;
}
void Update_Sm_Sch_State(void)
{
  BYTE uchData=0;

	switch (Sm_Sch_Info.State)
	{
		case SM_SCHEDULER_NOT_STARTED:
			break;
		case SM_NO_DATA_RECEIVED:
			if (RC1IF)
				{
				uchData = RCREG1;
				Com1RecvObject.Msg_Buffer[0] = uchData;
				Com1RecvObject.Index = 1;
				Com1RecvObject.Msg_Length = SM_MESSAGE_LENGTH;
				Sm_Sch_Info.State = SM_DATA_RECEPTION_STARTED;
				Sm_Sch_Info.Timeout_ms = SM_DATA_RECEIVE_TIMEOUT;
				break;
				}
			if (Sm_Sch_Info.Timeout_ms == TIMEOUT_EVENT)
				{
				Initialise_RB_Info();
				Set_Sm_Sch_Idle();
				}
			break;
		case SM_DATA_RECEPTION_STARTED:
			if (RC1IF)
				{
				uchData = RCREG1;
				Com1RecvObject.Msg_Buffer[Com1RecvObject.Index] = uchData;
				Com1RecvObject.Index = Com1RecvObject.Index + 1;
				Sm_Sch_Info.Timeout_ms = SM_DATA_RECEIVE_TIMEOUT;
				break;
				}
			if (Com1RecvObject.Index >= SM_MESSAGE_LENGTH)
				{
				Sm_Sch_Info.State = SM_DATA_RECEPTION_COMPLETE;
				Sm_Sch_Info.Timeout_ms = 0;
				break;
				}
			if (Sm_Sch_Info.Timeout_ms == TIMEOUT_EVENT)
				{
				Set_Sm_Sch_Idle();
				}
			break;
		case SM_DATA_RECEPTION_COMPLETE:
			if (Crc16((const BYTE *)Com1RecvObject.Msg_Buffer,Com1RecvObject.Msg_Length)== 0)
				{
				/* Valid Data received from SM Unit */
				Process_SM_Message();
				NOP();
				}
			Set_Sm_Sch_Idle();
			break;
		case SM_CLEAR_FRAME_ERROR:
			uchData = RCREG1;
			Set_Sm_Sch_Idle();
			break;
		case SM_CLEAR_OVERRUN_ERROR:
			TXEN = SET_LOW;
			CREN = SET_LOW;
			CREN = SET_HIGH;
			Set_Sm_Sch_Idle();
			break;
		default:
			Set_Sm_Sch_Idle();
			break;
	}
}

void Set_Sm_Sch_Idle(void)
	{
    Sm_Sch_Info.State =  SM_NO_DATA_RECEIVED;
	Sm_Sch_Info.Timeout_ms = SM_COMM_LINK_FAILURE_TIMEOUT;
	}

void Decrement_Sm_Sch_msTmr(void)
{
	if (Sm_Sch_Info.Timeout_ms > 0)
	{
		Sm_Sch_Info.Timeout_ms = Sm_Sch_Info.Timeout_ms - 1;
	}	
}

void Process_SM_Message(void)
{
	longtype_t Sys_Time;

	RB_Info.DAC_Type = Com1RecvObject.Msg_Buffer[UNIT_TYPE_OFFSET];
	RB_Info.DAC_Checksum[0] = Com1RecvObject.Msg_Buffer[CHECKSUM_OFFSET];
	RB_Info.DAC_Checksum[1] = Com1RecvObject.Msg_Buffer[CHECKSUM_OFFSET+1];
	RB_Info.DAC_Checksum[2] = Com1RecvObject.Msg_Buffer[CHECKSUM_OFFSET+2];
	RB_Info.DAC_Checksum[3] = Com1RecvObject.Msg_Buffer[CHECKSUM_OFFSET+3];
	RB_Info.DAC_SW_Version = Com1RecvObject.Msg_Buffer[SOFWARE_VERSION_OFFSET];

	Sys_Time.DWord.LoWord.Byte.Lo = Com1RecvObject.Msg_Buffer[TIMESTAMP_OFFSET];
	Sys_Time.DWord.LoWord.Byte.Hi = Com1RecvObject.Msg_Buffer[TIMESTAMP_OFFSET + 1];
	Sys_Time.DWord.HiWord.Byte.Lo = Com1RecvObject.Msg_Buffer[TIMESTAMP_OFFSET + 2];
	Sys_Time.DWord.HiWord.Byte.Hi = Com1RecvObject.Msg_Buffer[TIMESTAMP_OFFSET + 3];
	/*
	 * Update the system clock.
	 */	
	SystemClock = Sys_Time.LWord;

	switch (RB_Info.DAC_Type)
		{
 		case DAC_TYPE_SF:
			Save_DS_Data();
			break;
 		case DAC_TYPE_EF:
			Save_US_Data();
			break;
 		case DAC_TYPE_CF:
			if (RB_Info.RB_Type == RESET_BOX_TYPE_DS)
				{
				Save_DS_Data();
				}
			else
				{
				if (RB_Info.RB_Type == RESET_BOX_TYPE_US)
					{
					Save_US_Data();
					}
				else
					{
					NOP();
					}
				}
			break;
 		case DAC_TYPE_3D_A:
			Save_3DP1S_A_Data();
			break;
 		case DAC_TYPE_3D_B:
			Save_3DP1S_B_Data();
			break;
 		case DAC_TYPE_3D_C:
			Save_3DP1S_C_Data();
			break;
		default:
			NOP();
			break;
		}
}

void Save_DS_Data()
{
	BYTE uchBuf;

	uchBuf = Com1RecvObject.Msg_Buffer[MODE_OFFSET] & 0x0F;
	if (uchBuf <= MAX_TYPE_OF_STATUS)
		{
		RB_Info.Local_DP_Status = uchStatusLookup[uchBuf][1];
		RB_Info.Remote_DP_Status = uchStatusLookup[uchBuf][0];
		}
	RB_Info.Local_DP_Count.Byte.Lo = Com1RecvObject.Msg_Buffer[DS_LOCAL_AXLE_COUNT_OFFSET];
	RB_Info.Local_DP_Count.Byte.Hi = Com1RecvObject.Msg_Buffer[DS_LOCAL_AXLE_COUNT_OFFSET+1];
	RB_Info.Remote_DP_Count.Byte.Lo = Com1RecvObject.Msg_Buffer[DS_REMOTE_AXLE_COUNT_OFFSET];
	RB_Info.Remote_DP_Count.Byte.Hi = Com1RecvObject.Msg_Buffer[DS_REMOTE_AXLE_COUNT_OFFSET+1];
	RB_Info.Local_DP_Error = Com1RecvObject.Msg_Buffer[DS_ERR_CODE_OFFSET];
}

void Save_US_Data()
{
	BYTE uchBuf;

	uchBuf = Com1RecvObject.Msg_Buffer[MODE_OFFSET] & 0xF0;
	uchBuf = (uchBuf >> 4);
	if (uchBuf <= MAX_TYPE_OF_STATUS)
		{
		RB_Info.Local_DP_Status = uchStatusLookup[uchBuf][1];
		RB_Info.Remote_DP_Status = uchStatusLookup[uchBuf][0];
		}
	RB_Info.Local_DP_Count.Byte.Lo = Com1RecvObject.Msg_Buffer[US_LOCAL_AXLE_COUNT_OFFSET];
	RB_Info.Local_DP_Count.Byte.Hi = Com1RecvObject.Msg_Buffer[US_LOCAL_AXLE_COUNT_OFFSET+1];
	RB_Info.Remote_DP_Count.Byte.Lo = Com1RecvObject.Msg_Buffer[US_REMOTE_AXLE_COUNT_OFFSET];
	RB_Info.Remote_DP_Count.Byte.Hi = Com1RecvObject.Msg_Buffer[US_REMOTE_AXLE_COUNT_OFFSET+1];
	RB_Info.Local_DP_Error = Com1RecvObject.Msg_Buffer[US_ERR_CODE_OFFSET];
}

void Save_3DP1S_A_Data()
{
	BYTE uchBuf;

	uchBuf = Com1RecvObject.Msg_Buffer[MODE_OFFSET] & 0x0F;
	if (uchBuf <= MAX_TYPE_OF_STATUS)
		{
		RB_Info.Local_DP_Status = uchStatusLookup[uchBuf][1];
		RB_Info.Remote_DP_Status = uchStatusLookup[uchBuf][0];
		}
	RB_Info.DP_A_Count.Byte.Lo = Com1RecvObject.Msg_Buffer[DS_LOCAL_AXLE_COUNT_OFFSET];
	RB_Info.DP_A_Count.Byte.Hi = Com1RecvObject.Msg_Buffer[DS_LOCAL_AXLE_COUNT_OFFSET+1];
	RB_Info.DP_B_Count.Byte.Lo = Com1RecvObject.Msg_Buffer[DS_REMOTE_AXLE_COUNT_OFFSET];
	RB_Info.DP_B_Count.Byte.Hi = Com1RecvObject.Msg_Buffer[DS_REMOTE_AXLE_COUNT_OFFSET+1];
	RB_Info.DP_C_Count.Byte.Lo = Com1RecvObject.Msg_Buffer[US_REMOTE_AXLE_COUNT_OFFSET];
	RB_Info.DP_C_Count.Byte.Hi = Com1RecvObject.Msg_Buffer[US_REMOTE_AXLE_COUNT_OFFSET+1];
	RB_Info.Local_DP_Error = Com1RecvObject.Msg_Buffer[DS_ERR_CODE_OFFSET];
}

void Save_3DP1S_B_Data()
{
	BYTE uchBuf;

	uchBuf = Com1RecvObject.Msg_Buffer[MODE_OFFSET] & 0x0F;
	if (uchBuf <= MAX_TYPE_OF_STATUS)
		{
		RB_Info.Local_DP_Status = uchStatusLookup[uchBuf][1];
		RB_Info.Remote_DP_Status = uchStatusLookup[uchBuf][0];
		}
	RB_Info.DP_A_Count.Byte.Lo = Com1RecvObject.Msg_Buffer[US_REMOTE_AXLE_COUNT_OFFSET];
	RB_Info.DP_A_Count.Byte.Hi = Com1RecvObject.Msg_Buffer[US_REMOTE_AXLE_COUNT_OFFSET+1];
	RB_Info.DP_B_Count.Byte.Lo = Com1RecvObject.Msg_Buffer[DS_LOCAL_AXLE_COUNT_OFFSET];
	RB_Info.DP_B_Count.Byte.Hi = Com1RecvObject.Msg_Buffer[DS_LOCAL_AXLE_COUNT_OFFSET+1];
	RB_Info.DP_C_Count.Byte.Lo = Com1RecvObject.Msg_Buffer[DS_REMOTE_AXLE_COUNT_OFFSET];
	RB_Info.DP_C_Count.Byte.Hi = Com1RecvObject.Msg_Buffer[DS_REMOTE_AXLE_COUNT_OFFSET+1];
	RB_Info.Local_DP_Error = Com1RecvObject.Msg_Buffer[DS_ERR_CODE_OFFSET];
}

void Save_3DP1S_C_Data()
{
	BYTE uchBuf;

	uchBuf = Com1RecvObject.Msg_Buffer[MODE_OFFSET] & 0x0F;
	if (uchBuf <= MAX_TYPE_OF_STATUS)
		{
		RB_Info.Local_DP_Status = uchStatusLookup[uchBuf][1];
		RB_Info.Remote_DP_Status = uchStatusLookup[uchBuf][0];
		}
	RB_Info.DP_A_Count.Byte.Lo = Com1RecvObject.Msg_Buffer[DS_REMOTE_AXLE_COUNT_OFFSET];
	RB_Info.DP_A_Count.Byte.Hi = Com1RecvObject.Msg_Buffer[DS_REMOTE_AXLE_COUNT_OFFSET+1];
	RB_Info.DP_B_Count.Byte.Lo = Com1RecvObject.Msg_Buffer[US_REMOTE_AXLE_COUNT_OFFSET];
	RB_Info.DP_B_Count.Byte.Hi = Com1RecvObject.Msg_Buffer[US_REMOTE_AXLE_COUNT_OFFSET+1];
	RB_Info.DP_C_Count.Byte.Lo = Com1RecvObject.Msg_Buffer[DS_LOCAL_AXLE_COUNT_OFFSET];
	RB_Info.DP_C_Count.Byte.Hi = Com1RecvObject.Msg_Buffer[DS_LOCAL_AXLE_COUNT_OFFSET+1];
	RB_Info.Local_DP_Error = Com1RecvObject.Msg_Buffer[DS_ERR_CODE_OFFSET];
}
