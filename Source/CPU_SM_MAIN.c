/*********************************************************************************************************************
	Project		: 	Single Section Digital Axle Counter
	Version		: 	2.0 
	Revision	:	1	
	Filename	: 	cpu_sm.c
	Target MCU	: 	PIC24FJ256GB210   ``1`
    Compiler	: 	XC16 Compiler V1.21  
	Author		:	Sudheer Herle
	Date		:	
	Company Name: 	Insys Digital Systems
	Modification History:
					Rev No			Date		Description
					 --				 --				--    
	Functions	:   void main(void)
					void Initialise_System(void)
					void Check_Boards(void)
					void Check_Flash(void)
					void Start_Sub_Systems(void)
					void DisplaySM_InfoLine(void)
					void Itoac(UINT16 uiInput, BYTE uchOutput[])
		
*********************************************************************************************************************/
#include <xc.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
void Update_USB_Sch_State(void);
void Decrement_Reset_Seq_10msTmr(void);
void Initialise_Reset_Seq(void);
void Update_Network_Configuration(void);
void Update_switch_config(void);
/*
 * Set configuration bits/fuses
 */
// PIC24FJ256GB210 Configuration Bit Settings

// 'C' source line config statements

// CONFIG4

// CONFIG3
#pragma config WPFP = WPFP255           // Write Protection Flash Page Segment Boundary (Highest Page (same as page 170))
#pragma config SOSCSEL = SOSC             // Secondary Oscillator Power Mode Select (External clock (SCLKI) or Digital I/O mode()
#pragma config WUTSEL = LEG             // Voltage Regulator Wake-up Time Select (Default regulator start-up time is used)
#pragma config ALTPMP = ALPMPDIS        // Alternate PMP Pin Mapping (EPMP pins are in default location mode)
#pragma config WPDIS = WPDIS            // Segment Write Protection Disable (Segmented code protection is disabled)
#pragma config WPCFG = WPCFGDIS         // Write Protect Configuration Page Select (Last page (at the top of program memory) and Flash Configuration Words are not write-protected)
#pragma config WPEND = WPENDMEM         // Segment Write Protection End Page Select (Protected code segment upper boundary is at the last page of program memory; the lower boundary is the code page specified by WPFP)

// CONFIG2
#pragma config POSCMOD = HS             // Primary Oscillator Select (HS Oscillator mode is selected)
#pragma config IOL1WAY = OFF            // IOLOCK One-Way Set Enable (The IOLOCK bit can be set and cleared as needed, provided the unlock sequence has been completed)
#pragma config OSCIOFNC = OFF           // OSCO Pin Configuration (OSCO/CLKO/RC15 functions as CLKO (FOSC/2))
#pragma config FCKSM = CSDCMD           // Clock Switching and Fail-Safe Clock Monitor (Clock switching and Fail-Safe Clock Monitor are disabled)
#pragma config FNOSC = PRIPLL           // Initial Oscillator Select (Primary Oscillator with PLL module (XTPLL, HSPLL, ECPLL))
#pragma config PLL96MHZ = ON            // 96MHz PLL Startup Select (96 MHz PLL is enabled automatically on start-up)
#pragma config PLLDIV = DIV2            // 96 MHz PLL Prescaler Select (Oscillator input is divided by 2 (8 MHz input))
#pragma config IESO = OFF               // Internal External Switchover (IESO mode (Two-Speed Start-up) is disabled)

// CONFIG1
#pragma config WDTPS = PS32768          // Watchdog Timer Postscaler (1:32,768)
#pragma config FWPSA = PR128            // WDT Prescaler (Prescaler ratio of 1:128)
#pragma config ALTVREF = ALTVREDIS      // Alternate VREF location Enable (VREF is on a default pin (VREF+ on RA9 and VREF- on RA10))
#pragma config WINDIS = OFF             // Windowed WDT (Standard Watchdog Timer enabled,(Windowed-mode is disabled))
#pragma config FWDTEN = OFF             // Watchdog Timer (Watchdog Timer is disabled)
#pragma config ICS = PGx2               // Emulator Pin Placement Select bits (Emulator functions are shared with PGEC2/PGED2)
#pragma config GWRP = OFF               // General Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF                // General Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF             // JTAG Port Enable (JTAG port is disabled)


#include "COMMON.H"
#include "CPU_SM.H"
#include "COMM_DAC.H"
#include "command_proc.h"
#include "comm_host.h"
#include "COMM_SMC.H"
#include "CRC16.H"
#include "CRC32.H"
#include "DRV_A2D.H"
#include "DRV_I2C.H"
#include "DRV_LCD.H"
#include "DISPLAY.H"
#include "DRV_MEM.H"
#include "DRV_RTC.H"
#include "EVENTS.H"
#include "EEPROM.H"
#include "DRV_GLCD_SPI.h"
#include "DRV_LED.H"
#include "DRV_DI.H"
#include "TRAINMON.H"
#include "PRINT.H"
#include "DRV_CNT.H"
#include "AUTO_DRV_CNT.h"

sm_status_t Status;					/* sm status information */
rb_status_t RB_Status;
time_t SystemClock;					/* holds system clock time   */
time_t SystemClock;					/* holds system clock time   */
time_t SystemDate; 
BYTE YLED_FB_ERROR = 0, RLED_FB_ERROR = 0, SPK_FB_ERROR = 0, THFT_FB_ERROR = 0, DOOR_FB_ERROR = 0, feedback_error = 0, FB_error_ID =0;

dac_sysinfo_t DAC_sysinfo;					/* Structure to hold DAC System Information */
const BYTE BitMask_List[8] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 }; /* this table is used mask all bits,except one bit */
BYTE uchDateTime[21] = "                    "; 		/* Holds Date and Time in Display format */

BYTE uchIDInfo[15];							/* smcpu ID information */
BYTE uchCheckSum[10];						/* Calculated CheckSum value of FLASH Memory */
BYTE CPU1_Address;						/* holds cpu1 address */
BYTE CPU2_Address;						/* holds cpu2 address */
BYTE Network_config;
BYTE HA_config;
BYTE Pilot_mode_config;
reset_seq_t Reset_Seq;						/* Reset Sequence information */
auto_reset_seq_t Auto_Reset_Seq;
rb_info_t RB_Info;
extern longtype_t cpu_crc;
dac_comm_error Dac_Comm_Err;				/* Dac external communication CRC error count information */
char inspect_CPU1_data_done;
char inspect_CPU2_data_done;
char inspect_DAC_info_done;
/*********************************************************************************
File name 			:cpu_sm.c
Function Name		:void Initialise_System(void)
Created By			:Sudheer Herle
Date Created		:
Modification History:
					Rev No			Date		Description
					 --				 --				--
Tracability:
		SRS()    	:

Abstract			:Initialise all I/O ports,timer configuration,port direction,disable all priority interrupt levels,
					 disable A/D converter,Disable Capture/Comparator/PWM  Modules,default status  values ,
					 calls all initialisation function modules.	
Algorithm			:
Description			: 
Input Element		:None
Output Element		:void

**********************************************************************************/
void USBDeviceInit();
void delay();
extern smc_sch_info_t             Smc_Sch_Info;
void Initialise_System(void)
{
    unsigned int pll_startup_counter = 600;
    /*Set clock frequency*/
    CLKDIVbits.CPDIV = 0;   // 8MHz input, 32MHz System Clock
                            //8MHz crystal, Fosc = 32MHz, Fcy = FOSC/2 = 16Mhz
            CLKDIVbits.PLLEN = 1;
    while(pll_startup_counter--);

	/* Clear ports */
	PORTA = 0;
	PORTB = 0;
	PORTC = 0;
	PORTD = 0;
	PORTE = 0;
    PORTF = 0;
    PORTG = 0;
	/* set direction registers */
	TRISA = PORT_A_DIRECTION;
	TRISB = PORT_B_DIRECTION;
	TRISC = PORT_C_DIRECTION;
	TRISD = PORT_D_DIRECTION;
	TRISE = PORT_E_DIRECTION;
	TRISF = PORT_F_DIRECTION;
	TRISG = PORT_G_DIRECTION;

    ANSA   = 0x0400;
    ANSB   = 0; // it was 0x0038 
    ANSC   = DIGITAL_CONFIG;
    ANSD   = DIGITAL_CONFIG;
    ANSE   = DIGITAL_CONFIG;
    ANSF   = DIGITAL_CONFIG;
    ANSG   = DIGITAL_CONFIG;

    AD1CON1 = ADCON1_CONFIG;
	AD1CON2 = ADCON2_CONFIG;
	AD1CON3 = ADCON3_CONFIG;
    AD1CHS  = A2D_CS_DEFAULT;
    ANCFG   = A2D_BG_DEFAULT;
    AD1CSSL = A2D_SC_DEFAULT;
    AD1CSSH = A2D_SC_DEFAULT;

	AD1CON1 = ADCON1_CONFIG;
	AD1CON2 = ADCON2_CONFIG;
	AD1CON3 = ADCON3_CONFIG;
    AD1CHS  = A2D_CS_DEFAULT;
    ANCFG   = A2D_BG_DEFAULT;
    AD1CSSL = A2D_SC_DEFAULT;
    AD1CSSH = A2D_SC_DEFAULT;

    CVRCON = 0;
	CM1CON = 0;
    CM2CON = 0;
    CM3CON = 0;
    /* Turn Comparators Off */

    IC1CON1 = 0;
    IC1CON2 = 0;
    IC2CON1 = 0;
    IC2CON2 = 0;
    IC3CON1 = 0;
    IC3CON2 = 0;
    IC4CON1 = 0;
    IC4CON2 = 0;
    IC5CON1 = 0;
    IC5CON2 = 0;
    IC6CON1 = 0;
    IC6CON2 = 0;
    IC7CON1 = 0;
    IC7CON2 = 0;
    IC8CON1 = 0;
    IC8CON2 = 0;
    IC9CON1 = 0;
    IC9CON2 = 0;

    OC1CON1 = 0;
    OC1CON2 = 0;
    OC2CON1 = 0;
    OC2CON2 = 0;
    OC3CON1 = 0;
    OC3CON2 = 0;
    OC4CON1 = 0;
    OC4CON2 = 0;
    OC5CON1 = 0;
    OC5CON2 = 0;
    OC6CON1 = 0;
    OC6CON2 = 0;
    OC7CON1 = 0;
    OC7CON2 = 0;
    OC8CON1 = 0;
    OC8CON2 = 0;
    OC9CON1 = 0;
    OC9CON2 = 0;
									/* Disable Capture/Comparator/PWM  Modules */


	/* Load Default PORT Values */
	LATA = PORT_A_DEFAULT;
	LATB = PORT_B_DEFAULT;
	LATC = PORT_C_DEFAULT;
	LATD = PORT_D_DEFAULT;
	LATE = PORT_E_DEFAULT;
	LATF = PORT_F_DEFAULT;
	LATG = PORT_G_DEFAULT;
    
	/* Configure Timers, CCP Module */
	T1CON = TIMER1_CONFIG;
	T2CON = TIMER2_CONFIG;
    PR1   = PR1_SET;
    PR2   = PR2_SET;
    //T3CON = TIMER2_CONFIG;

	TMR1 = TIMER1_SETPOINT;
	TMR2 = TIMER2_SETPOINT;
	//PR3 =  TIMER3_SETPOINT;

	
    INTCON1 = 0;
    INTCON2 = 0;

    IEC0 = 0;
    IEC1 = 0;
    IEC2 = 0;
    IEC3 = 0;
    IEC4 = 0;
    IEC5 = 0;

	Status.Byte[0] = (BYTE) 0b11011111;			/* Initialise Status Register */
	Status.Byte[1] = (BYTE) 0b01001000;			/* Initialise Status Register */

    RB_Status.Byte[0] = (BYTE) 0b00000100;			/* Initialise Status Register */
    RB_Status.Flags.Reset_Mode = PORTAbits.RA6; // Pilot mode

	//Initialise_LCD_Driver();					/* from drv_lcd.c */
	//Check_Boards();			    				/* Check whether CPU1 and CPU2 boards are present in rack */
	//Check_Flash();			    				/* Check for corruption of On-Board FLASH */
        Status.Flags.Flash_CheckSum = CRC32_CHECKSUM_OK;
	/*
	 * Initialise sub-systems
	 *
	 * Please ensure that Initialise_EEPROM_State() 
	 * is called before calling following functions
	 * - Initialse_SPI()
	 * - Initialise_Events_Sch() 
	 */
    GS_LED_PORT = 0;
	Initialise_Smc_CommSch();	/* from comm_sm.c */
    delay();
    cpu_crc.DWord.HiWord.Byte.Hi = 0x9a;
    cpu_crc.DWord.HiWord.Byte.Lo = 0xfa;
    cpu_crc.DWord.LoWord.Byte.Hi = 0x0f;
    cpu_crc.DWord.LoWord.Byte.Lo = 0x4b;
    Initialise_Printer();		/* from print.c */
	Initialise_LED_Driver();	/* from drv_led.c */
	Initialise_Cnt_Driver();	/* from drv_cnt.c */
    Initialise_Auto_Cnt_Driver();

    TRISFbits.TRISF4 = 0;
	Initialise_DI_Driver();		/* from drv_di.c */
	Initialise_Reset_Seq();
    Initialise_Auto_Reset_Seq();
//	Initialise_RB_Mode();
	Initialise_RB_Info();
	Initialise_TrainMon();	
    Initialise_A2D_Driver();	/* from drv_a2d.c */
	Initialise_I2C_Driver();	/* from drv_i2c.c */
	Initialise_RTC_Sch();		/* from drv_rtc.c */
	Initialise_Host_CommSch();	/* from comm_host.c */
//	Initialise_Smc_CommSch();	/* from comm_smc.c */
	Initialise_EEPROM_State();	/* from eeprom.c */
//	Initialise_SPI();			/* from comm_dac.c */
	Initialise_Events_Sch();	/* from events.c */
    USBDeviceInit();                //usb_device.c.  Initializes USB module SFRs and firmware variables to known states.//Initialise_Display_State(); /* from display.c */
    Initialise_GLCD_Driver();
    Update_Network_Configuration();
    Update_switch_config();
//	Read_RTC_Registers();
	Dac_Comm_Err.CPU1_CommB_Error_Count = 0;     /*Initialise all Communication Err Counts to zero */
	Dac_Comm_Err.CPU1_CommA_Error_Count = 0;
	Dac_Comm_Err.CPU2_CommB_Error_Count = 0;
	Dac_Comm_Err.CPU2_CommA_Error_Count = 0;	
    Smc_Sch_Info.CD_timeout_ms = CD_LINK_FAILURE_TIMEOUT;
//    Set_Modem_RX_Mode();
	//DisplaySM_InfoLine();                        /* Display System Id and CheckSum */
}

/*********************************************************************************
File name 			:cpu_sm.c
Function Name		:void Check_Flash(void)
Created By			:Sudheer Herle
Date Created		:
Modification History:
					Rev No			Date		Description
					 --				 --				--
Tracability:
		SRS()    	:

Abstract			:This function is used to detect the Rom corruption.
					 It performs crc32 bit operation on entire rom.
					 It compares the calculated checksum with stored checksum value. 

Algorithm			:1.Read the saved checksum from the system ID locations.
					 2.Calculate crc32 bit for entire Rom.
					 3.Compare stored checksum and calculated checksum.
					 4.It sets the checksum status bit to "OK or BAD" depends on the comparison result.
Description			: 
Input Element		:None
Output Element		:void

**********************************************************************************/
BYTE flash_read_c (UINT32 temp)
{
    return (BYTE)temp;
}

void Check_Flash(void)
{
	const unsigned char *pOnChipFlash = 0;
	longtype_t CalculatedSum;
	longtype_t SavedSum;
	BYTE uchHiNibble, uchLoNibble;

	//uchTBLPTRU = TBLPTRU;	/* Save TBLPTRU register */
	/* Read ID Locations to retrieve Checksum */
	uchLoNibble = flash_read_c((UINT32) CHECKSUM_LOCATION);
	uchHiNibble = flash_read_c((UINT32) CHECKSUM_LOCATION + 1);
	SavedSum.DWord.LoWord.Byte.Lo = ((BYTE)(uchHiNibble << 4)) | uchLoNibble;
	uchLoNibble = flash_read_c((UINT32) CHECKSUM_LOCATION + 2);
	uchHiNibble = flash_read_c((UINT32) CHECKSUM_LOCATION + 3);
	SavedSum.DWord.LoWord.Byte.Hi = ((BYTE)(uchHiNibble << 4)) | uchLoNibble;
	uchLoNibble = flash_read_c((UINT32) CHECKSUM_LOCATION + 4);
	uchHiNibble = flash_read_c((UINT32) CHECKSUM_LOCATION + 5);
	SavedSum.DWord.HiWord.Byte.Lo = ((BYTE)(uchHiNibble << 4)) | uchLoNibble;
	uchLoNibble = flash_read_c((UINT32) CHECKSUM_LOCATION + 6);
	uchHiNibble = flash_read_c((UINT32) CHECKSUM_LOCATION + 7);
	SavedSum.DWord.HiWord.Byte.Hi = ((BYTE)(uchHiNibble << 4)) | uchLoNibble;
	//TBLPTRU = uchTBLPTRU;	/* Restore TBLPTRU register */

	 /* Please ensure that source files are compiled with
	 * -CP24 to generate 24-bits wide pointers. This will ensure that entire program
	 * space will be dereferenced by pointer correctly. In case of 16-bit pointers, 
	 * addresses below the upper limit of RAM will access the RAM and above upper limit
	 * of RAM will access FLASH, which means entire FLASH is not accessible.
	 */
	CalculatedSum.LWord = Crc32((const unsigned char *) pOnChipFlash, (INT32) ROMSIZE );
	sprintf((char *)uchCheckSum, "%04x%04x", CalculatedSum.DWord.HiWord.Word, CalculatedSum.DWord.LoWord.Word);
	sprintf((char *)uchIDInfo, " DAC SM CPU ");
	if (CalculatedSum.LWord == SavedSum.LWord)
	{
		/* CRC-32 Check sum computed matches with the one stored in ID LOCATION */
		Status.Flags.Flash_CheckSum = CRC32_CHECKSUM_OK;
	}
	else	
	{
		Status.Flags.Flash_CheckSum = CRC32_CHECKSUM_BAD;
	}	
}
/*********************************************************************************
File name 			:cpu_sm.c
Function Name		:void Start_Sub_Systems(void)
Created By			:Sudheer Herle
Date Created		:
Modification History:
					Rev No			Date		Description
					 --				 --				--
Tracability:
		SRS()    	:

Abstract			:Starts the RTC, EEPROM, Host communication and SMC communication shecduler.
Algorithm			:
Description			: 
Input Element		:None
Output Element		:void

**********************************************************************************/

void Start_Sub_Systems(void)
{
    Start_RTC_Sch();
	//Start_Smc_Communication();              /* from comm_sm.c */
	Start_DI_Scan();			/* from drv_di.c */
	Start_Reset_Seq();
    Start_Auto_Reset_Seq();
	Start_TrainMon();			/* from trainmon.c */
	Start_Printer();			/* from print.c */
}


/*********************************************************************************
File name 			:cpu_sm.c
Function Name		:void main(void)
Created By			:Sudheer Herle
Date Created		:
Modification History:
					Rev No			Date		Description
					 --				 --				--
Tracability:
		SRS()    	:

Abstract			:main function which calls all other modules.
Algorithm			:
Description			:
Input Element		:None
Output Element		:void

**********************************************************************************/
volatile char inspect_event_done;
int USB_Check(void);
//
//extern glcd_info_t GLCD_Info;		/* structure that handles Lcd scheduler and holds lcd Information */

//#define GS_LED          LATFbits.LATF4 //SS2
#define AUTO_RESET_PORT_PIN  LATDbits.LATD9;
#define AUTO_RESET_FB_PORT_PIN LATDbits.LATD11;
#include "COMMON.h"
//extern smc_info_t					Smc1XmitObject;
int main(void)
{
    inspect_event_done = 0;
    inspect_CPU1_data_done = 0;
    inspect_CPU2_data_done = 0;
    inspect_DAC_info_done = 0;
//    GLCD_Info.State = GLCD_IDLE;    
//    GLCD_Info.Comm_Timeout_ms = MAX_COMM_TIMEOUT;
	Initialise_System();					 /* Initialise Ports and All Schedulers */
    Start_Sub_Systems();					 /* Start Schedulers */
    G_SPI_SS= SET_HIGH;
    G_SPI_SS2= SET_HIGH;
    TRISFbits.TRISF4 = 0;
	T1CONbits.TON = 1;	/* Enable Timer 1 */
	T2CONbits.TON = 1;	/* Enable Timer 2 */
    TRISDbits.TRISD12 = 0;
    LATDbits.LATD12 = 1;
	Add_SM_Event_to_Queue(EVENT_RESET_BOX_TURNED_ON);	/* POWER ON event is added to Event Queue */
//    Smc1XmitObject.Msg_Length = 0;
	do {
//        GS_LED_PORT = 0;
		if (IFS0bits.T1IF)
			{
			/* 10-mS Timer has overflowed */
			IFS0bits.T1IF = 0;
			TMR1 = TIMER1_SETPOINT;
			Decrement_RTC_10msTmr();		/* from drv_rtc.c */
			Decrement_A2D_10msTmr();		/* from drv_a2d.c */
			Decrement_Events_Sch_10msTmr();	/* from events.c */
//          Decrement_Smc_Sch_msTmr();		/* from comm_smc.c */
            Decrement_LED_10msTmr();
            Decrement_Cnt_10msTmr();			/* from Drv_Cnt.c */
            Decrement_Auto_Cnt_10msTmr();
			Decrement_DI_10msTmr();				/* from drv_di.c */
            Decrement_Reset_Seq_10msTmr();
            Decrement_Auto_Reset_Seq_10msTmr();
//			Decrement_RB_Mode_10msTmr();		/* from display.c*/

                   	}
		if (IFS0bits.T2IF)
		{
			/* 1-mS Timer has overflowed */
			IFS0bits.T2IF = 0;
            
			TMR2 = TIMER2_SETPOINT;
//			Decrement_SPI_Sch_msTmr();		/* from comm_dac.c */
//			Decrement_LCD_msTmr();			/* from drv_lcd.c */
			Decrement_Host_Sch_msTmr();		/* from comm_host.c */
			Decrement_Smc_Sch_msTmr();		/* from comm_smc.c */
//			Decrement_itoa_msTmr();			/* from display.c */
			Decrement_EEPROM_msTmr();		/* from eeprom.c */
			Decrement_GLCD_msTmr();
        }
		Update_Host_Sch_State();			/* from comm_host.c */
        Update_Preparatory_LED_State();		/* from drv_led.c */
		Update_Cnt_State();					/* from drv_cnt.c */
        Update_Auto_Cnt_State();
		Update_Smc_Sch_State();				/* from comm_sm.c */
		Update_DI_State();					/* from drv_di.c */
		Update_Reset_Seq_State();
        Update_Auto_Reset_Seq_State();
//		Update_RB_Mode_State();
        Update_EEPROM_State();				/* from eeprom.c */
		Update_RtcSch_State();				/* from drv_rtc.c */
        Update_Events_Sch_State();
        Update_USB_Sch_State(); 
        Update_GLCD_State();
        Update_Printer_State();
		} 
        while (1);
        
}

void delay(){
    int t=0;
    for(;t<999;t++){
        Nop();
    }
}
void Initialise_RB_Info(void)
{

	RB_Info.RB_Type = RESET_BOX_TYPE_UNKNOWN;

	if (PORTGbits.RG1 == 0)
        {
	if (PORTGbits.RG0 == 0)
            {
            /* 00 is 2DP1S */
                RB_Info.RB_Type = RESET_BOX_TYPE_DS;
            }
        else
            {
            /* 01 is DS */
                RB_Info.RB_Type = RESET_BOX_TYPE_2DP1S;
            }
        }
	else
		{
		if (PORTGbits.RG0 == 0)
			{
			/* 10 is US */
            RB_Info.RB_Type = RESET_BOX_TYPE_3DP1S;
			}
		else
			{
			/* 11 is 3DP1S */
            RB_Info.RB_Type = RESET_BOX_TYPE_US;
			}
		}

		RB_Info.DAC_Type = DAC_TYPE_UNKNOWN;
		RB_Info.Local_DP_Status = RB_MODE_NOT_STARTED;
		RB_Info.Remote_DP_Status = RB_MODE_NOT_STARTED;
		RB_Info.Local_DP_Count.Word = 0;
		RB_Info.Remote_DP_Count.Word = 0;
		RB_Info.DP_A_Count.Word = 0;
		RB_Info.DP_B_Count.Word = 0;
		RB_Info.DP_C_Count.Word = 0;
		RB_Info.Local_DP_Error = 0;
		RB_Info.Remote_DP_Error = 0;
		RB_Info.Prev_Local_DP_Count.Word = 0;
		RB_Info.Prev_Remote_DP_Count.Word = 0;
		RB_Info.Prev_DP_A_Count.Word = 0;
		RB_Info.Prev_DP_B_Count.Word = 0;
		RB_Info.Prev_DP_C_Count.Word = 0;
//		Itoac( RB_Info.Local_DP_Count.Word, RB_Info.Local_DP_Count_Str);
//		Itoac( RB_Info.Remote_DP_Count.Word, RB_Info.Remote_DP_Count_Str);
//		Itoac( RB_Info.DP_A_Count.Word, RB_Info.DP_A_Count_Str);
//		Itoac( RB_Info.DP_B_Count.Word, RB_Info.DP_B_Count_Str);
//		Itoac( RB_Info.DP_C_Count.Word, RB_Info.DP_C_Count_Str);
}

void Initialise_Reset_Seq(void)
{
	Reset_Seq.State = RESET_SYS_NOT_ON;
	Reset_Seq.Timeout_10ms = 0;
}

void Start_Reset_Seq(void)
{
	Reset_Seq.State = RESET_CHK_INITIAL_CONDITION;
	Reset_Seq.Timeout_10ms = 0;
}

void Initialise_Auto_Reset_Seq(void)
{
	Auto_Reset_Seq.State = AUTO_RESET_SYS_NOT_ON;
	Auto_Reset_Seq.Timeout_10ms = 0;
}

void Start_Auto_Reset_Seq(void)
{
	Auto_Reset_Seq.State = AUTO_RESET_CHK_CONDITION;
	Auto_Reset_Seq.Timeout_10ms = 0;
}

void Update_Reset_Seq_State(void)
{
	switch (Reset_Seq.State)
		{
		case RESET_SYS_NOT_ON:
			break;
		case RESET_CHK_INITIAL_CONDITION:
			if (RB_Status.Flags.VR1_Contact_Status == SET_HIGH &&
				RB_Status.Flags.PR1_Contact_Status == SET_HIGH)
				{
				/* Both VR and PR are down, Hence reset permitted */
				Reset_Seq.State = WAIT_FOR_RESET_INPUT;
				Reset_Seq.Timeout_10ms = 0;
				}
			break;
		case WAIT_FOR_RESET_INPUT:
			if (RB_Status.Flags.Reset_PB_Status == SET_LOW) {
                    if(LATDbits.LATD11 == 1){
                        Reset_Seq.State = WAIT_FOR_RESET_INPUT;
                        break;
                        break;
                    }
				/*
				 * Reset Push Button has been depressed, but we do not know
				 * whether SYSTEM is in RESET MODE or section is occupied.
				 */
                    if(LATAbits.LATA6 == 0){

//                            }
//				if (PORTAbits.RA6 == PILOT_MODE)
//					{
					Set_Preparatory_LED_Flashing();
					Reset_Seq.State = RESET_WAIT_FOR_PR_PICKUP;
					Reset_Seq.Timeout_10ms = PR_PICKUP_TIMEOUT;
					}
				else
                                        {
					Set_Preparatory_LED_Off();
					//Increment_Reset_Counter();
					Reset_Seq.State = CHECK_VR1_PICKUP;
					}
				}
			break;

        case CHECK_VR1_PICKUP:
                if(RB_Status.Flags.VR1_Contact_Status == SET_LOW){
                    Increment_Reset_Counter();
                    Reset_Seq.State = RESET_WAIT_FOR_INPUT_TO_CLEAR;                                            
                }
                break;
		case RESET_WAIT_FOR_PR_PICKUP:
			if (RB_Status.Flags.PR1_Contact_Status == SET_LOW)
				{
				Set_Preparatory_LED_On();
				Increment_Reset_Counter();
				Reset_Seq.State = CHK_POST_RESET_CONDITION;
				Reset_Seq.Timeout_10ms = 0;
				break;
				}
			if (Reset_Seq.Timeout_10ms == TIMEOUT_EVENT)
				{
				Set_Preparatory_LED_Off();
				Reset_Seq.State = RESET_CHK_INITIAL_CONDITION;
				Reset_Seq.Timeout_10ms = 0;
				}
			break;

		case CHK_POST_RESET_CONDITION:
			if (RB_Status.Flags.VR1_Contact_Status == SET_HIGH &&
				RB_Status.Flags.PR1_Contact_Status == SET_HIGH)
				{
				/*
				 * Both VR and PR are down, Which can happen when system is in
				 * ERROR occurs when piloting.
				 */
				Set_Preparatory_LED_Off();
				Reset_Seq.State = RESET_CHK_INITIAL_CONDITION;
				Reset_Seq.Timeout_10ms = 0;
				break;
				}

			if (RB_Status.Flags.VR1_Contact_Status == SET_LOW &&
				RB_Status.Flags.PR1_Contact_Status == SET_HIGH)
				{
				Set_Preparatory_LED_Off();
				Reset_Seq.State = RESET_WAIT_FOR_INPUT_TO_CLEAR;
				Reset_Seq.Timeout_10ms = 0;
				}
			break;
		case RESET_WAIT_FOR_INPUT_TO_CLEAR:
			if (RB_Status.Flags.Reset_PB_Status == SET_HIGH)
				{
				Set_Preparatory_LED_Off();
				Reset_Seq.State = RESET_CHK_INITIAL_CONDITION;
				Reset_Seq.Timeout_10ms = 0;
				}
			break;
		}
}

void Update_Auto_Reset_Seq_State(void)
{
	switch (Auto_Reset_Seq.State)
		{
        case CHK_POST__AUTO_RESET_CONDITION:
            break;
        case AUTO_RESET_WAIT_FOR_INPUT_TO_CLEAR:
            break;
		case AUTO_RESET_SYS_NOT_ON:
			break;
		case AUTO_RESET_CHK_CONDITION:
			if (RB_Status.Flags.VR1_Contact_Status !=
				RB_Status.Flags.VR2_Contact_Status)
				{
				Auto_Reset_Seq.State = AUTO_RELAY_STATES_DIFFERENT;
                Auto_Reset_Seq.Timeout_10ms = AUTO_RESET_WAIT_TIMEOUT;
				}
            else if (RB_Status.Flags.VR1_Contact_Status == SET_HIGH
				&& RB_Status.Flags.VR2_Contact_Status == SET_HIGH)
				{
				/* Both VR and PR are down, Hence auto reset is required */
				Auto_Reset_Seq.State = AUTO_RELAY_STATES_DIFFERENT;
                Auto_Reset_Seq.Timeout_10ms = AUTO_RESET_WAIT_TIMEOUT;
				}
			break;
		case AUTO_RELAY_STATES_DIFFERENT:			
            if (RB_Status.Flags.VR1_Contact_Status ==
				RB_Status.Flags.VR2_Contact_Status)
				{
                if (RB_Status.Flags.VR1_Contact_Status == SET_LOW
                    && RB_Status.Flags.VR2_Contact_Status == SET_LOW)
                    Auto_Reset_Seq.State = AUTO_RESET_CHK_CONDITION;
                }
            
                if(Auto_Reset_Seq.Timeout_10ms == TIMEOUT_EVENT){
                    Auto_Reset_Seq.State = PERFORM_AUTO_RESET;
                    Auto_Reset_Seq.Timeout_10ms = 0;
                }            				
			break;
            
        case PERFORM_AUTO_RESET:
            LATDbits.LATD9 = 1;
            Auto_Reset_Seq.State = CHECK_AUTO_RESET_FEEDBACK;
            break;
            
        case CHECK_AUTO_RESET_FEEDBACK: 
            if(LATDbits.LATD11 == SET_HIGH){                
                Auto_Reset_Seq.Timeout_10ms = AUTO_RESET_HOLD_TIME;
            }
            
        case AUTO_RESET_HOLD:
            if(Auto_Reset_Seq.Timeout_10ms == TIMEOUT_EVENT){
                LATDbits.LATD9 = 0; 
                Auto_Reset_Seq.State = CHECK_POST_AUTO_RESET;                          
                Auto_Reset_Seq.Timeout_10ms = 0;
            }
            break;
        case CHECK_POST_AUTO_RESET:
            if (RB_Status.Flags.VR1_Contact_Status == SET_LOW 
                    &&  RB_Status.Flags.VR2_Contact_Status == SET_LOW)
            {
            Increment_Auto_Reset_Counter();
            Auto_Reset_Seq.State = AUTO_RESET_CHK_CONDITION;
            Auto_Reset_Seq.Timeout_10ms = 0;
            }
            break;
		}
}


void Decrement_Reset_Seq_10msTmr(void)
{
	if (Reset_Seq.Timeout_10ms > 0)
		{
		Reset_Seq.Timeout_10ms = Reset_Seq.Timeout_10ms - 1;
		}
}

void Decrement_Auto_Reset_Seq_10msTmr(void)
{
	if (Auto_Reset_Seq.Timeout_10ms > 0)
		{
		Auto_Reset_Seq.Timeout_10ms = Auto_Reset_Seq.Timeout_10ms - 1;
		}
}
void Update_Network_Configuration(void)
{
    unsigned char DIP_val, uchTemp;
    TRISDbits.TRISD6 = 1;
    TRISDbits.TRISD7 = 1;    
    TRISFbits.TRISF0 = 1;
    TRISFbits.TRISF1 = 1;
    
    TRISAbits.TRISA6 = 0;
    TRISGbits.TRISG0 = 0;
    TRISGbits.TRISG1 = 0;

    LATAbits.LATA6 = 1;
    LATGbits.LATG0 = 1;
    LATGbits.LATG1 = 0;
    
    DIP_val = 0;
    for(uchTemp=0;uchTemp<100;uchTemp++);
    DIP_val = (~((PORTDbits.RD6) | ((((BYTE)PORTDbits.RD7))<<1) | ((((BYTE)PORTFbits.RF0))<<2) | ((((BYTE)PORTFbits.RF1))<<3))) & 0X0F;
    
    LATAbits.LATA6 = 1;
    LATGbits.LATG0 = 0;
    LATGbits.LATG1 = 1;
    
    for(uchTemp=0;uchTemp<100;uchTemp++);
    DIP_val |= ((~((PORTDbits.RD6) | ((((BYTE)PORTDbits.RD7))<<1) | ((((BYTE)PORTFbits.RF0))<<2) | ((((BYTE)PORTFbits.RF1))<<3))) & 0X0f) <<4;
    
    Network_config = DIP_val;
}

void Update_switch_config(void)
{
    unsigned char DIP_val, uchTemp;
    TRISDbits.TRISD6 = 1;
    TRISDbits.TRISD7 = 1;    
    TRISFbits.TRISF0 = 1;
    TRISFbits.TRISF1 = 1;
    
    TRISAbits.TRISA6 = 0;

    LATAbits.LATA6 = 0;
    
    for(uchTemp=0;uchTemp<100;uchTemp++);
    DIP_val = (~((PORTDbits.RD6) | ((((BYTE)PORTDbits.RD7))<<1) | ((((BYTE)PORTFbits.RF0))<<2) | ((((BYTE)PORTFbits.RF1))<<3))) & (0X0F);
    
    
    if(((DIP_val) & (0x1)) == 1){
        HA_config = 1;
    }
    else{
        HA_config = 0;
    }
    
    if((((DIP_val) >> 1 )& (0x1)) == 1){
        Pilot_mode_config = 1;
    }else{
        Pilot_mode_config = 0;
    }
}