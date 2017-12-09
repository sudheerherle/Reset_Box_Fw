/*********************************************************************************************************************
	Project		: 	Single Section Digital Axle Counter
	Version		: 	2.0 
	Revision	:	1	
	Filename	: 	events.h
	Target MCU	: 	PIC24FJ256GB210   
    Compiler	: 	XC16 Compiler V1.21  
	Author		:	Sudheer Herle
	Date		:	
	Company Name: 	Insys Digital Systems
	Modification History:
					Rev No			Date		Description
					 --				 --				--    
*********************************************************************************************************************/
#ifndef _EVENTS_H_
#define _EVENTS_H_


#define EVENTS_QUEUE_SIZE			(100)		/* Maximum events can be stored into Event Queue */
#define SERIAL_EEPROM_TWC_TIMEOUT	(2)			/* 20ms - Time gap is maintained between Record write into Serial EEPROM */


/* Event Register Default Values */

/* b0-US_Block-occupied,            b1-DS_Block -occupied, b2-DAC_Reset_DS-Not Occured, b3-System -Normal
   b4-Direct_Out_Count-Not occured, b5-not Used,           b6 -not Used,             b7- DAC_Reset_US-Not Occured */
#define EVENT_REGISTER1_DEFAULT		0b10001100	

/* b0-Board-Present,         b1-Pd1_Board-Present,     b2-Pd2_Board-Present,      b3-Modem_Board_A-Present 
   b4-Modem_Board_B-Present, b5-Relay_Board_A-Present, b6 -Relay_Board_B-Present, b7-Peer_Cpu_Board-Present*/
#define EVENT_REGISTER2_DEFAULT		0b11111111

/* b0-LU1_US1_Comm-Ok, b1- LU1_US2_Comm-Ok	,b2-LU1_DS1_Comm-ok , b3-LU1_DS2_Comm-ok
   b4-LU2_US1_Comm-Ok, b5- LU2_US2_Comm-oK	,b6-LU2_DS1_Comm-oK , b7-LU2_DS2_Comm-oK*/
#define EVENT_REGISTER3_DEFAULT		0b11111111


/* b0-US1-LU1_Comm-Ok, b1- US2-LU1_Comm-Ok	,b2-DS1-LU1_Comm-ok , b3-DS2-LU1_Comm-ok
   b4-US1-LU2_Comm-Ok, b5- US2-LU2_Comm-oK	,b6-DS1-LU2_Comm-oK , b7-DS2-LU2_Comm-oK*/
#define EVENT_REGISTER4_DEFAULT		0b11111111

/* b0-PD1-Ok,            b1- PD2-Ok,           b2-Peer_CPU-ok,   b3-Peer_CPU_Comm-ok
   b4-Vital_Relay_A -Ok, b5- Vital_Relay_B-oK, b6-Prep_Relay_A-oK, b7-Prep_Relay_B-oK*/
#define EVENT_REGISTER5_DEFAULT		0b11111111

/* b0-Modem_A-Ok,   b1-Modem_B-Ok	,b2-US_DAC-ok,    b3-DS_DAC-ok
   b4-US1_Power-Ok, b5-US2_Power-oK	,b6-DS1_Power-oK, b7-DS2_Power-oK*/
#define EVENT_REGISTER6_DEFAULT		0b11111111


/* Events Scheduler States */
typedef enum {
			INSPECT_FIRST_EVENT = 0,		/* In this State First Record is Read from Events EEPROM and Token of that Event is analysed*/
			LOCATE_LAST_TOKEN_IN_CW_DIR,	/* it will search Last Record in Top to Down */
			LOCATE_LAST_TOKEN_IN_CCW_DIR,	/* it will search Last Record in Down to Top */
			LOCATE_FIRST_TOKEN_IN_CCW_DIR,  /* it will search First Record in Down to Top*/
			LOCATE_FIRST_TOKEN_IN_CW_DIR,	/* it will search First Record in Top to Down*/			
			SYNCHRONISE_EVENT_REGISTERS,	/* it reterives the CPU1 and CPU2 Event Register values from Onchip EEPROM */	
			EVENTS_SCHEDULER_NOT_STARTED,   /* Default State -POwer On State*/  
			EVENTS_SCHEDULER_IDLE,			/* Idle State.In this state Scheduler checks for new events in Queue to Save into Serial EEPROM*/ 
			SAVE_EVENTS_ON_EEPROM,			/* Save One Event Record into Serial EEPROM */ 
			SAVE_EVENTS_ON_EEPROM_WAIT,		/* Delay Time between Records */
			MODIFY_OLD_EVENTS_TOKEN,		/* Modify Old events with "OlD_EVENT_TOKEN" */
			MODIFY_OLD_EVENTS_TOKEN_WAIT,	/* Delay Time between Records */
			ERASE_EEPROM_BLOCK,				/* calls Erase_Serial_EEProm_Block() function with Block No to be Erased*/
			ERASE_EEPROM_BLOCK_WAIT			/* Delay Time Between Two Erase Blcoks */
} events_sch_state_t;


/* Ring Buffer -which is used to access the Events Memory */
typedef struct {
		UINT16 Head;	/* This pointer holds the address of Old Event Token i.e one previous to Lastest Event Token*/
		UINT16 Tail;	/* This pointer holds the address of Lastest Event Token*/
		BYTE Count;		/* it counts the number of record logged into EEPROM */
} ring_buffer_t;


/* Event Queue Structure used to hold Events Record which is to be stored into Serial EEPROM */
typedef struct {
		BYTE	FrontIndex;				
		BYTE	RearIndex;
		struct {
			BYTE	Source_Id;					/* CPU ID */
			BYTE	Event_Id;					/* Event Identification code */
			UINT16	US_Fwd_AxleCount;			/* Upstream Forward Axle Count */
			UINT16	US_Rev_AxleCount;			/* Upstream Reverse Axle Count */
			UINT16	DS_Fwd_AxleCount;			/* Downstream Forward Axle Count */
			UINT16	DS_Rev_AxleCount;			/* Downstream Reverse Axle Count */
		} Data[EVENTS_QUEUE_SIZE];
} events_queue_t;


/* Event scheduler */ 
typedef struct {
			events_sch_state_t	State;			/* Event Scheduler States */
			BYTE				Timeout_10ms;	/* 10ms Timer Variable for Event scheduler*/
			events_sch_state_t	Next_State;		/* Event Scheduler next State */
			UINT16				Block_Number;	/* Block No to be accessed */
			INT32				Record_No;		/* Record No to be accessed */
} events_sch_info_t;

extern void Initialise_Events_Sch(void);
extern void Detect_DAC_Events(BYTE, event_register_t *);
extern void Start_Events_Sch(void);
extern void Update_Events_Sch_State(void);
extern void Decrement_Events_Sch_10msTmr(void);
extern BOOL Setup_Events_Erasure(void);
#endif
