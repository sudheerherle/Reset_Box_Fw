/*********************************************************************************************************************
	Project		: 	Single Section Digital Axle Counter
	Version		: 	2.0
	Revision	:	1
	Filename	: 	comm_smc.c
	Target MCU	: 	PIC24FJ256GB210
    Compiler	: 	XC16 Compiler V1.21
	Author		:	Sudheer Herle
	Date		:
	Company Name: 	Insys Digital Systems
	Modification History:
					Rev No			Date		Description
					 --				 --				--
	Functions	:   void Initialise_Smc_CommSch(void)
					void SetupCOM1BaudRate(BYTE uchBaudRate)
					void Start_Smc_Communication(void)
					void Update_Smc_Sch_State(void)
					void Decrement_Smc_Sch_msTmr(void)
					void Clear_COM1_Receive_Buffer(void)
					void Clear_Com1_Error(void)

*********************************************************************************************************************/
#include <xc.h>
#include <time.h>
#include <limits.h>

#include "COMMON.h"
#include "command_proc.h"
#include "COMM_SMC.h"
#include "DRV_MEM.h"
#include "EVENTS.h"
#include "CRC16.h"
#include "DRV_LED.H"
#include "DRV_GLCD_SPI.h"

extern  sm_status_t Status;					/* From cpu_sm.c */
const longtype_t cpu_crc;
extern time_t SystemClock;
extern const BYTE uchCommand_Length_Table[7][2];	/* From command_proc.c */
extern const BYTE BitMask_List[8];					/* From cpu_sm.c */
unsigned char No_smcpu = 0;
BYTE Selected_CPU_Addr = 1;
extern dac_sysinfo_t DAC_sysinfo;					/* from cpu_sm.c */
//event_register_t Shadows[MAXIMUM_NO_OF_CPU]; /* shadow will have present status of Both Cpus */
smc_sch_info_t             Smc_Sch_Info;			/* structure holds station master interface communication scheduler */
smc_info_t					Smc1XmitObject;
query_info_t QueryInfObject;
UINT16 temp_crc_SPI;
extern system_t SystemObject;
extern glcd_info_t GLCD_Info;
//extern rb_info_t RB_Info;					/* from ACC_RB.C */
extern dac_comm_error Dac_Comm_Err;				/* Dac external communication CRC error count information */
//unsigned char System_error_code;
unsigned char temp8 = 0, temp18=0;
event_register_t Shadow[MAX_SMCPU][MAXIMUM_NO_OF_CPU]; /* shadow will have present status of Both Cpus */
const unsigned char uchStatusLookup[13][2] =
	{{ 0, 0}, { 1, 1}, { 1, 2}, { 2, 1}, { 2, 2},
	 { 3, 3}, { 4, 4}, { 5, 5}, { 6, 6}, { 6, 2},
	 { 2, 6}, { 6, 1}, { 1, 6}
	};

static BYTE uchSelectedCPU = 1;

void Update_Shadow_Register(BYTE);
void Synchronise_Shadow_Register(void);
void Update_Comm_Err_Counter(BYTE);


extern  BYTE CPU1_Address;				/* from cpu_sm.c */
extern  BYTE CPU2_Address;				/* from cpu_sm.c */
extern BYTE Network_config;
BYTE Network_ID;
extern BYTE Network_status[MAX_SMCPU];
BYTE var_count, SF_Error,EF_Error;
disp_info_t Disp_Info;
BYTE CPU1_data_GLCD[MAX_SMCPU][GCPU_SPI_MESSAGE_LENGTH];			/* Buffer to store data recived from Cpu1 */
BYTE CPU2_data_GLCD[MAX_SMCPU][GCPU_SPI_MESSAGE_LENGTH];			/* Buffer to store data recived from Cpu2 */
extern char inspect_CPU1_data_done, inspect_CPU2_data_done, inspect_DAC_info_done;
char CPU1_System_error,CPU2_System_error;
BYTE Latest_DAC_data[MAX_SMCPU][SPI_MESSAGE_LENGTH];	/* From comm_dac.c */
BYTE CPU1_data[MAX_SMCPU][SPI_MESSAGE_LENGTH];			/* Buffer to store data recived from Cpu1 */
BYTE CPU2_data[MAX_SMCPU][SPI_MESSAGE_LENGTH];			/* Buffer to store data recived from Cpu2 */

void SetupCOM1BaudRate(BYTE);
void Clear_Com1_Error(void);
void Clear_COM1_Receive_Buffer(void);
void Set_Smc_Sch_Idle(void);
void Build_smc_broadcast_message(void);
void Initialise_Modem(void);
void Set_Modem_TX_Mode(void);
void Set_Modem_RX_Mode(void);
void Process_SM_Message(BYTE uchCPU_ID);
void Save_DS_Data(void);
void Save_US_Data(void);
void Save_3DP1S_A_Data(void);
void Save_3DP1S_B_Data(void);
void Save_3DP1S_C_Data(void);

/*********************************************************************************
File name 			:comm_smc.c
Function Name		:void Initialise_Smc_CommSch(void)
Created By			:Sudheer Herle
Date Created		:
Modification History:
					Rev No			Date		Description
					 --				 --				--
Tracability:
		SRS()    	:

Abstract			:Initialise smc communication scheduler .
Algorithm			:
Description			:
Input Element		:None
Output Element		:void

**********************************************************************************/
void Initialise_Smc_CommSch(void)
{
    Set_Modem_RX_Mode();
    Smc_Sch_Info.Query.CPU_Index = 1;
    Smc_Sch_Info.Query.Network_Index = 1;
    Smc_Sch_Info.Query.Header_byte =  QUERY_HEADER;
	Smc_Sch_Info.State      = SMC_SCHEDULE_DECIDE;		 /*  set smc comm scheduler to "SMC_SCHEDULER_NOT_STARTED" state  */
    Smc_Sch_Info.Timeout_ms = 5000;
	Smc_Sch_Info.ScanPeriod = SMC_COMM_SCHEDULER_SCAN_RATE;
	Smc_Sch_Info.BytePeriod = BYTE_PERIOD_9MS;				     /* set byte PERIOD to 1ms for 1200 Baud Rate* */
	SetupCOM1BaudRate((BYTE)BAUDRATE_1200);						     /*  set smc comm baudrate to 1200bps */	
	Smc_Sch_Info.Setup_Timeout_ms = MODEM_TRC_ON_TIMEOUT;
	Status.Flags.MDM_TX_Mode = SET_LOW;
}
/*********************************************************************************
File name 			:comm_smc.c
Function Name		:void SetupCOM1BaudRate(BYTE uchBaudRate)
Created By			:Sudheer Herle
Date Created		:
Modification History:
					Rev No			Date		Description
					 --				 --				--
Tracability:
		SRS()    	:

Abstract			:sets Baudrate for smc communication
Algorithm			:
Description			:
Input Element		:uchBaudRate
Output Element		:void

**********************************************************************************/

void SetupCOM1BaudRate(BYTE uchBaudRate)
{
     // Below code is for display uart
    TRISFbits.TRISF4 = 0;       // RF5 is Tx, Output
    TRISFbits.TRISF5 = 1;       // RF4 is Rx, Input
    
    TRISFbits.TRISF13 = 0;

    TRISBbits.TRISB9 = 0;       //M0
    TRISBbits.TRISB10 = 0;      //M1


    /* Configure Remappable pins */
    //*************************************************************
    // Unlock Registers
    //*************************************************************
    __builtin_write_OSCCONL(OSCCON & 0xbf); //clear the bit 6 of OSCCONL to unlock Pin Re-map
//        RPINR18bits.U1RXR = 10;
//        RPOR8bits.RP17R = 3;
          RPINR18bits.U1RXR = 17;       // For communication with SMCPU. DO not change
          RPOR5bits.RP10R = 3;          // For communication with SMCPU. DO not change COM1

          RPINR19bits.U2RXR  = 30;      // For communication with Android
          RPOR8bits.RP16R = 5;          // For communication with Android COM2
          
          RPOR15bits.RP31R = 28;
//                iPPSInput(IN_FN_PPS_U1RX,IN_PIN_PPS_RP10);        //RP25 - RD4 - SDI
//        iPPSOutput(OUT_PIN_PPS_RP17,OUT_FN_PPS_U1TX);    //RP20 - RD5 - SCK

    //************************************************************
    // Lock Registers
    //************************************************************
    __builtin_write_OSCCONL(OSCCON | 0x40); //set the bit 6 of OSCCONL to lock Pin Re-map
    //******************************************************************


    U1MODE = 0;
    U1STA = 0;
    
    U1MODEbits.RTSMD = 1;   // Operate in Simplex Mode

    U1MODEbits.BRGH = 0;    // Low Speed Clocks to BRG @ 16x of Fp
    U1BRG = 3332;
    /*
     * FCY = FOSC/2 = 32MHz/2 = 16Mz
     * U1BRG =FCY/(16 * Baud Rate) - 1
     * U1BRG = 16000000/(16*1200) - 1
     * U1BRG = 3332
     */
    

    IEC4bits.U1ERIE = 0;
    IEC0bits.U1RXIE = 0;
    IEC0bits.U1TXIE = 0;

    IFS4bits.U1ERIF = 0;
    IFS0bits.U1RXIF = 0;
    IFS0bits.U1TXIF = 0;
    //U1MODEbits.LPBACK = 1;
    U1MODEbits.UARTEN = 1;  // Enable UART
    U1STAbits.UTXEN = 1;

    U2MODE = 0;
    U2STA = 0;

    U2MODEbits.RTSMD = 1;   // Operate in Simplex Mode

    U2MODEbits.BRGH = 1;    // Low Speed Clocks to BRG @ 16x of Fp

    /*
     * FCY = FOSC/2 = 32MHz/2 = 16Mz
     * U1BRG =FCY/(16 * Baud Rate) - 1
     * U1BRG = 16000000/(16*1200) - 1
     * U1BRG = 3332
     */
   
    U2BRG = 104; // 38400 baud rate

    IEC4bits.U2ERIE = 0;
    IEC1bits.U2RXIE = 0;
    IEC1bits.U2TXIE = 0;

    IFS4bits.U2ERIF = 0;
    IFS1bits.U2RXIF = 0;
    IFS1bits.U2TXIF = 0;
    //U1MODEbits.LPBACK = 1;
    U2MODEbits.UARTEN = 1;  // Enable UART


	switch (uchBaudRate)
	{
		case BAUDRATE_1200:
			U1BRG = SPBRG_FOR_1200;
			break;
		case BAUDRATE_2400:
			U1BRG = SPBRG_FOR_2400;
                        break;
		case BAUDRATE_9600:
			U1BRG = SPBRG_FOR_9600;
			break;
		case BAUDRATE_19200:
			U1BRG = SPBRG_FOR_19200;
			break;
	}

    IEC4bits.U1ERIE = 0;
    IEC0bits.U1RXIE = 0;
    IEC0bits.U1TXIE = 0;

    IFS4bits.U1ERIF = 0;
    IFS0bits.U1RXIF = 0;
    IFS0bits.U1TXIF = 0;
    //U1MODEbits.LPBACK = 1;
    U1MODEbits.UARTEN = 1;  // Enable UART

}
/*********************************************************************************
File name 			:comm_smc.c
Function Name		:void Start_Smc_Communication(void)
Created By			:Sudheer Herle
Date Created		:
Modification History:
					Rev No			Date		Description
					 --				 --				--
Tracability:
		SRS()    	:

Abstract			:This function starts smc comm scheduler.
Algorithm			:
Description			:
Input Element		:None
Output Element		:void

**********************************************************************************/
void Start_Smc_Communication(void)
{
	/*
	 * We use this channel only to transmit data periodically
	 * to Reset Box.
	 */
//	U1STAbits.UTXEN = SET_HIGH;	/* Enable Transmitter */
	//CREN = SET_LOW;		/* Disable Receive */
    Smc_Sch_Info.State =  SMC_SCHEDULE_DECIDE;
	Smc_Sch_Info.Timeout_ms = SM_COMM_LINK_FAILURE_TIMEOUT;
}
/*********************************************************************************
File name 			:comm_smc.c
Function Name		:void Update_Smc_Sch_State(void)
Created By			:Sudheer Herle
Date Created		:
Modification History:
					Rev No			Date		Description
					 --				 --				--
Tracability:
		SRS()    	:

Abstract			:Smc communication scheduler to Interface with Station Master through FSK Modem.
Algorithm			:
					 1.Initially Smc comm scheduler will be in Idle state
					 2.It will Change scheduler state to "SET_MODEM_RS_LOW"
					 3.In "SET_MODEM_RS_LOW" RS signal is made Low and  then goes to "WAIT_FOR_CS_LOW" state.
					 5.After CS goes Low, Scheduler go to "WAIT_FOR_CARRIER_STABALISE" State.
					 6.Once the "TRANSMIT_WAIT_TIME" or Carrier Stabilisation Time completed,build the messages need to send and
					   Scheduler goes to Next State Value to "TRANSMIT_SMC_DATA" state.
					 7.In "TRANSMIT_SMC_DATA" State All Records available in Xmit Queue is Transmitted.
					   Once All Records Transmitted, Scheduler state is set to "HOLD_SMC_MODEM_RS_LOW"
					 8.In "HOLD_SMC_MODEM_RS_LOW" the Scheduler state is set to "SMC_COMM_SCHEDULER_IDLE"
					 9.Once the timeout completed the Schedular will start next Transmission
Description			:
Input Element		:None
Output Element		:void

**********************************************************************************/
unsigned char CD_check=1;

void Update_Smc_Sch_State(void)
{
        BYTE uchData=0;
        wordtype_t query_crc;
        
               
	switch (Smc_Sch_Info.State)
	{
		case SMC_SCHEDULE_DECIDE:
			if(Smc_Sch_Info.Timeout_ms != TIMEOUT_EVENT)//check for  5000ms
			{
				//check for CD
				if(MODEM_CD == 1)
				{
					//Listner
					Smc_Sch_Info.Timeout_ms = 8000;
					Smc_Sch_Info.State = SMC_LISTEN_SCHEDULER;
				}
			}
			else{
				//No CD be master
                Clear_Com1_Error(); 
                Set_Modem_TX_Mode();
                SetupCOM1BaudRate((BYTE)BAUDRATE_1200);
                Smc_Sch_Info.Timeout_ms = 100;
				Smc_Sch_Info.State = SMC_SCHEDULER_NOT_STARTED;
			}
			
			break;
		case SMC_SCHEDULER_NOT_STARTED:          
            LATGbits.LATG14 = 1;
            if(Smc_Sch_Info.Timeout_ms != TIMEOUT_EVENT){
                return;
            }
            LATGbits.LATG14 = 0;
            Set_Modem_TX_Mode();
            SetupCOM1BaudRate((BYTE)BAUDRATE_1200);
            Clear_Com1_Error(); 
            Smc_Sch_Info.Timeout_ms = DELAY_20MS;
            Smc_Sch_Info.State = WAIT_FOR_TX_MODE;
            break;
            
        case WAIT_FOR_TX_MODE:
            if(Smc_Sch_Info.Timeout_ms != TIMEOUT_EVENT)
                return;                      
            QueryInfObject.Index = 0; 
            Smc_Sch_Info.State = SEND_QUERY;            
			break;
            
        case SEND_QUERY: 
            for(uchData=0;uchData<QUERY_SIZE;uchData++){
                QueryInfObject.query_buf[uchData] = Smc_Sch_Info.Query.Query_buffer[uchData];
            }
            QueryInfObject.Msg_Length = QUERY_SIZE;
            query_crc.Word = Crc16(SMC_QUERY,QueryInfObject.Msg_Length-2);
            QueryInfObject.query_buf[3] = query_crc.Byte.Lo;
            QueryInfObject.query_buf[4] = query_crc.Byte.Hi;
            if ((U1STAbits.TRMT==1 && U1STAbits.UTXBF == 0) && (QueryInfObject.Index < 5))
			{
				IFS0bits.U1TXIF = 0;	
				while((U1STAbits.UTXBF == 0) && QueryInfObject.Index < (5))
				{
					U1TXREG = QueryInfObject.query_buf[QueryInfObject.Index];
					QueryInfObject.Index = QueryInfObject.Index + 1;
				}
			}
            if(QueryInfObject.Index==QUERY_SIZE && U1STAbits.TRMT==1){
                    Smc_Sch_Info.Timeout_ms = DELAY_20MS;
                    Smc_Sch_Info.State = SET_MODEM_RX;
            }          
            
            break;
            
        case SET_MODEM_RX:
            if(Smc_Sch_Info.Timeout_ms != TIMEOUT_EVENT)
                return;
            if(Smc_Sch_Info.Query.Network_Index > Network_config){
                Smc_Sch_Info.Query.Network_Index = 1;
                Smc_Sch_Info.QUERY_WAIT_FOR_OTHER_PARTY = 1;
            }
            if(Smc_Sch_Info.Query.CPU_Index == 1){
                Smc_Sch_Info.Query.CPU_Index = 2;
            }
            else if(Smc_Sch_Info.Query.CPU_Index == 2){
                Smc_Sch_Info.Query.CPU_Index = 1;
                Smc_Sch_Info.Query.Network_Index++;
                    if(Smc_Sch_Info.Query.Network_Index > Network_config){
                    Smc_Sch_Info.Query.Network_Index = 1;
                    Smc_Sch_Info.QUERY_WAIT_FOR_OTHER_PARTY = 1;
                    }
            }else{
                
            }
            Set_Modem_RX_Mode();
            Smc_Sch_Info.Timeout_ms = DELAY_20MS;
//            if(Smc_Sch_Info.QUERY_WAIT_FOR_OTHER_PARTY == 1){
//                Smc_Sch_Info.State = WAIT_FOR_OTHER_PARTY;
//                Smc_Sch_Info.Timeout_ms = DELAY_10_SEC;
//            }
//            else 
                Smc_Sch_Info.State = CHECK_FOR_CD_WAIT;
            break;
//        case  WAIT_FOR_OTHER_PARTY:
//            if(Smc_Sch_Info.Timeout_ms != TIMEOUT_EVENT)
//                return;
//            else Smc_Sch_Info.State = CHECK_FOR_CD_WAIT;
//            break;
        case CHECK_FOR_CD_WAIT:
            if(Smc_Sch_Info.Timeout_ms != TIMEOUT_EVENT)
                return;
            else {
                Smc_Sch_Info.Timeout_ms = 1000;
                Smc_Sch_Info.State = CHECK_FOR_CD_STATUS;
            }
        case  CHECK_FOR_CD_STATUS:
            if(MODEM_CD == 1){
                Smc_Sch_Info.State =  SMC_NO_DATA_RECEIVED;
                Smc_Sch_Info.Timeout_ms = 1000;
                Smc1XmitObject.Index = 0;
                Smc1XmitObject.Msg_Length = SM_MESSAGE_LENGTH;
                Clear_Com1_Error();
            }else{
                SetupCOM1BaudRate((BYTE)BAUDRATE_1200);
                Clear_Com1_Error();
            }
            if(Smc_Sch_Info.Timeout_ms ==  TIMEOUT_EVENT)
                Smc_Sch_Info.State =  SMCPU_DEAD;
            break;
        case SMC_NO_DATA_RECEIVED:
//			if(Smc_Sch_Info.Timeout_ms != TIMEOUT_EVENT)
//                return;            
//            Clear_Com1_Error();
            if(IFS0bits.U1RXIF)
            {
                while(U1STAbits.URXDA)
				{
				uchData = U1RXREG;
				Smc1XmitObject.Msg_Buffer[Smc1XmitObject.Index] = uchData;
				Smc1XmitObject.Index = Smc1XmitObject.Index + 1;
				}
                IFS0bits.U1RXIF = 0;
            }
			if (Smc_Sch_Info.Timeout_ms == TIMEOUT_EVENT)
				{
				Smc_Sch_Info.State = SMCPU_DEAD;
				}
            if (Smc1XmitObject.Index >= SM_MESSAGE_LENGTH)
				{
				Smc_Sch_Info.State = SMC_DATA_RECEPTION_COMPLETE;
				Smc_Sch_Info.Timeout_ms = 0;
				break;
				}
		break;
		case SMC_DATA_RECEPTION_STARTED:
			if (U1STAbits.URXDA)
				{
				uchData = U1RXREG;
				Smc1XmitObject.Msg_Buffer[Smc1XmitObject.Index] = uchData;
				Smc1XmitObject.Index = Smc1XmitObject.Index + 1;
				Smc_Sch_Info.Timeout_ms = SM_DATA_RECEIVE_TIMEOUT;
				break;
				}
			if (Smc1XmitObject.Index >= SM_MESSAGE_LENGTH)
				{
				Smc_Sch_Info.State = SMC_DATA_RECEPTION_COMPLETE;
				Smc_Sch_Info.Timeout_ms = 0;
				break;
				}
			if (Smc_Sch_Info.Timeout_ms == TIMEOUT_EVENT)
				{
				Smc_Sch_Info.State = SMCPU_DEAD;
                Smc1XmitObject.Index = 0;
				}
			break;
		case SMC_DATA_RECEPTION_COMPLETE:
                        temp_crc_SPI = Crc16(SMC_OBJ, SPI_MESSAGE_LENGTH-2);
		if (temp_crc_SPI == ((UINT16)((UINT16)Smc1XmitObject.Msg_Buffer[SPI_MESSAGE_LENGTH-1]<<8)+(Smc1XmitObject.Msg_Buffer[SPI_MESSAGE_LENGTH-2])))
		{
                         //have to compare the first byte which is CPU address to 0 because SMCPU sends 80 bytes with 0 in it and it will match checksum too.
                                    BYTE f = QueryInfObject.query_buf[2];
                                Process_SM_Message(f);
                                Network_status[QueryInfObject.query_buf[1]-1] = 1;
                                if(Smc1XmitObject.Msg_Buffer[76] != 0x8 )
                                {
                                    if(Smc1XmitObject.Msg_Buffer[76]==0x18){
                                    temp18 = Smc1XmitObject.Msg_Buffer[76];
                                    }
                                    else{
                                    temp18=0;
                                    }
                                }
                                
                                if(Smc1XmitObject.Msg_Buffer[76] != 0x18 )
                                {
                                    if(Smc1XmitObject.Msg_Buffer[76]==0x8){
                                        temp8 = Smc1XmitObject.Msg_Buffer[76];
                                    }else{
                                        temp8=0;
                                    }
                                }
                                No_smcpu = 0;
                                BI_COLOR_GREEN_LED_PORT=1;
                                BI_COLOR_RED_LED_PORT =0;
                                
                                if(temp18==0x18 && temp8==0x8){                                    
                                    temp18=0;
                                    temp8=0;
                                }
//                                else
//                                {
//                                    GS_LED = 0;
//                                }
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
                                Smc_Sch_Info.State = SMC_SCHEDULER_NOT_STARTED;
                                Smc_Sch_Info.Timeout_ms = DELAY_10_SEC;
                                Smc1XmitObject.Index =0;
				Nop();
				}
                                else{
                                    BI_COLOR_GREEN_LED_PORT = 0;
                                    BI_COLOR_RED_LED_PORT = 1;
                                    Smc1XmitObject.Index = 0;
                                    Smc_Sch_Info.Timeout_ms = DELAY_1_SEC;
                                    Smc_Sch_Info.State = SMC_SCHEDULER_NOT_STARTED;
                                }
                        Set_Smc_Sch_Idle();
			break;
        case SMCPU_DEAD:
            if (Smc_Sch_Info.Timeout_ms == TIMEOUT_EVENT)
			{
                BI_COLOR_RED_LED_PORT = 1;
                BI_COLOR_GREEN_LED_PORT = 0;
                Network_status[QueryInfObject.query_buf[1]-1] = 0;
				Smc_Sch_Info.State =  SMC_SCHEDULER_NOT_STARTED;
			}
			Set_Smc_Sch_Idle();
			break;
			
		case SMC_LISTEN_SCHEDULER:
			if(MODEM_CD == 1)
			{
				Smc1XmitObject.Index = 0;
				Smc1XmitObject.Msg_Length = SM_MESSAGE_LENGTH;
				Clear_Com1_Error();
				Smc_Sch_Info.State = SMC_LISTEN_DATA;
			}
			else
			{
				Smc_Sch_Info.Timeout_ms = 5000;
				Smc_Sch_Info.State = SMC_SCHEDULE_DECIDE;
			}
			break;
		case SMC_LISTEN_DATA:
			if(MODEM_CD == 1)
			{
				if(IFS0bits.U1RXIF)
				{
					while(U1STAbits.URXDA)
					{
						uchData = U1RXREG;
						Smc1XmitObject.Msg_Buffer[Smc1XmitObject.Index] = uchData;
						Smc1XmitObject.Index = Smc1XmitObject.Index + 1;
					}
					IFS0bits.U1RXIF = 0;
				}
				if (Smc1XmitObject.Index >= SM_MESSAGE_LENGTH)
				{
					Smc_Sch_Info.State = SM_LISTEN_DATA_RECEPTION_COMPLETE;
					Smc_Sch_Info.Timeout_ms = 0;
					break;
				}
//                else{         // uncomment this to check if the CRC fail does not happen
//                    Smc1XmitObject.Index = 0;
//                    Clear_Com1_Error();
//                }
			}
			else
			{
//                SetupCOM1BaudRate(BAUDRATE_1200); //uncomment this to check if the CRC fail does not happen
//                Clear_Com1_Error();
				Smc_Sch_Info.Timeout_ms = 5000;
				Smc_Sch_Info.State = SMC_SCHEDULE_DECIDE;
			}	
			break;
		case SM_LISTEN_DATA_RECEPTION_COMPLETE:
             temp_crc_SPI = Crc16(SMC_OBJ, SPI_MESSAGE_LENGTH-2);
		if (temp_crc_SPI == ((UINT16)((UINT16)Smc1XmitObject.Msg_Buffer[SPI_MESSAGE_LENGTH-1]<<8)+(Smc1XmitObject.Msg_Buffer[SPI_MESSAGE_LENGTH-2])))
		{
                Process_SM_Message(uchSelectedCPU);
                if(Smc1XmitObject.Msg_Buffer[76] != 0x8 )
                {
                    if(Smc1XmitObject.Msg_Buffer[76]==0x18){
                    temp18 = Smc1XmitObject.Msg_Buffer[76];
                    }
                    else{
                    temp18=0;
                    }
                }
                
                if(Smc1XmitObject.Msg_Buffer[76] != 0x18 )
                {
                    if(Smc1XmitObject.Msg_Buffer[76]==0x8){
                        temp8 = Smc1XmitObject.Msg_Buffer[76];
                    }else{
                        temp8=0;
                    }
                }
                BI_COLOR_GREEN_LED_PORT=1;
                BI_COLOR_RED_LED_PORT =0;
                                
                if(temp18==0x18 && temp8==0x8){                                    
                    temp18=0;
                    temp8=0;
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
                Smc1XmitObject.Index =0;
                Smc_Sch_Info.State = SMC_LISTEN_SCHEDULER;
				Nop();
			}
            else{
                BI_COLOR_GREEN_LED_PORT = 0;
//                BI_COLOR_RED_LED_PORT = 1;
                Smc1XmitObject.Index = 0;
                Smc_Sch_Info.State = SMC_LISTEN_SCHEDULER;
            }
			break;
		default:
			Set_Smc_Sch_Idle();
			break;
	  }
}

//unsigned char CD_check=1;
void Update_Smc_Multiple_Sch_State(void)
{
        BYTE uchData=0;
        Set_Modem_RX_Mode();
        if(MODEM_CD == 1)
        {
            if(CD_check==1)
            {
                CD_check = 0;
                U1STA = 0;
                SetupCOM1BaudRate(BAUDRATE_1200);
                Clear_Com1_Error();
            }
            //start CD timer
        }
        else
        {
            Smc1XmitObject.Index = 0;
            Smc_Sch_Info.State = SMC_SCHEDULER_NOT_STARTED;
            CD_check = 1;
            return;
        }
	switch (Smc_Sch_Info.State)
	{

		case SMC_SCHEDULER_NOT_STARTED:
             SetupCOM1BaudRate(BAUDRATE_1200);
            Clear_Com1_Error();
            Set_Modem_RX_Mode();
//            Smc_Sch_Info.Timeout_ms = DELAY_20MS;
//            Smc_Sch_Info.State = CHECK_FOR_CD_WAIT;
//            
//			break;
//            
//            case CHECK_FOR_CD_WAIT:
            if(Smc_Sch_Info.Timeout_ms != TIMEOUT_EVENT)
                return;
            else {
                Smc_Sch_Info.Timeout_ms = DELAY_1_SEC;
                //Clear_Com1_Error();
                Smc_Sch_Info.State = SMC_NO_DATA_RECEIVED;
            }
            
            break;
            
            case SMC_NO_DATA_RECEIVED:
			if (U1STAbits.URXDA)
				{
				uchData = U1RXREG;
				Smc1XmitObject.Msg_Buffer[Smc1XmitObject.Index] = uchData;
				Smc1XmitObject.Index = Smc1XmitObject.Index + 1;
				//Smc_Sch_Info.Timeout_ms = SM_DATA_RECEIVE_TIMEOUT;
				break;
				}
			if (Smc1XmitObject.Index >= SM_MESSAGE_LENGTH)
				{
				Smc_Sch_Info.State = SMC_DATA_RECEPTION_COMPLETE;
//				Smc_Sch_Info.Timeout_ms = 0;
				break;
				}
			if (Smc_Sch_Info.Timeout_ms == TIMEOUT_EVENT)
				{
				Smc_Sch_Info.State = SMC_SCHEDULER_NOT_STARTED;
                Smc_Sch_Info.Timeout_ms = DELAY_100MS;
                Smc1XmitObject.Index = 0;
				}
			break;
		case SMC_DATA_RECEPTION_COMPLETE:
                        temp_crc_SPI = Crc16(SMC_OBJ, SPI_MESSAGE_LENGTH-2);
		if (temp_crc_SPI == ((UINT16)((UINT16)Smc1XmitObject.Msg_Buffer[SPI_MESSAGE_LENGTH-1]<<8)+(Smc1XmitObject.Msg_Buffer[SPI_MESSAGE_LENGTH-2])))
		{
                         //have to compare the first byte which is CPU address to 0 because SMCPU sends 80 bytes with 0 in it and it will match checksum too.
                                    //BYTE f = QueryInfObject.query_buf[2];
                                Process_SM_Message(uchSelectedCPU);
                                Network_status[0] = 1;
                                
                                if(Smc1XmitObject.Msg_Buffer[76] != 0x8 )
                                {
                                    if(Smc1XmitObject.Msg_Buffer[76]==0x18){
                                    temp18 = Smc1XmitObject.Msg_Buffer[76];
                                    }
                                    else{
                                    temp18=0;
                                    }
                                }
                                
                                if(Smc1XmitObject.Msg_Buffer[76] != 0x18 )
                                {
                                    if(Smc1XmitObject.Msg_Buffer[76]==0x8){
                                        temp8 = Smc1XmitObject.Msg_Buffer[76];
                                    }else{
                                        temp8=0;
                                    }
                                }
                                No_smcpu = 0;
                                BI_COLOR_GREEN_LED_PORT=1;
                                BI_COLOR_RED_LED_PORT =0;
                                
                                if(temp18==0x18 && temp8==0x8){                                    
                                    temp18=0;
                                    temp8=0;
                                }
//                                else
//                                {
//                                    GS_LED = 0;
//                                }
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
                                Smc_Sch_Info.State = SMC_SCHEDULER_NOT_STARTED;
                                Smc_Sch_Info.Timeout_ms = DELAY_100MS;
                                Smc1XmitObject.Index =0;
				Nop();
				}
                                else{
                                    BI_COLOR_GREEN_LED_PORT = 0;
                                    BI_COLOR_RED_LED_PORT = 1;
                                    Smc1XmitObject.Index = 0;
                                    Smc_Sch_Info.Timeout_ms = DELAY_100MS;
                                    Smc_Sch_Info.State = SMC_SCHEDULER_NOT_STARTED;
                                }
                        //Set_Smc_Sch_Idle();
			break;

            

		default:
			Set_Smc_Sch_Idle();
			break;
	  }
}

/*********************************************************************************
File name 			:comm_smc.c
Function Name		:void Set_Smc_Sch_Idle(void)
Created By			:Sudheer Herle
Date Created		:
Modification History:
					Rev No			Date		Description
					 --				 --				--
Tracability:
		SRS()    	:

Abstract			:Change Schedular State to "SMC_COMM_SCHEDULER_IDLE"
Algorithm			:
Description			:
Input Element		:None
Output Element		:void

**********************************************************************************/
void Set_Smc_Sch_Idle(void)
{
	Smc_Sch_Info.State =  SMC_SCHEDULER_NOT_STARTED;
	Smc_Sch_Info.Timeout_ms = DELAY_10_SEC;
}

/*********************************************************************************
File name 			:comm_smc.c
Function Name		:void Decrement_Smc_Sch_msTmr(void)
Created By			:Sudheer Herle
Date Created		:
Modification History:
					Rev No			Date		Description
					 --				 --				--
Tracability:
		SRS()    	:

Abstract			:one milliSecond Timer Variables present Smc_sch_info and
					 com1 receive object are decremented.
Algorithm			:
Description			:
Input Element		:None
Output Element		:void

**********************************************************************************/
void Decrement_Smc_Sch_msTmr(void)
{
	if (Smc_Sch_Info.Timeout_ms > 0)
	{
		Smc_Sch_Info.Timeout_ms = Smc_Sch_Info.Timeout_ms - 1;
	}
        if(Smc_Sch_Info.CD_timeout_ms > 0)
        {
            Smc_Sch_Info.CD_timeout_ms = Smc_Sch_Info.CD_timeout_ms - 1;
        }
}

/*********************************************************************************
File name 			:comm_smc.c
Function Name		:void Clear_Com1_Error(void)
Created By			:Sudheer Herle
Date Created		:
Modification History:
					Rev No			Date		Description
					 --				 --				--
Tracability:
		SRS()    	:

Abstract			:This function Clears the communication errors,if occured.
Algorithm			:
Description			:
Input Element		:None
Output Element		:void

**********************************************************************************/
void Clear_Com1_Error(void)
{
  BYTE uchCnt=0;

	if (U1STAbits.OERR)
	{
		/* Overrun Error! Clear the error */
        U1STAbits.OERR = 0;
		U1MODEbits.UARTEN = SET_LOW;
        U1MODEbits.UARTEN = SET_HIGH;
        IFS0bits.U1RXIF = 0;
	}
	if (U1STAbits.FERR)
	{
		/* Framing Error! Clear the error */
		uchCnt = (BYTE) U1RXREG;
        uchCnt = uchCnt;
        IFS0bits.U1RXIF = 0;
        U1MODEbits.UARTEN = SET_LOW;
        U1MODEbits.UARTEN = SET_HIGH;
	}
//    U1STAbits.UTXEN = SET_HIGH;
} 
 


void Initialise_Modem(void)
{
 M0 = SET_HIGH;
 M1 = SET_HIGH;
}

void Set_Modem_RX_Mode(void)
{
 M0 = SET_LOW;
 M1 = SET_HIGH;
}

void Set_Modem_TX_Mode(void)
{
 M0 = SET_HIGH;
 M1 = SET_LOW;
}

BYTE log_event, System_error, System_error_code;
void Process_SM_Message(BYTE uchCPU_ID)
{
        int i;
//	longtype_t Sys_Time;
//        dac_status_t DAC_Status;
        BYTE uchCommand = Smc1XmitObject.Msg_Buffer[MESSAGE_TYPE_OFFSET];
        Network_ID = Smc1XmitObject.Msg_Buffer[MESSAGE_TYPE_OFFSET - 1] - 1;
       if (uchCPU_ID == CPU1_ID)
	{
		/* If Data received from CPU1, then store into CPU1 Buffer */
		for(i =0 ;i<SPI_MESSAGE_LENGTH;i++)
		{
            CPU1_data[Network_ID][i] = Smc1XmitObject.Msg_Buffer[i];
            CPU1_data_GLCD[Network_ID][i] = Smc1XmitObject.Msg_Buffer[i];     
			Latest_DAC_data[Network_ID][i] = Smc1XmitObject.Msg_Buffer[i];
		}
//            if(CPU1_data_GLCD[0]==4){
//                int p = CPU1_data_GLCD[0];
//            }
            CPU1_Address = CPU1_data[Network_ID][0];		/* Save the address of CPU1 into Cpu1_Addrss Variable */
            inspect_CPU1_data_done = 1;
            CPU1_System_error = CPU1_data[Network_ID][69];
            System_error_code = CPU1_data[Network_ID][69];
	}
	if (uchCPU_ID == CPU2_ID)
	{
		/* If Data received from CPU2, then store into CPU2 Buffer */
		for(i =0 ;i<SPI_MESSAGE_LENGTH;i++)
		{
            CPU2_data[Network_ID][i] = Smc1XmitObject.Msg_Buffer[i];
            CPU2_data_GLCD[Network_ID][i] = Smc1XmitObject.Msg_Buffer[i];
            Latest_DAC_data[Network_ID][i] = Smc1XmitObject.Msg_Buffer[i];
		}		
            inspect_CPU2_data_done = 1;
            CPU2_Address = CPU2_data[Network_ID][0];		/* Save the address of CPU2 into Cpu2_Addrss Variable */
            CPU2_System_error = CPU2_data[Network_ID][69];
            System_error_code = CPU2_data[Network_ID][69];
    }
        
        if(CPU1_data[Network_ID][69]!=255 || CPU2_data[Network_ID][69]!=255)
        {
            System_error = 1;
        }
    uchCommand = Smc1XmitObject.Msg_Buffer[MESSAGE_TYPE_OFFSET];
    if(uchCommand == READ_RESET_INFO)
       {
		DAC_sysinfo.Unit_Type = Smc1XmitObject.Msg_Buffer[UNIT_TYPE_OFFSET];
		DAC_sysinfo.SW_Version = Smc1XmitObject.Msg_Buffer[SOFWARE_VERSION_OFFSET];
        DAC_sysinfo.Checksum.DWord.HiWord.Byte.Hi = Smc1XmitObject.Msg_Buffer[VALIDATED_CHECKSUM_OFFSET];
        DAC_sysinfo.Checksum.DWord.HiWord.Byte.Lo = Smc1XmitObject.Msg_Buffer[VALIDATED_CHECKSUM_OFFSET + 1];
        DAC_sysinfo.Checksum.DWord.LoWord.Byte.Hi = Smc1XmitObject.Msg_Buffer[VALIDATED_CHECKSUM_OFFSET + 2];
        DAC_sysinfo.Checksum.DWord.LoWord.Byte.Lo = Smc1XmitObject.Msg_Buffer[VALIDATED_CHECKSUM_OFFSET + 3];
		if (DAC_sysinfo.Checksum.LWord == 0L)
			{
	        DAC_sysinfo.Checksum.DWord.HiWord.Byte.Hi = Smc1XmitObject.Msg_Buffer[LOCAL_CHECKSUM_OFFSET];
    	    DAC_sysinfo.Checksum.DWord.HiWord.Byte.Lo = Smc1XmitObject.Msg_Buffer[LOCAL_CHECKSUM_OFFSET + 1];
	        DAC_sysinfo.Checksum.DWord.LoWord.Byte.Hi = Smc1XmitObject.Msg_Buffer[LOCAL_CHECKSUM_OFFSET + 2];
    	    DAC_sysinfo.Checksum.DWord.LoWord.Byte.Lo = Smc1XmitObject.Msg_Buffer[LOCAL_CHECKSUM_OFFSET + 3];
			}
    	}
    else{
        DAC_sysinfo.Unit_Type = Smc1XmitObject.Msg_Buffer[70];
		DAC_sysinfo.SW_Version = Smc1XmitObject.Msg_Buffer[71];
        DAC_sysinfo.Checksum.DWord.HiWord.Byte.Hi = Smc1XmitObject.Msg_Buffer[31];
        DAC_sysinfo.Checksum.DWord.HiWord.Byte.Lo = Smc1XmitObject.Msg_Buffer[31 + 1];
        DAC_sysinfo.Checksum.DWord.LoWord.Byte.Hi = Smc1XmitObject.Msg_Buffer[31 + 2];
        DAC_sysinfo.Checksum.DWord.LoWord.Byte.Lo = Smc1XmitObject.Msg_Buffer[31 + 3];
        
//        if( cpu_crc.DWord.HiWord.Word == DAC_sysinfo.Checksum.DWord.HiWord.Word ){
//            BI_COLOR_GREEN_LED_PORT == 1;
//            BI_COLOR_RED_LED_PORT == 0;
//        }else{
//            BI_COLOR_GREEN_LED_PORT == 0;
//            BI_COLOR_RED_LED_PORT == 1;
//        }       
    }
//    if(cpu_crc.DWord.HiWord.Byte.Hi == DAC_sysinfo.Checksum.DWord.HiWord.Byte.Hi
//            && cpu_crc.DWord.HiWord.Byte.Lo == DAC_sysinfo.Checksum.DWord.HiWord.Byte.Lo
//            && cpu_crc.DWord.LoWord.Byte.Hi == DAC_sysinfo.Checksum.DWord.LoWord.Byte.Hi
//            && cpu_crc.DWord.LoWord.Byte.Lo == DAC_sysinfo.Checksum.DWord.LoWord.Byte.Lo){
//            BI_COLOR_GREEN_LED_PORT == 1;
//            BI_COLOR_RED_LED_PORT == 0;
//            PORTEbits.RE9 = 0x0;
//            PORTEbits.RE8 = 0x1;
//        }else{
//            BI_COLOR_GREEN_LED_PORT == 0;
//            BI_COLOR_RED_LED_PORT == 1;
//            PORTEbits.RE9 = 0x1;
//            PORTEbits.RE8 = 0x0;
//        }
    inspect_DAC_info_done = 1;
	Update_Comm_Err_Counter(uchCPU_ID);	/* update Count for the Crc error Occured in External Communication */
	Update_Shadow_Register(uchCPU_ID);	/* Update our copy of event register */
	Detect_DAC_Events(uchCPU_ID,Shadow[Network_ID][(uchCPU_ID - 1)]);	/* Event detection */
}


/*********************************************************************************
File name 			:comm_dac.c
Function Name		:void Update_Comm_Err_Counter(BYTE uchCPU_ID)
Created By			:Sudheer Herle
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
		Buffer.Byte = CPU1_data[Network_ID][7];
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
		Buffer.Byte = CPU2_data[Network_ID][7];
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

/*********************************************************************************
File name 			:comm_dac.c
Function Name		:void Update_Shadow_Register(BYTE uchCPU)
Created By			:Sudheer Herle
Date Created		:
Modification History:
					Rev No			Date		Description
					 --				 --				--
Tracability:
		SRS()    	:

Abstract			: Shadow Register will be updated by New Data Received from CPU
Algorithm			:
Description			:
Input Element		:uchCPU - represent the CPU No (CPU1 or Cpu2)
Output Element		:void

**********************************************************************************/
void Update_Shadow_Register(BYTE uchCPU)
{
	BYTE uchbyte1, uchbyte2;
	dac_status_t DAC_Status;
	BOOL bUS_Healthy = FALSE;
	BOOL bDS_Healthy = FALSE;

	if (uchCPU != CPU1_ID && uchCPU != CPU2_ID)
	{
		return;		/* Invalid range */
	}
	if (Status.Flags.Inhibit_Logging_P == SET_HIGH && Status.Flags.Inhibit_Logging_N == SET_LOW)
	{
		return;		/* Data logging is INHIBITED, no point in updating */
	}
	uchbyte1 = (uchCPU - 1);

	/* Initialise the pointer to Appropriate Cpu Buffers location*/

	if (uchCPU == CPU1_ID)
	{
        for (uchbyte2 = 0; uchbyte2 < 7; uchbyte2++)
        {
            DAC_Status.Byte[uchbyte2] = CPU1_data[Network_ID][uchbyte2+1];
        }
	}
	else
	{
        for (uchbyte2 = 0; uchbyte2 < 7; uchbyte2++)
        {
            DAC_Status.Byte[uchbyte2] = CPU2_data[Network_ID][uchbyte2+1];
        }
	}

	/* MAP Status data to Shadow event register */
	Shadow[Network_ID][uchbyte1].Id.US_Block  = DAC_Status.Flags.US_Track_Status;
	Shadow[Network_ID][uchbyte1].Id.DS_Block  = DAC_Status.Flags.DS_Track_Status;
	Shadow[Network_ID][uchbyte1].Id.DAC_DS_Reset = DAC_Status.Flags.Local_Reset_Done;
	Shadow[Network_ID][uchbyte1].Id.DAC_US_Reset = DAC_Status.Flags.Local_Reset_Done2;
	Shadow[Network_ID][uchbyte1].Id.System    = DAC_Status.Flags.System_Status;
	Shadow[Network_ID][uchbyte1].Id.Direct_Out_Count = DAC_Status.Flags.Direct_Out_Count;
	Shadow[Network_ID][uchbyte1].Id.Board     = DAC_Status.Flags.Unit_Board_Status;
	Shadow[Network_ID][uchbyte1].Id.PD1_Board = DAC_Status.Flags.PD1_Board_Present;
	Shadow[Network_ID][uchbyte1].Id.PD2_Board = DAC_Status.Flags.PD2_Board_Present;
	Shadow[Network_ID][uchbyte1].Id.Modem_Board_A = DAC_Status.Flags.Modem_Card_A_Present;
	Shadow[Network_ID][uchbyte1].Id.Modem_Board_B = DAC_Status.Flags.Modem_Card_B_Present;
	Shadow[Network_ID][uchbyte1].Id.Relay_Board_A = DAC_Status.Flags.Relay_Drive_A_Present;
	Shadow[Network_ID][uchbyte1].Id.Relay_Board_B = DAC_Status.Flags.Relay_Drive_B_Present;
	Shadow[Network_ID][uchbyte1].Id.Peer_CPU_Board = DAC_Status.Flags.Peer_Cpu_Present;
	Shadow[Network_ID][uchbyte1].Id.LU1_US1_Comm = DAC_Status.Flags.LU1_to_US1_Link;
	Shadow[Network_ID][uchbyte1].Id.LU1_US2_Comm = DAC_Status.Flags.LU1_to_US2_Link;
	Shadow[Network_ID][uchbyte1].Id.LU1_DS1_Comm = DAC_Status.Flags.LU1_to_DS1_Link;
	Shadow[Network_ID][uchbyte1].Id.LU1_DS2_Comm = DAC_Status.Flags.LU1_to_DS2_Link;
	Shadow[Network_ID][uchbyte1].Id.LU2_US1_Comm = DAC_Status.Flags.LU2_to_US1_Link;
	Shadow[Network_ID][uchbyte1].Id.LU2_US2_Comm = DAC_Status.Flags.LU2_to_US2_Link;
	Shadow[Network_ID][uchbyte1].Id.LU2_DS1_Comm = DAC_Status.Flags.LU2_to_DS1_Link;
	Shadow[Network_ID][uchbyte1].Id.LU2_DS2_Comm = DAC_Status.Flags.LU2_to_DS2_Link;
	Shadow[Network_ID][uchbyte1].Id.US1_LU1_Comm = DAC_Status.Flags.US1_to_LU1_Link;
	Shadow[Network_ID][uchbyte1].Id.US2_LU1_Comm = DAC_Status.Flags.US2_to_LU1_Link;
	Shadow[Network_ID][uchbyte1].Id.DS1_LU1_Comm = DAC_Status.Flags.DS1_to_LU1_Link;
	Shadow[Network_ID][uchbyte1].Id.DS2_LU1_Comm = DAC_Status.Flags.DS2_to_LU1_Link;
	Shadow[Network_ID][uchbyte1].Id.US1_LU2_Comm = DAC_Status.Flags.US1_to_LU2_Link;
	Shadow[Network_ID][uchbyte1].Id.US2_LU2_Comm = DAC_Status.Flags.US2_to_LU2_Link;
	Shadow[Network_ID][uchbyte1].Id.DS1_LU2_Comm = DAC_Status.Flags.DS1_to_LU2_Link;
	Shadow[Network_ID][uchbyte1].Id.DS2_LU2_Comm = DAC_Status.Flags.DS2_to_LU2_Link;
	Shadow[Network_ID][uchbyte1].Id.PD1 = DAC_Status.Flags.PD1_Status;
	Shadow[Network_ID][uchbyte1].Id.PD2 = DAC_Status.Flags.PD2_Status;
	Shadow[Network_ID][uchbyte1].Id.Peer_CPU = DAC_Status.Flags.Peer_System_Status;
	Shadow[Network_ID][uchbyte1].Id.Peer_CPU_Comm = DAC_Status.Flags.Peer_CPU_Link;
	Shadow[Network_ID][uchbyte1].Id.Modem_A = DAC_Status.Flags.Modem_A;
	Shadow[Network_ID][uchbyte1].Id.Modem_B = DAC_Status.Flags.Modem_B;
	Shadow[Network_ID][uchbyte1].Id.US_DAC  = DAC_Status.Flags.US_System_Status;
	Shadow[Network_ID][uchbyte1].Id.DS_DAC  = DAC_Status.Flags.DS_System_Status;
	Shadow[Network_ID][uchbyte1].Id.US1_Power = DAC_Status.Flags.Power_Fail_at_US1;
	Shadow[Network_ID][uchbyte1].Id.US2_Power = DAC_Status.Flags.Power_Fail_at_US2;
	Shadow[Network_ID][uchbyte1].Id.DS1_Power = DAC_Status.Flags.Power_Fail_at_DS1;
	Shadow[Network_ID][uchbyte1].Id.DS2_Power = DAC_Status.Flags.Power_Fail_at_DS2;
	if (DAC_Status.Flags.System_Status == SET_HIGH && DAC_Status.Flags.US_System_Status == SET_HIGH)
	{
		bUS_Healthy = TRUE;			/* UP Stream section healthy */
	}
	if (DAC_Status.Flags.System_Status == SET_HIGH && DAC_Status.Flags.DS_System_Status == SET_HIGH)
	{
		bDS_Healthy = TRUE;			/* DOWN Stream section healthy */
	}
	if (bUS_Healthy == SET_LOW)
	{
		/* Failure in UP Stream section */
		if (DAC_Status.Flags.Vital_Relay_A != SET_HIGH)
		{
			Shadow[Network_ID][uchbyte1].Id.Vital_Relay_A = DAC_Status.Flags.Vital_Relay_A;
		}
		if (DAC_Status.Flags.Preparatory_Relay1 != SET_HIGH)
		{
			Shadow[Network_ID][uchbyte1].Id.Prep_Relay_A = DAC_Status.Flags.Preparatory_Relay1;
		}
	}
	else
	{
		/* UP Stream section healthy */
		if (DAC_Status.Flags.Vital_Relay_A != SET_LOW)
		{
			Shadow[Network_ID][uchbyte1].Id.Vital_Relay_A = DAC_Status.Flags.Vital_Relay_A;
		}
		if (DAC_Status.Flags.Preparatory_Relay1 != SET_LOW)
		{
			Shadow[Network_ID][uchbyte1].Id.Prep_Relay_A = DAC_Status.Flags.Preparatory_Relay1;
		}
	}

	if (bDS_Healthy == SET_LOW)
	{
		/* Failure in DOWN Stream section */
		if (DAC_Status.Flags.Vital_Relay_B != SET_HIGH)
		{
			Shadow[Network_ID][uchbyte1].Id.Vital_Relay_B = DAC_Status.Flags.Vital_Relay_B;
		}
		if (DAC_Status.Flags.Preparatory_Relay != SET_HIGH)
		{
			Shadow[Network_ID][uchbyte1].Id.Prep_Relay_B = DAC_Status.Flags.Preparatory_Relay;
		}
	}
	else
	{
		/* DOWN Stream section healthy */
		if (DAC_Status.Flags.Vital_Relay_B != SET_LOW)
		{
			Shadow[Network_ID][uchbyte1].Id.Vital_Relay_B = DAC_Status.Flags.Vital_Relay_B;
		}
		if (DAC_Status.Flags.Preparatory_Relay != SET_LOW)
		{
			Shadow[Network_ID][uchbyte1].Id.Prep_Relay_B = DAC_Status.Flags.Preparatory_Relay;
		}
	}
}

//void Save_DS_Data()
//{
//	BYTE uchBuf;
//
//	uchBuf = Smc1XmitObject.Msg_Buffer[MODE_OFFSET] & 0x0F;
//	if (uchBuf <= MAX_TYPE_OF_STATUS)
//		{
//		RB_Info.Local_DP_Status = uchStatusLookup[uchBuf][1];
//		RB_Info.Remote_DP_Status = uchStatusLookup[uchBuf][0];
//		}
//	RB_Info.Local_DP_Count.Byte.Lo = Smc1XmitObject.Msg_Buffer[DS_LOCAL_AXLE_COUNT_OFFSET];
//	RB_Info.Local_DP_Count.Byte.Hi = Smc1XmitObject.Msg_Buffer[DS_LOCAL_AXLE_COUNT_OFFSET+1];
//	RB_Info.Remote_DP_Count.Byte.Lo = Smc1XmitObject.Msg_Buffer[DS_REMOTE_AXLE_COUNT_OFFSET];
//	RB_Info.Remote_DP_Count.Byte.Hi = Smc1XmitObject.Msg_Buffer[DS_REMOTE_AXLE_COUNT_OFFSET+1];
//	RB_Info.Local_DP_Error = Smc1XmitObject.Msg_Buffer[DS_ERR_CODE_OFFSET];
//}
//
//void Save_US_Data()
//{
//	BYTE uchBuf;
//
//	uchBuf = Smc1XmitObject.Msg_Buffer[MODE_OFFSET] & 0xF0;
//	uchBuf = (uchBuf >> 4);
//	if (uchBuf <= MAX_TYPE_OF_STATUS)
//		{
//		RB_Info.Local_DP_Status = uchStatusLookup[uchBuf][1];
//		RB_Info.Remote_DP_Status = uchStatusLookup[uchBuf][0];
//		}
//	RB_Info.Local_DP_Count.Byte.Lo = Smc1XmitObject.Msg_Buffer[US_LOCAL_AXLE_COUNT_OFFSET];
//	RB_Info.Local_DP_Count.Byte.Hi = Smc1XmitObject.Msg_Buffer[US_LOCAL_AXLE_COUNT_OFFSET+1];
//	RB_Info.Remote_DP_Count.Byte.Lo = Smc1XmitObject.Msg_Buffer[US_REMOTE_AXLE_COUNT_OFFSET];
//	RB_Info.Remote_DP_Count.Byte.Hi = Smc1XmitObject.Msg_Buffer[US_REMOTE_AXLE_COUNT_OFFSET+1];
//	RB_Info.Local_DP_Error = Smc1XmitObject.Msg_Buffer[US_ERR_CODE_OFFSET];
//}
//
//void Save_3DP1S_A_Data()
//{
//	BYTE uchBuf;
//
//	uchBuf = Smc1XmitObject.Msg_Buffer[MODE_OFFSET] & 0x0F;
//	if (uchBuf <= MAX_TYPE_OF_STATUS)
//		{
//		RB_Info.Local_DP_Status = uchStatusLookup[uchBuf][1];
//		RB_Info.Remote_DP_Status = uchStatusLookup[uchBuf][0];
//		}
//	RB_Info.DP_A_Count.Byte.Lo = Smc1XmitObject.Msg_Buffer[DS_LOCAL_AXLE_COUNT_OFFSET];
//	RB_Info.DP_A_Count.Byte.Hi = Smc1XmitObject.Msg_Buffer[DS_LOCAL_AXLE_COUNT_OFFSET+1];
//	RB_Info.DP_B_Count.Byte.Lo = Smc1XmitObject.Msg_Buffer[DS_REMOTE_AXLE_COUNT_OFFSET];
//	RB_Info.DP_B_Count.Byte.Hi = Smc1XmitObject.Msg_Buffer[DS_REMOTE_AXLE_COUNT_OFFSET+1];
//	RB_Info.DP_C_Count.Byte.Lo = Smc1XmitObject.Msg_Buffer[US_REMOTE_AXLE_COUNT_OFFSET];
//	RB_Info.DP_C_Count.Byte.Hi = Smc1XmitObject.Msg_Buffer[US_REMOTE_AXLE_COUNT_OFFSET+1];
//	RB_Info.Local_DP_Error = Smc1XmitObject.Msg_Buffer[DS_ERR_CODE_OFFSET];
//}
//
//void Save_3DP1S_B_Data()
//{
//	BYTE uchBuf;
//
//	uchBuf = Smc1XmitObject.Msg_Buffer[MODE_OFFSET] & 0x0F;
//	if (uchBuf <= MAX_TYPE_OF_STATUS)
//		{
//		RB_Info.Local_DP_Status = uchStatusLookup[uchBuf][1];
//		RB_Info.Remote_DP_Status = uchStatusLookup[uchBuf][0];
//		}
//	RB_Info.DP_A_Count.Byte.Lo = Smc1XmitObject.Msg_Buffer[US_REMOTE_AXLE_COUNT_OFFSET];
//	RB_Info.DP_A_Count.Byte.Hi = Smc1XmitObject.Msg_Buffer[US_REMOTE_AXLE_COUNT_OFFSET+1];
//	RB_Info.DP_B_Count.Byte.Lo = Smc1XmitObject.Msg_Buffer[DS_LOCAL_AXLE_COUNT_OFFSET];
//	RB_Info.DP_B_Count.Byte.Hi = Smc1XmitObject.Msg_Buffer[DS_LOCAL_AXLE_COUNT_OFFSET+1];
//	RB_Info.DP_C_Count.Byte.Lo = Smc1XmitObject.Msg_Buffer[DS_REMOTE_AXLE_COUNT_OFFSET];
//	RB_Info.DP_C_Count.Byte.Hi = Smc1XmitObject.Msg_Buffer[DS_REMOTE_AXLE_COUNT_OFFSET+1];
//	RB_Info.Local_DP_Error = Smc1XmitObject.Msg_Buffer[DS_ERR_CODE_OFFSET];
//}
//
//void Save_3DP1S_C_Data()
//{
//	BYTE uchBuf;
//
//	uchBuf = Smc1XmitObject.Msg_Buffer[MODE_OFFSET] & 0x0F;
//	if (uchBuf <= MAX_TYPE_OF_STATUS)
//		{
//		RB_Info.Local_DP_Status = uchStatusLookup[uchBuf][1];
//		RB_Info.Remote_DP_Status = uchStatusLookup[uchBuf][0];
//		}
//	RB_Info.DP_A_Count.Byte.Lo = Smc1XmitObject.Msg_Buffer[DS_REMOTE_AXLE_COUNT_OFFSET];
//	RB_Info.DP_A_Count.Byte.Hi = Smc1XmitObject.Msg_Buffer[DS_REMOTE_AXLE_COUNT_OFFSET+1];
//	RB_Info.DP_B_Count.Byte.Lo = Smc1XmitObject.Msg_Buffer[US_REMOTE_AXLE_COUNT_OFFSET];
//	RB_Info.DP_B_Count.Byte.Hi = Smc1XmitObject.Msg_Buffer[US_REMOTE_AXLE_COUNT_OFFSET+1];
//	RB_Info.DP_C_Count.Byte.Lo = Smc1XmitObject.Msg_Buffer[DS_LOCAL_AXLE_COUNT_OFFSET];
//	RB_Info.DP_C_Count.Byte.Hi = Smc1XmitObject.Msg_Buffer[DS_LOCAL_AXLE_COUNT_OFFSET+1];
//	RB_Info.Local_DP_Error = Smc1XmitObject.Msg_Buffer[DS_ERR_CODE_OFFSET];
//}
//
