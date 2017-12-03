/**********************************************************************************************************************
File: user_app1.h                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
1. Follow the instructions at the top of user_app1.c
2. Use ctrl-h to find and replace all instances of "user_app1" with "yournewtaskname"
3. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
4. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
5. Add #include yournewtaskname.h" to configuration.h
6. Add/update any special configurations required in configuration.h (e.g. peripheral assignment and setup values)
7. Delete this text (between the dashed lines)
----------------------------------------------------------------------------------------------------------------------

Description:
Header file for user_app1.c

**********************************************************************************************************************/

#ifndef __USER_APP1_H
#define __USER_APP1_H

/**********************************************************************************************************************
Type Definitions
**********************************************************************************************************************/


/**********************************************************************************************************************
Constants / Definitions
**********************************************************************************************************************/
#define ANT_CHANNEL_USERAPP             ANT_CHANNEL_0         
#define ANT_CHANNEL_TYPE_USERAPP        CHANNEL_TYPE_SLAVE    
#define ANT_DEVICEID_LO_USERAPP         (u8)0x42               
#define ANT_DEVICEID_HI_USERAPP         (u8)0xA4        
#define ANT_DEVICE_TYPE_USERAPP         (u8)120        
#define ANT_TRANSMISSION_TYPE_USERAPP   (u8)0            
#define ANT_CHANNEL_PERIOD_LO_USERAPP   (u8)0x86       
#define ANT_CHANNEL_PERIOD_HI_USERAPP   (u8)0x1F      
#define ANT_FREQUENCY_USERAPP           (u8)57           
#define ANT_TX_POWER_USERAPP            (u8)RADIO_TX_POWER_4DBM 

#define TIMEOUT_VALUE                   30000

#define ARRAY_LENGTH                    20

#define BUZZER1_FREQUENCE               300
/**********************************************************************************************************************
Function Declarations
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/
void UserApp1Initialize(void);
void UserApp1RunActiveState(void);
void UserApp1SM_WaitChannelAssign(void);  
void UserAppSM_ChannelOpen(void);
void UserAppSM_WaitChannelOpen(void);
/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/
void AntGetdBmAscii(s8 s8RssiValue_, u8* pu8Result_);
void CombineArray( u8 au8Array1[ARRAY_LENGTH],u8 au8Array2[ARRAY_LENGTH] , u8 au8Array3[ARRAY_LENGTH] );
void ASCIIHEX(u8 u8Num,u8* pu8Result_);
/***********************************************************************************************************************
State Machine Declarations
***********************************************************************************************************************/
static void UserApp1SM_Idle(void);    

static void UserApp1SM_Error(void);         


#endif /* __USER_APP1_H */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
