#ifndef _DRV_CNT_H_
#define _DRV_CNT_H_

typedef enum {
			CNT_IDLE = 0,
			CNT_SET_HIGH,
			CNT_HOLD_HIGH,
			CNT_SET_LOW
} auto_cnt_state_t;

#define AUTO_RESET_COUNTER_PORT				PORTDbits.RD8

/* Timeouts in multiples of 10-ms */
#define CNT_PLUSE_HIGH_TIMEOUT			(100)

typedef struct {
			auto_cnt_state_t State;
			UINT16	Timeout_10ms;
} Auto_Reset_Counter_t;

extern void Initialise_Auto_Cnt_Driver(void);
extern void Update_Auto_Cnt_State(void);
extern void Decrement_Auto_Cnt_10msTmr(void);
extern void Increment_Auto_Reset_Counter(void);

#endif

