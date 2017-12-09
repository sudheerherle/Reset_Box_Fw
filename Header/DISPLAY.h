#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#define RB_DISPLAY_REFRESH_RATE		(3)

#define LDP_DATA_DISPLAY_LINE		(0)
#define RDP_DATA_DISPLAY_LINE		(1)
#define LDP_STATUS_DISPLAY_LINE		(2)
#define RDP_STATUS_DISPLAY_LINE		(3)
#define DP_A_DATA_DISPLAY_LINE		(0)
#define DP_B_DATA_DISPLAY_LINE		(1)
#define DP_C_DATA_DISPLAY_LINE		(2)
#define DP_ABC_STATUS_DISPLAY_LINE	(3)

typedef enum {
			RB_CONVERT_LDP_COUNT_TO_STR,
			RB_CONVERT_RDP_COUNT_TO_STR,
			RB_CONVERT_DPA_COUNT_TO_STR,
			RB_CONVERT_DPB_COUNT_TO_STR,
			RB_CONVERT_DPC_COUNT_TO_STR
} rb_itoa_state_t;

typedef struct {
		rb_itoa_state_t State;
} rb_itoa_t;

extern void Initialise_RB_Mode(void);
extern void Update_RB_Mode_State(void);
extern void Decrement_RB_Mode_10msTmr(void);

#endif
