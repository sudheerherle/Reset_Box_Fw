/*********************************************************************************************************************
	Project		: 	Single Section Digital Axle Counter
	Version		: 	2.0 
	Revision	:	1	
	Filename	: 	comm_smc.h
	Target MCU	: 	PIC24FJ256GB210   
    Compiler	: 	XC16 Compiler V1.21  
	Author		:	Sudheer Herle
	Date		:	
	Company Name: 	Insys Digital Systems
	Modification History:
					Rev No			Date		Description
					 --				 --				--    
*********************************************************************************************************************/
#ifndef _COMM_SMC_H_
#define _COMM_SMC_H_
/*
 *      Comm_SM routines header file
 */

/*
 * I/O Ports - Communication related
 */
#define M0						LATBbits.LATB9         /*Set RJ0 port to MODEM Request to Send Signal*/
#define MODEM_CD					PORTBbits.RB8         /*Set RJ1 port to MODEM Carrier detect Signal */
#define M1						LATBbits.LATB10         /*Set RJ2 port to MODEM Clear to Send Signal*/

/* SMC Comm Process time-outs in milli-seconds */
#define MODEM_TRC_ON_TIMEOUT			(20)		/* This is Maximum Time Scheduler Waits to Get Clear to Send Signal from Modem*/
#define TRANSMIT_WAIT_TIME				(50)		/* Pre Transmission Delay Time */
#define RS_HOLD_LOW_TIMEOUT				(40)		/* Once All Bytes are Transmitted, Release RS Signal after this time Completes */ 	

#define SM_DATA_RECEIVE_TIMEOUT    		 (2000)		/* (18millsec)Maximum time allowed between Bytes of one Command from Station Master Unit*/
#define SM_COMM_LINK_FAILURE_TIMEOUT		(6000)	/* 6 Seconds */


#define SMC_COMMAND_RECEIVE_TIMEOUT     (18)		/* (18millsec)Maximum time allowed between Bytes of one Command from Station Master Unit*/
#define TIME_BETWEEN_RECORDS_FOR_SMC	(20)		/* Transmission delay Between Records - 10ms */
#define SMC_RESPONSE_WAIT_TIME			(2000)		/* (2Sec)- Max time Smc scheduler wait for SM Reply */

#define SMC_COMM_SCHEDULER_SCAN_RATE	(875)
#define SMC_XMIT_MESSAGE_LENGTH			(80)
#define CRC16_LENGTH                            (2)
#define QUERY_SIZE                      (5)
#define SM_COMM_LINK_FAILURE_TIMEOUT		(6000)	/* 6 Seconds */
#define CD_LINK_FAILURE_TIMEOUT     		(500)	/* 0.5 Seconds */
#define AUTO_RESET_WAIT_TIMEOUT     		(1000)	/* 0.5 Seconds */
#define AUTO_RESET_HOLD_TIME                (100)

/* Smc Scheduler -States */
typedef enum {
            
            SMC_SCHEDULER_NOT_STARTED = 0,		/* Default State */
			SMC_NO_DATA_RECEIVED,			/* when there is no commands to service scheduler will be in this State */
			SMC_DATA_RECEPTION_STARTED,
			SMC_DATA_RECEPTION_COMPLETE,
			SMC_CLEAR_FRAME_ERROR,
			SMC_CLEAR_OVERRUN_ERROR,
            SMCPU_DEAD,
            CHECK_FOR_CD_WAIT,
            CHECK_FOR_CD_STATUS,
            WAIT_TO_SEND_NEXT_QUERY,
            EXPECT_RESPONSE_FROM_SMPU,
            SEND_QUERY,
            WAIT_FOR_TX_MODE,
            SET_MODEM_RX,
            SET_MODEM_TX,
            SMC_SCHEDULE_DECIDE,
                    SMC_LISTEN_SCHEDULER,
                    SMC_LISTEN_DATA,
                    SM_LISTEN_DATA_RECEPTION_COMPLETE,
                    
                    
                    
                    
} smc_sch_state_t;

 typedef union {
                struct{
                        BYTE	Header_byte;						
                        BYTE	Network_Index;	
                        BYTE    CPU_Index;
                        BYTE	CRC[CRC16_LENGTH];
                };
                BYTE Query_buffer[5];
            } query_t;
/* Smc Scheduler */
typedef struct {	
			smc_sch_state_t	State;			/* Smc scheduler present state */	
			smc_sch_state_t	Next_State;		/* Smc scheduler Next state */
			UINT16			Timeout_ms;		/* One milliSecond Variable for Smc scheduler*/
            UINT16          CD_timeout_ms;
			UINT16			ScanPeriod;	
			UINT16			ElapsedTime;
			BYTE			BytePeriod;		/* Transmission Time of One Byte deponds on Baud Rate */
			UINT16          Setup_Timeout_ms;
            BYTE            CPU_Counts;
            query_t         Query;           

} smc_sch_info_t;

typedef struct {
			BYTE	Msg_Length;						
			BYTE	Index;			
			BYTE	Msg_Buffer[SMC_XMIT_MESSAGE_LENGTH];
} smc_info_t;

typedef struct {
			BYTE	Msg_Length;						
			BYTE	Index;			
			BYTE	query_buf[QUERY_SIZE];//Msg_Buffer[SMC_XMIT_MESSAGE_LENGTH];
} query_info_t;

typedef struct {
			BOOL 	redundant_available;						
			BYTE	CPU_Addrs;			
			BYTE	Index;
}system_t;
            
typedef enum {
			SM_SCHEDULER_NOT_STARTED = 0,		/* Default State */
			SM_NO_DATA_RECEIVED,			/* when there is no commands to service scheduler will be in this State */
			SM_DATA_RECEPTION_STARTED,
			SM_DATA_RECEPTION_COMPLETE,
            SM_TRANSMIT_SMC_DATA,
            HOLD_SMC_MODEM_RS_LOW,
			SM_CLEAR_FRAME_ERROR,
			SM_CLEAR_OVERRUN_ERROR,
            
} sm_sch_state_t;


typedef struct {
			sm_sch_state_t	State;			/* Sm scheduler present state */
			UINT16			Timeout_ms;		/* One milliSecond Variable for Sm scheduler*/
			BYTE			BytePeriod;		/* Transmission Time of One Byte deponds on Baud Rate */
} sm_sch_info_t;

typedef struct {
			BYTE	Msg_Length;
			BYTE	Index;
			BYTE	Msg_Buffer[SM_MESSAGE_LENGTH];
} sm_info_t;

extern void Initialise_Smc_CommSch(void);
extern void Start_Smc_Communication(void);
extern void Update_Smc_Sch_State(void);
extern void Decrement_Smc_Sch_msTmr(void);
extern void Start_Reset_Seq();
#endif
