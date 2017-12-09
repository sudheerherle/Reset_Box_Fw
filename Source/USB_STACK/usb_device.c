#include "usb.h"
#include "HardwareProfile.h"

#include "usb_device_local.h"
#include "comm_host.h"

#define _DTS_CHECKING_ENABLED _DTSEN


#define NO_PTR

USB_VOLATILE USB_DEVICE_STATE USBDeviceState;
USB_VOLATILE BYTE USBActiveConfiguration;
USB_VOLATILE BYTE USBAlternateInterface[USB_MAX_NUM_INT];
#ifdef NO_PTR
volatile BYTE BDT_INDEX_EO_C_VALUE;
volatile BYTE BDT_INDEX_EO_N_VALUE;

//volatile BDT_ENTRY BDTEntryEP0OutCurrent;
//volatile BDT_ENTRY BDTEntryEP0OutNext;
#else
volatile BDT_ENTRY *pBDTEntryEP0OutCurrent;
volatile BDT_ENTRY *pBDTEntryEP0OutNext;
#endif

volatile BDT_ENTRY *pBDTEntryOut[USB_MAX_EP_NUMBER+1];
volatile BDT_ENTRY *pBDTEntryIn[USB_MAX_EP_NUMBER+1];
USB_VOLATILE BYTE shortPacketStatus;
USB_VOLATILE BYTE controlTransferState;
USB_VOLATILE IN_PIPE inPipes[1];
USB_VOLATILE OUT_PIPE outPipes[1];
USB_VOLATILE BOOL RemoteWakeup;
USB_VOLATILE BOOL USBBusIsSuspended;
USB_VOLATILE USTAT_FIELDS USTATcopy;
USB_VOLATILE BYTE endpoint_number;
USB_VOLATILE BOOL BothEP0OutUOWNsSet;
USB_VOLATILE EP_STATUS ep_data_in[USB_MAX_EP_NUMBER+1];
USB_VOLATILE EP_STATUS ep_data_out[USB_MAX_EP_NUMBER+1];
USB_VOLATILE BYTE USBStatusStageTimeoutCounter;
volatile BOOL USBDeferStatusStagePacket;
volatile BOOL USBStatusStageEnabledFlag1;
volatile BOOL USBStatusStageEnabledFlag2;
volatile BOOL USBDeferINDataStagePackets;
volatile BOOL USBDeferOUTDataStagePackets;


    #define BDT_NUM_ENTRIES      ((USB_MAX_EP_NUMBER + 1) * 4)

volatile BDT_ENTRY BDT[BDT_NUM_ENTRIES] BDT_BASE_ADDR_TAG;

volatile CTRL_TRF_SETUP SetupPkt;
volatile BYTE CtrlTrfData[USB_EP0_BUFF_SIZE];


extern ROM USB_DEVICE_DESCRIPTOR device_dsc;

extern ROM BYTE configDescriptor1[];




BOOL USER_USB_CALLBACK_EVENT_HANDLER(int event);

//Internal Functions
static void USBCtrlEPService(void);
static void USBCtrlTrfSetupHandler(void);
static void USBCtrlTrfInHandler(void);
static void USBCheckStdRequest(void);
static void USBStdGetDscHandler(void);
static void USBCtrlEPServiceComplete(void);
static void USBCtrlTrfTxService(void);
static void USBCtrlTrfRxService(void);
static void USBStdSetCfgHandler(void);
static void USBStdGetStatusHandler(void);
static void USBStdFeatureReqHandler(void);
static void USBCtrlTrfOutHandler(void);
static void USBConfigureEndpoint(BYTE EPNum, BYTE direction);
static void USBWakeFromSuspend(void);
static void USBSuspend(void);
static void USBStallHandler(void);


#define USBAdvancePingPongBuffer(buffer) ((BYTE_VAL*)buffer)->Val ^= USB_NEXT_PING_PONG;
#define USBHALPingPongSetToOdd(buffer)   {((BYTE_VAL*)buffer)->Val |= USB_NEXT_PING_PONG;}
#define USBHALPingPongSetToEven(buffer)  {((BYTE_VAL*)buffer)->Val &= ~USB_NEXT_PING_PONG;}


extern host_sch_info_t      		Host_Sch_Info;			/* Structure holds host comm scheduler information */
    BYTE i_count=0;
void USBDeviceInit(void)
{
Host_Sch_Info.USB_Timeout_ms = USB_DELAY_TIME;
Host_Sch_Info.USB_Connection_status = 0;
//Host_Sch_Info.Tx_USB_Logged_events = 0;
    USBDisableInterrupts();

    // Clear all USB error flags
    USBClearInterruptRegister(U1EIR);

    // Clears all USB interrupts
    USBClearInterruptRegister(U1IR);

    //Clear all of the endpoint control registers
    U1EP0 = 0;

    DisableNonZeroEndpoints(USB_MAX_EP_NUMBER);

    SetConfigurationOptions();

    //power up the module (if not already powered)
    USBPowerModule();

    //set the address of the BDT (if applicable)
    USBSetBDTAddress(BDT);

    //Clear all of the BDT entries
    for(i_count=0;i_count<(sizeof(BDT)/sizeof(BDT_ENTRY));i_count++)
    {
        BDT[i_count].Val = 0x00;
    }

    // Assert reset request to all of the Ping Pong buffer pointers
    USBPingPongBufferReset = 1;

    // Reset to default address
    U1ADDR = 0x00;

    // Make sure packet processing is enabled
    USBPacketDisable = 0;

    //Stop trying to reset ping pong buffer pointers
    USBPingPongBufferReset = 0;

    // Flush any pending transactions
    while(USBTransactionCompleteIF == 1)
    {
        USBClearInterruptFlag(USBTransactionCompleteIFReg,USBTransactionCompleteIFBitNum);
        //Initialize USB stack software state variables
        inPipes[0].info.Val = 0;
        outPipes[0].info.Val = 0;
        outPipes[0].wCount.Val = 0;
    }

    //Set flags to TRUE, so the USBCtrlEPAllowStatusStage() function knows not to
    //try and arm a status stage, even before the first control transfer starts.
    USBStatusStageEnabledFlag1 = TRUE;
    USBStatusStageEnabledFlag2 = TRUE;
    //Initialize other flags
    USBDeferINDataStagePackets = FALSE;
    USBDeferOUTDataStagePackets = FALSE;
    USBBusIsSuspended = FALSE;

	//Initialize all pBDTEntryIn[] and pBDTEntryOut[]
	//pointers to NULL, so they don't get used inadvertently.
	for(i_count = 0; i_count < (BYTE)(USB_MAX_EP_NUMBER+1u); i_count++)
	{
		pBDTEntryIn[i_count] = 0u;
		pBDTEntryOut[i_count] = 0u;
		ep_data_in[i_count].Val = 0u;
        ep_data_out[i_count].Val = 0u;
	}

    //Get ready for the first packet
    pBDTEntryIn[0] = (volatile BDT_ENTRY*)&BDT[EP0_IN_EVEN];
    // Initialize EP0 as a Ctrl EP
    U1EP0 = EP_CTRL|USB_HANDSHAKE_ENABLED;
	//Prepare for the first SETUP on EP0 OUT
    BDT[EP0_OUT_EVEN].ADR = ConvertToPhysicalAddress(&SetupPkt);
    BDT[EP0_OUT_EVEN].CNT = USB_EP0_BUFF_SIZE;
    BDT[EP0_OUT_EVEN].STAT.Val = _USIE|_DAT0|_BSTALL;

    // Clear active configuration
    USBActiveConfiguration = 0;

    //Indicate that we are now in the detached state
    USBDeviceState = DETACHED_STATE;
}
volatile int count_usb_tracsaction = 0;
BYTE i_count2;
extern volatile long int Debug_flag;
void USBDeviceTasks(void)
{
    if (USB_BUS_SENSE != 1)
    {
         // Disable module & detach from bus
         U1CON = 0;

         // Mask all USB interrupts
         U1IE = 0;

         //Move to the detached state
         USBDeviceState = DETACHED_STATE;
         USBClearUSBInterrupt();
         return;
    }
    if(Debug_flag==0 &&  USBDeviceState == CONFIGURED_STATE)
    {
        Debug_flag++;
        Nop();
    }

    if(USBDeviceState == DETACHED_STATE)
    {
	    //Initialize register to known value
        U1CON = 0;

        // Mask all USB interrupts
        U1IE = 0;

        SetConfigurationOptions();

        // Enable module & attach to bus
        while(!U1CONbits.USBEN){U1CONbits.USBEN = 1;}

        //moved to the attached state
        USBDeviceState = ATTACHED_STATE;
    }
    if(USBDeviceState == ATTACHED_STATE)
    {

        if(!USBSE0Event)
        {
            USBClearInterruptRegister(U1IR);// Clear all USB interrupts
            U1IE=0;                        // Mask all USB interrupts
            USBResetIE = 1;             // Unmask RESET interrupt
            USBIdleIE = 1;             // Unmask IDLE interrupt
            USBDeviceState = POWERED_STATE;
        }
    }
    if(USBActivityIF && USBActivityIE)
    {
        USBClearInterruptFlag(USBActivityIFReg,USBActivityIFBitNum);
        USBWakeFromSuspend();
    }
    if(USBSuspendControl==1)
    {
        USBClearUSBInterrupt();
        return;
    }
    if(USBResetIF && USBResetIE)
    {
        USBDeviceInit();

        USBUnmaskInterrupts();

        USBDeviceState = DEFAULT_STATE;

        USBClearInterruptFlag(USBResetIFReg,USBResetIFBitNum);
    }
    if(USBIdleIF && USBIdleIE)
    {
        USBSuspend();
        USBClearInterruptFlag(USBIdleIFReg,USBIdleIFBitNum);
    }

    if(USBSOFIF)
    {
        if(USBSOFIE)
        {
            USB_SOF_HANDLER(EVENT_SOF);
        }
        USBClearInterruptFlag(USBSOFIFReg,USBSOFIFBitNum);

            //Decrement our status stage counter.
            if(USBStatusStageTimeoutCounter != 0u)
            {
                USBStatusStageTimeoutCounter--;
            }

            if(USBStatusStageTimeoutCounter == 0)
            {
                USBCtrlEPAllowStatusStage();    //Does nothing if the status stage was already armed.
            }

    }

    if(USBStallIF && USBStallIE)
    {
        USBStallHandler();
    }

    if(USBErrorIF && USBErrorIE)
    {
        USB_ERROR_HANDLER(EVENT_BUS_ERROR);
        USBClearInterruptRegister(U1EIR);               // This clears UERRIF
            USBClearInterruptFlag( USBErrorIFReg, USBErrorIFBitNum );
    }

    if(USBDeviceState < DEFAULT_STATE)
    {
	    USBClearUSBInterrupt();
	    return;
    }

    if(USBTransactionCompleteIE)
    {
	    for(i_count2 = 0; i_count2 < 4u; i_count2++)	//Drain or deplete the USAT FIFO entries.  If the USB FIFO ever gets full, USB bandwidth
		{						//utilization can be compromised, and the device won't be able to receive SETUP packets.
		    if(USBTransactionCompleteIF)
		    {
                        count_usb_tracsaction++;
    		    //Save and extract USTAT register info.  Will use this info later.
                USTATcopy.Val = U1STAT;
                endpoint_number = USBHALGetLastEndpoint(USTATcopy);

                USBClearInterruptFlag(USBTransactionCompleteIFReg,USBTransactionCompleteIFBitNum);
                #if (USB_PING_PONG_MODE == USB_PING_PONG__ALL_BUT_EP0) || (USB_PING_PONG_MODE == USB_PING_PONG__FULL_PING_PONG)
                    if(USBHALGetLastDirection(USTATcopy) == OUT_FROM_HOST)
                    {
                        ep_data_out[endpoint_number].bits.ping_pong_state ^= 1;
                    }
                    else
                    {
                        ep_data_in[endpoint_number].bits.ping_pong_state ^= 1;
                    }
                #endif

                if(endpoint_number == 0)
                {
                    USBCtrlEPService();
                }
                else
                {
                    USB_TRANSFER_COMPLETE_HANDLER(EVENT_TRANSFER);
                }
		    }//end if(USBTransactionCompleteIF)
		    else
		    	break;	//USTAT FIFO must be empty.
		}//end for()
	}//end if(USBTransactionCompleteIE)

    USBClearUSBInterrupt();
}//end of USBDeviceTasks()

void USBEnableEndpoint(BYTE ep, BYTE options)
{

    if(options & USB_OUT_ENABLED)
    {
        USBConfigureEndpoint(ep, OUT_FROM_HOST);
    }
    if(options & USB_IN_ENABLED)
    {
        USBConfigureEndpoint(ep, IN_TO_HOST);
    }
    switch(ep)
    {
        case 0:
            U1EP0 = options;
            break;
        case 1:
            U1EP1 = options;
            break;
        case 2:
            U1EP2 = options;
            break;
        case 3:
            U1EP3 = options;
            break;
        case 4:
            U1EP4 = options;
            break;
        case 5:
            U1EP5 = options;
            break;
        case 6:
            U1EP6 = options;
            break;
        case 7:
            U1EP7 = options;
            break;
        case 8:
            U1EP8 = options;
            break;
        case 9:
            U1EP9 = options;
            break;
        case 10:
            U1EP10 = options;
            break;
        case 11:
            U1EP11 = options;
            break;
        case 12:
            U1EP12 = options;
            break;
        case 13:
            U1EP13 = options;
            break;
        case 14:
            U1EP14 = options;
            break;
        case 15:
            U1EP15 = options;
            break;
        default:
            break;
    }
}

extern USB_HANDLE CDCDataOutHandle;
extern USB_HANDLE CDCDataInHandle;

extern volatile FAR unsigned char cdc_data_tx[CDC_DATA_IN_EP_SIZE] ;
extern volatile FAR unsigned char cdc_data_rx[CDC_DATA_OUT_EP_SIZE] ;

void USBTransferOnePacket(BYTE ep,BYTE dir,BYTE data,BYTE len)
{
    if(dir != 0)
    {
        //point to the IN BDT of the specified endpoint
        if(pBDTEntryIn[ep] != 0)
        {
            switch(data)
            {
                case 0:
                    pBDTEntryIn[ep]->ADR = ConvertToPhysicalAddress(0);
                    break;
                case 1:
                    pBDTEntryIn[ep]->ADR = ConvertToPhysicalAddress(&cdc_data_tx);
                    break;
                case 2:
                    pBDTEntryIn[ep]->ADR = ConvertToPhysicalAddress(&cdc_data_rx);
                    break;
            }
            pBDTEntryIn[ep]->CNT = len;
            pBDTEntryIn[ep]->STAT.Val &= _DTSMASK;
            pBDTEntryIn[ep]->STAT.Val |= _USIE | (_DTSEN & _DTS_CHECKING_ENABLED);
            USBAdvancePingPongBuffer(&pBDTEntryIn[ep]);
            CDCDataInHandle = (USB_HANDLE)pBDTEntryIn[ep];
        }
    }
    else
    {
        //else point to the OUT BDT of the specified endpoint
        if(pBDTEntryOut[ep] != 0)
        {
            switch(data)
            {
                case 0:
                    pBDTEntryOut[ep]->ADR = ConvertToPhysicalAddress(0);
                    break;
                case 1:
                    pBDTEntryOut[ep]->ADR = ConvertToPhysicalAddress(&cdc_data_tx);
                    break;
                case 2:
                    pBDTEntryOut[ep]->ADR = ConvertToPhysicalAddress(&cdc_data_rx);
                    break;
            }
            pBDTEntryOut[ep]->CNT = len;
            pBDTEntryOut[ep]->STAT.Val &= _DTSMASK;
            pBDTEntryOut[ep]->STAT.Val |= _USIE | (_DTSEN & _DTS_CHECKING_ENABLED);
            USBAdvancePingPongBuffer(&pBDTEntryOut[ep]);
            CDCDataOutHandle = (USB_HANDLE)pBDTEntryOut[ep];
        }
    }
}

void USBStallEndpoint(BYTE ep, BYTE dir)
{
    if(ep == 0)
    {
#ifdef NO_PTR

        BDT[BDT_INDEX_EO_N_VALUE].CNT = USB_EP0_BUFF_SIZE;
        BDT[BDT_INDEX_EO_N_VALUE].ADR = ConvertToPhysicalAddress(&SetupPkt);
        BDT[BDT_INDEX_EO_N_VALUE].STAT.Val = _USIE|_DAT0|(_DTSEN & _DTS_CHECKING_ENABLED)|_BSTALL;
#else
        pBDTEntryEP0OutNext->CNT = USB_EP0_BUFF_SIZE;
        pBDTEntryEP0OutNext->ADR = ConvertToPhysicalAddress(&SetupPkt);
        pBDTEntryEP0OutNext->STAT.Val = _USIE|_DAT0|(_DTSEN & _DTS_CHECKING_ENABLED)|_BSTALL;
#endif
		pBDTEntryIn[0]->STAT.Val = _USIE|_BSTALL;
    }
    else
    {

        BDT[EP(ep,dir,0)].STAT.Val |= _BSTALL | _USIE;

        //If the device is in FULL or ALL_BUT_EP0 ping pong modes
        //then stall that entry as well
        #if (USB_PING_PONG_MODE == USB_PING_PONG__FULL_PING_PONG) || (USB_PING_PONG_MODE == USB_PING_PONG__ALL_BUT_EP0)


        BDT[EP(ep,dir,1)].STAT.Val |= _BSTALL | _USIE;
        #endif
    }
}


void USBCancelIO(BYTE endpoint)
{
    if(USBPacketDisable == 1)
    {
    	//The PKTDIS bit is currently set right now.  It is therefore "safe"
    	//to mess with the BDT right now.
    	pBDTEntryIn[endpoint]->Val &= _DTSMASK;	//Makes UOWN = 0 (_UCPU mode).  Deactivates endpoint.  Only sends NAKs.
    	pBDTEntryIn[endpoint]->Val ^= _DTSMASK;	//Toggle the DTS bit.  This packet didn't get sent yet, and the next call to USBTransferOnePacket() will re-toggle the DTS bit back to the original (correct) value.

    	//Need to do additional handling if ping-pong buffering is being used
        #if ((USB_PING_PONG_MODE == USB_PING_PONG__FULL_PING_PONG) || (USB_PING_PONG_MODE == USB_PING_PONG__ALL_BUT_EP0))
        //Point to the next buffer for ping pong purposes.  UOWN getting cleared
        //(either due to SIE clearing it after a transaction, or the firmware
        //clearing it) makes hardware ping pong pointer advance.
        USBAdvancePingPongBuffer(&pBDTEntryIn[endpoint]);

    	pBDTEntryIn[endpoint]->STAT.Val &= _DTSMASK;
    	pBDTEntryIn[endpoint]->STAT.Val ^= _DTSMASK;
        #endif
    }
}


void USBCtrlEPAllowStatusStage(void)
{
    if(USBStatusStageEnabledFlag1 == FALSE)
    {
        USBStatusStageEnabledFlag1 = TRUE;
        if(USBStatusStageEnabledFlag2 == FALSE)
        {
            USBStatusStageEnabledFlag2 = TRUE;

            //Determine which endpoints (EP0 IN or OUT needs arming for the status
            //stage), based on the type of control transfer currently pending.
            if(controlTransferState == CTRL_TRF_RX)
            {
                pBDTEntryIn[0]->CNT = 0;
                pBDTEntryIn[0]->STAT.Val = _USIE|_DAT1|(_DTSEN & _DTS_CHECKING_ENABLED);
            }
            else if(controlTransferState == CTRL_TRF_TX)
            {
        		BothEP0OutUOWNsSet = FALSE;	//Indicator flag used in USBCtrlTrfOutHandler()

                //This buffer (when ping pong buffering is enabled on EP0 OUT) receives the
                //next SETUP packet.
        		#if((USB_PING_PONG_MODE == USB_PING_PONG__EP0_OUT_ONLY) || (USB_PING_PONG_MODE == USB_PING_PONG__FULL_PING_PONG))

                        #ifdef NO_PTR
                        BDT[BDT_INDEX_EO_C_VALUE].CNT = USB_EP0_BUFF_SIZE;
        		BDT[BDT_INDEX_EO_C_VALUE].ADR = ConvertToPhysicalAddress(&SetupPkt);
        		BDT[BDT_INDEX_EO_C_VALUE].STAT.Val = _USIE|_BSTALL; //Prepare endpoint to accept a SETUP transaction

                        #else
        		pBDTEntryEP0OutCurrent->CNT = USB_EP0_BUFF_SIZE;
        		pBDTEntryEP0OutCurrent->ADR = ConvertToPhysicalAddress(&SetupPkt);
        		pBDTEntryEP0OutCurrent->STAT.Val = _USIE|_BSTALL; //Prepare endpoint to accept a SETUP transaction
                        #endif


        		BothEP0OutUOWNsSet = TRUE;	//Indicator flag used in USBCtrlTrfOutHandler()
        		#endif

                //This EP0 OUT buffer receives the 0-byte OUT status stage packet.
#ifdef NO_PTR
                        BDT[BDT_INDEX_EO_N_VALUE].CNT = USB_EP0_BUFF_SIZE;
        		BDT[BDT_INDEX_EO_N_VALUE].ADR = ConvertToPhysicalAddress(&SetupPkt);
        		BDT[BDT_INDEX_EO_N_VALUE].STAT.Val = _USIE;           // Note: DTSEN is 0
#else
                        pBDTEntryEP0OutNext->CNT = USB_EP0_BUFF_SIZE;
        		pBDTEntryEP0OutNext->ADR = ConvertToPhysicalAddress(&SetupPkt);
        		pBDTEntryEP0OutNext->STAT.Val = _USIE;           // Note: DTSEN is 0
#endif
            }
        }
    }
}


void USBCtrlEPAllowDataStage(void)
{
    USBDeferINDataStagePackets = FALSE;
    USBDeferOUTDataStagePackets = FALSE;

    if(controlTransferState == CTRL_TRF_RX) //(<setup><out><out>...<out><in>)
    {
        //Prepare EP0 OUT to receive the first OUT data packet in the data stage sequence.
#ifdef NO_PTR
	BDT[BDT_INDEX_EO_N_VALUE].CNT = USB_EP0_BUFF_SIZE;
        BDT[BDT_INDEX_EO_N_VALUE].ADR = ConvertToPhysicalAddress(&CtrlTrfData);
        BDT[BDT_INDEX_EO_N_VALUE].STAT.Val = _USIE|_DAT1|(_DTSEN & _DTS_CHECKING_ENABLED);
#else
        pBDTEntryEP0OutNext->CNT = USB_EP0_BUFF_SIZE;
        pBDTEntryEP0OutNext->ADR = ConvertToPhysicalAddress(&CtrlTrfData);
        pBDTEntryEP0OutNext->STAT.Val = _USIE|_DAT1|(_DTSEN & _DTS_CHECKING_ENABLED);
#endif
    }
    else    //else must be controlTransferState == CTRL_TRF_TX (<setup><in><in>...<in><out>)
    {
        //Error check the data stage byte count.  Make sure the user specified
        //value was no greater than the number of bytes the host requested.
		if(SetupPkt.wLength < inPipes[0].wCount.Val)
		{
			inPipes[0].wCount.Val = SetupPkt.wLength;
		}
		USBCtrlTrfTxService();  //Copies one IN data packet worth of data from application buffer
		                        //to CtrlTrfData buffer.  Also keeps track of how many bytes remaining.

	    //Cnt should have been initialized by responsible request owner (ex: by
	    //using the USBEP0SendRAMPtr() or USBEP0SendROMPtr() API function).
		pBDTEntryIn[0]->ADR = ConvertToPhysicalAddress(&CtrlTrfData);
		pBDTEntryIn[0]->STAT.Val = _USIE|_DAT1|(_DTSEN & _DTS_CHECKING_ENABLED);
    }
}


static void USBConfigureEndpoint(BYTE EPNum, BYTE direction)
{

    //Compute a pointer to the even BDT entry corresponding to the
    //EPNum and direction values passed to this function.
    //handle = (volatile BDT_ENTRY*)&BDT[EP0_OUT_EVEN]; //Get address of start of BDT
    //handle += EP(EPNum,direction,0u);     //Add in offset to the BDT of interest

    BDT[EP0_OUT_EVEN + EP(EPNum,direction,0u)].STAT.UOWN = 0;  //mostly redundant, since USBStdSetCfgHandler()
    //already cleared the entire BDT table

    //Make sure our pBDTEntryIn/Out[] pointer is initialized.  Needed later
    //for USBTransferOnePacket() API calls.
    if(direction == OUT_FROM_HOST)
    {
        pBDTEntryOut[EPNum] = &BDT[EP0_OUT_EVEN + EP(EPNum,direction,0u)];
    }
    else
    {
        pBDTEntryIn[EPNum] = &BDT[EP0_OUT_EVEN + EP(EPNum,direction,0u)];
    }

    #if (USB_PING_PONG_MODE == USB_PING_PONG__FULL_PING_PONG)
        BDT[EP0_OUT_EVEN + EP(EPNum,direction,0u)].STAT.DTS = 0;
        BDT[EP0_OUT_EVEN + EP(EPNum,direction,0u) + 1].STAT.DTS = 1;
    #elif (USB_PING_PONG_MODE == USB_PING_PONG__NO_PING_PONG)
        //Set DTS to one because the first thing we will do
        //when transmitting is toggle the bit
        handle->STAT.DTS = 1;
    #elif (USB_PING_PONG_MODE == USB_PING_PONG__EP0_OUT_ONLY)
        if(EPNum != 0)
        {
            handle->STAT.DTS = 1;
        }
    #elif (USB_PING_PONG_MODE == USB_PING_PONG__ALL_BUT_EP0)
        if(EPNum != 0)
        {
            handle->STAT.DTS = 0;
            (handle+1)->STAT.DTS = 1;
        }
    #endif
}


static void USBCtrlEPServiceComplete(void)
{
    USBPacketDisable = 0;

    if(inPipes[0].info.bits.busy == 0)
    {
        if(outPipes[0].info.bits.busy == 1)
        {
            controlTransferState = CTRL_TRF_RX;

            //1. Prepare OUT EP to receive data, unless a USB class request handler
            //   function decided to defer the data stage (ex: because the intended
            //   RAM buffer wasn't available yet) by calling USBDeferDataStage().
            //   If it did so, it is then responsible for calling USBCtrlEPAllowDataStage(),
            //   once it is ready to begin receiving the data.
            if(USBDeferOUTDataStagePackets == FALSE)
            {
                USBCtrlEPAllowDataStage();
            }

            //2.  IN endpoint 0 status stage will be armed by USBCtrlEPAllowStatusStage()
            //after all of the OUT data has been received and consumed, or if a timeout occurs.
            USBStatusStageEnabledFlag2 = FALSE;
            USBStatusStageEnabledFlag1 = FALSE;
        }
        else
        {
            /*
             * If no one knows how to service this request then stall.
             * Must also prepare EP0 to receive the next SETUP transaction.
             */
#ifdef NO_PTR
            BDT[BDT_INDEX_EO_N_VALUE].CNT = USB_EP0_BUFF_SIZE;
            BDT[BDT_INDEX_EO_N_VALUE].ADR = ConvertToPhysicalAddress(&SetupPkt);
            BDT[BDT_INDEX_EO_N_VALUE].STAT.Val = _USIE|_DAT0|(_DTSEN & _DTS_CHECKING_ENABLED)|_BSTALL;

#else
            pBDTEntryEP0OutNext->CNT = USB_EP0_BUFF_SIZE;
            pBDTEntryEP0OutNext->ADR = ConvertToPhysicalAddress(&SetupPkt);
            pBDTEntryEP0OutNext->STAT.Val = _USIE|_DAT0|(_DTSEN & _DTS_CHECKING_ENABLED)|_BSTALL;
#endif
            pBDTEntryIn[0]->STAT.Val = _USIE|_BSTALL;
        }
    }
    else    // A module has claimed ownership of the control transfer session.
    {
		if(SetupPkt.DataDir == USB_SETUP_DEVICE_TO_HOST_BITFIELD)
		{
			controlTransferState = CTRL_TRF_TX;
			if(USBDeferINDataStagePackets == FALSE)
            {
                USBCtrlEPAllowDataStage();
			}
            USBStatusStageEnabledFlag2 = FALSE;
            USBStatusStageEnabledFlag1 = FALSE;
            if(USBDeferStatusStagePacket == FALSE)
            {
                USBCtrlEPAllowStatusStage();
            }
		}
		else   // (SetupPkt.DataDir == USB_SETUP_DIRECTION_HOST_TO_DEVICE)
		{
			//This situation occurs for special types of control transfers,
			//such as that which occurs when the host sends a SET_ADDRESS
			//control transfer.  Ex:
			//
			//<SETUP[0]><IN[1]> | <SETUP[0]>

			//Although the data direction is HOST_TO_DEVICE, there is no data stage
			//(hence: outPipes[0].info.bits.busy == 0).  There is however still
			//an IN status stage.

			controlTransferState = CTRL_TRF_RX;     //Since this is a HOST_TO_DEVICE control transfer

			//1. Prepare OUT EP to receive the next SETUP packet.
#ifdef NO_PTR
			BDT[BDT_INDEX_EO_N_VALUE].CNT = USB_EP0_BUFF_SIZE;
			BDT[BDT_INDEX_EO_N_VALUE].ADR = ConvertToPhysicalAddress(&SetupPkt);
			BDT[BDT_INDEX_EO_N_VALUE].STAT.Val = _USIE|_BSTALL;
#else
			pBDTEntryEP0OutNext->CNT = USB_EP0_BUFF_SIZE;
			pBDTEntryEP0OutNext->ADR = ConvertToPhysicalAddress(&SetupPkt);
			pBDTEntryEP0OutNext->STAT.Val = _USIE|_BSTALL;
#endif
			//2. Prepare for IN status stage of the control transfer
            USBStatusStageEnabledFlag2 = FALSE;
            USBStatusStageEnabledFlag1 = FALSE;
			if(USBDeferStatusStagePacket == FALSE)
            {
                USBCtrlEPAllowStatusStage();
            }
		}

    }//end if(ctrl_trf_session_owner == MUID_NULL)

}//end USBCtrlEPServiceComplete


    BYTE byteToSend_count,count2;
static void USBCtrlTrfTxService(void)
{


    //Figure out how many bytes of data to send in the next IN transaction.
    //Assume a full size packet, unless otherwise determined below.
    byteToSend_count = USB_EP0_BUFF_SIZE;
    if(inPipes[0].wCount.Val < (BYTE)USB_EP0_BUFF_SIZE)
    {
        byteToSend_count = inPipes[0].wCount.Val;

        //Keep track of whether or not we have sent a "short packet" yet.
        //This is useful so that later on, we can configure EP0 IN to STALL,
        //after we have sent all of the intended data.  This makes sure the
        //hardware STALLs if the host erroneously tries to send more IN token
        //packets, requesting more data than intended in the control transfer.
        if(shortPacketStatus == SHORT_PKT_NOT_USED)
        {
            shortPacketStatus = SHORT_PKT_PENDING;
        }
        else if(shortPacketStatus == SHORT_PKT_PENDING)
        {
            shortPacketStatus = SHORT_PKT_SENT;
        }
    }

    //Keep track of how many bytes remain to be sent in the transfer, by
    //subtracting the number of bytes about to be sent from the total.
    inPipes[0].wCount.Val = inPipes[0].wCount.Val - byteToSend_count;

    //Next, load the number of bytes to send to BC7..0 in buffer descriptor.
    //Note: Control endpoints may never have a max packet size of > 64 bytes.
    //Therefore, the BC8 and BC9 bits should always be maintained clear.
    pBDTEntryIn[0]->CNT = byteToSend_count;

    //Now copy the data from the source location, to the CtrlTrfData[] buffer,
    //which we will send to the host.
    count2 = 0;
    if(inPipes[0].info.bits.ctrl_trf_mem == USB_EP0_ROM)   // Determine type of memory source
    {
        while(byteToSend_count)
        {
            CtrlTrfData[count2] = *inPipes[0].pSrc.bRom++;
            count2++;
            byteToSend_count--;
        }//end while(byte_to_send.Val)
    }
    else  // RAM
    {
        while(byteToSend_count)
        {
            CtrlTrfData[count2] = *inPipes[0].pSrc.bRam++;
            count2++;
            byteToSend_count--;
        }//end while(byte_to_send.Val)
    }//end if(usb_stat.ctrl_trf_mem == _ROM)
}//end USBCtrlTrfTxService

BYTE byteToRead;
BYTE i_cnt;

static void USBCtrlTrfRxService(void)
{

    //Load byteToRead with the number of bytes the host just sent us in the
    //last OUT transaction.


#ifdef NO_PTR
    byteToRead = BDT[BDT_INDEX_EO_C_VALUE].CNT;
#else
    byteToRead = pBDTEntryEP0OutCurrent->CNT;
#endif

    //Update the "outPipes[0].wCount.Val", which keeps track of the total number
    //of remaining bytes expected to be received from the host, in the control
    //transfer.  First check to see if the host sent us more bytes than the
    //application firmware was expecting to receive.
    if(byteToRead > outPipes[0].wCount.Val)
    {
        byteToRead = outPipes[0].wCount.Val;
    }
    //Reduce the number of remaining bytes by the number we just received.
	outPipes[0].wCount.Val = outPipes[0].wCount.Val - byteToRead;

    //Copy the OUT DATAx packet bytes that we just received from the host,
    //into the user application buffer space.
    for(i_cnt=0;i_cnt<byteToRead;i_cnt++)
    {
        *outPipes[0].pDst.bRam++ = CtrlTrfData[i_cnt];
    }//end while(byteToRead.Val)

    //If there is more data to receive, prepare EP0 OUT so that it can receive
	//the next packet in the sequence.
    if(outPipes[0].wCount.Val > 0)
    {

#ifdef NO_PTR
        BDT[BDT_INDEX_EO_N_VALUE].CNT = USB_EP0_BUFF_SIZE;
        BDT[BDT_INDEX_EO_N_VALUE].ADR = ConvertToPhysicalAddress(&CtrlTrfData);
        if(BDT[BDT_INDEX_EO_C_VALUE].STAT.DTS == 0)
        {
            BDT[BDT_INDEX_EO_N_VALUE].STAT.Val = _USIE|_DAT1|(_DTSEN & _DTS_CHECKING_ENABLED);
        }
        else
        {
            BDT[BDT_INDEX_EO_N_VALUE].STAT.Val = _USIE|_DAT0|(_DTSEN & _DTS_CHECKING_ENABLED);
        }
#else
        pBDTEntryEP0OutNext->CNT = USB_EP0_BUFF_SIZE;
        pBDTEntryEP0OutNext->ADR = ConvertToPhysicalAddress(&CtrlTrfData);
        if(pBDTEntryEP0OutCurrent->STAT.DTS == 0)
        {
            pBDTEntryEP0OutNext->STAT.Val = _USIE|_DAT1|(_DTSEN & _DTS_CHECKING_ENABLED);
        }
        else
        {
            pBDTEntryEP0OutNext->STAT.Val = _USIE|_DAT0|(_DTSEN & _DTS_CHECKING_ENABLED);
        }
#endif

    }
    else
    {
	    //We have received all OUT packets that we were expecting to
	    //receive for the control transfer.  Prepare EP0 OUT to receive
		//the next SETUP transaction that may arrive.
#ifdef NO_PTR
        BDT[BDT_INDEX_EO_N_VALUE].CNT = USB_EP0_BUFF_SIZE;
        BDT[BDT_INDEX_EO_N_VALUE].ADR = ConvertToPhysicalAddress(&SetupPkt);
        BDT[BDT_INDEX_EO_N_VALUE].STAT.Val = _USIE|_BSTALL;
#else
        pBDTEntryEP0OutNext->CNT = USB_EP0_BUFF_SIZE;
        pBDTEntryEP0OutNext->ADR = ConvertToPhysicalAddress(&SetupPkt);
        pBDTEntryEP0OutNext->STAT.Val = _USIE|_BSTALL;
#endif
        //Configure EP0 OUT to receive the next SETUP transaction for any future
        //control transfers.  However, set BSTALL in case the host tries to send
        //more data than it claims it was going to send.


		//All data bytes for the host to device control write (OUT) have now been
		//received successfully.
		//Go ahead and call the user specified callback function, to use/consume
		//the control transfer data (ex: if the "void (*function)" parameter
		//was non-NULL when USBEP0Receive() was called).
        if(outPipes[0].pFunc != NULL)
        {
                outPipes[0].pFunc();    //Call the user's callback function
        }
        outPipes[0].info.bits.busy = 0;

        if(USBDeferStatusStagePacket == FALSE)
        {
            USBCtrlEPAllowStatusStage();
        }
    }

}//end USBCtrlTrfRxService


    BYTE i_cnt2;
    static void USBStdSetCfgHandler(void)
{


    // This will generate a zero length packet
    inPipes[0].info.bits.busy = 1;

    //Clear all of the endpoint control registers
    DisableNonZeroEndpoints(USB_MAX_EP_NUMBER);

    //Clear all of the BDT entries
    memset((void*)&BDT[0], 0x00, sizeof(BDT));

    // Assert reset request to all of the Ping Pong buffer pointers
    USBPingPongBufferReset = 1;

	//Re-Initialize all ping pong software state bits to 0 (which corresponds to
	//the EVEN buffer being the next one that will be used), since we are also
	//doing a hardware ping pong pointer reset above.
	for(i_cnt2 = 0; i_cnt2 < (BYTE)(USB_MAX_EP_NUMBER+1u); i_cnt2++)
	{
		ep_data_in[i_cnt2].Val = 0u;
                ep_data_out[i_cnt2].Val = 0u;
	}

    //clear the alternate interface settings
    memset((void*)&USBAlternateInterface,0x00,USB_MAX_NUM_INT);

    //Stop trying to reset ping pong buffer pointers
    USBPingPongBufferReset = 0;

    pBDTEntryIn[0] = (volatile BDT_ENTRY*)&BDT[EP0_IN_EVEN];

	//Set the next out to the current out packet

#ifdef NO_PTR
    BDT_INDEX_EO_C_VALUE = EP0_OUT_EVEN;
    BDT_INDEX_EO_N_VALUE = EP0_OUT_EVEN;

//    BDTEntryEP0OutCurrent.Val = BDT[EP0_OUT_EVEN].Val;
//    BDTEntryEP0OutNext.Val = BDTEntryEP0OutCurrent.Val;
#else
    pBDTEntryEP0OutCurrent = (volatile BDT_ENTRY*)&BDT[EP0_OUT_EVEN];
    pBDTEntryEP0OutNext = pBDTEntryEP0OutCurrent;
#endif

    //set the current configuration
    USBActiveConfiguration = SetupPkt.bConfigurationValue;

    //if the configuration value == 0
    if(USBActiveConfiguration == 0)
    {
        //Go back to the addressed state
        USBDeviceState = ADDRESS_STATE;
    }
    else
    {
        //initialize the required endpoints
        USB_SET_CONFIGURATION_HANDLER(EVENT_CONFIGURED);

        //Otherwise go to the configured state.  Update the state variable last,
        //after performing all of the set configuration related initialization
        //tasks.
        USBDeviceState = CONFIGURED_STATE;
    }//end if(SetupPkt.bConfigurationValue == 0)
}//end USBStdSetCfgHandler

extern ROM struct{BYTE bLength;BYTE bDscType;WORD string[1];}sd000;
extern ROM struct{BYTE bLength;BYTE bDscType;WORD string[25];}sd001;
extern ROM struct{BYTE bLength;BYTE bDscType;WORD string[25];}sd002;
static void USBStdGetDscHandler(void)
{
    if(SetupPkt.bmRequestType == 0x80)
    {
        inPipes[0].info.Val = USB_EP0_ROM | USB_EP0_BUSY | USB_EP0_INCLUDE_ZERO;

        switch(SetupPkt.bDescriptorType)
        {
            case USB_DESCRIPTOR_DEVICE:
                #if !defined(USB_USER_DEVICE_DESCRIPTOR)
                    inPipes[0].pSrc.bRom = (ROM BYTE*)&device_dsc;
                #else
                    inPipes[0].pSrc.bRom = (ROM BYTE*)&device_dsc;
                #endif
                inPipes[0].wCount.Val = sizeof(device_dsc);
                break;
            case USB_DESCRIPTOR_CONFIGURATION:
                #if !defined(USB_USER_CONFIG_DESCRIPTOR)
                    inPipes[0].pSrc.bRom = *(USB_CD_Ptr+SetupPkt.bDscIndex);
                #else
                    inPipes[0].pSrc.bRom = (&configDescriptor1[0]+SetupPkt.bDscIndex);
                #endif

                //This must be loaded using byte addressing.  The source pointer
                //  may not be word aligned for the 16 or 32 bit machines resulting
                //  in an address error on the dereference.
                inPipes[0].wCount.byte.LB = *(inPipes[0].pSrc.bRom+2);
                inPipes[0].wCount.byte.HB = *(inPipes[0].pSrc.bRom+3);
                break;
            case USB_DESCRIPTOR_STRING:
                //USB_NUM_STRING_DESCRIPTORS was introduced as optional in release v2.3.  In v2.4 and
                //  later it is now manditory.  This should be defined in usb_config.h and should
                //  indicate the number of string descriptors.
                if(SetupPkt.bDscIndex<USB_NUM_STRING_DESCRIPTORS)
                {
                    //Get a pointer to the String descriptor requested
                    switch(SetupPkt.bDscIndex)
                    {
                        case 0:
                            inPipes[0].pSrc.bRom = (ROM BYTE *)(&sd000 + SetupPkt.bDscIndex);
                            break;
                        case 1:
                            inPipes[0].pSrc.bRom = (ROM BYTE *)(&sd001 + SetupPkt.bDscIndex);
                            break;
                        case 2:
                            inPipes[0].pSrc.bRom = (ROM BYTE *)(&sd002 + SetupPkt.bDscIndex);
                            break;
                        default:
                            break;
                    }
                    inPipes[0].wCount.Val = *inPipes[0].pSrc.bRom;
                }
                else
                {
                    inPipes[0].info.Val = 0;
                }
                break;
            default:
                inPipes[0].info.Val = 0;
                break;
        }//end switch
    }//end if
}//end USBStdGetDscHandler


static void USBStdGetStatusHandler(void)
{
    CtrlTrfData[0] = 0;                 // Initialize content
    CtrlTrfData[1] = 0;

    switch(SetupPkt.Recipient)
    {
        case USB_SETUP_RECIPIENT_DEVICE_BITFIELD:
            inPipes[0].info.bits.busy = 1;
            /*
             * [0]: bit0: Self-Powered Status [0] Bus-Powered [1] Self-Powered
             *      bit1: RemoteWakeup        [0] Disabled    [1] Enabled
             */
            if(self_power == 1) // self_power is defined in HardwareProfile.h
            {
                CtrlTrfData[0]|=0x01;
            }

            if(RemoteWakeup == TRUE)
            {
                CtrlTrfData[0]|=0x02;
            }
            break;
        case USB_SETUP_RECIPIENT_INTERFACE_BITFIELD:
            inPipes[0].info.bits.busy = 1;     // No data to update
            break;
        case USB_SETUP_RECIPIENT_ENDPOINT_BITFIELD:
            inPipes[0].info.bits.busy = 1;
            /*
             * [0]: bit0: Halt Status [0] Not Halted [1] Halted
             */
            {
                if(SetupPkt.EPDir == 0)
                {
                    if((pBDTEntryOut[SetupPkt.EPNum]->STAT.UOWN == 1) && (pBDTEntryOut[SetupPkt.EPNum]->STAT.BSTALL == 1))
                        CtrlTrfData[0]=0x01;    // Set bit0
                }
                else
                {
                    if((pBDTEntryIn[SetupPkt.EPNum]->STAT.UOWN == 1) && (pBDTEntryIn[SetupPkt.EPNum]->STAT.BSTALL == 1))
                        CtrlTrfData[0]=0x01;    // Set bit0
                }


                break;
            }
    }//end switch

    if(inPipes[0].info.bits.busy == 1)
    {
        inPipes[0].pSrc.bRam = (BYTE*)&CtrlTrfData;            // Set Source
        inPipes[0].info.bits.ctrl_trf_mem = USB_EP0_RAM;               // Set memory type
        inPipes[0].wCount.v[0] = 2;                         // Set data count
    }//end if(...)
}//end USBStdGetStatusHandler

static void USBStallHandler(void)
{
    if(U1EP0bits.EPSTALL == 1)
    {
        // UOWN - if 0, owned by CPU, if 1, owned by SIE
#ifdef NO_PTR
        if((BDT[BDT_INDEX_EO_C_VALUE].STAT.Val == _USIE) && (pBDTEntryIn[0]->STAT.Val == (_USIE|_BSTALL)))
        {
            // Set ep0Bo to stall also
            BDT[BDT_INDEX_EO_C_VALUE].STAT.Val = _USIE|_DAT0|(_DTSEN & _DTS_CHECKING_ENABLED)|_BSTALL;
        }//end if
#else
        if((pBDTEntryEP0OutCurrent->STAT.Val == _USIE) && (pBDTEntryIn[0]->STAT.Val == (_USIE|_BSTALL)))
        {
            // Set ep0Bo to stall also
            pBDTEntryEP0OutCurrent->STAT.Val = _USIE|_DAT0|(_DTSEN & _DTS_CHECKING_ENABLED)|_BSTALL;
        }//end if
#endif
        U1EP0bits.EPSTALL = 0;               // Clear stall status
    }//end if

    USBClearInterruptFlag(USBStallIFReg,USBStallIFBitNum);
}


static void USBSuspend(void)
{
    USBActivityIE = 1;                     // Enable bus activity interrupt
    USBClearInterruptFlag(USBIdleIFReg,USBIdleIFBitNum);

      USBBusIsSuspended = TRUE;

    USB_SUSPEND_HANDLER(EVENT_SUSPEND);
}


static void USBWakeFromSuspend(void)
{
    USBBusIsSuspended = FALSE;

    USB_WAKEUP_FROM_SUSPEND_HANDLER(EVENT_RESUME);



    USBActivityIE = 0;

    {
        USBClearInterruptFlag(USBActivityIFReg,USBActivityIFBitNum);
    }  // Added

}//end USBWakeFromSuspend

unsigned char setup_cnt;
static void USBCtrlEPService(void)
{
    //If we get to here, that means a successful transaction has just occurred
    //on EP0.  This means "progress" has occurred in the currently pending
    //control transfer, so we should re-initialize our timeout counter.
    #if defined(USB_ENABLE_STATUS_STAGE_TIMEOUTS)
        USBStatusStageTimeoutCounter = USB_STATUS_STAGE_TIMEOUT;
    #endif

	//Check if the last transaction was on EP0 OUT endpoint (of any kind, to either the even or odd buffer if ping pong buffers used)
    if((USTATcopy.Val & USTAT_EP0_PP_MASK) == USTAT_EP0_OUT_EVEN)
    {
		//Point to the EP0 OUT buffer of the buffer that arrived
            #ifdef NO_PTR
                BDT_INDEX_EO_C_VALUE = (USTATcopy.Val & USTAT_EP_MASK)>>2;
                BDT_INDEX_EO_N_VALUE = (USTATcopy.Val & USTAT_EP_MASK)>>2;
//                BDTEntryEP0OutCurrent.Val= BDT[(USTATcopy.Val & USTAT_EP_MASK)>>2].Val;
//                BDTEntryEP0OutNext.Val = BDTEntryEP0OutCurrent.Val;
            #else
                pBDTEntryEP0OutCurrent = (volatile BDT_ENTRY*)&BDT[(USTATcopy.Val & USTAT_EP_MASK)>>2];
                pBDTEntryEP0OutNext = pBDTEntryEP0OutCurrent;
            #endif


		//Set the next out to the current out packet

		//Toggle it to the next ping pong buffer (if applicable)


		//If the current EP0 OUT buffer has a SETUP packet
#ifdef NO_PTR
                BDT_INDEX_EO_N_VALUE ^= 1;
        if(BDT[BDT_INDEX_EO_C_VALUE].STAT.PID == PID_SETUP)
#else
            ((BYTE_VAL*)&pBDTEntryEP0OutNext)->Val ^= USB_NEXT_EP0_OUT_PING_PONG;
        if(pBDTEntryEP0OutCurrent->STAT.PID == PID_SETUP)
#endif

        {


	        //The SETUP transaction data may have gone into the the CtrlTrfData
	        //buffer, or elsewhere, depending upon how the BDT was prepared
	        //before the transaction.  Therefore, we should copy the data to the
	        //SetupPkt buffer so it can be processed correctly by USBCtrlTrfSetupHandler().
            for(setup_cnt = 0; setup_cnt < 8u; setup_cnt++) //SETUP data packets always contain exactly 8 bytes.
            {
#ifdef  NO_PTR
                *(BYTE*)((BYTE*)&SetupPkt + setup_cnt) = *(BYTE*)ConvertToVirtualAddress(BDT[BDT_INDEX_EO_C_VALUE].ADR);
                BDT[BDT_INDEX_EO_C_VALUE].ADR++;
            }
            BDT[BDT_INDEX_EO_C_VALUE].ADR = ConvertToPhysicalAddress(&SetupPkt);
#else
                *(BYTE*)((BYTE*)&SetupPkt + setup_cnt) = *(BYTE*)ConvertToVirtualAddress(pBDTEntryEP0OutCurrent->ADR);
                pBDTEntryEP0OutCurrent->ADR++;
            }
            pBDTEntryEP0OutCurrent->ADR = ConvertToPhysicalAddress(&SetupPkt);
#endif

			//Handle the control transfer (parse the 8-byte SETUP command and figure out what to do)
            USBCtrlTrfSetupHandler();
        }
        else
        {
			//Handle the DATA transfer
            USBCtrlTrfOutHandler();
        }
    }
    else if((USTATcopy.Val & USTAT_EP0_PP_MASK) == USTAT_EP0_IN)
    {
		//Otherwise the transmission was and EP0 IN
		//  so take care of the IN transfer
        USBCtrlTrfInHandler();
    }

}//end USBCtrlEPService

static void USBCtrlTrfSetupHandler(void)
{
    //--------------------------------------------------------------------------
    //1. Re-initialize state tracking variables related to control transfers.
    //--------------------------------------------------------------------------
    shortPacketStatus = SHORT_PKT_NOT_USED;
    USBDeferStatusStagePacket = FALSE;
    USBDeferINDataStagePackets = FALSE;
    USBDeferOUTDataStagePackets = FALSE;
    BothEP0OutUOWNsSet = FALSE;
    controlTransferState = WAIT_SETUP;

    //Abandon any previous control transfers that might have been using EP0.
    //Ordinarily, nothing actually needs abandoning, since the previous control
    //transfer would have completed successfully prior to the host sending the next
    //SETUP packet.  However, in a timeout error case, or after an EP0 STALL event,
    //one or more UOWN bits might still be set.  If so, we should clear the UOWN bits,
    //so the EP0 IN/OUT endpoints are in a known inactive state, ready for re-arming
    //by the class request handler that will be called next.
    pBDTEntryIn[0]->STAT.Val &= ~(_USIE);
    ((BYTE_VAL*)&pBDTEntryIn[0])->Val ^= USB_NEXT_EP0_IN_PING_PONG;
    pBDTEntryIn[0]->STAT.Val &= ~(_USIE);
    ((BYTE_VAL*)&pBDTEntryIn[0])->Val ^= USB_NEXT_EP0_IN_PING_PONG;
#ifdef NO_PTR
    BDT[BDT_INDEX_EO_N_VALUE].STAT.Val &= ~(_USIE);
#else
    pBDTEntryEP0OutNext->STAT.Val &= ~(_USIE);
#endif


    inPipes[0].info.Val = 0;
    inPipes[0].wCount.Val = 0;
    outPipes[0].info.Val = 0;
    outPipes[0].wCount.Val = 0;


    //--------------------------------------------------------------------------
    //2. Now find out what was in the SETUP packet, and begin handling the request.
    //--------------------------------------------------------------------------
    USBCheckStdRequest();                                               //Check for standard USB "Chapter 9" requests.
    USB_NONSTANDARD_EP0_REQUEST_HANDLER(EVENT_EP0_REQUEST); //Check for USB device class specific requests


    //--------------------------------------------------------------------------
    //3. Re-arm EP0 IN and EP0 OUT endpoints, based on the control transfer in
    //   progress.  If one of the above handlers (in step 2) knew how to process
    //   the request, it will have set one of the inPipes[0].info.bits.busy or
    //   outPipes[0].info.bits.busy flags = 1.  This lets the
    //   USBCtrlEPServiceComplete() function know how and which endpoints to
    //   arm.  If both info.bits.busy flags are = 0, then no one knew how to
    //   process the request.  In this case, the default behavior will be to
    //   perform protocol STALL on EP0.
    //--------------------------------------------------------------------------
    USBCtrlEPServiceComplete();
}//end USBCtrlTrfSetupHandler


static void USBCtrlTrfOutHandler(void)
{
    if(controlTransferState == CTRL_TRF_RX)
    {
        USBCtrlTrfRxService();	//Copies the newly received data into the appropriate buffer and configures EP0 OUT for next transaction.
    }
    else //In this case the last OUT transaction must have been a status stage of a CTRL_TRF_TX (<setup><in><in>...<OUT>  <-- this last OUT just occurred as the status stage)
    {
        //If the status stage is complete, this means we are done with the
        //control transfer.  Go back to the idle "WAIT_SETUP" state.
        controlTransferState = WAIT_SETUP;

	    //Prepare EP0 OUT for the next SETUP transaction, however, it may have
	    //already been prepared if ping-pong buffering was enabled on EP0 OUT,
	    //and the last control transfer was of direction: device to host, see
	    //USBCtrlEPServiceComplete().  If it was already prepared, do not want
	    //to do anything to the BDT.
		if(BothEP0OutUOWNsSet == FALSE)
		{
#ifdef NO_PTR
	        BDT[BDT_INDEX_EO_N_VALUE].CNT = USB_EP0_BUFF_SIZE;
	        BDT[BDT_INDEX_EO_N_VALUE].ADR = ConvertToPhysicalAddress(&SetupPkt);
	        BDT[BDT_INDEX_EO_N_VALUE].STAT.Val = _USIE|_DAT0|(_DTSEN & _DTS_CHECKING_ENABLED)|_BSTALL;
#else
	        pBDTEntryEP0OutNext->CNT = USB_EP0_BUFF_SIZE;
	        pBDTEntryEP0OutNext->ADR = ConvertToPhysicalAddress(&SetupPkt);
	        pBDTEntryEP0OutNext->STAT.Val = _USIE|_DAT0|(_DTSEN & _DTS_CHECKING_ENABLED)|_BSTALL;
#endif
		}
		else
		{
			BothEP0OutUOWNsSet = FALSE;
		}
    }
}

    BYTE lastDTS;
static void USBCtrlTrfInHandler(void)
{


    lastDTS = pBDTEntryIn[0]->STAT.DTS;


    ((BYTE_VAL*)&pBDTEntryIn[0])->Val ^= USB_NEXT_EP0_IN_PING_PONG;

    //Must check if in ADR_PENDING_STATE.  If so, we need to update the address
    //now, since the IN status stage of the (set address) control transfer has
    //evidently completed successfully.
    if(USBDeviceState == ADR_PENDING_STATE)
    {
        U1ADDR = SetupPkt.bDevADR.Val;
        if(U1ADDR != 0u)
        {
            USBDeviceState=ADDRESS_STATE;
        }
        else
        {
            USBDeviceState=DEFAULT_STATE;
        }
    }//end if


    if(controlTransferState == CTRL_TRF_TX)
    {
        pBDTEntryIn[0]->ADR = ConvertToPhysicalAddress(CtrlTrfData);
        USBCtrlTrfTxService();

        //Check if we have already sent a short packet.  If so, configure
        //the endpoint to STALL in response to any further IN tokens (in the
        //case that the host erroneously tries to receive more data than it
        //should).
        if(shortPacketStatus == SHORT_PKT_SENT)
        {
            // If a short packet has been sent, don't want to send any more,
            // stall next time if host is still trying to read.
            pBDTEntryIn[0]->STAT.Val = _USIE|_BSTALL;
        }
        else
        {
            if(lastDTS == 0)
            {
                pBDTEntryIn[0]->STAT.Val = _USIE|_DAT1|(_DTSEN & _DTS_CHECKING_ENABLED);
            }
            else
            {
                pBDTEntryIn[0]->STAT.Val = _USIE|_DAT0|(_DTSEN & _DTS_CHECKING_ENABLED);
            }
        }//end if(...)else
    }
	else // must have been a CTRL_TRF_RX status stage IN packet (<setup><out><out>...<IN>  <-- this last IN just occurred as the status stage)
	{
        //if someone is still expecting data from the control transfer
        //  then make sure to terminate that request and let them know that
        //  they are done
        if(outPipes[0].info.bits.busy == 1)
        {
            if(outPipes[0].pFunc != NULL)
            {
                outPipes[0].pFunc();
            }
            outPipes[0].info.bits.busy = 0;
        }

        controlTransferState = WAIT_SETUP;
        //Don't need to arm EP0 OUT here.  It was already armed by the last <out> that
        //got processed by the USBCtrlTrfRxService() handler.
	}

}


static void USBCheckStdRequest(void)
{
    if(SetupPkt.RequestType != USB_SETUP_TYPE_STANDARD_BITFIELD) return;

    switch(SetupPkt.bRequest)
    {
        case USB_REQUEST_SET_ADDRESS:
            inPipes[0].info.bits.busy = 1;            // This will generate a zero length packet
            USBDeviceState = ADR_PENDING_STATE;       // Update state only
            break;
        case USB_REQUEST_GET_DESCRIPTOR:
            USBStdGetDscHandler();
            break;
        case USB_REQUEST_SET_CONFIGURATION:
            USBStdSetCfgHandler();
            break;
        case USB_REQUEST_GET_CONFIGURATION:
            inPipes[0].pSrc.bRam = (BYTE*)&USBActiveConfiguration;         // Set Source
            inPipes[0].info.bits.ctrl_trf_mem = USB_EP0_RAM;               // Set memory type
            inPipes[0].wCount.v[0] = 1;                         // Set data count
            inPipes[0].info.bits.busy = 1;
            break;
        case USB_REQUEST_GET_STATUS:
            USBStdGetStatusHandler();
            break;
        case USB_REQUEST_CLEAR_FEATURE:
        case USB_REQUEST_SET_FEATURE:
            USBStdFeatureReqHandler();
            break;
        case USB_REQUEST_GET_INTERFACE:
            inPipes[0].pSrc.bRam = (BYTE*)&USBAlternateInterface[SetupPkt.bIntfID];  // Set source
            inPipes[0].info.bits.ctrl_trf_mem = USB_EP0_RAM;               // Set memory type
            inPipes[0].wCount.v[0] = 1;                         // Set data count
            inPipes[0].info.bits.busy = 1;
            break;
        case USB_REQUEST_SET_INTERFACE:
            inPipes[0].info.bits.busy = 1;
            USBAlternateInterface[SetupPkt.bIntfID] = SetupPkt.bAltID;
            break;
        case USB_REQUEST_SET_DESCRIPTOR:
            USB_SET_DESCRIPTOR_HANDLER(EVENT_SET_DESCRIPTOR);
            break;
        case USB_REQUEST_SYNCH_FRAME:
        default:
            break;
    }//end switch
}//end USBCheckStdRequest

EP_STATUS current_ep_data;
static void USBStdFeatureReqHandler(void)
{
        //Check if the host sent a valid SET or CLEAR feature (remote wakeup) request.
    if((SetupPkt.bFeature == USB_FEATURE_DEVICE_REMOTE_WAKEUP)&&
       (SetupPkt.Recipient == USB_SETUP_RECIPIENT_DEVICE_BITFIELD))
    {
        inPipes[0].info.bits.busy = 1;
        if(SetupPkt.bRequest == USB_REQUEST_SET_FEATURE)
            RemoteWakeup = TRUE;
        else
            RemoteWakeup = FALSE;
    }//end if

    //Check if the host sent a valid SET or CLEAR endpoint halt request.
    if((SetupPkt.bFeature == USB_FEATURE_ENDPOINT_HALT)&&
       (SetupPkt.Recipient == USB_SETUP_RECIPIENT_ENDPOINT_BITFIELD)&&
       (SetupPkt.EPNum != 0) && (SetupPkt.EPNum <= USB_MAX_EP_NUMBER)&&
       (USBDeviceState == CONFIGURED_STATE))
    {
		//The request was valid.  Take control of the control transfer and
		//perform the host requested action.
		inPipes[0].info.bits.busy = 1;

        //Fetch a pointer to the BDT that the host wants to SET/CLEAR halt on.
        if(SetupPkt.EPDir == OUT_FROM_HOST)
        {
            current_ep_data.Val = ep_data_out[SetupPkt.EPNum].Val;
        }
        else
        {
            current_ep_data.Val = ep_data_in[SetupPkt.EPNum].Val;
        }

        //If ping pong buffering is enabled on the requested endpoint, need
        //to point to the one that is the active BDT entry which the SIE will
        //use for the next attempted transaction on that EP number.
        #if (USB_PING_PONG_MODE == USB_PING_PONG__ALL_BUT_EP0) || (USB_PING_PONG_MODE == USB_PING_PONG__FULL_PING_PONG)
            if(current_ep_data.bits.ping_pong_state == 0) //Check if even
            {
                if(SetupPkt.EPDir == OUT_FROM_HOST)
                {
                    USBHALPingPongSetToEven(pBDTEntryOut[SetupPkt.EPNum]);
                }
                else
                {
                    USBHALPingPongSetToEven(pBDTEntryIn[SetupPkt.EPNum]);
                }
            }
            else //else must have been odd
            {
                if(SetupPkt.EPDir == OUT_FROM_HOST)
                {
                    USBHALPingPongSetToOdd(pBDTEntryOut[SetupPkt.EPNum]);
                }
                else
                {
                    USBHALPingPongSetToOdd(pBDTEntryIn[SetupPkt.EPNum]);
                }
            }
        #endif

        //Update the BDT pointers with the new, next entry based on the feature
        //  request
        if(SetupPkt.EPDir == OUT_FROM_HOST)
        {
            pBDTEntryOut[SetupPkt.EPNum] = pBDTEntryIn[SetupPkt.EPNum];
        }
        else
        {
            pBDTEntryIn[SetupPkt.EPNum] = pBDTEntryOut[SetupPkt.EPNum];
        }

		//Check if it was a SET_FEATURE endpoint halt request
        if(SetupPkt.bRequest == USB_REQUEST_SET_FEATURE)
        {
                if(SetupPkt.EPDir == OUT_FROM_HOST)
                {
                    if(pBDTEntryOut[SetupPkt.EPNum]->STAT.UOWN == 1)
                    {
                    //Mark that we are terminating this transfer and that the user
                    //  needs to be notified later
                        if(SetupPkt.EPDir == OUT_FROM_HOST)
                        {
                            ep_data_out[SetupPkt.EPNum].bits.transfer_terminated = 1;
                        }
                        else
                        {
                            ep_data_in[SetupPkt.EPNum].bits.transfer_terminated = 1;
                        }
                    }
                    pBDTEntryOut[SetupPkt.EPNum]->STAT.Val |= _USIE|_BSTALL;
                }
                else
                {
                    if(pBDTEntryIn[SetupPkt.EPNum]->STAT.UOWN == 1)
                    {
                    //Mark that we are terminating this transfer and that the user
                    //  needs to be notified later
                        if(SetupPkt.EPDir == OUT_FROM_HOST)
                        {
                            ep_data_out[SetupPkt.EPNum].bits.transfer_terminated = 1;
                        }
                        else
                        {
                            ep_data_in[SetupPkt.EPNum].bits.transfer_terminated = 1;
                        }
                    }
                    pBDTEntryIn[SetupPkt.EPNum]->STAT.Val |= _USIE|_BSTALL;
                }

        }//if(SetupPkt.bRequest == USB_REQUEST_SET_FEATURE)
        else
        {
            		if(SetupPkt.EPDir == OUT_FROM_HOST)
        {
            //Else the request must have been a CLEAR_FEATURE endpoint halt
            //toggle over the to the non-active BDT
                USBAdvancePingPongBuffer(pBDTEntryOut[SetupPkt.EPNum]);

                if(pBDTEntryOut[SetupPkt.EPNum]->STAT.UOWN == 1)
                {
                    //Clear UOWN and set DTS state so it will be correct the next time
                    //the application firmware uses USBTransferOnePacket() on the EP.
                    pBDTEntryOut[SetupPkt.EPNum]->STAT.Val &= (~_USIE);    //Clear UOWN bit
                    pBDTEntryOut[SetupPkt.EPNum]->STAT.Val |= _DAT1;       //Set DTS to DATA1
                    USB_TRANSFER_TERMINATED_HANDLER(EVENT_TRANSFER_TERMINATED);
                }
                else
                {
                    //UOWN already clear, but still need to set DTS to DATA1
					pBDTEntryOut[SetupPkt.EPNum]->STAT.Val |= _DAT1;
                }

                //toggle back to the active BDT (the one the SIE is currently looking at
                //and will use for the next successful transaction to take place on the EP
                USBAdvancePingPongBuffer(pBDTEntryOut[SetupPkt.EPNum]);

                //Check if we are currently terminating, or have previously terminated
                //a transaction on the given endpoint.  If so, need to clear UOWN,
                //set DTS to the proper state, and call the application callback
                //function.
                if((current_ep_data.bits.transfer_terminated != 0) || (pBDTEntryOut[SetupPkt.EPNum]->STAT.UOWN == 1))
                {
                    if(SetupPkt.EPDir == OUT_FROM_HOST)
                    {
                        ep_data_out[SetupPkt.EPNum].bits.transfer_terminated = 0;
                    }
                    else
                    {
                        ep_data_in[SetupPkt.EPNum].bits.transfer_terminated = 0;
                    }
                    //clear UOWN, clear DTS to DATA0, and finally remove the STALL condition
                    pBDTEntryOut[SetupPkt.EPNum]->STAT.Val &= ~(_USIE | _DAT1 | _BSTALL);
                    //Call the application event handler callback function, so it can
					//decide if the endpoint should get re-armed again or not.
                    USB_TRANSFER_TERMINATED_HANDLER(EVENT_TRANSFER_TERMINATED);
                }
                else
                {
                    //clear UOWN, clear DTS to DATA0, and finally remove the STALL condition
                    pBDTEntryOut[SetupPkt.EPNum]->STAT.Val &= ~(_USIE | _DAT1 | _BSTALL);
                }
			switch(SetupPkt.EPNum)
			{
				case 0:
					U1EP0 &= ~UEP_STALL;
					break;
				case 1:
					U1EP1 &= ~UEP_STALL;
					break;
				case 2:
					U1EP2 &= ~UEP_STALL;
					break;
				case 3:
					U1EP3 &= ~UEP_STALL;
					break;
				case 4:
					U1EP4 &= ~UEP_STALL;
					break;
				case 5:
					U1EP5 &= ~UEP_STALL;
					break;
				case 6:
					U1EP6 &= ~UEP_STALL;
					break;
				case 7:
					U1EP7 &= ~UEP_STALL;
					break;
				case 8:
					U1EP8 &= ~UEP_STALL;
					break;
				case 9:
					U1EP9 &= ~UEP_STALL;
					break;
				case 10:
					U1EP10 &= ~UEP_STALL;
					break;
				case 11:
					U1EP11 &= ~UEP_STALL;
					break;
				case 12:
					U1EP12 &= ~UEP_STALL;
					break;
				case 13:
					U1EP13 &= ~UEP_STALL;
					break;
				case 14:
					U1EP14 &= ~UEP_STALL;
					break;
				case 15:
					U1EP15 &= ~UEP_STALL;
					break;
				default:
					break;
			}
		}
		else
		{
			//Else the request must have been a CLEAR_FEATURE endpoint halt
            //toggle over the to the non-active BDT
                USBAdvancePingPongBuffer(pBDTEntryOut[SetupPkt.EPNum]);

                if(pBDTEntryIn[SetupPkt.EPNum]->STAT.UOWN == 1)
                {
                    //Clear UOWN and set DTS state so it will be correct the next time
                    //the application firmware uses USBTransferOnePacket() on the EP.
                    pBDTEntryIn[SetupPkt.EPNum]->STAT.Val &= (~_USIE);    //Clear UOWN bit
                    pBDTEntryIn[SetupPkt.EPNum]->STAT.Val |= _DAT1;       //Set DTS to DATA1
                    USB_TRANSFER_TERMINATED_HANDLER(EVENT_TRANSFER_TERMINATED);
                }
                else
                {
                    //UOWN already clear, but still need to set DTS to DATA1
					pBDTEntryIn[SetupPkt.EPNum]->STAT.Val |= _DAT1;
                }

                //toggle back to the active BDT (the one the SIE is currently looking at
                //and will use for the next successful transaction to take place on the EP
                USBAdvancePingPongBuffer(pBDTEntryIn[SetupPkt.EPNum]);

                //Check if we are currently terminating, or have previously terminated
                //a transaction on the given endpoint.  If so, need to clear UOWN,
                //set DTS to the proper state, and call the application callback
                //function.
                if((current_ep_data.bits.transfer_terminated != 0) || (pBDTEntryIn[SetupPkt.EPNum]->STAT.UOWN == 1))
                {
                    if(SetupPkt.EPDir == OUT_FROM_HOST)
                    {
                        ep_data_out[SetupPkt.EPNum].bits.transfer_terminated = 0;
                    }
                    else
                    {
                        ep_data_in[SetupPkt.EPNum].bits.transfer_terminated = 0;
                    }
                    //clear UOWN, clear DTS to DATA0, and finally remove the STALL condition
                    pBDTEntryIn[SetupPkt.EPNum]->STAT.Val &= ~(_USIE | _DAT1 | _BSTALL);
                    //Call the application event handler callback function, so it can
					//decide if the endpoint should get re-armed again or not.
                    USB_TRANSFER_TERMINATED_HANDLER(EVENT_TRANSFER_TERMINATED);
                }
                else
                {
                    //clear UOWN, clear DTS to DATA0, and finally remove the STALL condition
                    pBDTEntryIn[SetupPkt.EPNum]->STAT.Val &= ~(_USIE | _DAT1 | _BSTALL);
                }
			switch(SetupPkt.EPNum)
			{
				case 0:
					U1EP0 &= ~UEP_STALL;
					break;
				case 1:
					U1EP1 &= ~UEP_STALL;
					break;
				case 2:
					U1EP2 &= ~UEP_STALL;
					break;
				case 3:
					U1EP3 &= ~UEP_STALL;
					break;
				case 4:
					U1EP4 &= ~UEP_STALL;
					break;
				case 5:
					U1EP5 &= ~UEP_STALL;
					break;
				case 6:
					U1EP6 &= ~UEP_STALL;
					break;
				case 7:
					U1EP7 &= ~UEP_STALL;
					break;
				case 8:
					U1EP8 &= ~UEP_STALL;
					break;
				case 9:
					U1EP9 &= ~UEP_STALL;
					break;
				case 10:
					U1EP10 &= ~UEP_STALL;
					break;
				case 11:
					U1EP11 &= ~UEP_STALL;
					break;
				case 12:
					U1EP12 &= ~UEP_STALL;
					break;
				case 13:
					U1EP13 &= ~UEP_STALL;
					break;
				case 14:
					U1EP14 &= ~UEP_STALL;
					break;
				case 15:
					U1EP15 &= ~UEP_STALL;
					break;
				default:
					break;
			}
		}
        }//end if(SetupPkt.bRequest == USB_REQUEST_SET_FEATURE)
    }//end if (lots of checks for set/clear endpoint halt)
}//end USBStdFeatureReqHandler

