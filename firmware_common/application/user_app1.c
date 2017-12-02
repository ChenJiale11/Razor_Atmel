/**********************************************************************************************************************
File: user_app1.c                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
 1. Copy both user_app1.c and user_app1.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app1" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

Description:
This is a user_app1.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void UserApp1Initialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserApp1RunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */

extern u32 G_u32AntApiCurrentDataTimeStamp;                       
extern AntApplicationMessageType G_eAntApiCurrentMessageClass;   
extern u8 G_au8AntApiCurrentMessageBytes[ANT_APPLICATION_MESSAGE_BYTES];
extern AntExtendedDataType G_sAntApiCurrentMessageExtData;       
/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
static u32 UserApp_u32Timeout;                      /* Timeout counter used across states */


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
Function: UserApp1Initialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void UserApp1Initialize(void)
{
  u8 au8WelcomeMessage[] = "Heartbeat Scan";
  u8 au8Instructions[] = "Press B0 to Start";

  AntAssignChannelInfoType sAntSetupData;
  
  /* Clear screen and place start messages */
  LCDCommand(LCD_CLEAR_CMD);
  LCDMessage(LINE1_START_ADDR, au8WelcomeMessage); 
  LCDMessage(LINE2_START_ADDR, au8Instructions); 

 /* Configure ANT for this application */
  sAntSetupData.AntChannel          = ANT_CHANNEL_USERAPP;
  sAntSetupData.AntChannelType      = ANT_CHANNEL_TYPE_USERAPP;
  sAntSetupData.AntChannelPeriodLo  = ANT_CHANNEL_PERIOD_LO_USERAPP;
  sAntSetupData.AntChannelPeriodHi  = ANT_CHANNEL_PERIOD_HI_USERAPP;
  
  sAntSetupData.AntDeviceIdLo       = ANT_DEVICEID_LO_USERAPP;
  sAntSetupData.AntDeviceIdHi       = ANT_DEVICEID_HI_USERAPP;
  sAntSetupData.AntDeviceType       = ANT_DEVICE_TYPE_USERAPP;
  sAntSetupData.AntTransmissionType = ANT_TRANSMISSION_TYPE_USERAPP;
  sAntSetupData.AntFrequency        = ANT_FREQUENCY_USERAPP;
  sAntSetupData.AntTxPower          = ANT_TX_POWER_USERAPP;

  sAntSetupData.AntNetwork = ANT_NETWORK_DEFAULT;
  /*0xB9, 0xA5, 0x21, 0xFB, 0xBD, 0x72, 0xC3, 0x45*/
  sAntSetupData.AntNetworkKey[0] = 0xB9;
  sAntSetupData.AntNetworkKey[1] = 0xA5;
  sAntSetupData.AntNetworkKey[2] = 0x21;
  sAntSetupData.AntNetworkKey[3] = 0xFB;
  sAntSetupData.AntNetworkKey[4] = 0xBD;
  sAntSetupData.AntNetworkKey[5] = 0x72;
  sAntSetupData.AntNetworkKey[6] = 0xC3;
  sAntSetupData.AntNetworkKey[7] = 0x45;
  /* If good initialization, set state to Idle */
  if( AntAssignChannel(&sAntSetupData) )
  {
    UserApp1_StateMachine = UserApp1SM_WaitChannelAssign;
  }
} /* end UserAppInitialize() */

void UserApp1SM_WaitChannelAssign(void)  
{
    if(AntRadioStatusChannel(ANT_CHANNEL_USERAPP) == ANT_CONFIGURED)
    {
        UserApp1_StateMachine = UserApp1SM_Idle;
    }
}
/*----------------------------------------------------------------------------------------------------------------------
Function UserApp1RunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void UserApp1RunActiveState(void)
{
  UserApp1_StateMachine();

} /* end UserApp1RunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp1SM_Idle(void)
{
    u8 au8WelcomeMessage[] = "Heartbeat Scan";
    u8 au8Instructions[] = "                     ";
  
    if(WasButtonPressed(BUTTON0))
    {
        ButtonAcknowledge(BUTTON0);
        LedOn(LCD_BLUE);
        LCDCommand(LCD_CLEAR_CMD);
        LCDMessage(LINE1_START_ADDR, au8WelcomeMessage); 
        LCDMessage(LINE2_START_ADDR, au8Instructions);  
        UserApp1_StateMachine = UserAppSM_WaitChannelOpen;
        AntOpenChannelNumber(ANT_CHANNEL_USERAPP);
        UserApp_u32Timeout = G_u32SystemTime1ms;
        LedOn(LCD_BLUE);
    }
} /* end UserApp1SM_Idle() */
    
static void UserAppSM_WaitChannelOpen(void)
{
    /* Monitor the channel status to check if channel is opened */
    if(AntRadioStatusChannel(ANT_CHANNEL_USERAPP) == ANT_OPEN)
    {
        UserApp1_StateMachine = UserAppSM_ChannelOpen;
    }

    /* Check for timeout */
    if( IsTimeUp(&UserApp_u32Timeout, TIMEOUT_VALUE) )
    {
        AntCloseChannelNumber(ANT_CHANNEL_USERAPP);
        UserApp1_StateMachine = UserApp1SM_Idle;
    }
} /* end UserAppSM_WaitChannelOpen() */


static void UserAppSM_ChannelOpen(void)
{
    static u8 au8Memmory[100];
    static u8 u8Count=0;
    if( AntReadAppMessageBuffer() )
    {
        /* New data message: check what it is */
        if(G_eAntApiCurrentMessageClass == ANT_DATA)
        {
            au8Memmory[u8Count]=G_au8AntApiCurrentMessageBytes[7];
            u8Count++;
            
            LedOn(BLUE);
        } /* end if(G_eAntApiCurrentMessageClass == ANT_DATA) */

        else if(G_eAntApiCurrentMessageClass == ANT_TICK)
        {
            
        } /* end else if(G_eAntApiCurrentMessageClass == ANT_TICK) */
    } /* end AntReadAppMessageBuffer() */
}
/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */

static void UserAppSM_WaitChannelClose(void)
{
    /* Monitor the channel status to check if channel is closed */
    if(AntRadioStatusChannel(ANT_CHANNEL_USERAPP) == ANT_CLOSED)
    {
        UserApp1_StateMachine = UserApp1SM_Idle;
    }

    /* Check for timeout */
    if( IsTimeUp(&UserApp_u32Timeout, TIMEOUT_VALUE) )
    {
        UserApp1_StateMachine = UserApp1SM_Error;
    }
    
} /* end UserAppSM_WaitChannelClose() */

/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
