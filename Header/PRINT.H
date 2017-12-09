#ifndef _PRINT_H_
#define _PRINT_H_

#define FORM_FEED					(0x0C)
#define CARRIAGE_RETURN				(0x0D)
#define LINE_FEED					(0x0A)
/* Input PORT Lines */
#define PRINTER_ACK                             PORTCbits.RC1
#define PRINTER_BUSY				PORTGbits.RG9
#define PRINTER_PAPER_OUT			PORTCbits.RC4
#define PRINTER_SELECT				PORTBbits.RB4
#define PRINTER_FAULT				PORTBbits.RB3
/* Output PORT Lines */
#define PRINTER_STROBE				PORTAbits.RA0
#define PRINTER_AUTOFD				PORTCbits.RC2
#define PRINTER_INIT				PORTCbits.RC3
#define PRINTER_SELECTLN			PORTBbits.RB5
#define PRINTER_DATA				PORTE

/*  - States */
typedef enum {
			PR_NOT_STARTED = 0,
			PR_CHECK_QUEUE,
			PR_REPORT_RECORD_1,
			PR_REPORT_RECORD_2,
			PR_REPORT_RECORD_3,
			PR_REPORT_RECORD_4,
			PR_REPORT_RECORD_5,
			PR_REPORT_RECORD_6,
			PR_REPORT_RECORD_7,
			PR_REPORT_RECORD_8,
			PR_REPORT_RECORD_9,
			PR_REPORT_RECORD_10,
			PR_REPORT_RECORD_11,
			PR_REPORT_RECORD_12,
			PR_REPORT_RECORD_13,
			PR_REPORT_RECORD_14,
			PR_REPORT_RECORD_15,
			PR_REPORT_RECORD_16,
			PR_REPORT_RECORD_17,
			PR_REPORT_RECORD_18,
			PR_REPORT_RECORD_19,
			PR_REPORT_RECORD_20,
			PR_REPORT_RECORD_21,
			PR_REPORT_RECORD_22,
			PR_REPORT_RECORD_23,
			PR_REPORT_RECORD_24,
			PR_FORM_FEED,
			PR_PAPER_OUT,
			PR_PRINTER_ERROR
} pr_state_t;

typedef struct {
			pr_state_t State;
			BYTE 		Page_No;
			BYTE		Col_No;
			BYTE		Line_No;
			BYTE		Buffer[PRINT_RECORD_SIZE];
} print_info_t;

/* Ring Buffer - for buffering print records */

typedef struct {
		BYTE 		Head;	/* Pointer for printing records */
		BYTE 		Tail;	/* Pointer for saving the records */
		print_rec_t	Output[TOTAL_NO_PRINT_RECORDS];
} ring_print_buffer_t;

extern void Initialise_Printer(void);
extern void Start_Printer(void);
extern void Update_Printer_State(void);

#endif
