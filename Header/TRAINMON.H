#ifndef _TRACKMON_H_
#define _TRACKMON_H_

/* Sm Scheduler -States */
typedef enum {
			TM_NOT_STARTED = 0,
			TM_WAITING_FOR_TRAIN, 
			TM_TRAIN_ENTERED_LOADING_AREA,
			TM_TRAIN_LEAVING_LOADING_AREA,
			TM_COMPUTE_DURATION,
			TM_SAVE_AXLE_COUNT,
			TM_SAVE_SERIAL_NUMBER,
			TM_SAVE_ENTRY_TIME,
			TM_SAVE_EXIT_TIME,
			TM_POST_DATA_TO_PRINT_QUEUE,
			TM_OUT_OF_SYNC					/* Default case */
} tm_state_t;

typedef struct {
		tm_state_t	State;
		UINT16		Serial_Number;
		UINT16		Fwd_Axle_Count;
		UINT16		Rev_Axle_Count;
		time_t		Entry_Time;
		time_t		Exit_Time;
		time_t		Load_Time;
		time_t		Occupied_Duration;
		time_t		Loading_Duration;
} train_info_t;

extern void Initialise_TrainMon(void);
extern void Start_TrainMon(void);
extern void Update_TrainMon_State(void);

#endif
