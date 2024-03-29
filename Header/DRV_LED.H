#ifndef _DRV_LED_H_
#define _DRV_LED_H_

typedef enum {
			LED_STEADY_OFF = 0,
			LED_STEADY_ON,
			LED_FLASHING_OFF,
			LED_FLASHING_OFF_WAIT,
			LED_FLASHING_ON,
			LED_FLASHING_ON_WAIT
} led_state_t;

#define PR1_LED_PORT	LATBbits.LATB15
#define PR2_LED_PORT	LATBbits.LATB12
#define AUTO_RESET_LED_PORT	LATDbits.LATD2
#define GS_LED_PORT LATFbits.LATF4
#define BI_COLOR_RED_LED_PORT LATEbits.LATE9
#define BI_COLOR_GREEN_LED_PORT LATEbits.LATE8

/* Timeouts in multiples of 10-ms */
#define LED_FLASHING_ON_TIMEOUT			(50)
#define LED_FLASHING_OFF_TIMEOUT		(50)

typedef struct {
			led_state_t State;
			UINT16	Timeout_10ms;
} Preparatory_Reset_LED_t;

extern void Initialise_LED_Driver(void);
extern void Update_Preparatory_LED_State(void);
extern void Decrement_LED_10msTmr(void);
extern void Set_Preparatory_LED_Off(void);
extern void Set_Preparatory_LED_On(void);
extern void Set_Preparatory_LED_Flashing(void);

#endif

