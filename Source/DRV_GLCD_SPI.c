/*********************************************************************************************************************
	Project		: 	Single Section Digital Axle Counter
	Version		: 	2.0
	Revision	:	1
	Filename	: 	drv_lcd.c
	Target MCU	: 	PIC24FJ256GB210
    Compiler	: 	XC16 Compiler V1.21
	Author		:	EM003 
	Date		:
	Company Name: 	Insys Digital Systems.
	Modification History:
					Rev No			Date		Description
					 --				 --				--
	Functions	:   void Initialise_LCD_Driver(void)
					void Initialise_LCD_Display(void)
					void Update_LCD_State(void)
					void Decrement_LCD_msTmr(void)
					void WriteShiftRegister(BYTE uchLCDdata)
					void Write_LCD_data(BYTE uchData)
					void Write_LCD_command(BYTE uchCommand)
					void Display_on_LCD(BYTE uchRow, BYTE uchColumn, BYTE *String)

*********************************************************************************************************************/
#include <xc.h>
#include <stddef.h>
#include <time.h>
#include "COMMON.H"
#include "DRV_GLCD_SPI.h"
#include "command_proc.h"
#include "DRV_I2C.H"
#include "DRV_MEM.H"
#include "COMM_GSM.h"
typedef struct {
		UINT16 Head;	/* This pointer holds the address of Old Event Token i.e one previous to Lastest Event Token*/
		UINT16 Tail;	/* This pointer holds the address of Lastest Event Token*/
		BYTE Count;		/* it counts the number of record logged into EEPROM */
} ring_buffer_t;


glcd_info_t GLCD_Info;		/* structure that handles Lcd scheduler and holds lcd Information */

void Initialise_GLCD_Display(void);
void Build_packet_GLCD(void);
BOOL Compare_Data_Sent_prev(void);
/*********************************************************************************
File name 			:drv_lcd.c
Function Name		:void Initialise_LCD_Driver(void)
Created By			:EM003 
Date Created		:
Modification History:
					Rev No			Date		Description
					 --				 --				--
Tracability:
		SRS()    	:

Abstract			:Initialisation of lcd driver module.
Algorithm			:
Description			:
Input Element		:None
Output Element		:void

**********************************************************************************/
unsigned char temp;
void Initialise_GLCD_Driver(void)
{
    if(SPI1STATbits.SPIRBF)
                            {
        while(!SPI1STATbits.SRXMPT)
                                {
            temp = SPI1BUF;
                                }
                            }
    SPI1CON1 = 0;
    SPI1CON2 = 0;
    SPI1STAT = 0;
            /* Configure Remappable pins */
        //*************************************************************
        // Unlock Registers
        //*************************************************************
        __builtin_write_OSCCONL(OSCCON & 0xbf); //clear the bit 6 of OSCCONL to unlock Pin Re-map

        RPINR20bits.SDI1R = 22;
        RPOR10bits.RP20R = 8;
        RPOR12bits.RP25R = 7;

        //************************************************************
        // Lock Registers
        //************************************************************
        __builtin_write_OSCCONL(OSCCON | 0x40); //set the bit 6 of OSCCONL to lock Pin Re-map
        //************************************************************

        TRISDbits.TRISD4 = 0;            // set  SDI1 as an input
        TRISDbits.TRISD3 = 1;            // set  SDO1 as an output
        TRISDbits.TRISD5 = 0;             // set  SCK1 as an output
        TRISDbits.TRISD13 = 0;             //  set  SS as an input
        TRISGbits.TRISG13 = 0;
        
        SPI1CON2bits.SPIBEN = 1;
      IFS0bits.SPI1IF = 0;

      IEC0bits.SPI1IE = 0;
    SPI1CON1 = SPI_MASTER;  // select mode
    SPI1STAT = SPI_ENABLE;  // enable the peripheral

    TRISGbits.TRISG12 = 1;  //G_ACTIVE2
    TRISGbits.TRISG6 = 1;
    
        G_SPI_SS = SET_HIGH;
        G_SPI_SS2= SET_HIGH;
        temp = SPI1BUF;
}

/*********************************************************************************
File name 			:drv_lcd.c
Function Name		:void Update_LCD_State(void)
Created By			:EM003 
Date Created		:
Modification History:
					Rev No			Date		Description
					 --				 --				--
Tracability:
		SRS()    	:

Abstract			:Module for updating the values in the lcd display
Algorithm			:1.check for lcd module plugged in or not.
					 2.Go for initialisation if module is plugged in  else go to step1.
					 3.After Initialisation of LCD completed,it will go for display the characters
					 4.All lcd info four lines are written character by character into display with
				       a time interval of 2ms for each character.
					 5.After go to step4.
Description			:
Input Element		:None
Output Element		:void

**********************************************************************************/
unsigned char count_rx;
extern char inspect_event_done;
extern char  inspect_CPU1_data_done;
extern char inspect_CPU2_data_done;
void Update_GLCD_State(void)
{
//	BYTE uchBuf=0;
if(inspect_event_done == 0)
            return;
 if(inspect_CPU1_data_done != 1 && inspect_CPU2_data_done != 1)
        return;
        //if(0)
        if(G_ACTIVE == 0)
        GLCD_Info.GLCD1_On_state = 0;
    if(G_ACTIVE2 == 0)
	    GLCD_Info.GLCD2_On_state = 0;
	if((G_ACTIVE == 0) && (G_ACTIVE2 == 0))
        {
        GLCD_Info.State = GLCD_IDLE;
		return;
	}
	else if(GLCD_Info.State == GLCD_IDLE)
	{
        if((GLCD_Info.GLCD1_On_state!=G_ACTIVE) || (GLCD_Info.GLCD2_On_state!=G_ACTIVE2))
        {
            Initialise_GLCD_Driver();
            GLCD_Info.Comm_Timeout_ms = MAX_COMM_TIMEOUT;
            GLCD_Info.State = GLCD_IDLE;	
        }
	}
	
	GLCD_Info.GLCD1_On_state = G_ACTIVE;
	GLCD_Info.GLCD2_On_state = G_ACTIVE2;

	switch (GLCD_Info.State)
	{
        case GLCD_IDLE:
            if(GLCD_Info.Comm_Timeout_ms == 0)
            {
                GLCD_Info.State = GLCD_COMPARE_DATA;
				if(GLCD_Info.GLCD1_On_state == 1)
				{
					GLCD_Info.GLCD_Disp = DISPLAY1;
				}
				else
				{
					GLCD_Info.GLCD_Disp = DISPLAY2;
				}
            }
            break;
        case GLCD_COMPARE_DATA:
            if(Compare_Data_Sent_prev())
            {
                //Data has been modified and need to be sent.
				Initialise_GLCD_Driver();
				if(GLCD_Info.GLCD_Disp == DISPLAY1)
				{
					G_SPI_SS = SET_LOW;
//                    G_SPI_SS2 = SET_HIGH;
				}
				else 
				{
					G_SPI_SS2 = SET_LOW;
//                    G_SPI_SS = SET_HIGH;
				}
                GLCD_Info.Comm_Timeout_ms = SS_TIMEOUT; //wait for 10 ms for SS to stabilize
                GLCD_Info.State = GLCD_SS_WAIT;
            }
            else
            {
                GLCD_Info.Comm_Timeout_ms = MAX_COMM_TIMEOUT; //wait for 10 ms for SS to stabilize
                GLCD_Info.State = GLCD_IDLE;
            }
			break;
        case GLCD_SS_WAIT:
            if(GLCD_Info.Comm_Timeout_ms == 0)
            {
				Build_packet_GLCD();
				GLCD_Info.Packet_Max_length = MAX_G_PACKET_LEN;
//                if(GLCD_Info.Message_Buffer[114] == 0) break;
				GLCD_Info.State = GLCD_TX_DATA;
				GLCD_Info.Packet_index = 0;
            }
            break;
		case GLCD_TX_DATA:
            //send 8 bytes through SPI1
            //SPI1BUF = data;
            if(SPI1STATbits.SPIRBF)
            {
                while(!SPI1STATbits.SRXMPT)
                {
                                    GLCD_Info.Rx_Message_Buffer[count_rx] = SPI1BUF;
                                    count_rx++;
                }
            }
            while((SPI1STATbits.SPITBF == 0) && (GLCD_Info.Packet_index < GLCD_Info.Packet_Max_length))//if (BF == 0)					   /* Still all Bytes are not completely transmitted */
            {
                /* SSPBUF empty - Transmit Status, So we can Put the Next Byte */
                SPI1BUF = GLCD_Info.Message_Buffer[GLCD_Info.Packet_index];//GLCD_Info.Packet_index; for testing
                GLCD_Info.Packet_index++;
            }
			if((GLCD_Info.Packet_index >= GLCD_Info.Packet_Max_length) && (SPI1STATbits.SRMPT==1))
            {
			    GLCD_Info.Comm_Timeout_ms = TX_TIMEOUT;
				GLCD_Info.State = GLCD_CM_WAIT;
			}
            break;
        case GLCD_CM_WAIT:
            if(GLCD_Info.Comm_Timeout_ms == 0)
            {
				//end of transmission
				if((GLCD_Info.GLCD_Disp == DISPLAY1) && (GLCD_Info.GLCD2_On_state==1))
				{
					G_SPI_SS = SET_HIGH;
					GLCD_Info.GLCD_Disp = DISPLAY2;
					GLCD_Info.State = GLCD_COMPARE_DATA;
				}
				else if(GLCD_Info.GLCD_Disp == DISPLAY2)
				{
					G_SPI_SS2 = SET_HIGH;
					GLCD_Info.GLCD_Disp = NO_LCD_DISP;
					GLCD_Info.Comm_Timeout_ms = MAX_COMM_TIMEOUT;
					GLCD_Info.State = GLCD_IDLE;
				}
                else
                {
					GLCD_Info.Comm_Timeout_ms = MAX_COMM_TIMEOUT;
					GLCD_Info.State = GLCD_IDLE;                    
                }
			}
            break;
        default:
			break;
	}
}

/*********************************************************************************
File name 			:drv_lcd.c
Function Name		:void Decrement_LCD_msTmr(void)
Created By			:EM003 
Date Created		:
Modification History:
					Rev No			Date		Description
					 --				 --				--
Tracability:
		SRS()    	:

Abstract			:For every 1ms,lcd info timeout variable is decremented.
Algorithm			:
Description			:
Input Element		:None
Output Element		:void

**********************************************************************************/
void Decrement_GLCD_msTmr(void)
{
	if (GLCD_Info.Comm_Timeout_ms > 0)
	{
		GLCD_Info.Comm_Timeout_ms = GLCD_Info.Comm_Timeout_ms - 1;
	}

}
extern glcd_info_t GLCD_Info;
extern dac_sysinfo_t DAC_sysinfo;;
unsigned char u_count;
extern BYTE CPU1_data_GLCD[GCPU_SPI_MESSAGE_LENGTH];			/* Buffer to store data recived from Cpu1 */
extern BYTE CPU2_data_GLCD[GCPU_SPI_MESSAGE_LENGTH];			/* Buffer to store data recived from Cpu2 */

void Build_packet_GLCD(void)
{
    unsigned char track;
    //clear the buffer
    for(u_count = 0;u_count<(MAX_G_PACKET_LEN);u_count++)
    {
        GLCD_Info.Message_Buffer[u_count]    = 0;
    }

    for(u_count = 0;u_count<8;u_count++) // blue
    {
        GLCD_Info.Message_Buffer[u_count]    = CPU1_data_GLCD[u_count];
        GLCD_Info.Message_Buffer[u_count+CPU1_PACKET_LEN] = CPU2_data_GLCD[u_count];
    }
    track = u_count;//8 - 21
    for(;u_count<21;u_count++) //yellow
    {
        GLCD_Info.Message_Buffer[u_count]    = CPU1_data_GLCD[u_count + 24 - track];
        GLCD_Info.Message_Buffer[u_count + CPU1_PACKET_LEN] = CPU2_data_GLCD[u_count + 24 - track];
    }
        GLCD_Info.Message_Buffer[u_count]    = CPU1_data_GLCD[77];                  //Message ID
        GLCD_Info.Message_Buffer[u_count + CPU1_PACKET_LEN] = CPU2_data_GLCD[77];   //Message ID
        u_count++;
    track = u_count;
    for(;u_count<55;u_count++) //yellow     //u_count will start from 22 to 54 //Disp Count
    {
        GLCD_Info.Message_Buffer[u_count]    = CPU1_data_GLCD[u_count + 37 - track];
        GLCD_Info.Message_Buffer[u_count + CPU1_PACKET_LEN] = CPU2_data_GLCD[u_count + 37 - track];
    }
    GLCD_Info.Message_Buffer[OFFSET_CODE_CHECKSUM] = DAC_sysinfo.Checksum.DWord.HiWord.Byte.Hi;
    GLCD_Info.Message_Buffer[OFFSET_CODE_CHECKSUM + 1] = DAC_sysinfo.Checksum.DWord.HiWord.Byte.Lo;
    GLCD_Info.Message_Buffer[OFFSET_CODE_CHECKSUM + 2] = DAC_sysinfo.Checksum.DWord.LoWord.Byte.Hi;
    GLCD_Info.Message_Buffer[OFFSET_CODE_CHECKSUM + 3] = DAC_sysinfo.Checksum.DWord.LoWord.Byte.Lo;
    GLCD_Info.Message_Buffer[OFFSET_UNIT_TYPE] = DAC_sysinfo.Unit_Type;
//    if(CPU1_data_GLCD[70]==1 || CPU2_data_GLCD[70]==1) //Adjustment for LCWS
//    {
//       GLCD_Info.Message_Buffer[OFFSET_UNIT_TYPE] = 9;
//    }
    GLCD_Info.Message_Buffer[OFFSET_SW_V] = DAC_sysinfo.SW_Version;
    Update_SMPU_data();
//    GLCD_Info.Message_Buffer[OFFSET_CRC    ]   = 0xAA;
//    GLCD_Info.Message_Buffer[OFFSET_CRC + 1]   = 0xAA;
    GLCD_Info.Message_Buffer[OFFSET_UNUSED + 1] = CPU1_data_GLCD[71];
    GLCD_Info.Message_Buffer[OFFSET_UNUSED + 2] = CPU1_data_GLCD[72];
    GLCD_Info.Message_Buffer[OFFSET_UNUSED + 3] = CPU1_data_GLCD[73];
    GLCD_Info.Message_Buffer[OFFSET_UNUSED + 4] = 0xAA;
    GLCD_Info.Message_Buffer[OFFSET_UNUSED + 5] = 0xAA;
//    CheckSum_G.Word = Crc16(GLCD_INFO, MAX_G_PACKET_LEN - 2);
    GLCD_Info.Message_Buffer[OFFSET_CRC]   = 0;
    GLCD_Info.Message_Buffer[OFFSET_CRC + 1]   = 0;
//    GLCD_Info.Message_Buffer[13] = Event_Logger_ID ;
//    GLCD_Info.Message_Buffer[14] = 0;
//    GLCD_Info.Message_Buffer[15] = (High_Availability | (GSM_Sch_Info.GSM_Enable<<1)) & 0x03;
//    GLCD_Info.Message_Buffer[16] = 0;
    
}

extern ring_buffer_t     Events_Ring_Buffer;
extern time_t SystemClock;					/* holds system clock time   */
void Update_SMPU_data(void)
{
        event_record_t count_record;
    	long_t S_Time, E_Time, Event_count, P_Time, SMCPU_CRC;

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
        GLCD_Info.Message_Buffer[OFFSET_EVENT_COUNT]     = Event_count.Byte.Byte1; 			   /* Send nuber of events */
	GLCD_Info.Message_Buffer[OFFSET_EVENT_COUNT + 1] = Event_count.Byte.Byte2;
	GLCD_Info.Message_Buffer[OFFSET_EVENT_COUNT + 2] = Event_count.Byte.Byte3;
	GLCD_Info.Message_Buffer[OFFSET_EVENT_COUNT + 3] = Event_count.Byte.Byte4;
        GLCD_Info.Message_Buffer[OFFSET_START_TIME   ]  = S_Time.Byte.Byte1; 			   /* First event time */
	GLCD_Info.Message_Buffer[OFFSET_START_TIME + 1]  = S_Time.Byte.Byte2;
	GLCD_Info.Message_Buffer[OFFSET_START_TIME + 2]  = S_Time.Byte.Byte3;
	GLCD_Info.Message_Buffer[OFFSET_START_TIME + 3]  = S_Time.Byte.Byte4;
        GLCD_Info.Message_Buffer[OFFSET_END_TIME      ]  = E_Time.Byte.Byte1; 			   /* End event time */
	GLCD_Info.Message_Buffer[OFFSET_END_TIME + 1  ]  = E_Time.Byte.Byte2;
	GLCD_Info.Message_Buffer[OFFSET_END_TIME + 2  ]  = E_Time.Byte.Byte3;
	GLCD_Info.Message_Buffer[OFFSET_END_TIME + 3  ]  = E_Time.Byte.Byte4;

        P_Time.LWord = SystemClock;
        GLCD_Info.Message_Buffer[OFFSET_PRESENT_TIME     ]  = P_Time.Byte.Byte1;
        GLCD_Info.Message_Buffer[OFFSET_PRESENT_TIME + 1 ]  = P_Time.Byte.Byte2;
        GLCD_Info.Message_Buffer[OFFSET_PRESENT_TIME + 2 ]  = P_Time.Byte.Byte3;
        GLCD_Info.Message_Buffer[OFFSET_PRESENT_TIME + 3 ]  = P_Time.Byte.Byte4;

        SMCPU_CRC.LWord = 0x87654321;//substitute with code crc
        GLCD_Info.Message_Buffer[OFFSET_SMCPU_CRC     ]  = SMCPU_CRC.Byte.Byte1;
        GLCD_Info.Message_Buffer[OFFSET_SMCPU_CRC + 1 ]  = SMCPU_CRC.Byte.Byte2;
        GLCD_Info.Message_Buffer[OFFSET_SMCPU_CRC + 2 ]  = SMCPU_CRC.Byte.Byte3;
        GLCD_Info.Message_Buffer[OFFSET_SMCPU_CRC + 3 ]  = SMCPU_CRC.Byte.Byte4;


//        CheckSum.Word = Crc16(puchMsg,14);				/* Calculate CRC 16 for Xmit Buffer */

}

BOOL Compare_Data_Sent_prev()
{
    if(1)       // get in if there is change in packet sent previously and now.
    {
        return TRUE;
    }
    return FALSE;
}
