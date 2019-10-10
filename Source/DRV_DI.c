#include <xc.h>

#include "COMMON.H"
#include "DRV_DI.H"
#include "DRV_LED.H"

//rb_status_t RB_Status;					/* from resetbox.c */
extern  rb_status_t RB_Status;
reset_pb_info_t Reset_PB;
relay_info_t Vital_Relay;
relay_info_t Vital_Relay2;
relay_info_t Prep_Relay;
relay_info_t Prep_Relay2;


void Update_PB_State(void);
void Update_Vital_Relay_State(void);
void Update_Prep_Relay_State(void);
void Update_Vital_Relay2_State(void);
void Update_Prep_Relay2_State(void);

void Initialise_DI_Driver(void)
{
	Reset_PB.State = PB_SCH_NOT_STARTED;
	Reset_PB.Timeout_10ms = 0;
	Vital_Relay.State = RELAY_SCH_NOT_STARTED;
	Vital_Relay.Timeout_10ms = 0;
	Prep_Relay.State = RELAY_SCH_NOT_STARTED;
	Prep_Relay.Timeout_10ms = 0;
}

void Start_DI_Scan(void)
{
	Reset_PB.State = PB_NOT_PRESSED;
    RB_Status.Flags.Reset_PB_Status = 1;
	Reset_PB.Timeout_10ms = 0;
	RB_Status.Flags.VR1_Contact_Status = VR1_NO_CONTACT_PORT;
    RB_Status.Flags.VR2_Contact_Status = VR2_NO_CONTACT_PORT;
	Vital_Relay.State = RELAY_DROPPED;
	Vital_Relay.Timeout_10ms = 0;
    Vital_Relay2.State = RELAY_DROPPED;
	Vital_Relay2.Timeout_10ms = 0;
	RB_Status.Flags.PR1_Contact_Status = PR1_NO_CONTACT_PORT;
    RB_Status.Flags.PR2_Contact_Status = PR2_NO_CONTACT_PORT;
	Prep_Relay.State = RELAY_DROPPED;
	Prep_Relay.Timeout_10ms = 0;
    Prep_Relay2.State = RELAY_DROPPED;
	Prep_Relay2.Timeout_10ms = 0;
}


void Update_DI_State(void)
{
	Update_PB_State();
	Update_Vital_Relay_State();
	Update_Prep_Relay_State();
    Update_Vital_Relay2_State();
	Update_Prep_Relay2_State();
}

void Update_PB_State(void)
{
	switch (Reset_PB.State)
		{
		case PB_SCH_NOT_STARTED:
			break;
		case PB_NOT_PRESSED:
			if (RESET_PB_PORT == SET_LOW)
				{
				Reset_PB.State = PB_DEBOUNCE_WAIT;
				Reset_PB.Timeout_10ms = PUSH_BUTTON_DEBOUNCE_TIMEOUT;
				}
			break;
		case PB_DEBOUNCE_WAIT:
			if (RESET_PB_PORT == SET_HIGH)
				{
				Reset_PB.State = PB_NOT_PRESSED;
				Reset_PB.Timeout_10ms = 0;
				break;
				}
			if (Reset_PB.Timeout_10ms == TIMEOUT_EVENT)
				{
				Reset_PB.State = PB_PRESSED_EVENT_DETECTED;
				}
			break;
		case PB_PRESSED_EVENT_DETECTED:
			RB_Status.Flags.Reset_PB_Status = SET_LOW;
			Reset_PB.State = PB_WAIT_FOR_BUTTON_RELEASE;
			break;
		case PB_WAIT_FOR_BUTTON_RELEASE:
			if (RESET_PB_PORT == SET_HIGH)
				{
				RB_Status.Flags.Reset_PB_Status = SET_HIGH;
				Reset_PB.State = PB_NOT_PRESSED;
				Reset_PB.Timeout_10ms = 0;
				}
			break;
		}
}

void Update_Vital_Relay_State(void)
{
	switch (Vital_Relay.State)
		{
		case RELAY_SCH_NOT_STARTED:
			break;
		case RELAY_DROPPED:
			if (VR1_NO_CONTACT_PORT == SET_LOW)
				{
				Vital_Relay.State = RELAY_PICKUP_WAIT;
				Vital_Relay.Timeout_10ms = VITAL_RELAY_DEBOUNCE_TIMEOUT;
				}
			break;
		case RELAY_PICKUP_WAIT:
			if (VR1_NO_CONTACT_PORT == SET_HIGH)
				{
				Vital_Relay.State = RELAY_DROPPED;
				Vital_Relay.Timeout_10ms = 0;
				break;
				}
			if (Vital_Relay.Timeout_10ms == TIMEOUT_EVENT)
				{
				Vital_Relay.State = RELAY_PICKUP_DETECTED;
				}
			break;
		case RELAY_PICKUP_DETECTED:
			RB_Status.Flags.VR1_Contact_Status = SET_LOW;
			Vital_Relay.State = RELAY_WAIT_FOR_DROP;
			break;
		case RELAY_WAIT_FOR_DROP:
			if (VR1_NO_CONTACT_PORT == SET_HIGH)
				{
				RB_Status.Flags.VR1_Contact_Status = SET_HIGH;
				Vital_Relay.State = RELAY_DROPPED;
				Vital_Relay.Timeout_10ms = 0;
				}
			break;
		}
}

void Update_Vital_Relay2_State(void)
{
	switch (Vital_Relay2.State)
		{
		case RELAY_SCH_NOT_STARTED:
			break;
		case RELAY_DROPPED:
			if (VR2_NO_CONTACT_PORT == SET_LOW)
				{
				Vital_Relay2.State = RELAY_PICKUP_WAIT;
				Vital_Relay2.Timeout_10ms = VITAL_RELAY_DEBOUNCE_TIMEOUT;
				}
			break;
		case RELAY_PICKUP_WAIT:
			if (VR2_NO_CONTACT_PORT == SET_HIGH)
				{
				Vital_Relay2.State = RELAY_DROPPED;
				Vital_Relay2.Timeout_10ms = 0;
				break;
				}
			if (Vital_Relay2.Timeout_10ms == TIMEOUT_EVENT)
				{
				Vital_Relay2.State = RELAY_PICKUP_DETECTED;
				}
			break;
		case RELAY_PICKUP_DETECTED:
			RB_Status.Flags.VR2_Contact_Status = SET_LOW;
			Vital_Relay2.State = RELAY_WAIT_FOR_DROP;
			break;
		case RELAY_WAIT_FOR_DROP:
			if (VR2_NO_CONTACT_PORT == SET_HIGH)
				{
				RB_Status.Flags.VR2_Contact_Status = SET_HIGH;
				Vital_Relay2.State = RELAY_DROPPED;
				Vital_Relay2.Timeout_10ms = 0;
				}
			break;
		}
}

void Update_Prep_Relay_State(void)
{
    	switch (Prep_Relay.State)
		{
		case RELAY_SCH_NOT_STARTED:
			break;
		case RELAY_DROPPED:
			if (PR1_NO_CONTACT_PORT == SET_LOW)
				{
				Prep_Relay.State = RELAY_PICKUP_WAIT;
				Prep_Relay.Timeout_10ms = VITAL_RELAY_DEBOUNCE_TIMEOUT;
				}
			break;
		case RELAY_PICKUP_WAIT:
			if (PR1_NO_CONTACT_PORT == SET_HIGH)
				{
				Prep_Relay.State = RELAY_DROPPED;
				Prep_Relay.Timeout_10ms = 0;
				break;
				}
			if (Prep_Relay.Timeout_10ms == TIMEOUT_EVENT)
				{
				Prep_Relay.State = RELAY_PICKUP_DETECTED;
				}
			break;
		case RELAY_PICKUP_DETECTED:
			RB_Status.Flags.PR1_Contact_Status = SET_LOW;
			Prep_Relay.State = RELAY_WAIT_FOR_DROP;
			break;
		case RELAY_WAIT_FOR_DROP:
			if (PR1_NO_CONTACT_PORT == SET_HIGH)
				{
				RB_Status.Flags.PR1_Contact_Status = SET_HIGH;
				Prep_Relay.State = RELAY_DROPPED;
				Prep_Relay.Timeout_10ms = 0;
				}
			break;
		}
}

void Update_Prep_Relay2_State(void)
{
    	switch (Prep_Relay2.State)
		{
		case RELAY_SCH_NOT_STARTED:
			break;
		case RELAY_DROPPED:
			if (PR2_NO_CONTACT_PORT == SET_LOW)
				{
				Prep_Relay2.State = RELAY_PICKUP_WAIT;
				Prep_Relay2.Timeout_10ms = VITAL_RELAY_DEBOUNCE_TIMEOUT;
				}
			break;
		case RELAY_PICKUP_WAIT:
			if (PR2_NO_CONTACT_PORT == SET_HIGH)
				{
				Prep_Relay2.State = RELAY_DROPPED;
				Prep_Relay2.Timeout_10ms = 0;
				break;
				}
			if (Prep_Relay2.Timeout_10ms == TIMEOUT_EVENT)
				{
				Prep_Relay2.State = RELAY_PICKUP_DETECTED;
				}
			break;
		case RELAY_PICKUP_DETECTED:
			RB_Status.Flags.PR2_Contact_Status = SET_LOW;
			Prep_Relay2.State = RELAY_WAIT_FOR_DROP;
			break;
		case RELAY_WAIT_FOR_DROP:
			if (PR2_NO_CONTACT_PORT == SET_HIGH)
				{
				RB_Status.Flags.PR2_Contact_Status = SET_HIGH;
				Prep_Relay2.State = RELAY_DROPPED;
				Prep_Relay2.Timeout_10ms = 0;
				}
			break;
		}
}

void Decrement_DI_10msTmr(void)
{
	if (Reset_PB.Timeout_10ms > 0)
		{
		Reset_PB.Timeout_10ms = Reset_PB.Timeout_10ms - 1;
		}
	if (Vital_Relay.Timeout_10ms > 0)
		{
		Vital_Relay.Timeout_10ms = Vital_Relay.Timeout_10ms - 1;
		}
	if (Prep_Relay.Timeout_10ms > 0)
		{
		Prep_Relay.Timeout_10ms = Prep_Relay.Timeout_10ms - 1;
		}
    if (Vital_Relay2.Timeout_10ms > 0)
		{
		Vital_Relay2.Timeout_10ms = Vital_Relay.Timeout_10ms - 1;
		}
	if (Prep_Relay2.Timeout_10ms > 0)
		{
		Prep_Relay2.Timeout_10ms = Prep_Relay2.Timeout_10ms - 1;
		}
}

