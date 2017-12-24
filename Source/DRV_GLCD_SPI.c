/*********************************************************************************************************************
	Project		: 	Single Section Digital Axle Counter
	Version		: 	2.0
	Revision	:	1
	Filename	: 	drv_lcd.c
	Target MCU	: 	PIC24FJ256GB210
    Compiler	: 	XC16 Compiler V1.21
	Author		:	Sudheer Herle 
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

#include "COMMON.h"
#include "DRV_GLCD_SPI.h"
#include "command_proc.h"
#include "DRV_MEM.h"
#include "CRC16.h"
#include "EVENTS.h"

extern unsigned char No_smcpu;
extern ring_buffer_t Events_Ring_Buffer;
extern BOOL E_status;/*lint -e552 */
unsigned char NID;
BYTE Network_status[MAX_SMCPU];
glcd_info_t GLCD_Info;		/* structure that handles Lcd scheduler and holds lcd Information */
extern BYTE HA_config;
wordtype_t Glcd_csum;

extern time_t SystemClock;					/* holds system clock time   */

/*********************************************************************************
File name 			:drv_lcd.c
Function Name		:void Initialise_LCD_Driver(void)
Created By			:Sudheer Herle 
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
Created By			:Sudheer Herle 
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
extern BYTE Network_config;
extern char  inspect_CPU1_data_done;
extern char inspect_CPU2_data_done;
extern char inspect_DAC_info_done;
unsigned char Interested_Nw_Index = 0;
extern UINT32 SMCPU_CRC_checksum;

void Update_GLCD_State(void)
{

//    if(inspect_event_done == 0)
//        return;
    if(inspect_CPU1_data_done != 1 
         && inspect_CPU2_data_done != 1
         && inspect_DAC_info_done != 1)
        return;
        //if(0)
//        if(G_ACTIVE == 0)
        GLCD_Info.GLCD1_On_state = 1;
//    if(G_ACTIVE2 == 0)
	    GLCD_Info.GLCD2_On_state = 1;
//	if((G_ACTIVE == 0) && (G_ACTIVE2 == 0))
//        {
//        GLCD_Info.State = GLCD_IDLE;
//		return;
//	}
    
//    if(G_ACTIVE == 0)
//    {
//        GLCD_Info.State = GLCD_MODULE_NOT_ACTIVE;
//        return;
//    }
//    
//    GLCD_Info.GLCD1_On_state = G_ACTIVE;
//	GLCD_Info.GLCD2_On_state = G_ACTIVE2;
//    if(HA_config==0){
//        GLCD_Info.GLCD2_On_state = 0;
//    }
	switch (GLCD_Info.State)
	{
		case GLCD_MODULE_NOT_ACTIVE:
//            if (G_ACTIVE == 1 || G_ACTIVE2 == 1)
			{
                count_rx = 0;
                GLCD_Info.Comm_Timeout_ms = SS_TIMEOUT; //wait for 5 ms for SS to stabilize
                GLCD_Info.State = GLCD_SS_WAIT;
			}
			break;
        case GLCD_IDLE:
            if(GLCD_Info.Comm_Timeout_ms == 0)
            {
                count_rx = 0;
                GLCD_Info.State = GLCD_COMPARE_DATA;
				if(GLCD_Info.GLCD_Disp == DISPLAY1 && HA_config == 1)
				{
					GLCD_Info.GLCD_Disp = DISPLAY2;
				}
				else
				{
					GLCD_Info.GLCD_Disp = DISPLAY1;
				}
            }
            break;
        case GLCD_COMPARE_DATA:
         
				if(GLCD_Info.GLCD_Disp == DISPLAY1)
				{
					G_SPI_SS = SET_LOW;
                    G_SPI_SS2 = SET_HIGH;
				}
				else 
				{
					G_SPI_SS2 = SET_LOW;
                    G_SPI_SS = SET_HIGH;
				}
                GLCD_Info.Comm_Timeout_ms = SS_TIMEOUT; //wait for 10 ms for SS to stabilize
                GLCD_Info.State = GLCD_SS_WAIT;
            
                break;
        case GLCD_SS_WAIT:
            if(GLCD_Info.Comm_Timeout_ms == 0)
            {
                GLCD_Info.State = GLCD_INIT_DATA;
				
            }
            break;
		case GLCD_INIT_DATA:
            Build_packet_GLCD();            
            GLCD_Info.Packet_Max_length = MAX_G_PACKET_LEN;
            GLCD_Info.State = GLCD_TX_DATA;
            GLCD_Info.Packet_index = 0;
			break;
		case GLCD_TX_DATA:            
            //send 8 bytes through SPI1
            //SPI1BUF = data;
            if(GLCD_Info.Comm_Timeout_ms != 0)
                break;
            while((SPI1STATbits.SPITBF == 0) && (GLCD_Info.Packet_index < GLCD_Info.Packet_Max_length))//if (BF == 0)					   /* Still all Bytes are not completely transmitted */
            {
                if(GLCD_Info.GLCD_Disp == DISPLAY2 /*&& GLCD_Info.Message_Buffer[REDUNDANT_NW_INDEX][0]!=0*/){
//                    /* SSPBUF empty - Transmit Status, So we can Put the Next Byte */
                    SPI1BUF = GLCD_Info.Message_Buffer[REDUNDANT_NW_INDEX][GLCD_Info.Packet_index];//GLCD_Info.Packet_index; for testing
                       
                }else if(GLCD_Info.GLCD_Disp == DISPLAY1/* && GLCD_Info.Message_Buffer[Interested_Nw_Index][0]!=0*/) {
//                    PORTe can Put the Next Byte */
                    SPI1BUF = GLCD_Info.Message_Buffer[Interested_Nw_Index][GLCD_Info.Packet_index];//GLCD_Info.Packet_index; for testing
                }    
                GLCD_Info.Packet_index++;                
            }
			if((GLCD_Info.Packet_index >= GLCD_Info.Packet_Max_length) && (SPI1STATbits.SRMPT==1))
            {
			    GLCD_Info.Comm_Timeout_ms = TX_TIMEOUT;
				GLCD_Info.State = GLCD_CM_WAIT;
			}
			
            break;
           
        case GLCD_CM_WAIT:
            if(SPI1STATbits.SPIRBF==1)
            {
                while(SPI1STATbits.SRXMPT==0)
                {
                    GLCD_Info.Rx_Message_Buffer[count_rx] = (BYTE)SPI1BUF;
                    count_rx++;
                }
                if((count_rx%8)==0)
                {
                    if(GLCD_Info.Packet_index >= GLCD_Info.Packet_Max_length)
                    {
                        GLCD_Info.Comm_Timeout_ms = MAX_COMM_TIMEOUT;
                        GLCD_Info.State = GLCD_IDLE;
                        G_SPI_SS = SET_HIGH;
                        G_SPI_SS2 = SET_HIGH;
                    }
                    else
                    {
                        GLCD_Info.Comm_Timeout_ms = TX_TIMEOUT;
                        GLCD_Info.State = GLCD_TX_DATA;
                    }
                }
                }
if(GLCD_Info.Comm_Timeout_ms == 0)
            {
				//end of transmission
//				if((GLCD_Info.GLCD_Disp == DISPLAY1) && (GLCD_Info.GLCD2_On_state==1) && (HA_config==1))
//				{
//					G_SPI_SS = SET_HIGH;
//                    G_SPI_SS2 = SET_LOW;
//					GLCD_Info.GLCD_Disp = DISPLAY2;
//					GLCD_Info.State = GLCD_COMPARE_DATA;
//				}
//				else if(GLCD_Info.GLCD_Disp == DISPLAY2)
//				{
//					G_SPI_SS2 = SET_HIGH;
//                    G_SPI_SS = SET_LOW;
//					GLCD_Info.GLCD_Disp = NO_LCD_DISP;
//					GLCD_Info.Comm_Timeout_ms = MAX_COMM_TIMEOUT;
//					GLCD_Info.State = GLCD_IDLE;
//				}
//                else
//                {
					GLCD_Info.Comm_Timeout_ms = MAX_COMM_TIMEOUT;
					GLCD_Info.State = GLCD_IDLE;                    
//                }
			}
            
            break;
        default:
			break;
	}
}
/*********************************************************************************
File name 			:drv_lcd.c
Function Name		:void Decrement_LCD_msTmr(void)
Created By			:Sudheer Herle 
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
extern dac_sysinfo_t DAC_sysinfo;

unsigned char u_count,s_count;
extern BYTE CPU1_data_GLCD[MAX_SMCPU][GCPU_SPI_MESSAGE_LENGTH];			/* Buffer to store data recived from Cpu1 */
extern BYTE CPU2_data_GLCD[MAX_SMCPU][GCPU_SPI_MESSAGE_LENGTH];			/* Buffer to store data recived from Cpu2 */

void Build_packet_GLCD(void)
{
    unsigned char track;
    //clear the buffer
    NID = GLCD_Info.Build_network_index;
//    for(s_count =0; s_count<MAX_SMCPU; s_count++){
       for(u_count = 0;u_count<(MAX_G_PACKET_LEN);u_count++)
        {           
            GLCD_Info.Message_Buffer[NID][u_count]    = 0;
        } 
//    GLCD_Info.Message_Buffer[NID][21]  = 0xAA;
//    return;
//    }    

//    for(NID =0 ; NID < MAX_SMCPU;NID ++){
    
    for(u_count = 0;u_count<8;u_count++) // blue
    {
        GLCD_Info.Message_Buffer[NID][u_count]    = CPU1_data_GLCD[NID][u_count];
        GLCD_Info.Message_Buffer[NID][u_count+CPU1_PACKET_LEN] = CPU2_data_GLCD[NID][u_count];
    }
    track = u_count;//8 - 21
    for(;u_count<21;u_count++) //yellow
    {
        GLCD_Info.Message_Buffer[NID][u_count]    = CPU1_data_GLCD[NID][u_count + 24 - track];
        GLCD_Info.Message_Buffer[NID][u_count + CPU1_PACKET_LEN] = CPU2_data_GLCD[NID][u_count + 24 - track];
    }
        GLCD_Info.Message_Buffer[NID][u_count]    = CPU1_data_GLCD[NID][77];                  //Message ID
        GLCD_Info.Message_Buffer[NID][u_count + CPU1_PACKET_LEN] = CPU2_data_GLCD[NID][77];   //Message ID
        u_count++;
    track = u_count;
    for(;u_count<55;u_count++) //yellow     //u_count will start from 22 to 54 //Disp Count
    {
        GLCD_Info.Message_Buffer[NID][u_count]    = CPU1_data_GLCD[NID][u_count + 37 - track];
        GLCD_Info.Message_Buffer[NID][u_count + CPU1_PACKET_LEN] = CPU2_data_GLCD[NID][u_count + 37 - track];
    }
    GLCD_Info.Message_Buffer[NID][OFFSET_CODE_CHECKSUM] =  CPU1_data_GLCD[NID][72];// DAC_sysinfo.Checksum.DWord.HiWord.Byte.Hi;
    GLCD_Info.Message_Buffer[NID][OFFSET_CODE_CHECKSUM + 1] = CPU1_data_GLCD[NID][73];//DAC_sysinfo.Checksum.DWord.HiWord.Byte.Lo;
    GLCD_Info.Message_Buffer[NID][OFFSET_CODE_CHECKSUM + 2] = CPU1_data_GLCD[NID][74];//DAC_sysinfo.Checksum.DWord.LoWord.Byte.Hi;
    GLCD_Info.Message_Buffer[NID][OFFSET_CODE_CHECKSUM + 3] = CPU1_data_GLCD[NID][75];// DAC_sysinfo.Checksum.DWord.LoWord.Byte.Lo;
    GLCD_Info.Message_Buffer[NID][OFFSET_UNIT_TYPE] = CPU1_data_GLCD[NID][70];
    
//    if(CPU1_data_GLCD[70]==1 || CPU2_data_GLCD[70]==1) //Adjustment for LCWS
//    {
//       GLCD_Info.Message_Buffer[OFFSET_UNIT_TYPE] = 9;
//    }
    GLCD_Info.Message_Buffer[NID][OFFSET_SW_V] = DAC_sysinfo.SW_Version;
    Update_SMPU_data();
//    GLCD_Info.Message_Buffer[OFFSET_CRC    ]   = 0xAA;
//    GLCD_Info.Message_Buffer[OFFSET_CRC + 1]   = 0xAA;
//    GLCD_Info.Message_Buffer[NID][OFFSET_UNUSED + 1] = CPU1_data_GLCD[NID][71];
//    GLCD_Info.Message_Buffer[NID][OFFSET_UNUSED + 2] = CPU1_data_GLCD[NID][72];
//    GLCD_Info.Message_Buffer[NID][OFFSET_UNUSED + 3] = CPU1_data_GLCD[NID][73];
    GLCD_Info.Message_Buffer[NID][OFFSET_SMCPU_CRC + 5 + 3] = 0xAA;
    GLCD_Info.Message_Buffer[NID][OFFSET_SMCPU_CRC + 5 + 4] = 0xAA;
//    CheckSum_G.Word = Crc16(GLCD_INFO, MAX_G_PACKET_LEN - 2);
    GLCD_Info.Message_Buffer[NID][OFFSET_CRC]   = 0;
    GLCD_Info.Message_Buffer[NID][OFFSET_CRC + 1]   = 0;
//    GLCD_Info.Message_Buffer[13] = Event_Logger_ID ;
//    GLCD_Info.Message_Buffer[14] = 0;
//    GLCD_Info.Message_Buffer[15] = (High_Availability | (GSM_Sch_Info.GSM_Enable<<1)) & 0x03;
//    GLCD_Info.Message_Buffer[16] = 0;
    
//}
    GLCD_Info.Build_network_index++;
    if(GLCD_Info.Build_network_index >= Network_config){
        GLCD_Info.Build_network_index = 0;
    }
    if(Network_status[NID] == 0){
        GLCD_Info.Message_Buffer[NID][21] = 0x3C;
    }
//    GLCD_Info.Message_Buffer[NID][136] = 0;
//    GLCD_Info.Message_Buffer[NID][137] = 0;
//    GLCD_Info.Message_Buffer[NID][138]= 0;
//    GLCD_Info.Message_Buffer[NID][139]= 0;
            
    Glcd_csum.Word = Crc16(GLCD_INFO, GLCD_Info.Packet_Max_length-2);
    GLCD_Info.Message_Buffer[NID][142]   = Glcd_csum.Byte.Lo;
    GLCD_Info.Message_Buffer[NID][142 + 1]   = Glcd_csum.Byte.Hi;            
}


void Update_SMPU_data(void)
{
        
    BYTE uchBuf;
    event_record_t count_record;
    long_t S_Time, E_Time, Event_count, P_Time, SMCPU_CRC;
    for (uchBuf = 0; uchBuf <	EVENT_RECORD_SIZE; uchBuf++)
    {
        Event_Record_R.Byte[uchBuf] = 0;
    }
    E_status = Read_Event_from_Serial_EEProm(Events_Ring_Buffer.Head);
    for (uchBuf = 0; uchBuf <	EVENT_RECORD_SIZE; uchBuf++)
    {
        count_record.Byte[uchBuf] = Event_Record_R.Byte[uchBuf];
    }
    if(count_record.Field.Token == LATEST_EVENT_TOKEN)
    {
        // This is the latest recorded event
        E_Time.LWord = count_record.Field.Date_Time;
    }
    else
    {   // 0 if there is any error
        E_Time.LWord = 0x0000;
    }
    for (uchBuf = 0; uchBuf <	EVENT_RECORD_SIZE; uchBuf++)
    {
        Event_Record_R.Byte[uchBuf] = 0;
    }
    E_status = Read_Event_from_Serial_EEProm(Events_Ring_Buffer.Head + 1);
    for (uchBuf = 0; uchBuf <	EVENT_RECORD_SIZE; uchBuf++)
    {
        count_record.Byte[uchBuf] = Event_Record_R.Byte[uchBuf];
    }
    if(count_record.Field.Token != LATEST_EVENT_TOKEN || count_record.Field.Token != OLD_EVENT_TOKEN)
    {
        // record 0 is the 1st event.
        for (uchBuf = 0; uchBuf <	EVENT_RECORD_SIZE; uchBuf++)
        {
            Event_Record_R.Byte[uchBuf] = 0;
        }
        E_status = Read_Event_from_Serial_EEProm(0);
        for (uchBuf = 0; uchBuf <	EVENT_RECORD_SIZE; uchBuf++)
        {
            count_record.Byte[uchBuf] = Event_Record_R.Byte[uchBuf];
        }
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
        GLCD_Info.Message_Buffer[NID][OFFSET_EVENT_COUNT]     = Event_count.Byte.Byte1; 			   /* Send nuber of events */
	GLCD_Info.Message_Buffer[NID][OFFSET_EVENT_COUNT + 1] = Event_count.Byte.Byte2;
	GLCD_Info.Message_Buffer[NID][OFFSET_EVENT_COUNT + 2] = Event_count.Byte.Byte3;
	GLCD_Info.Message_Buffer[NID][OFFSET_EVENT_COUNT + 3] = Event_count.Byte.Byte4;
        GLCD_Info.Message_Buffer[NID][OFFSET_START_TIME   ]  = S_Time.Byte.Byte1; 			   /* First event time */
	GLCD_Info.Message_Buffer[NID][OFFSET_START_TIME + 1]  = S_Time.Byte.Byte2;
	GLCD_Info.Message_Buffer[NID][OFFSET_START_TIME + 2]  = S_Time.Byte.Byte3;
	GLCD_Info.Message_Buffer[NID][OFFSET_START_TIME + 3]  = S_Time.Byte.Byte4;
        GLCD_Info.Message_Buffer[NID][OFFSET_END_TIME      ]  = E_Time.Byte.Byte1; 			   /* End event time */
	GLCD_Info.Message_Buffer[NID][OFFSET_END_TIME + 1  ]  = E_Time.Byte.Byte2;
	GLCD_Info.Message_Buffer[NID][OFFSET_END_TIME + 2  ]  = E_Time.Byte.Byte3;
	GLCD_Info.Message_Buffer[NID][OFFSET_END_TIME + 3  ]  = E_Time.Byte.Byte4;

        P_Time.LWord = SystemClock;
        GLCD_Info.Message_Buffer[NID][OFFSET_PRESENT_TIME     ]  = P_Time.Byte.Byte1;
        GLCD_Info.Message_Buffer[NID][OFFSET_PRESENT_TIME + 1 ]  = P_Time.Byte.Byte2;
        GLCD_Info.Message_Buffer[NID][OFFSET_PRESENT_TIME + 2 ]  = P_Time.Byte.Byte3;
        GLCD_Info.Message_Buffer[NID][OFFSET_PRESENT_TIME + 3 ]  = P_Time.Byte.Byte4;

//        SMCPU_CRC.LWord = 0x87654321;//substitute with code crc
        SMCPU_CRC.LWord = (long)SMCPU_CRC_checksum;
        GLCD_Info.Message_Buffer[NID][OFFSET_SMCPU_CRC     ]  = SMCPU_CRC.Byte.Byte1;
        GLCD_Info.Message_Buffer[NID][OFFSET_SMCPU_CRC + 1 ]  = SMCPU_CRC.Byte.Byte2;
        GLCD_Info.Message_Buffer[NID][OFFSET_SMCPU_CRC + 2 ]  = SMCPU_CRC.Byte.Byte3;
        GLCD_Info.Message_Buffer[NID][OFFSET_SMCPU_CRC + 3 ]  = SMCPU_CRC.Byte.Byte4;
        GLCD_Info.Message_Buffer[NID][132]  = NID + 1;
        GLCD_Info.Message_Buffer[NID][133]  = 0x3c;       //Source ID that means it is from reset box

        GLCD_Info.Message_Buffer[NID][134] = CPU1_data_GLCD[NID][70];
        GLCD_Info.Message_Buffer[NID][135] = CPU1_data_GLCD[NID][71];
        GLCD_Info.Message_Buffer[NID][136] = CPU1_data_GLCD[NID][72];
        GLCD_Info.Message_Buffer[NID][137] = CPU1_data_GLCD[NID][73];
        
        
//        CheckSum.Word = Crc16(puchMsg,14);				/* Calculate CRC 16 for Xmit Buffer */

}

//BOOL Compare_Data_Sent_prev()
//{
//    if(1)       // get in if there is change in packet sent previously and now.
//    {
//        return TRUE;
//    }
//    return FALSE;
//}
