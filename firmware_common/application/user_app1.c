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

extern u32 G_u32AntApiCurrentDataTimeStamp;                       /* From ant_api.c */
extern AntApplicationMessageType G_eAntApiCurrentMessageClass;    /* From ant_api.c */
extern u8 G_au8AntApiCurrentMessageBytes[ANT_APPLICATION_MESSAGE_BYTES];  /* From ant_api.c */
extern AntExtendedDataType G_sAntApiCurrentMessageExtData;        /* From ant_api.c */
/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */

static AntAssignChannelInfoType UserApp1_sMasterChannel;
static AntAssignChannelInfoType UserApp1_sSlaveChannel;
static u8 au8Master[20]="Master              ";
static u8 au8Slave[20]="Slave               ";
static u8 au8LCD_Line_1[20]="Master              ";
static u8 au8LCD_Line_2[20]="PRESS B0 TO START   ";
static u8 au8Message3[]="PRESS B0 TO START   ";
static u8 au8Msssage1[]="PRESS B0 TO BE M";
static u8 au8Msssage2[]="PRESS B1 TO BE S";
static s8 s8RssiChannel=-99;
static u8 UserApp1_au8MasterName[9]   = "0\0\0\0\0\0\0\0";
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
    LCDCommand(LCD_CLEAR_CMD);
    
    /*LedOff  WHITE, PURPLE, BLUE, CYAN, GREEN, YELLOW, ORANGE, RED, LCD_RED, LCD_GREEN, LCD_BLUE*/
    LedOff(WHITE);
    LedOff(PURPLE);
    LedOff(BLUE);
    LedOff(CYAN);
    LedOff(GREEN);
    LedOff(YELLOW);
    LedOff(ORANGE);
    LedOff(RED);
    LedOff(LCD_RED);
    LedOff(LCD_GREEN);
    LedOff(LCD_BLUE);
    
    /* For channel 0 , as master ,Configure ANT for this application */
    UserApp1_sMasterChannel.AntChannel          = ANT_CHANNEL0_USERAPP;
    UserApp1_sMasterChannel.AntChannelType      = ANT_CHANNEL0_TYPE_USERAPP;
    UserApp1_sMasterChannel.AntChannelPeriodLo  = ANT_CHANNEL_PERIOD_LO_USERAPP;
    UserApp1_sMasterChannel.AntChannelPeriodHi  = ANT_CHANNEL_PERIOD_HI_USERAPP;

    UserApp1_sMasterChannel.AntDeviceIdLo       = ANT_DEVICEID_LO_USERAPP;
    UserApp1_sMasterChannel.AntDeviceIdHi       = ANT_DEVICEID_HI_USERAPP;
    UserApp1_sMasterChannel.AntDeviceType       = ANT_DEVICE_TYPE_USERAPP;
    UserApp1_sMasterChannel.AntTransmissionType = ANT_TRANSMISSION_TYPE_USERAPP;
    UserApp1_sMasterChannel.AntFrequency        = ANT_FREQUENCY_USERAPP;
    UserApp1_sMasterChannel.AntTxPower          = ANT_TX_POWER_USERAPP;

    UserApp1_sMasterChannel.AntNetwork = ANT_NETWORK_DEFAULT;
    for(u8 i = 0; i < ANT_NETWORK_NUMBER_BYTES; i++)
    {
        UserApp1_sMasterChannel.AntNetworkKey[i] = ANT_DEFAULT_NETWORK_KEY;
    }
    
    /* For channel 1 , as master ,Configure ANT for this application */
    UserApp1_sSlaveChannel.AntChannel          = ANT_CHANNEL1_USERAPP;
    UserApp1_sSlaveChannel.AntChannelType      = ANT_CHANNEL1_TYPE_USERAPP;
    UserApp1_sSlaveChannel.AntChannelPeriodLo  = ANT_CHANNEL_PERIOD_LO_USERAPP;
    UserApp1_sSlaveChannel.AntChannelPeriodHi  = ANT_CHANNEL_PERIOD_HI_USERAPP;

    UserApp1_sSlaveChannel.AntDeviceIdLo       = ANT_DEVICEID_LO_USERAPP;
    UserApp1_sSlaveChannel.AntDeviceIdHi       = ANT_DEVICEID_HI_USERAPP;
    UserApp1_sSlaveChannel.AntDeviceType       = ANT_DEVICE_TYPE_USERAPP;
    UserApp1_sSlaveChannel.AntTransmissionType = ANT_TRANSMISSION_TYPE_USERAPP;
    UserApp1_sSlaveChannel.AntFrequency        = ANT_FREQUENCY_USERAPP;
    UserApp1_sSlaveChannel.AntTxPower          = ANT_TX_POWER_USERAPP;

    UserApp1_sSlaveChannel.AntNetwork = ANT_NETWORK_DEFAULT;
    for(u8 i = 0; i < ANT_NETWORK_NUMBER_BYTES; i++)
    {
        UserApp1_sSlaveChannel.AntNetworkKey[i] = ANT_DEFAULT_NETWORK_KEY;
    }

    /*Master and slaver set OK*/
    
    /* If good initialization, set state to Idle */
    AntAssignChannel(&UserApp1_sMasterChannel);
    
    UserApp1_StateMachine = UserApp1SM_WaitMasterAssign;
    LedOn(GREEN);
    UserApp1_u32Timeout = G_u32SystemTime1ms;
} /* end UserApp1Initialize() */

  
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

void AntGetdBmAscii(s8 s8RssiValue_, u8* pu8Result_)
{
  u8 u8AbsoluteValue;
  
  /* Handle the positive number */
  if(s8RssiValue_ >= 0)
  {
    /* Print '+' but only for numbers larger than 0 */
    *pu8Result_ = '+';
    if(s8RssiValue_ == 0)
    {
      *pu8Result_ = ' ';
    }
    
    u8AbsoluteValue = (u8)s8RssiValue_;
  }
  /* Handle the negative number */
  else
  {
    *pu8Result_ = '-';
    
    /* #EIE Task 2
    s8RssiValue is a two's complement (bu(3) ma(3)) signed value
    Convert to the unsigned absolute value u8AbsoluteValue
    Example:
    s8RssiValue_ = -50
    u8AbsoluteValue = 50
    Hint: what character is used to invert bits in C language?
    */
    u8AbsoluteValue = (u8)(~s8RssiValue_ + 1);
  }
  
  /* Limit any display to two digit */
  if(u8AbsoluteValue > 99)
  {
    u8AbsoluteValue = 99;
  }
  

  /* Write the numeric value */
  pu8Result_++;
  *pu8Result_ = (u8AbsoluteValue / 10) + NUMBER_ASCII_TO_DEC;
  pu8Result_++;
  *pu8Result_ = (u8AbsoluteValue % 10) + NUMBER_ASCII_TO_DEC;
  
} /* end AntGetdBmAscii() */
/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */



static void UserApp1SM_Idle(void)
{
    if(WasButtonPressed(BUTTON0))
    {   
        ButtonAcknowledge(BUTTON0);
        
        for(u8 i = 0 ; i<LCD_DISPLAY_LENGTH ; i++)
        {
            au8LCD_Line_1[i]=au8Master[i];
            au8LCD_Line_2[i]=au8Message3[i];
        }
        
        AntOpenChannelNumber(ANT_CHANNEL0_USERAPP);
        LCDMessage(LINE1_START_ADDR, au8LCD_Line_1);
        LCDMessage(LINE2_START_ADDR, au8LCD_Line_2);
        LedOn(LCD_GREEN);
        LedOn(LCD_RED);
        UserApp1_StateMachine = UserApp1SM_ChoiceWaiting;
    }
    
    else if(WasButtonPressed(BUTTON1))
    {   
        ButtonAcknowledge(BUTTON1);
        for(u8 i = 0 ; i<LCD_DISPLAY_LENGTH ; i++)
        {
            au8LCD_Line_1[i]=au8Slave[i];
            au8LCD_Line_2[i]=au8Message3[i];
        }
        
        AntOpenChannelNumber(ANT_CHANNEL1_USERAPP);
        LCDMessage(LINE1_START_ADDR, au8LCD_Line_1);
        LCDMessage(LINE2_START_ADDR, au8LCD_Line_2);
        LedOn(LCD_BLUE);
        
        UserApp1_StateMachine = UserApp1SM_ChoiceWaiting;
    }
    
    UserApp1_u32Timeout = G_u32SystemTime1ms;
} /* end UserApp1SM_Idle() */

static void UserApp1SM_ChoiceWaiting(void)
{
    if(WasButtonPressed(BUTTON0))
    {   
        ButtonAcknowledge(BUTTON0);
        UserApp1_StateMachine = UserApp1SM_CountBackwards;
        LCDMessage(LINE2_START_ADDR, "                    ");
    }
    
}/* end UserApp1SM_ChoiceWaiting() */

static void UserApp1SM_CountBackwards(void)
{
    static s8 s8Count=10;
    static u16 u16Count=0;
    u16Count++;
    
    if(u16Count==1000)
    {
        u16Count=0;
        
        for(u8 i = 0 ; i<LCD_DISPLAY_LENGTH; i++)
        {
            au8LCD_Line_2[i]='\0';
        }
        
        au8LCD_Line_2[0]=s8Count+48;
        
        if(s8Count==10)
        {
            for(u8 i = 0 ; i<LCD_DISPLAY_LENGTH; i++)
            {
                au8LCD_Line_2[i]='\0';
            }
            
            au8LCD_Line_2[0]='1';
            au8LCD_Line_2[1]='0';
        }
        
        if(s8Count==9)
        {
            LCDMessage(LINE2_START_ADDR, "                    ");
        }
        LCDMessage(LINE2_START_ADDR, au8LCD_Line_2);
        s8Count--;
    }
    
    if(s8Count==-1)
    {
        UserApp1_StateMachine = UserApp1SM_WaitChannelOpen;
        s8Count=10;
        u16Count=0;
        UserApp1_u32Timeout = G_u32SystemTime1ms;
        
        for(u8 i = 0 ; i<LCD_DISPLAY_LENGTH; i++)
        {
            au8LCD_Line_2[i]='\0';
        }
      
        LCDMessage(LINE2_START_ADDR, "                     ");
    }
    
    UserApp1_u32Timeout = G_u32SystemTime1ms;
} /* end UserApp1SM_CountBackwards() */


static void UserApp1SM_RadioOpening(void)
{
    LedNumberType aeLedDisplayLevels[] = {RED, ORANGE, YELLOW, GREEN, CYAN, BLUE, PURPLE, WHITE};
    s8 as8dBmLevels[] = {DBM_LEVEL1, DBM_LEVEL2, DBM_LEVEL3, DBM_LEVEL4, 
                       DBM_LEVEL5, DBM_LEVEL6, DBM_LEVEL7, DBM_LEVEL8};
    static s8 s8StrongestRssi = -99;

    /*Master*/
    if( AntRadioStatusChannel(ANT_CHANNEL0_USERAPP) == ANT_OPEN ) 
    {
        if( AntReadAppMessageBuffer() )
        {
            if(G_eAntApiCurrentMessageClass == ANT_DATA)
            {
                
            }

            else if(G_eAntApiCurrentMessageClass == ANT_TICK)
            {
                AntQueueBroadcastMessage(ANT_CHANNEL_0,&UserApp1_au8MasterName[0]);
            }
            
            LCDMessage(LINE2_START_ADDR, au8LCD_Line_2);
        }
    }
    
    /*Slaver*/
    if( AntRadioStatusChannel(ANT_CHANNEL1_USERAPP) == ANT_OPEN ) 
    {
        if( AntReadAppMessageBuffer() )
        {
            if(G_eAntApiCurrentMessageClass == ANT_DATA)
            {
                s8RssiChannel = G_sAntApiCurrentMessageExtData.s8RSSI;
                AntGetdBmAscii(s8RssiChannel,&au8LCD_Line_2[0]);
                LCDClearChars(LINE2_START_ADDR, 2);
                LCDMessage(LINE2_START_ADDR, au8LCD_Line_2);
            }

            else if(G_eAntApiCurrentMessageClass == ANT_TICK)
            {
                
            }
            
            s8StrongestRssi = s8RssiChannel;
            if(s8RssiChannel > s8RssiChannel)
            {
                s8StrongestRssi = s8RssiChannel;
            }

            /* Loop through all of the levels to check which LEDs to turn on */
            for(u8 i = 0; i < NUM_DBM_LEVELS; i++)
            {
                if(s8StrongestRssi > as8dBmLevels[i])
                {
                    LedOn(aeLedDisplayLevels[i]);
                }
                else
                {
                    LedOff(aeLedDisplayLevels[i]);
                }
            }
            
        }
        if(s8StrongestRssi > DBM_MAX_LEVEL)
        {
            UserApp1_StateMachine = UserApp1SM_Finish;
            LCDClearChars(LINE2_START_ADDR, 8);
            LCDMessage(LINE2_START_ADDR, "Find you");
            LedOff(WHITE);
            LedOff(PURPLE);
            LedOff(BLUE);
            LedOff(CYAN);
            LedOff(GREEN);
            LedOff(YELLOW);
            LedOff(ORANGE);
            LedOff(RED);
            LedOn(WHITE);
        }
    }
    
} /* end UserApp1SM_RadioOpening() */

static void UserApp1SM_Finish(void)
{
    
}
static void UserApp1SM_WaitChannelOpen(void)
{
    if( AntRadioStatusChannel(ANT_CHANNEL0_USERAPP) == ANT_OPEN ) 
    {
        UserApp1_StateMachine = UserApp1SM_RadioOpening;
    }
  
    if( AntRadioStatusChannel(ANT_CHANNEL1_USERAPP) == ANT_OPEN ) 
    {
        UserApp1_StateMachine = UserApp1SM_RadioOpening;
        
    }
    /* Check for timeout */
    if( IsTimeUp(&UserApp1_u32Timeout, 5000) )
    {
        AntCloseChannelNumber(ANT_CHANNEL0_USERAPP);
        AntCloseChannelNumber(ANT_CHANNEL1_USERAPP);
        LedOn(RED);
        LedOff(GREEN);
        LCDMessage(LINE1_START_ADDR, "OPEN CHANNEL ERROR");
        LCDMessage(LINE2_START_ADDR, "PRESS B0 TO RETRY");
        UserApp1_StateMachine = UserApp1SM_Error;
    }
    
} /* end UserApp1SM_WaitChannelOpen() */

static void UserApp1SM_WaitMasterAssign(void)
{
  /* Check if the channel assignment is complete */
  if(AntRadioStatusChannel(ANT_CHANNEL0_USERAPP) == ANT_CONFIGURED)
  {
    UserApp1_StateMachine = UserApp1SM_WaitSlaveAssign;
    UserApp1_u32Timeout = G_u32SystemTime1ms;
    AntAssignChannel(&UserApp1_sSlaveChannel);
  }
  
  if( IsTimeUp(&UserApp1_u32Timeout, 5000) )
  {
    DebugPrintf("\n\r***Channel assignment timeout***\n\n\r");
    LedOn(RED);
    UserApp1_StateMachine = UserApp1SM_Error;
  }
} /* end UserApp1SM_WaitMasterAssign() */


static void UserApp1SM_WaitSlaveAssign(void)
{
  /* Check if the channel assignment is complete */
  if(AntRadioStatusChannel(ANT_CHANNEL1_USERAPP) == ANT_CONFIGURED)
  {
    UserApp1_StateMachine = UserApp1SM_Idle;
    LedOff(GREEN);
    
    for(u8 i = 0 ; i<LCD_DISPLAY_LENGTH; i++)
    {
        au8LCD_Line_2[i]='\0';
        au8LCD_Line_1[i]='\0';
        au8LCD_Line_1[i]=au8Msssage1[i];
        au8LCD_Line_2[i]=au8Msssage2[i];
    }
    
    LCDMessage(LINE1_START_ADDR, au8LCD_Line_1);
    LCDMessage(LINE2_START_ADDR, au8LCD_Line_2);
  }
  
  if( IsTimeUp(&UserApp1_u32Timeout, 5000) )
  {
    DebugPrintf("\n\r***Channel assignment timeout***\n\n\r");
    LedOn(RED);
    UserApp1_StateMachine = UserApp1SM_Error;
  }
} /* end UserApp1SM_WaitSlaveAssign() */
/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{  
    
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
