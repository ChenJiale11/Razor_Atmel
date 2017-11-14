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
/* Required constants for ANT channel configuration */
#define ANT_CHANNEL0_USERAPP             ANT_CHANNEL_0         /* Channel 0 - 7 */
#define ANT_CHANNEL1_USERAPP             ANT_CHANNEL_1
#define ANT_CHANNEL0_TYPE_USERAPP        CHANNEL_TYPE_MASTER    /* ANT MASTER */
#define ANT_CHANNEL1_TYPE_USERAPP        CHANNEL_TYPE_SLAVE    /* ANT SLAVE */
#define ANT_DEVICEID_LO_USERAPP         (u8)01                 /* Low byte of two-byte Device # */
#define ANT_DEVICEID_HI_USERAPP         (u8)0x12                 /* High byte of two-byte Device # */
#define ANT_DEVICE_TYPE_USERAPP         (u8)1                 /* 1 - 255 */
#define ANT_TRANSMISSION_TYPE_USERAPP   (u8)1                 /* 1-127 (MSB is pairing bit) */
#define ANT_CHANNEL_PERIOD_LO_USERAPP   (u8)0x00              /* Low byte of two-byte channel period 0x0001 - 0x7fff */
#define ANT_CHANNEL_PERIOD_HI_USERAPP   (u8)0x20              /* High byte of two-byte channel period */
#define ANT_FREQUENCY_USERAPP           (u8)50                /* 2400MHz + this number 0 - 99 */
#define ANT_TX_POWER_USERAPP            RADIO_TX_POWER_4DBM   /* RADIO_TX_POWER_0DBM, RADIO_TX_POWER_MINUS5DBM, RADIO_TX_POWER_MINUS10DBM, RADIO_TX_POWER_MINUS20DBM */

#define LCD_DISPLAY_LENGTH              (u8)20

#define DBM_LEVEL1          (s8)-99
#define DBM_LEVEL2          (s8)-84
#define DBM_LEVEL3          (s8)-76
#define DBM_LEVEL4          (s8)-69
#define DBM_LEVEL5          (s8)-63
#define DBM_LEVEL6          (s8)-58
#define DBM_LEVEL7          (s8)-54
#define DBM_LEVEL8          (s8)-51
#define DBM_MAX_LEVEL       DBM_LEVEL8
#define NUM_DBM_LEVELS      (u8)8
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
void AntGetdBmAscii(s8 s8RssiValue_, u8* pu8Result_);

/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/

/***********************************************************************************************************************
State Machine Declarations
***********************************************************************************************************************/
static void UserApp1SM_Idle(void);    
static void UserApp1SM_ChoiceWaiting(void);
static void UserApp1SM_CountBackwards(void);
static void UserApp1SM_Error(void);         
static void UserApp1SM_WaitSlaveAssign(void);
static void UserApp1SM_WaitMasterAssign(void);
static void UserApp1SM_WaitChannelOpen(void);
static void UserApp1SM_RadioOpening(void);
static void UserApp1SM_Wait(void);
static void UserApp1SM_WaitChannelClose(void);

#endif /* __USER_APP1_H */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
