#ifndef _COMM_SM_H_
#define _COMM_SM_H_
/*
 *      Comm_SM routines header file
 */

/*
 * I/O Ports - Communication related
 */
#define MODEM_RS						RJ0         /*Set RJ0 port to MODEM Request to Send Signal*/
#define MODEM_CD						RJ1         /*Set RJ1 port to MODEM Carrier detect Signal */
#define MODEM_CS						RJ2         /*Set RJ2 port to MODEM Clear to Send Signal*/

/* SM Comm Process time-outs in milli-seconds */
#define MODEM_TRC_ON_TIMEOUT			(200)		/* This is Maximum Time Scheduler Waits to Get Clear to Send Signal from Modem*/
#define TRANSMIT_WAIT_TIME				(50)		/* Pre Transmission Delay Time */
#define RS_HOLD_LOW_TIMEOUT				(40)		/* Once All Bytes are Transmitted, Release RS Signal after this time Completes */ 	

#define SM_DATA_RECEIVE_TIMEOUT    		 (18)		/* (18millsec)Maximum time allowed between Bytes of one Command from Station Master Unit*/
#define SM_COMM_LINK_FAILURE_TIMEOUT		(6000)	/* 6 Seconds */

/* Sm Scheduler -States */
typedef enum {
			SM_SCHEDULER_NOT_STARTED = 0,		/* Default State */
			SM_NO_DATA_RECEIVED,			/* when there is no commands to service scheduler will be in this State */ 
			SM_DATA_RECEPTION_STARTED,
			SM_DATA_RECEPTION_COMPLETE,	
			SM_CLEAR_FRAME_ERROR,	
			SM_CLEAR_OVERRUN_ERROR	
} sm_sch_state_t;


/* Sm Scheduler */
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

extern void Initialise_Sm_CommSch(void);
extern void Start_Sm_Communication(void);
extern void Update_Sm_Sch_State(void);
extern void Decrement_Sm_Sch_msTmr(void);

#endif
