/*********************************************************************************************************************
	Project		: 	Single Section Digital Axle Counter
	Version		: 	2.0 
	Revision	:	1	
	Filename	: 	drv_i2c.h
	Target MCU	: 	PIC24FJ256GB210   
    Compiler	: 	XC16 Compiler V1.21  
	Author		:	EM003
	Date		:	
	Company Name: 	Insys Digital Systems
	Modification History:
					Rev No			Date		Description
					 --				 --				--    
********************************************************************************************************************/
#ifndef _DRV_I2C_H_
#define _DRV_I2C_H_

#define I2C_CONFIG      0b1000000000000000
#define I2C_HIGH_SPEED_MODE 157

#define I2C_TIMEOUT			100			/* 400KHz - 15 (approx 20uS), 100KHz - 70 (approx 100uS)*/

BOOL Has_SSPIF_gone_high(void);
void Initialise_I2C_Driver(void);
BOOL Generate_I2C_Start(void);
BOOL Generate_I2C_ReStart(void);
BOOL Generate_I2C_Stop(void);
void Give_I2C_ACK(void);
void Give_I2C_NAK(void);
BOOL Put_I2C_Byte(BYTE uchData);
BYTE Get_I2C_Byte(BOOL uchReadContinue);
BOOL Has_SSPIF_gone_high(void);
BOOL Read_I2C_ACK_Status(void);

#endif

