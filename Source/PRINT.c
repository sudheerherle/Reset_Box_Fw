#include <xc.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "COMMON.h"
#include "PRINT.H"

//const unsigned char Report_Line[22][PRINT_RECORD_SIZE] =
//	{
//	"                                              \r\n\r\n\r\n\r\n\r\n\r\n",
//	"          SSDAC-G36                  Serial Number:       \r\n",
//    "                                                          \r\n",
//	"         /-----------------------+-----------------------\\\r\n",
//	"         |                       |                       |\r\n",
//	"         | TRAIN ENTRY TIME      | TRAIN EXIT TIME       |\r\n",
//	"         |                       |                       |\r\n",
//	"         |                       |                       |\r\n",
//	"         |                       |                       |\r\n",
//	"         |-----------------------+-----------------------|\r\n",
//	"         |                       |                       |\r\n",
//	"         | OCCUPATION DURATION   | EXIT START/LOADING    |\r\n",
//	"         |      (in Hours)       | DURATION (in Hours)   |\r\n",
//	"         |                       |                       |\r\n",
//	"         |          (hh:mm:ss)   |          (hh:mm:ss)   |\r\n",
//	"         |                       |                       |\r\n",
//	"         |-----------------------+-----------------------+\r\n",
//	"         |                       |                        \r\n",
//	"         | AXLE COUNT            |                        \r\n",
//	"         |                       |                        \r\n",
//	"         |                       |                        \r\n",
//	"         \\-----------------------+                        \r\n"
//	};
//

const unsigned char Report_Line[24][PRINT_RECORD_SIZE] =
	{
	"                                                \r\n\r\n\r\n\r\n\r\n",
	"                          TIME CHART                      \r\n",
    "                          ----------                      \r\n",
	"          DIGITAL AXLE COUNTER SSDAC-G36     Sl.No:       \r\n",
    "                                                          \r\n",
	"         =================================================\r\n",
	"         |                       |                       |\r\n",
	"         | TRAIN ENTRY TIME      | TRAIN EXIT TIME       |\r\n",
	"         |                       |                       |\r\n",
	"         |                       |                       |\r\n",
	"         |                       |                       |\r\n",
	"         |=======================+=======================|\r\n",
	"         |                       |                       |\r\n",
	"         | OCCUPATION DURATION   | LOADING DURATION      |\r\n",
	"         |      (in Hours)       |     (in Hours)        |\r\n",
	"         |                       |                       |\r\n",
	"         |          (hh:mm:ss)   |          (hh:mm:ss)   |\r\n",
	"         |                       |                       |\r\n",
	"         |=======================+=======================|\r\n",
	"         |                       |                       |\r\n",
	"         | ENTRY AXLE COUNTS     |                       |\r\n",
	"         |                       |                       |\r\n",
	"         |                       |                       |\r\n",
	"         ========================+========================\r\n"
	};

ring_print_buffer_t Printer;

print_info_t	Print_Info;	

void Print_Byte(BYTE);
void Build_Record_4(void);
void Build_Record_10(void);
void Build_Record_17(void);
void Build_Record_22(void);

void Initialise_Printer(void)
{
	Printer.Head = 0;
	Printer.Tail = 0;
	Print_Info.State = PR_NOT_STARTED;
	Print_Info.Page_No = 0;
	Print_Info.Line_No = 0;
	Print_Info.Col_No = 0;
}

void Start_Printer(void)
{
	PRINTER_SELECTLN = SET_LOW;
	PRINTER_AUTOFD = SET_HIGH;
	PRINTER_INIT = SET_HIGH;
	Print_Info.State = PR_FORM_FEED;
}

void Update_Printer_State(void)
{
	switch (Print_Info.State)
		{
		case PR_CHECK_QUEUE:
//			if (Printer.Head != Printer.Tail)
				{
				/*
		 		 * Print record available in queue
		 		 */		
				Print_Info.Col_No = 0;
				Print_Info.State = PR_REPORT_RECORD_1;
				}
			break;
		case PR_REPORT_RECORD_1:
			if (PRINTER_FAULT == SET_LOW)
				{
				Print_Info.State = PR_PRINTER_ERROR;
				break;
				}
			if (PRINTER_BUSY == SET_LOW)
				{
				Print_Byte(Report_Line[0][Print_Info.Col_No++]);
				}
			if (Print_Info.Col_No >= PRINT_RECORD_SIZE)
				{			
				Print_Info.Col_No = 0;
				Print_Info.State = PR_REPORT_RECORD_2;
				}
			break;
		case PR_REPORT_RECORD_2:
			if (PRINTER_FAULT == SET_LOW)
				{
				Print_Info.State = PR_PRINTER_ERROR;
				break;
				}
			if (PRINTER_BUSY == SET_LOW)
				{
				Print_Byte(Report_Line[1][Print_Info.Col_No++]);
				}
			if (Print_Info.Col_No >= PRINT_RECORD_SIZE)
				{			
				Print_Info.Col_No = 0;
				Print_Info.State = PR_REPORT_RECORD_3;
				}
			break;
		case PR_REPORT_RECORD_3:
			if (PRINTER_FAULT == SET_LOW)
				{
				Print_Info.State = PR_PRINTER_ERROR;
				break;
				}
			if (PRINTER_BUSY == SET_LOW)
				{
				Print_Byte(Report_Line[2][Print_Info.Col_No++]);
				}
			if (Print_Info.Col_No >= PRINT_RECORD_SIZE)
				{			
				Build_Record_4();
				Print_Info.Col_No = 0;
				Print_Info.State = PR_REPORT_RECORD_4;
				}
			break;
		case PR_REPORT_RECORD_4:
			if (PRINTER_FAULT == SET_LOW)
				{
				Print_Info.State = PR_PRINTER_ERROR;
				break;
				}
			if (PRINTER_BUSY == SET_LOW)
				{
				Print_Byte(Print_Info.Buffer[Print_Info.Col_No++]);
				}
			if (Print_Info.Col_No >= PRINT_RECORD_SIZE)
				{			
				Print_Info.Col_No = 0;
				Print_Info.State = PR_REPORT_RECORD_5;
				}
			break;
		case PR_REPORT_RECORD_5:
			if (PRINTER_FAULT == SET_LOW)
				{
				Print_Info.State = PR_PRINTER_ERROR;
				break;
				}
			if (PRINTER_BUSY == SET_LOW)
				{
				Print_Byte(Report_Line[4][Print_Info.Col_No++]);
				}
			if (Print_Info.Col_No >= PRINT_RECORD_SIZE)
				{			
				Print_Info.Col_No = 0;
				Print_Info.State = PR_REPORT_RECORD_6;
				}
			break;
		case PR_REPORT_RECORD_6:
			if (PRINTER_FAULT == SET_LOW)
				{
				Print_Info.State = PR_PRINTER_ERROR;
				break;
				}
			if (PRINTER_BUSY == SET_LOW)
				{
				Print_Byte(Report_Line[5][Print_Info.Col_No++]);
				}
			if (Print_Info.Col_No >= PRINT_RECORD_SIZE)
				{			
				Print_Info.Col_No = 0;
				Print_Info.State = PR_REPORT_RECORD_7;
				}
			break;
		case PR_REPORT_RECORD_7:
			if (PRINTER_FAULT == SET_LOW)
				{
				Print_Info.State = PR_PRINTER_ERROR;
				break;
				}
			if (PRINTER_BUSY == SET_LOW)
				{
				Print_Byte(Report_Line[6][Print_Info.Col_No++]);
				}
			if (Print_Info.Col_No >= PRINT_RECORD_SIZE)
				{			
				Print_Info.Col_No = 0;
				Print_Info.State = PR_REPORT_RECORD_8;
				}
			break;
		case PR_REPORT_RECORD_8:
			if (PRINTER_FAULT == SET_LOW)
				{
				Print_Info.State = PR_PRINTER_ERROR;
				break;
				}
			if (PRINTER_BUSY == SET_LOW)
				{
				Print_Byte(Report_Line[7][Print_Info.Col_No++]);
				}
			if (Print_Info.Col_No >= PRINT_RECORD_SIZE)
				{			
				Print_Info.Col_No = 0;
				Print_Info.State = PR_REPORT_RECORD_9;
				}
			break;
		case PR_REPORT_RECORD_9:
			if (PRINTER_FAULT == SET_LOW)
				{
				Print_Info.State = PR_PRINTER_ERROR;
				break;
				}
			if (PRINTER_BUSY == SET_LOW)
				{
				Print_Byte(Report_Line[8][Print_Info.Col_No++]);
				}
			if (Print_Info.Col_No >= PRINT_RECORD_SIZE)
				{			
				Build_Record_10();
				Print_Info.Col_No = 0;
				Print_Info.State = PR_REPORT_RECORD_10;
				}
			break;
		case PR_REPORT_RECORD_10:
			if (PRINTER_FAULT == SET_LOW)
				{
				Print_Info.State = PR_PRINTER_ERROR;
				break;
				}
			if (PRINTER_BUSY == SET_LOW)
				{
				Print_Byte(Print_Info.Buffer[Print_Info.Col_No++]);
				}
			if (Print_Info.Col_No >= PRINT_RECORD_SIZE)
				{			
				Print_Info.Col_No = 0;
				Print_Info.State = PR_REPORT_RECORD_11;
				}
			break;
		case PR_REPORT_RECORD_11:
			if (PRINTER_FAULT == SET_LOW)
				{
				Print_Info.State = PR_PRINTER_ERROR;
				break;
				}
			if (PRINTER_BUSY == SET_LOW)
				{
				Print_Byte(Report_Line[10][Print_Info.Col_No++]);
				}
			if (Print_Info.Col_No >= PRINT_RECORD_SIZE)
				{			
				Print_Info.Col_No = 0;
				Print_Info.State = PR_REPORT_RECORD_12;
				}
			break;
		case PR_REPORT_RECORD_12:
			if (PRINTER_FAULT == SET_LOW)
				{
				Print_Info.State = PR_PRINTER_ERROR;
				break;
				}
			if (PRINTER_BUSY == SET_LOW)
				{
				Print_Byte(Report_Line[11][Print_Info.Col_No++]);
				}
			if (Print_Info.Col_No >= PRINT_RECORD_SIZE)
				{			
				Print_Info.Col_No = 0;
				Print_Info.State = PR_REPORT_RECORD_13;
				}
			break;
		case PR_REPORT_RECORD_13:
			if (PRINTER_FAULT == SET_LOW)
				{
				Print_Info.State = PR_PRINTER_ERROR;
				break;
				}
			if (PRINTER_BUSY == SET_LOW)
				{
				Print_Byte(Report_Line[12][Print_Info.Col_No++]);
				}
			if (Print_Info.Col_No >= PRINT_RECORD_SIZE)
				{			
				Print_Info.Col_No = 0;
				Print_Info.State = PR_REPORT_RECORD_14;
				}
			break;
		case PR_REPORT_RECORD_14:
			if (PRINTER_FAULT == SET_LOW)
				{
				Print_Info.State = PR_PRINTER_ERROR;
				break;
				}
			if (PRINTER_BUSY == SET_LOW)
				{
				Print_Byte(Report_Line[13][Print_Info.Col_No++]);
				}
			if (Print_Info.Col_No >= PRINT_RECORD_SIZE)
				{			
				Print_Info.Col_No = 0;
				Print_Info.State = PR_REPORT_RECORD_15;
				}
			break;
		case PR_REPORT_RECORD_15:
			if (PRINTER_FAULT == SET_LOW)
				{
				Print_Info.State = PR_PRINTER_ERROR;
				break;
				}
			if (PRINTER_BUSY == SET_LOW)
				{
				Print_Byte(Report_Line[14][Print_Info.Col_No++]);
				}
			if (Print_Info.Col_No >= PRINT_RECORD_SIZE)
				{			
				Print_Info.Col_No = 0;
				Print_Info.State = PR_REPORT_RECORD_16;
				}
			break;
		case PR_REPORT_RECORD_16:
			if (PRINTER_FAULT == SET_LOW)
				{
				Print_Info.State = PR_PRINTER_ERROR;
				break;
				}
			if (PRINTER_BUSY == SET_LOW)
				{
				Print_Byte(Report_Line[15][Print_Info.Col_No++]);
				}
			if (Print_Info.Col_No >= PRINT_RECORD_SIZE)
				{			
				Build_Record_17();
				Print_Info.Col_No = 0;
				Print_Info.State = PR_REPORT_RECORD_17;
				}
			break;
		case PR_REPORT_RECORD_17:
			if (PRINTER_FAULT == SET_LOW)
				{
				Print_Info.State = PR_PRINTER_ERROR;
				break;
				}
			if (PRINTER_BUSY == SET_LOW)
				{
				Print_Byte(Print_Info.Buffer[Print_Info.Col_No++]);
				}
			if (Print_Info.Col_No >= PRINT_RECORD_SIZE)
				{			
				Print_Info.Col_No = 0;
				Print_Info.State = PR_REPORT_RECORD_18;
				}
			break;
		case PR_REPORT_RECORD_18:
			if (PRINTER_FAULT == SET_LOW)
				{
				Print_Info.State = PR_PRINTER_ERROR;
				break;
				}
			if (PRINTER_BUSY == SET_LOW)
				{
				Print_Byte(Report_Line[17][Print_Info.Col_No++]);
				}
			if (Print_Info.Col_No >= PRINT_RECORD_SIZE)
				{			
				Print_Info.Col_No = 0;
				Print_Info.State = PR_REPORT_RECORD_19;
				}
			break;
		case PR_REPORT_RECORD_19:
			if (PRINTER_FAULT == SET_LOW)
				{
				Print_Info.State = PR_PRINTER_ERROR;
				break;
				}
			if (PRINTER_BUSY == SET_LOW)
				{
				Print_Byte(Report_Line[18][Print_Info.Col_No++]);
				}
			if (Print_Info.Col_No >= PRINT_RECORD_SIZE)
				{			
				Print_Info.Col_No = 0;
				Print_Info.State = PR_REPORT_RECORD_20;
				}
			break;
		case PR_REPORT_RECORD_20:
			if (PRINTER_FAULT == SET_LOW)
				{
				Print_Info.State = PR_PRINTER_ERROR;
				break;
				}
			if (PRINTER_BUSY == SET_LOW)
				{
				Print_Byte(Report_Line[19][Print_Info.Col_No++]);
				}
			if (Print_Info.Col_No >= PRINT_RECORD_SIZE)
				{			
				Print_Info.Col_No = 0;
				Print_Info.State = PR_REPORT_RECORD_21;
				}
			break;
		case PR_REPORT_RECORD_21:
			if (PRINTER_FAULT == SET_LOW)
				{
				Print_Info.State = PR_PRINTER_ERROR;
				break;
				}
			if (PRINTER_BUSY == SET_LOW)
				{
				Print_Byte(Report_Line[20][Print_Info.Col_No++]);
				}
			if (Print_Info.Col_No >= PRINT_RECORD_SIZE)
				{			
				Build_Record_22();
				Print_Info.Col_No = 0;
				Print_Info.State = PR_REPORT_RECORD_22;
				}
			break;
		case PR_REPORT_RECORD_22:
			if (PRINTER_FAULT == SET_LOW)
				{
				Print_Info.State = PR_PRINTER_ERROR;
				break;
				}
			if (PRINTER_BUSY == SET_LOW)
				{
				Print_Byte(Print_Info.Buffer[Print_Info.Col_No++]);
				}
			if (Print_Info.Col_No >= PRINT_RECORD_SIZE)
				{			
				Print_Info.Col_No = 0;
				Print_Info.State = PR_REPORT_RECORD_23;
				}
			break;
		case PR_REPORT_RECORD_23:
			if (PRINTER_FAULT == SET_LOW)
				{
				Print_Info.State = PR_PRINTER_ERROR;
				break;
				}
			if (PRINTER_BUSY == SET_LOW)
				{
				Print_Byte(Report_Line[22][Print_Info.Col_No++]);
				}
			if (Print_Info.Col_No >= PRINT_RECORD_SIZE)
				{			
				Print_Info.Col_No = 0;
				Print_Info.State = PR_REPORT_RECORD_24;
				}
			break;
		case PR_REPORT_RECORD_24:
			if (PRINTER_FAULT == SET_LOW)
				{
				Print_Info.State = PR_PRINTER_ERROR;
				break;
				}
			if (PRINTER_BUSY == SET_LOW)
				{
				Print_Byte(Report_Line[23][Print_Info.Col_No++]);
				}
			if (Print_Info.Col_No >= PRINT_RECORD_SIZE)
				{			
				/*
				 * Increment the head pointer of Ring Buffer
				 */	
				Printer.Head = (Printer.Head + 1) % TOTAL_NO_PRINT_RECORDS;
				Print_Info.Col_No = 0;
				Print_Info.State = PR_FORM_FEED;
				}
			break;
		case PR_FORM_FEED:
			if (PRINTER_BUSY == SET_LOW)
				{
				Print_Byte(FORM_FEED);
				Print_Info.Line_No = 0;
				Print_Info.Col_No = 0;
				Print_Info.State = PR_CHECK_QUEUE;
				}
			break;
		case PR_PAPER_OUT:
			break;
                case PR_NOT_STARTED:
                        break;
		case PR_PRINTER_ERROR:
			if (PRINTER_FAULT == SET_HIGH)
				{
				Nop();		Nop();		Nop();
				Nop();		Nop();		Nop();
				if (PRINTER_FAULT == SET_HIGH)
					{
					PRINTER_SELECTLN = SET_LOW;
					Print_Info.State = PR_CHECK_QUEUE;
					}
				}
			break;
		}
}

void Post_Data_to_Print_Queue(print_rec_t *Record)
{
	BYTE uchCnt;
//	BYTE uchCnt;


	for (uchCnt = 0; uchCnt < SL_NO_SIZE; uchCnt++)
		{
		Printer.Output[Printer.Tail].Sl_No[uchCnt] = Record->Sl_No[uchCnt];
		Printer.Output[Printer.Tail].Axle_Count[uchCnt] = Record->Axle_Count[uchCnt];
		}
	for (uchCnt = 0; uchCnt < TIME_SIZE; uchCnt++)
		{
		Printer.Output[Printer.Tail].Entry_Time[uchCnt] = Record->Entry_Time[uchCnt];
		Printer.Output[Printer.Tail].Exit_Time[uchCnt] = Record->Exit_Time[uchCnt];
		}
	for (uchCnt = 0; uchCnt < DURATION_SIZE; uchCnt++)
		{
		Printer.Output[Printer.Tail].Occupied_Duration[uchCnt] = Record->Occupied_Duration[uchCnt];
		Printer.Output[Printer.Tail].Loading_Duration[uchCnt] = Record->Loading_Duration[uchCnt];
		}

	/*
	 * Increment the tail pointer of Ring Buffer
	 */	
	Printer.Tail = (Printer.Tail + 1) % TOTAL_NO_PRINT_RECORDS;
}

void Print_Byte(BYTE Data)
{
		PRINTER_DATA = PRINTER_DATA | Data;

		Nop(); 		Nop();
		Nop();		Nop();
		Nop();		Nop();
		Nop(); 		Nop();
		Nop();		Nop();
		Nop();		Nop();Nop(); 		Nop();
		Nop();		Nop();
		Nop();		Nop();Nop(); 		Nop();
		Nop();		Nop();		Nop();		Nop();Nop(); 		Nop();
		Nop();		Nop();
		Nop();		Nop();Nop(); 		Nop();
		Nop();		Nop();
		Nop();		Nop();Nop(); 		Nop();
		Nop();		Nop();
		Nop();		Nop();Nop(); 		Nop();
		Nop();		Nop();
		Nop();		Nop();
		PRINTER_STROBE = SET_LOW;

		Nop(); 		Nop();
		Nop();		Nop();
		Nop();		Nop();
        Nop(); 		Nop();
		Nop();		Nop();
		Nop();		Nop();Nop(); 		Nop();
		Nop();		Nop();
		Nop();		Nop();Nop(); 		Nop();
		Nop();		Nop();
		Nop();		Nop();Nop(); 		Nop();
		Nop();		Nop();
		Nop();		Nop();Nop(); 		Nop();
		Nop();		Nop();
		Nop();		Nop();Nop(); 		Nop();
		Nop();		Nop();
		Nop();		Nop();Nop(); 		Nop();
		Nop();		Nop();
		Nop();		Nop();Nop(); 		Nop();
		Nop();		Nop();
		Nop();		Nop();Nop(); 		Nop();
		Nop();		Nop();
		Nop();		Nop();

		PRINTER_STROBE = SET_HIGH;
}

void Build_Record_4(void)
{
	BYTE uchSrcCnt;
	BYTE uchDestCnt;

	for (uchSrcCnt = 0; uchSrcCnt < PRINT_RECORD_SIZE; uchSrcCnt++)
		{
		Print_Info.Buffer[uchSrcCnt] = Report_Line[3][uchSrcCnt];
		}
	for (uchSrcCnt = 0, uchDestCnt = 52; uchSrcCnt < SL_NO_SIZE; uchSrcCnt++, uchDestCnt++)
		{
		Print_Info.Buffer[uchDestCnt] = Printer.Output[Printer.Head].Sl_No[uchSrcCnt];
		}
}

void Build_Record_10(void)
{
	BYTE uchSrcCnt;
	BYTE uchDestCnt;

	for (uchSrcCnt = 0; uchSrcCnt < PRINT_RECORD_SIZE; uchSrcCnt++)
		{
		Print_Info.Buffer[uchSrcCnt] = Report_Line[9][uchSrcCnt];
		}
	for (uchSrcCnt = 0, uchDestCnt = 11; uchSrcCnt < TIME_SIZE; uchSrcCnt++, uchDestCnt++)
		{
		Print_Info.Buffer[uchDestCnt] = Printer.Output[Printer.Head].Entry_Time[uchSrcCnt];
		}
	for (uchSrcCnt = 0, uchDestCnt = 35; uchSrcCnt < TIME_SIZE; uchSrcCnt++, uchDestCnt++)
		{
		Print_Info.Buffer[uchDestCnt] = Printer.Output[Printer.Head].Exit_Time[uchSrcCnt];
		}
}

void Build_Record_17(void)
{
	BYTE uchSrcCnt;
	BYTE uchDestCnt;

	for (uchSrcCnt = 0; uchSrcCnt < PRINT_RECORD_SIZE; uchSrcCnt++)
		{
		Print_Info.Buffer[uchSrcCnt] = Report_Line[16][uchSrcCnt];
		}
	for (uchSrcCnt = 0, uchDestCnt = 11; uchSrcCnt < DURATION_SIZE; uchSrcCnt++, uchDestCnt++)
		{
		Print_Info.Buffer[uchDestCnt] = Printer.Output[Printer.Head].Occupied_Duration[uchSrcCnt];
		}
	for (uchSrcCnt = 0, uchDestCnt = 35; uchSrcCnt < DURATION_SIZE; uchSrcCnt++, uchDestCnt++)
		{
		Print_Info.Buffer[uchDestCnt] = Printer.Output[Printer.Head].Loading_Duration[uchSrcCnt];
		}
}

void Build_Record_22(void)
{
	BYTE uchSrcCnt;
	BYTE uchDestCnt;

	for (uchSrcCnt = 0; uchSrcCnt < PRINT_RECORD_SIZE; uchSrcCnt++)
		{
		Print_Info.Buffer[uchSrcCnt] = Report_Line[21][uchSrcCnt];
		}
	for (uchSrcCnt = 0, uchDestCnt = 11; uchSrcCnt < SL_NO_SIZE; uchSrcCnt++, uchDestCnt++)
		{
		Print_Info.Buffer[uchDestCnt] = Printer.Output[Printer.Head].Axle_Count[uchSrcCnt];
		}
}

