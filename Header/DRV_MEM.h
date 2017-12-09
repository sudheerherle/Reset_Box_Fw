/*********************************************************************************************************************
	Project		: 	Single Section Digital Axle Counter
	Version		: 	2.0 
	Revision	:	1	
	Filename	: 	drv_mem.h
	Target MCU	: 	PIC24FJ256GB210   
    Compiler	: 	XC16 Compiler V1.21  
	Author		:	Sudheer Herle
	Date		:	
	Company Name: 	Insys Digital Systems
	Modification History:
					Rev No			Date		Description
					 --				 --				--    
*********************************************************************************************************************/
#ifndef _DRV_MEM_H_
#define _DRV_MEM_H_

#define MEM_WRITE_PROTECT		LATAbits.LATA4		/*port that Controls Memory Write Protect bit */

/* Address to i2c EEprom */
typedef struct {
		BYTE	Physical_Read_Addr;		/* Address to Read  a Byte from i2c EEPROM */
		BYTE	Physical_Write_Addr;	/* Address to Write a Byte into i2c EEPROM */
} ser_eeprom_addr_mapping_t;

typedef enum{
    START = 0,
            IN_PROGESS,
            DONE
}EEPROM_Erase_State;

extern BOOL Erase_Serial_EEProm_Block(UINT16);
extern BOOL Write_Event_on_Serial_EEProm(UINT16, event_record_t *);
extern BOOL Read_Event_from_Serial_EEProm(UINT16, event_record_t *);
extern BOOL Is_Serial_EEPROM_Write_Complete(void);

#endif

