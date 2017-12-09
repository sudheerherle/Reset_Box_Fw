#ifndef _DRV_CNT_H_
#define _DRV_CNT_H_

typedef enum {
			CNT_IDLE = 0,
			CNT_SET_HIGH,
			CNT_HOLD_HIGH,
			CNT_SET_LOW
} cnt_state_t;

#define RESET_COUNTER_PORT				PORTGbits.RG7

/* Timeouts in multiples of 10-ms */
#define CNT_PLUSE_HIGH_TIMEOUT			(100)

typedef struct {
			cnt_state_t State;
			UINT16	Timeout_10ms;
} Reset_Counter_t;

extern void Initialise_Cnt_Driver(void);
extern void Update_Cnt_State(void);
extern void Decrement_Cnt_10msTmr(void);
extern void Increment_Reset_Counter(void);

#endif

