#include <xc.h>
#include <stdio.h>	
#include <string.h>
#include <time.h>

#include "COMMON.H"
#include "TRAINMON.H"

extern rb_info_t RB_Info;					/* from ACC_RB.C */
extern time_t SystemClock;
const unsigned char Time_CharLookup[60][3] =
	{
	"00", "01", "02", "03", "04", "05", "06", "07", "08", "09", 
	"10", "11", "12", "13", "14", "15", "16", "17", "18", "19", 
	"20", "21", "22", "23", "24", "25", "26", "27", "28", "29", 
	"30", "31", "32", "33", "34", "35", "36", "37", "38", "39", 
	"40", "41", "42", "43", "44", "45", "46", "47", "48", "49", 
	"50", "51", "52", "53", "54", "55", "56", "57", "58", "59" 
	};

const unsigned char Time_Print_Init[TIME_SIZE+1] =
	{
	"00/00/0000 00:00:00"
	};

train_info_t Train_Info;
print_rec_t Print_Record;

void Initialise_Print_Record(void);
void Save_AxleCount(void);
void Save_Duration(void);
void Save_Serial_Number(void);
void Save_Entry_Time(void);
void Save_Exit_Time(void);
void Itoa2c(char , char *);


void Initialise_TrainMon(void)
{
	Train_Info.State = TM_NOT_STARTED;
	Train_Info.Serial_Number = 0;
	Train_Info.Fwd_Axle_Count = 0;
	Train_Info.Rev_Axle_Count = 0;
	Train_Info.Entry_Time = 0;
	Train_Info.Exit_Time = 0;
	Train_Info.Load_Time = 0;
	Train_Info.Occupied_Duration = 0;
	Train_Info.Loading_Duration = 0;

	Initialise_Print_Record();
}

void Start_TrainMon(void)
{
	Train_Info.State = TM_WAITING_FOR_TRAIN;
}

void Update_TrainMon_State(void)
{
	switch (Train_Info.State)
	{
		case TM_NOT_STARTED:
			break;
		case TM_WAITING_FOR_TRAIN:
			if (RB_Info.Local_DP_Status == RB_MODE_5)
				{
				/* Section Occupied */
				Train_Info.State = TM_TRAIN_ENTERED_LOADING_AREA;
				Train_Info.Fwd_Axle_Count = RB_Info.Local_DP_Count.Word;
				Train_Info.Entry_Time = SystemClock;
				}
			break;
		case TM_TRAIN_ENTERED_LOADING_AREA:
			if (RB_Info.Local_DP_Count.Word > Train_Info.Fwd_Axle_Count)
				{
				/*
				 * Train has not fully entered the loading area
				 */
				Train_Info.Fwd_Axle_Count = RB_Info.Local_DP_Count.Word;
				break;
				}
			if (RB_Info.Local_DP_Count.Word < Train_Info.Fwd_Axle_Count)
				{
				/*
				 * Loading is complete and Train has stated going back
				 */
				Train_Info.Rev_Axle_Count = RB_Info.Local_DP_Count.Word;
				Train_Info.Load_Time = SystemClock;
				Train_Info.State = TM_TRAIN_LEAVING_LOADING_AREA;
				}
			break;
		case TM_TRAIN_LEAVING_LOADING_AREA:
			if (RB_Info.Local_DP_Count.Word > Train_Info.Rev_Axle_Count)
				{
				Train_Info.Rev_Axle_Count = 0;
				Train_Info.State = TM_TRAIN_ENTERED_LOADING_AREA;
				break;
				}
			if (RB_Info.Local_DP_Status == RB_MODE_4)
				{
				/* Section Clear */
				Train_Info.Exit_Time = SystemClock;
				Train_Info.Serial_Number = (Train_Info.Serial_Number + 1);
				Train_Info.State = TM_COMPUTE_DURATION;
				}
			break;
		case TM_COMPUTE_DURATION:
			Train_Info.Occupied_Duration = (Train_Info.Exit_Time - Train_Info.Entry_Time);
			Train_Info.Loading_Duration = (Train_Info.Load_Time - Train_Info.Entry_Time);
			Save_Duration();
			Train_Info.State = TM_SAVE_AXLE_COUNT;
			break;
		case TM_SAVE_AXLE_COUNT:
			Save_AxleCount();
			Train_Info.State = TM_SAVE_SERIAL_NUMBER;
			break;
		case TM_SAVE_SERIAL_NUMBER:
			Save_Serial_Number();
			Train_Info.State = TM_SAVE_ENTRY_TIME;
			break;
		case TM_SAVE_ENTRY_TIME:
			Save_Entry_Time();
			Train_Info.State = TM_SAVE_EXIT_TIME;
			break;
		case TM_SAVE_EXIT_TIME:
			Save_Exit_Time();
			Train_Info.State = TM_POST_DATA_TO_PRINT_QUEUE;
			break;
		case TM_POST_DATA_TO_PRINT_QUEUE:
			Train_Info.Fwd_Axle_Count = 0;
			Train_Info.Rev_Axle_Count = 0;
			Train_Info.Entry_Time = 0;
			Train_Info.Exit_Time = 0;
			Train_Info.Load_Time = 0;
			Train_Info.Occupied_Duration = 0;
			Train_Info.Loading_Duration = 0;
			//Post_Data_to_Print_Queue(&Print_Record);
			Initialise_Print_Record();
			Train_Info.State = TM_WAITING_FOR_TRAIN;
			break;
		default:
			if (RB_Info.Local_DP_Status == RB_MODE_4)
				{
				Train_Info.State = TM_WAITING_FOR_TRAIN;
				}
			break;
	}
}

void Initialise_Print_Record(void)
{
	BYTE uchCnt;

	for (uchCnt = 0; uchCnt < SL_NO_SIZE; uchCnt++)
		{
			Print_Record.Sl_No[uchCnt] = ' ';
			Print_Record.Axle_Count[uchCnt] = ' ';
		}
	for (uchCnt = 0; uchCnt < TIME_SIZE; uchCnt++)
		{
			Print_Record.Entry_Time[uchCnt] = ' ';
			Print_Record.Exit_Time[uchCnt] = ' ';
		}
	for (uchCnt = 0; uchCnt < DURATION_SIZE; uchCnt++)
		{
			Print_Record.Occupied_Duration[uchCnt] = ' ';
			Print_Record.Loading_Duration[uchCnt] = ' ';
		}
}

void Save_Duration(void)
{
	UINT16 uiSum;
	BYTE uchReminder;

	Print_Record.Occupied_Duration[2] = ':';
	Print_Record.Occupied_Duration[5] = ':';

	if (Train_Info.Occupied_Duration < 0)
		{
		Print_Record.Occupied_Duration[0] = '?';
		Print_Record.Occupied_Duration[1] = '?';
		Print_Record.Occupied_Duration[3] = '?';
		Print_Record.Occupied_Duration[4] = '?';
		Print_Record.Occupied_Duration[6] = '?';
		Print_Record.Occupied_Duration[7] = '?';
		}
	else
		{
		if (Train_Info.Occupied_Duration >= SECONDS_PER_DAY)
			{
			Print_Record.Occupied_Duration[0] = '*';
			Print_Record.Occupied_Duration[1] = '*';
			Print_Record.Occupied_Duration[3] = '*';
			Print_Record.Occupied_Duration[4] = '*';
			Print_Record.Occupied_Duration[6] = '*';
			Print_Record.Occupied_Duration[7] = '*';
			}
		else
			{
			uiSum = Train_Info.Occupied_Duration;
			uchReminder = (BYTE)(uiSum % 60);
			Print_Record.Occupied_Duration[6] = Time_CharLookup[uchReminder][0];
			Print_Record.Occupied_Duration[7] = Time_CharLookup[uchReminder][1];
			uiSum /= 60;
			uchReminder = (BYTE)(uiSum % 60);
			Print_Record.Occupied_Duration[3] = Time_CharLookup[uchReminder][0];
			Print_Record.Occupied_Duration[4] = Time_CharLookup[uchReminder][1];
			uiSum /= 60;
			uchReminder = (BYTE)(uiSum % 24);
			Print_Record.Occupied_Duration[0] = Time_CharLookup[uchReminder][0];
			Print_Record.Occupied_Duration[1] = Time_CharLookup[uchReminder][1];
			}
		}

	Print_Record.Loading_Duration[2] = ':';
	Print_Record.Loading_Duration[5] = ':';

	if (Train_Info.Loading_Duration < 0)
		{
		Print_Record.Loading_Duration[0] = '?';
		Print_Record.Loading_Duration[1] = '?';
		Print_Record.Loading_Duration[3] = '?';
		Print_Record.Loading_Duration[4] = '?';
		Print_Record.Loading_Duration[6] = '?';
		Print_Record.Loading_Duration[7] = '?';
		}
	else
		{
		if (Train_Info.Loading_Duration >= SECONDS_PER_DAY)
			{
			Print_Record.Loading_Duration[0] = '*';
			Print_Record.Loading_Duration[1] = '*';
			Print_Record.Loading_Duration[3] = '*';
			Print_Record.Loading_Duration[4] = '*';
			Print_Record.Loading_Duration[6] = '*';
			Print_Record.Loading_Duration[7] = '*';
			}
		else
			{
			uiSum = Train_Info.Loading_Duration;
			uchReminder = (BYTE)(uiSum % 60);
			Print_Record.Loading_Duration[6] = Time_CharLookup[uchReminder][0];
			Print_Record.Loading_Duration[7] = Time_CharLookup[uchReminder][1];
			uiSum /= 60;
			uchReminder = (BYTE)(uiSum % 60);
			Print_Record.Loading_Duration[3] = Time_CharLookup[uchReminder][0];
			Print_Record.Loading_Duration[4] = Time_CharLookup[uchReminder][1];
			uiSum /= 60;
			uchReminder = (BYTE)(uiSum % 24);
			Print_Record.Loading_Duration[0] = Time_CharLookup[uchReminder][0];
			Print_Record.Loading_Duration[1] = Time_CharLookup[uchReminder][1];
			}
		}
}

void Save_AxleCount(void)
{
	UINT16 uiBuf;
	BYTE uchCnt;

	if (Train_Info.Fwd_Axle_Count > 9999)
		{
		Print_Record.Axle_Count[0] = '*';
		Print_Record.Axle_Count[1] = '*';
		Print_Record.Axle_Count[2] = '*';
		Print_Record.Axle_Count[3] = '*';
		return;
		}
	if (Train_Info.Fwd_Axle_Count > 999)
		{
		uchCnt = (Train_Info.Fwd_Axle_Count / 1000);
		Print_Record.Axle_Count[0] = uchCnt + '0';
		Print_Record.Axle_Count[1] = '0';
		Print_Record.Axle_Count[2] = '0';
		Print_Record.Axle_Count[3] = '0';
		}
	else
		{
		Print_Record.Axle_Count[0] = '0';
		Print_Record.Axle_Count[1] = '0';
		Print_Record.Axle_Count[2] = '0';
		Print_Record.Axle_Count[3] = '0';
		}

	uiBuf = (Train_Info.Fwd_Axle_Count % 1000);
	uchCnt = (BYTE) 2;
	 do {
		Print_Record.Axle_Count[uchCnt+1] = uiBuf % 10 + '0';
		uchCnt = uchCnt - 1;
    	} while ((uiBuf /= 10) > 0);
}

void Save_Serial_Number(void)
{
	UINT16 uiBuf;
	BYTE uchCnt;

	if (Train_Info.Serial_Number > 9999)
		{
		Print_Record.Sl_No[0] = '*';
		Print_Record.Sl_No[1] = '*';
		Print_Record.Sl_No[2] = '*';
		Print_Record.Sl_No[3] = '*';
		return;
		}
	if (Train_Info.Serial_Number > 999)
		{
		uchCnt = (Train_Info.Serial_Number / 1000);
		Print_Record.Sl_No[0] = uchCnt + '0';
		Print_Record.Sl_No[1] = '0';
		Print_Record.Sl_No[2] = '0';
		Print_Record.Sl_No[3] = '0';
		}

	uiBuf = (Train_Info.Serial_Number % 1000);
	uchCnt = (BYTE) 2;
	 do {
		Print_Record.Sl_No[uchCnt+1] = uiBuf % 10 + '0';
		uchCnt = uchCnt - 1;
    	} while ((uiBuf /= 10) > 0);
}

void Save_Entry_Time(void)
{
	struct tm *tp;
	BYTE uchCnt;

	for (uchCnt = 0; uchCnt < TIME_SIZE; uchCnt++)
		{
		Print_Record.Entry_Time[uchCnt] = Time_Print_Init[uchCnt];
		}
	tp = localtime(&Train_Info.Entry_Time);
//	Itoa2c(tp->tm_mday, Print_Record.Entry_Time+2);
//	Itoa2c(tp->tm_mon + 1, Print_Record.Entry_Time+5);
//	Itoa2c(tp->tm_year/100 + 19, Print_Record.Entry_Time+8);
//	Itoa2c(tp->tm_year%100, Print_Record.Entry_Time+10);
//	Itoa2c(tp->tm_hour, Print_Record.Entry_Time+13);
//	Itoa2c(tp->tm_min, Print_Record.Entry_Time+16);
//	Itoa2c(tp->tm_sec, Print_Record.Entry_Time+19);
}

void Save_Exit_Time(void)
{
	struct tm *tp;
	BYTE uchCnt;

	for (uchCnt = 0; uchCnt < TIME_SIZE; uchCnt++)
		{
		Print_Record.Exit_Time[uchCnt] = Time_Print_Init[uchCnt];
		}
	tp = localtime(&Train_Info.Exit_Time);
//	Itoa2c(tp->tm_mday, Print_Record.Exit_Time+2);
//	Itoa2c(tp->tm_mon + 1, Print_Record.Exit_Time+5);
//	Itoa2c(tp->tm_year/100 + 19, Print_Record.Exit_Time+8);
//	Itoa2c(tp->tm_year%100, Print_Record.Exit_Time+10);
//	Itoa2c(tp->tm_hour, Print_Record.Exit_Time+13);
//	Itoa2c(tp->tm_min, Print_Record.Exit_Time+16);
//	Itoa2c(tp->tm_sec, Print_Record.Exit_Time+19);
}

void Itoa2c(char uchInput, char * cp)
{
	*--cp = uchInput%10 + '0';
	if(uchInput /= 10)
		{
		*--cp = uchInput + '0';
		}
}

