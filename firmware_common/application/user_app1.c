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
  
  PWMAudioSetFrequency(BUZZER1, BUZZER1_FREQUENCE);
  
  LedOff(LCD_RED);
  LedOff(LCD_BLUE);
  LedOff(LCD_GREEN);
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
void AntGetdBmAscii(s8 s8RssiValue_, u8* pu8Result_)
{
    u8 u8AbsoluteValue;

    u8AbsoluteValue = (u8)s8RssiValue_;
    /* Write the numeric value */
    *pu8Result_ = (u8AbsoluteValue / 100) + NUMBER_ASCII_TO_DEC;
    pu8Result_++;
    *pu8Result_ = ((u8AbsoluteValue % 100)/10) + NUMBER_ASCII_TO_DEC;
    pu8Result_++;
    *pu8Result_ = (u8AbsoluteValue % 10) + NUMBER_ASCII_TO_DEC;
  
    if(*(pu8Result_-2)=='0')
    {
        *(pu8Result_-2)=*(pu8Result_-1);
        *(pu8Result_-1)=*(pu8Result_);
        *pu8Result_='\0';
    }
} /* end AntGetdBmAscii() */

void ASCIIHEX(u8 u8Num ,u8* pu8Result_)
{   
    
}
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
        LCDCommand(LCD_CLEAR_CMD);
        LCDMessage(LINE1_START_ADDR, au8WelcomeMessage); 
        LCDMessage(LINE2_START_ADDR, au8Instructions);  
        UserApp1_StateMachine = UserAppSM_WaitChannelOpen;
        AntOpenChannelNumber(ANT_CHANNEL_USERAPP);
        UserApp_u32Timeout = G_u32SystemTime1ms;
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
    static u8 au8NowASCII[10];
    static u8 au8List[100][3];
    static u8 u8Display=0;
    static u8 au8Display[20]="PresentHeartbeat:";
    static u8 au8Line1Num[3];
    static u8 u8NowHeartBeat=0;
    static bool bBuzzer=TRUE;
    u8 i=0,j=0;
    static bool bNewMessage=FALSE;
    static bool bWatchHistory=FALSE;
    
    if(WasButtonPressed(BUTTON0))
    {
        ButtonAcknowledge(BUTTON0);
        bWatchHistory=TRUE;
        u8Display++;
        
    }

    if(WasButtonPressed(BUTTON1))
    {
        ButtonAcknowledge(BUTTON1);
        bWatchHistory=TRUE;
        if(u8Display!=0)
        {
          u8Display--;
        }
    }
    
    if(WasButtonPressed(BUTTON2))
    {
        ButtonAcknowledge(BUTTON2);
        if(bBuzzer)
        {
            bBuzzer=FALSE;
            PWMAudioOff(BUZZER1);
        }
        else
        {
            bBuzzer=TRUE;
        }
        
    }
    if(WasButtonPressed(BUTTON3))
    {
        ButtonAcknowledge(BUTTON3);
        bWatchHistory=FALSE;
        u8Display=0;
    }
    
    if( AntReadAppMessageBuffer() )
    {
        /* New data message: check what it is */
        if(G_eAntApiCurrentMessageClass == ANT_DATA)
        {
            LedOn(LCD_BLUE);
            AntGetdBmAscii(G_au8AntApiCurrentMessageBytes[7],&au8NowASCII[0]);
            for(i=0;i<3;i++)
            {
                if(au8List[0][i]!=au8NowASCII[i])
                {
                    bNewMessage=TRUE;
                    break;
                }
            }
            if(bNewMessage)
            {
                
                for(j=99 ; j>0 ; j--)
                {
                    for(i=0;i<3;i++)
                    {
                        au8List[j][i]=0;
                        au8List[j][i]=au8List[j-1][i];
                    }
                }
                
                for(i=0;i<3;i++)
                {
                    au8List[0][i]=au8NowASCII[i];
                }
                
                bNewMessage=FALSE;
                if(bWatchHistory)
                {
                    u8Display++;
                    if(u8Display==99)
                    {
                        u8Display=99;
                    }
                    for(i=0;i<3;i++)
                    {
                        au8Line1Num[i]=au8List[u8Display][i];
                    }
                    
                    if(au8Line1Num[0]=='\0')
                    {
                        au8Line1Num[0]='T';
                        au8Line1Num[1]='O';
                        au8Line1Num[2]='P';
                    }
                }
                
                LCDCommand(LCD_CLEAR_CMD);
                LCDMessage(LINE2_START_ADDR,"History:");
                LCDMessage(LINE1_START_ADDR,au8Display);
                LCDMessage(LINE1_START_ADDR+17,au8NowASCII);
            }
            
            
            if(!bWatchHistory)
            {
                for(i=0;i<3;i++)
                {
                    au8Line1Num[i]=au8NowASCII[i];
                }
            }
            
            if(au8List[0][2]=='\0')
            {
                u8NowHeartBeat=(au8List[0][0]-NUMBER_ASCII_TO_DEC)*10+au8List[0][1]-NUMBER_ASCII_TO_DEC;
            }
            else  
            {
                u8NowHeartBeat=(au8List[0][0]-NUMBER_ASCII_TO_DEC)*100+(au8List[0][1]-NUMBER_ASCII_TO_DEC)*10+(au8List[0][2]-NUMBER_ASCII_TO_DEC);
            }
            
            if(u8NowHeartBeat>120)
            {
                LedOff(LCD_GREEN);
                LedOff(LCD_BLUE);
                LedOn(LCD_RED);
                if(bBuzzer==TRUE)
                {
                    PWMAudioOn(BUZZER1);
                }
            }
            else if(u8NowHeartBeat<=120&&u8NowHeartBeat>=85)
            {
                LedOn(LCD_RED);
                LedOff(LCD_GREEN);
                LedOn(LCD_BLUE);
                bBuzzer=TRUE;
            }
            else if(u8NowHeartBeat<85&&u8NowHeartBeat>=60)
            {
                LedOn(LCD_GREEN);
                LedOff(LCD_RED);
                LedOff(LCD_BLUE);
            }
            else if(u8NowHeartBeat<60)
            {
                LedOff(LCD_RED);
                LedOn(LCD_BLUE);
                LedOff(LCD_GREEN);
            }
            LCDMessage(LINE2_START_ADDR+17,au8Line1Num);
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
