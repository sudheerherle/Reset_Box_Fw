/*********************************************************************************************************************
	Project		: 	Single Section Digital Axle Counter
	Version		: 	2.0 
	Revision	:	1	
	Filename	: 	drv_a2d.h
	Target MCU	: 	PIC24FJ256GB210   
    Compiler	: 	XC16 Compiler V1.21  
	Author		:	Sudheer Herle
	Date		:	25/12/2005
	Company Name: 	Insys Digital Systems
	Modification History:
					Rev No			Date		Description
					 --				 --				--    
*********************************************************************************************************************/
#ifndef _DRV_A2D_H_
#define _DRV_A2D_H_

/* A/D configuration */
#define ADCON1_CONFIG		0x00E4; /*   auto conversion,sample after conversion ends  */
#define ADCON2_CONFIG		0x207D	/* Vref+ external, Vref- AVSS, interrupt on 32nd completion, 1  word 32 buffers */
#define ADCON3_CONFIG		0x1F00		/* System clock, TAD = 31, Tcy = 0 */

#define A2D_CS_DEFAULT          0x0004     /*Negative input is Vr- for Channel A as well as channel B, AN0*/
#define A2D_BG_DEFAULT          0b0000000000000000      /*Band gap is kept 0*/
#define A2D_SC_DEFAULT          0b0000000000000000      /*Default scan select is kept 0*/

#define CHANNEL4_SELECT		0b00010001		/* Select AN4 and Switch on A/D converter */

#define NO_OF_READINGS_TO_BE_AVGD	(10)		
/*
 *  Vdd o-----\/\/\/\/\----+----\/\/\/\-----| Ground
 *              10K        |      6.8K
 *                         |
 *                         |
 *                         o
 *                         AN4
 *
 *  Vdd will be scaled down to (6.8)/(10+6.8) = 0.4047691
 */
//#define VDD_HIGH_SET_VALUE			(854)	/* VDD >  5.2V, AN4 = 2.108V */
//#define VDD_HIGH_CLEAR_VALUE		(839)	/* VDD <= 5.1V, AN4 = 2.071V */
//#define VDD_LOW_SET_VALUE			(772)	/* VDD <  4.7V, AN4 = 1.905V */
//#define VDD_LOW_CLEAR_VALUE			(805)	/* VDD >= 4.9V, AN4 = 1.987V */
//#define VDD_DEFAULT_VALUE			(821)	/* VDD >= 5.0V, AN4 = 2.027V */

#define VDD_HIGH_SET_VALUE			(619)	/* VDD >= 3.7V, AN4 = 1.497V */
#define VDD_HIGH_CLEAR_VALUE                    (588)	/* VDD <= 3.5V, AN4 = 1.416V */
#define VDD_LOW_SET_VALUE			(421)	/* VDD <= 2.5V, AN4 = 1.011V */
#define VDD_LOW_CLEAR_VALUE			(459)	/* VDD >= 3.0V, AN4 = 1.104V */
#define VDD_DEFAULT_VALUE			(536)	/* VDD >= 3.2V, AN4 = 1.295V */


#define A2D_MAX_ITERATIONS			(14)	/* Number of iteration for averaging (16*14 = 224 samples) */

typedef enum {
			VDD_MON_NOT_STARTED = 0,
			VDD_OK,
			VDD_HIGH,
			VDD_LOW,
} vdd_mon_state_t;

typedef enum {
			A2D_NOT_STARTED = 0,
			A2D_IDLE,
			A2D_WAIT_FOR_CONVERSION,
			A2D_SAVE_RESULT
} a2d_state_t;

typedef struct {
			a2d_state_t	State;
			BYTE		Timeout_10ms;
			UINT16		Iteration;
			wordtype_t	Result;
			UINT32		Summation;
} a2d_data_t;

typedef struct {
			vdd_mon_state_t State;
			UINT16	Vdd_Value;
} vdd_mon_info_t;

extern void Initialise_A2D_Driver(void);
extern void Start_A2D_Driver(void);
extern void Update_Vdd_Mon_State(void);
extern void Decrement_A2D_10msTmr(void);

#endif

