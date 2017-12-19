#ifndef _DRV_AUTO_CNT_H_
#define _DRV_AUTO_CNT_H_

typedef enum {
			AUTO_RESET_CNT_IDLE = 0,
			AUTO_RESET_CNT_SET_HIGH,
			AUTO_RESET_CNT_HOLD_HIGH,
			AUTO_RESET_CNT_SET_LOW
} auto_cnt_state_t;

#define AUTO_RESET_COUNTER_PORT				PORTDbits.RD8

/* Timeouts in multiples of 10-ms */
#define CNT_PLUSE_HIGH_TIMEOUT			(100)

typedef struct {
			auto_cnt_state_t State;
			UINT16	Timeout_10ms;
} Auto_Reset_Counter_t;

void Initialise_Auto_Cnt_Driver(void);
void Update_Auto_Cnt_State(void);
void Initialise_Auto_Reset_Seq(void);
void Start_Auto_Reset_Seq(void);
void Update_Auto_Reset_Seq_State(void);
void Decrement_Auto_Reset_Seq_10msTmr(void);
void Update_Reset_Seq_State(void);
void Decrement_Auto_Cnt_10msTmr(void);
void Increment_Auto_Reset_Counter(void);

#endif

