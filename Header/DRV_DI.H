#ifndef _DRV_DI_H_
#define _DRV_DI_H_

#define VR1_NO_CONTACT_PORT				PORTBbits.RB13
#define PR1_NO_CONTACT_PORT				PORTBbits.RB14
#define VR2_NO_CONTACT_PORT				PORTAbits.RA9
#define PR2_NO_CONTACT_PORT				PORTBbits.RB11
#define RESET_PB_PORT					PORTAbits.RA1

/* Process Time-outs in multiples of 10ms */
#define PUSH_BUTTON_DEBOUNCE_TIMEOUT	(40)
#define VITAL_RELAY_DEBOUNCE_TIMEOUT	(500)
#define PREP_RELAY_DEBOUNCE_TIMEOUT		(500)

typedef enum {
			PB_SCH_NOT_STARTED = 0,
			PB_NOT_PRESSED,
			PB_DEBOUNCE_WAIT,
			PB_PRESSED_EVENT_DETECTED,
			PB_WAIT_FOR_BUTTON_RELEASE
} pb_state_t;

typedef enum {
			RELAY_SCH_NOT_STARTED = 0,
			RELAY_DROPPED,
			RELAY_PICKUP_WAIT,
			RELAY_PICKUP_DETECTED,
			RELAY_WAIT_FOR_DROP
} relay_state_t;

typedef struct {
			pb_state_t State;
			BYTE Timeout_10ms;
} reset_pb_info_t;

typedef struct {
			relay_state_t State;
			BYTE Timeout_10ms;
} relay_info_t;

extern void Initialise_DI_Driver(void);
extern void Start_DI_Scan(void);
extern void Update_DI_State(void);
extern void Decrement_DI_10msTmr(void);

#endif
