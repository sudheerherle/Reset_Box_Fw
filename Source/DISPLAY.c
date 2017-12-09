#include <xc.h>
#include <stdio.h>
#include <string.h>

#include "COMMON.h"
#include "DISPLAY.H"

 rb_itoa_t RB_Itoa;

extern  rb_info_t RB_Info;						/* from ACC_RB.C */

extern BYTE uchIDInfo[];
BYTE uchRBVer[] = "001";

const unsigned char uchHexadecimalLookup[16] =
	{	'0', '1', '2', '3', '4', '5', '6', '7', '8',
	 	'9', 'A', 'B', 'C', 'D', 'E', 'F'
	};

const unsigned char DAC_ModeLookup[MAX_TYPE_OF_MODES + 1][21] =
	{"Piloting in Progress",	/* Mode 0 - Unknown */
	 "  Waiting for RESET ",	/* Mode 1 */
	 "   Reset Applied    ",	/* Mode 2 */
	 "Wait for Pilot Train",	/* Mode 3 */
	 "   Section CLEAR    ",	/* Mode 4 */
	 "  Section OCCUPIED  ",	/* Mode 5 */
	 " Failure at REMOTE  ",	/* Mode 6 */
	};
const unsigned char Error_Table[TOTAL_NO_ERRORS+1][NO_OF_CHARACTERS_PER_LINE+1] = {
	/*	 123456789-123456789-    123456789-123456789-   123456789-123456789- */
		"  System in Error   ",
		"01 PD1 Card Missing ",	"02 PD2 Card Missing ",	"03 SM CPU Missing   ",
	 	"04 Modem A Missing  ",	"05 Modem B Missing  ", "06 RLYA Drv Missing ",
	    "07 RLYB Drv Missing ", "08 Peer CPU Missing ", "09 LU1-US1 Link Fail",
	    "10 LU1-US2 Link Fail", "11 LU1-DS1 Link Fail", "12 LU1-DS2 Link Fail",
		"13 US1-LU1 Link Fail",	"14 US2-LU1 Link Fail", "15 DS1-LU1 Link Fail",
		"16 DS2-LU1 Link Fail", "17 LU2-US1 Link Fail", "18 LU2-US2 Link Fail",
	    "19 LU2-DS1 Link Fail", "20 LU2-DS2 Link Fail", "21 US1-LU2 Link Fail",
		"22 US2-LU2 Link Fail", "23 DS1-LU2 Link Fail",	"24 DS2-LU2 Link Fail",
		"25 Modem No Carrier ", "26 PeerCPU Link Fail",	"27 Failure at REMOTE",
		"28 Failure at REMOTE", "29 Peer System Fail ", "*****************   ",
	    "*****************   ", "*****************   ",	"33 PD1 Sup Low      ",
		"34 PD2 Sup Low      ",	"35 PD1 Pulsating    ",	"36 PD2 Pulsating    ",
	    "37 PD State Missing ", "38 PD Sup Pulsating ",	"39 PD State Fail    ",
	    "40 PD Not Detecting ", "41 PR Relay A Fail  ",	"42 PR Relay B Fail  ",
		"43 VR Relay A Fail  ", "44 VR Relay B Fail  ",	"45 Direct-OUTCount  ",
		"*****************   ", "*****************   ", "*****************" ,
	    "49 Power fail at DS1", "50 Power fail at DS2",	"51 Power fail at US1",
        "52 Power fail at US2",	"*****************   ", "*****************   ",
    	"*****************   ", "*****************   ", "57 Invalid Net Addr ",
        "58 BAD CRC          ",	"59 Wrong Config     ",	"60 Invalid_Count    ",
        "61 RAM Test Failed  ", "*****************   ", "*****************   ",
	    "*****************   "
	};

void Update_RB_Itoa_State(void);
void Update_RB_Local_Mode_State(void);
void Update_RB_Remote_Mode_State(void);
void Display_SSDAC_Not_Powered_Up(void);
void Render_Local_DP_Checksum(void);
void Render_Remote_DP_Checksum(void);
void Render_Local_DP_Counts(void);
void Render_Remote_DP_Counts(void);
void Render_Local_DP_Status(void);
void Render_Remote_DP_Status(void);

void Initialise_RB_Mode(void)
{
	RB_Info.Timeout_10ms = 0;
}

void Update_RB_Mode_State(void)
{
	Update_RB_Itoa_State();

	if (RB_Info.Timeout_10ms == TIMEOUT_EVENT)
		{
		Update_RB_Local_Mode_State();
		Update_RB_Remote_Mode_State();
		RB_Info.Timeout_10ms = RB_DISPLAY_REFRESH_RATE;
		}

}

void Decrement_RB_Mode_10msTmr(void)
{
	if (RB_Info.Timeout_10ms > 0)
		{
		RB_Info.Timeout_10ms = RB_Info.Timeout_10ms - 1;
		}
}

void Update_RB_Local_Mode_State(void)
{
	switch (RB_Info.Local_DP_Status)
		{
		case RB_MODE_NOT_STARTED:
			Display_SSDAC_Not_Powered_Up();
			break;
		case RB_MODE_1:
			Render_Local_DP_Checksum();
			Render_Local_DP_Status();
			break;
		case RB_MODE_2:
			Render_Local_DP_Checksum();
			Render_Local_DP_Status();
			break;
		case RB_MODE_3:
			Render_Local_DP_Counts();
			Render_Local_DP_Status();
			break;
		case RB_MODE_4:
			Render_Local_DP_Counts();
			Render_Local_DP_Status();
			break;
		case RB_MODE_5:
			Render_Local_DP_Counts();
			Render_Local_DP_Status();
			break;
		case RB_MODE_6:
			Render_Local_DP_Counts();
			Render_Local_DP_Status();
			break;
	default:
		break;
		}
}

void Update_RB_Remote_Mode_State(void)
{
	switch (RB_Info.Remote_DP_Status)
		{
		case RB_MODE_NOT_STARTED:
			break;
		case RB_MODE_1:
			Render_Remote_DP_Checksum();
			Render_Remote_DP_Status();
			break;
		case RB_MODE_2:
			Render_Remote_DP_Checksum();
			Render_Remote_DP_Status();
			break;
		case RB_MODE_3:
			Render_Remote_DP_Counts();
			Render_Remote_DP_Status();
			break;
		case RB_MODE_4:
			Render_Remote_DP_Counts();
			Render_Remote_DP_Status();
			break;
		case RB_MODE_5:
			Render_Remote_DP_Counts();
			Render_Remote_DP_Status();
			break;
		case RB_MODE_6:
			Render_Remote_DP_Counts();
			Render_Remote_DP_Status();
			break;
	default:
		break;
		}
}

void Update_RB_Itoa_State(void)
{
	switch (RB_Itoa.State)
		{
		case RB_CONVERT_LDP_COUNT_TO_STR:
			 if (RB_Info.Local_DP_Count.Word != RB_Info.Prev_Local_DP_Count.Word)
				{
				Itoac( RB_Info.Local_DP_Count.Word, RB_Info.Local_DP_Count_Str);
				RB_Info.Prev_Local_DP_Count.Word = RB_Info.Local_DP_Count.Word;
				}
			RB_Itoa.State = RB_CONVERT_RDP_COUNT_TO_STR;
			break;
		case RB_CONVERT_RDP_COUNT_TO_STR:
			 if (RB_Info.Remote_DP_Count.Word != RB_Info.Prev_Remote_DP_Count.Word)
				{
				Itoac( RB_Info.Remote_DP_Count.Word, RB_Info.Remote_DP_Count_Str);
				RB_Info.Prev_Remote_DP_Count.Word = RB_Info.Remote_DP_Count.Word;
				}
			RB_Itoa.State = RB_CONVERT_DPA_COUNT_TO_STR;
			break;
		case RB_CONVERT_DPA_COUNT_TO_STR:
		 if (RB_Info.DP_A_Count.Word != RB_Info.Prev_DP_A_Count.Word)
			{
			Itoac( RB_Info.DP_A_Count.Word, RB_Info.DP_A_Count_Str);
			RB_Info.Prev_DP_A_Count.Word = RB_Info.DP_A_Count.Word;
			}
			RB_Itoa.State = RB_CONVERT_DPB_COUNT_TO_STR;
			break;
		case RB_CONVERT_DPB_COUNT_TO_STR:
		 if (RB_Info.DP_B_Count.Word != RB_Info.Prev_DP_B_Count.Word)
			{
			Itoac( RB_Info.DP_B_Count.Word, RB_Info.DP_B_Count_Str);
			RB_Info.Prev_DP_B_Count.Word = RB_Info.DP_B_Count.Word;
			}
			RB_Itoa.State = RB_CONVERT_DPC_COUNT_TO_STR;
			break;
		case RB_CONVERT_DPC_COUNT_TO_STR:
		 if (RB_Info.DP_C_Count.Word != RB_Info.Prev_DP_C_Count.Word)
			{
			Itoac( RB_Info.DP_C_Count.Word, RB_Info.DP_C_Count_Str);
			RB_Info.Prev_DP_C_Count.Word = RB_Info.DP_C_Count.Word;
			}
			RB_Itoa.State = RB_CONVERT_LDP_COUNT_TO_STR;
			break;
	default:
		RB_Itoa.State = RB_CONVERT_LDP_COUNT_TO_STR;
		break;
		}
}


void Itoac(UINT16 uiInput, BYTE uchOutput[])
{
	BYTE uchi=0;

	if (uiInput > LARGEST_CONVERTABLE_INTEGER)
	{
		/* Invalid Range */
		uchOutput[0] = '*';
		uchOutput[1] = '*';
		uchOutput[2] = '*';
		return;
	}
	else
	{
		uchOutput[0] = '0';
		uchOutput[1] = '0';
		uchOutput[2] = '0';
	}
	uchi = (BYTE) 2;

 do {
		uchOutput[uchi] = uiInput % 10 + '0';
		uchi = uchi - 1;								/* Store character in reverse order */
    } while ((uiInput /= 10) > 0);
}

void HexByte_to_String(BYTE uiInput, BYTE uchOutput[])
{
	BYTE uchi=0;

	uchi = (uiInput & 0x0F);
	uchOutput[1] = uchHexadecimalLookup[uchi];
	uchi = (uiInput >> 4);
	uchOutput[0] = uchHexadecimalLookup[uchi];
}

void Display_SSDAC_Not_Powered_Up(void)
{
	BYTE uchBuffer[] = "                    ";
	BYTE uchBuffer1[] = " Waiting for SSDAC  ";
	BYTE uchBuffer2[] = "   Communication    ";

	Display_on_LCD(0,0, uchIDInfo);
	Display_on_LCD(0,17, uchRBVer);
	Display_on_LCD(1,0, uchBuffer);
	Display_on_LCD(2,0, uchBuffer1);
	Display_on_LCD(3,0, uchBuffer2);
}

void Render_Local_DP_Checksum(void)
{
	BYTE uchBuf[21] = "                    ";

	HexByte_to_String(RB_Info.DAC_Checksum[0], uchBuf);
	HexByte_to_String(RB_Info.DAC_Checksum[1], &uchBuf[2]);
	HexByte_to_String(RB_Info.DAC_Checksum[2], &uchBuf[4]);
	HexByte_to_String(RB_Info.DAC_Checksum[3], &uchBuf[6]);
	uchBuf[9] = 'S';
	uchBuf[10] = '/';
	uchBuf[11] = 'W';
	uchBuf[13] = 'V';
	uchBuf[14] = 'e';
	uchBuf[15] = 'r';
	uchBuf[17] = '0';
	uchBuf[18] = '0';
	uchBuf[19] = '0'+RB_Info.DAC_SW_Version;
	Display_on_LCD(LDP_DATA_DISPLAY_LINE, 0, uchBuf);
	if (RB_Info.RB_Type == RESET_BOX_TYPE_3DP1S)
		{
		Display_on_LCD(DP_C_DATA_DISPLAY_LINE, 0, uchBuf);
		}
}

void Render_Remote_DP_Checksum(void)
{
	BYTE uchBuf[21] = "                    ";

	HexByte_to_String(RB_Info.DAC_Checksum[0], uchBuf);
	HexByte_to_String(RB_Info.DAC_Checksum[1], &uchBuf[2]);
	HexByte_to_String(RB_Info.DAC_Checksum[2], &uchBuf[4]);
	HexByte_to_String(RB_Info.DAC_Checksum[3], &uchBuf[6]);
	uchBuf[9] = 'S';
	uchBuf[10] = '/';
	uchBuf[11] = 'W';
	uchBuf[13] = 'V';
	uchBuf[14] = 'e';
	uchBuf[15] = 'r';
	uchBuf[17] = '0';
	uchBuf[18] = '0';
	uchBuf[19] = '0'+RB_Info.DAC_SW_Version;
	Display_on_LCD(RDP_DATA_DISPLAY_LINE, 0, uchBuf);
}

void Render_Local_DP_Counts(void)
{
	BYTE uchBuf[21] = "                    ";

	if (RB_Info.RB_Type != RESET_BOX_TYPE_3DP1S)
		{
		uchBuf[8] = RB_Info.Local_DP_Count_Str[0];
		uchBuf[9] = RB_Info.Local_DP_Count_Str[1];
		uchBuf[10] = RB_Info.Local_DP_Count_Str[2];
		Display_on_LCD(LDP_DATA_DISPLAY_LINE, 0, uchBuf);
		}
	else
		{
		uchBuf[8] = RB_Info.DP_A_Count_Str[0];
		uchBuf[9] = RB_Info.DP_A_Count_Str[1];
		uchBuf[10] = RB_Info.DP_A_Count_Str[2];
		Display_on_LCD(DP_A_DATA_DISPLAY_LINE, 0, uchBuf);
		uchBuf[8] = RB_Info.DP_B_Count_Str[0];
		uchBuf[9] = RB_Info.DP_B_Count_Str[1];
		uchBuf[10] = RB_Info.DP_B_Count_Str[2];
		Display_on_LCD(DP_B_DATA_DISPLAY_LINE, 0, uchBuf);
		uchBuf[8] = RB_Info.DP_C_Count_Str[0];
		uchBuf[9] = RB_Info.DP_C_Count_Str[1];
		uchBuf[10] = RB_Info.DP_C_Count_Str[2];
		Display_on_LCD(DP_C_DATA_DISPLAY_LINE, 0, uchBuf);
		}
}

void Render_Remote_DP_Counts(void)
{
	BYTE uchBuf[21] = "                    ";

	if (RB_Info.RB_Type != RESET_BOX_TYPE_3DP1S)
		{
		uchBuf[8] = RB_Info.Remote_DP_Count_Str[0];
		uchBuf[9] = RB_Info.Remote_DP_Count_Str[1];
		uchBuf[10] = RB_Info.Remote_DP_Count_Str[2];
		Display_on_LCD(RDP_DATA_DISPLAY_LINE, 0, uchBuf);
		}
}


void Render_Local_DP_Status(void)
{
	BYTE uchBuf[21];
	BYTE uchCnt;

	if (RB_Info.RB_Type != RESET_BOX_TYPE_3DP1S)
		{
		if (RB_Info.Local_DP_Status < MAX_TYPE_OF_MODES)
			{
			if (RB_Info.Local_DP_Count.Word > 0 && RB_Info.Local_DP_Status == RB_MODE_3)
				{
				for (uchCnt = 0; uchCnt < 20; uchCnt++)
					{
					uchBuf[uchCnt] = DAC_ModeLookup[0][uchCnt];
					}
				}
			else
				{
				for (uchCnt = 0; uchCnt < 20; uchCnt++)
					{
					uchBuf[uchCnt] = DAC_ModeLookup[RB_Info.Local_DP_Status][uchCnt];
					}
				}
            if (RB_Info.Local_DP_Error > 0 && RB_Info.Local_DP_Error <= TOTAL_NO_ERRORS)
				{
				for (uchCnt = 0; uchCnt < 20; uchCnt++)
					{
					uchBuf[uchCnt] =  Error_Table[RB_Info.Local_DP_Error][uchCnt];
					}
                }
			Display_on_LCD(LDP_STATUS_DISPLAY_LINE, 0, uchBuf);
			}
		else
			{
			/* ERROR */
			if (RB_Info.Local_DP_Error > 0 && RB_Info.Local_DP_Error <= TOTAL_NO_ERRORS)
				{
				for (uchCnt = 0; uchCnt < 20; uchCnt++)
					{
					uchBuf[uchCnt] =  Error_Table[RB_Info.Local_DP_Error][uchCnt];
					}
				Display_on_LCD(LDP_STATUS_DISPLAY_LINE, 0, uchBuf);
				}
			}
		}
	else
		{
		/* 3DP1S */
		if (RB_Info.Local_DP_Status < MAX_TYPE_OF_MODES)
			{
			for (uchCnt = 0; uchCnt < 20; uchCnt++)
				{
				uchBuf[uchCnt] = DAC_ModeLookup[RB_Info.Local_DP_Status][uchCnt];
				}
            if (RB_Info.Local_DP_Error > 0 && RB_Info.Local_DP_Error <= TOTAL_NO_ERRORS)
				{
				for (uchCnt = 0; uchCnt < 20; uchCnt++)
					{
					uchBuf[uchCnt] =  Error_Table[RB_Info.Local_DP_Error][uchCnt];
					}
                }
			Display_on_LCD(DP_ABC_STATUS_DISPLAY_LINE, 0, uchBuf);
			}
		else
			{
			/* ERROR */
			if (RB_Info.Local_DP_Error > 0 && RB_Info.Local_DP_Error <= TOTAL_NO_ERRORS)
				{
				for (uchCnt = 0; uchCnt < 20; uchCnt++)
					{
					uchBuf[uchCnt] =  Error_Table[RB_Info.Local_DP_Error][uchCnt];
					}
				Display_on_LCD(DP_ABC_STATUS_DISPLAY_LINE, 0, uchBuf);
				}
			}
		}
}

void Render_Remote_DP_Status(void)
{
	BYTE uchBuf[21];
	BYTE uchCnt;

	if (RB_Info.Remote_DP_Status < MAX_TYPE_OF_MODES)
		{
		if (RB_Info.Remote_DP_Count.Word > 0 && RB_Info.Remote_DP_Status == RB_MODE_3)
			{
			for (uchCnt = 0; uchCnt < 20; uchCnt++)
				{
				uchBuf[uchCnt] = DAC_ModeLookup[0][uchCnt];
				}
			}
		else
			{
			for (uchCnt = 0; uchCnt < 20; uchCnt++)
				{
				uchBuf[uchCnt] = DAC_ModeLookup[RB_Info.Remote_DP_Status][uchCnt];
				}
			}
		Display_on_LCD(RDP_STATUS_DISPLAY_LINE, 0, uchBuf);
		}
	else
		{
		if (RB_Info.RB_Type != RESET_BOX_TYPE_3DP1S)
			{
			for (uchCnt = 0; uchCnt < 20; uchCnt++)
				{
				uchBuf[uchCnt] =  Error_Table[0][uchCnt];
				}
			Display_on_LCD(RDP_STATUS_DISPLAY_LINE, 0, uchBuf);
			}
		}
}
