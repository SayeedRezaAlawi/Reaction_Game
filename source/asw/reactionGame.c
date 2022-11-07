/**
* \file <filename>
* \author <author-name>
* \date <date>
*
* \brief <Symbolic File name>
*
* \copyright Copyright ©2016
* Department of electrical engineering and information technology, Hochschule Darmstadt - University of applied sciences (h_da). All Rights Reserved.
* Permission to use, copy, modify, and distribute this software and its documentation for educational, and research purposes in the context of non-commercial
* (unless permitted by h_da) and official h_da projects, is hereby granted for enrolled students of h_da, provided that the above copyright notice,
* this paragraph and the following paragraph appear in all copies, modifications, and distributions.
* Contact Prof.Dr.-Ing. Peter Fromm, peter.fromm@h-da.de, Birkenweg 8 64295 Darmstadt - GERMANY for commercial requests.
*
* \warning This software is a PROTOTYPE version and is not designed or intended for use in production, especially not for safety-critical applications!
* The user represents and warrants that it will NOT use or redistribute the Software for such purposes.
* This prototype is for research purposes only. This software is provided "AS IS," without a warranty of any kind.
*/

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#include "reactionGame.h"
#include "seven.h"
#include "led.h"
#include "uart.h"
#include <stdlib.h> 


/*****************************************************************************/
/* Local pre-processor symbols/macros ('#define')                            */
/*****************************************************************************/

/*****************************************************************************/
/* Global variable definitions (declared in header file with 'extern')       */
/*****************************************************************************/
static int8 Score=0;
uint32 Time[10]={0};
BUTTON_id_t RandomButton;
BUTTON_id_t PressedButton;
eGame_s_t GameState=Is_Initial;
static uint8 Round=0;
uint8 MaxRound=10;

/*****************************************************************************/
/* Local type definitions ('typedef')                                        */
/*****************************************************************************/

/*****************************************************************************/
/* Local variable definitions ('static')                                     */
/*****************************************************************************/



/*****************************************************************************/
/* Local function prototypes ('static')                                      */
/*****************************************************************************/


/*****************************************************************************/
/* Function implementation - global ('extern') and local ('static')          */
/*****************************************************************************/

/**
 * Reaction Game Initialisation
 * @return RC_SUCCESS
 */

RC_t REACTIONGAME_Init()
{
    Score=0;
    Time[10]=0;
    Round=0;
    MaxRound=10;
    REACTIONGAME_Round_Increament();
    return RC_SUCCESS;
}

/**
 * Run the reaction Game according to event.
 * @param Game_event_t game_event   - type of event
 * @return RC_SUCCESS
 */
RC_t REACTIONGAME_Run(Game_event_t game_event)
{
     switch(GameState)
        {
            case Is_Initial: if(game_event == ev_button)
            {
                REACTIONGAME_CreateRandomTime();
                uint32 randtime=REACTIONGAME_RandTime();
                SetRelAlarm(Rand_Time,randtime,0);
                GameState=Is_WaitRandTime;
                
            };break;
            case Is_WaitRandTime:if(game_event == ev_timeout)
            {
                REACTIONGAME_RandButtonSelect();
                GameState=Is_ButtonPressed;
            };break;
            case Is_ButtonPressed:
            if(game_event == ev_button)
            {
                if(REACTIONGAME_CheckGameEnd() == TRUE)
                {
                    GameState=Is_GameEnd;
                    SEVEN_Clear(SEVEN_0);
                    SEVEN_Clear(SEVEN_1);
                    REACTIONGAME_ShowScore();
                    REACTIONGAME_ShowAvgTime();
                }
                else
                { 
                    REACTIONGAME_CheckValidButton();
                    GameState=Is_Initial;
                    SEVEN_Clear(SEVEN_0);
                    SEVEN_Clear(SEVEN_1);  
                }
                
            }
            if(game_event == ev_tooslow)
            {
                if(REACTIONGAME_CheckGameEnd() == TRUE)
                {
                    GameState=Is_GameEnd;
                    SEVEN_Clear(SEVEN_0);
                    SEVEN_Clear(SEVEN_1);
                    REACTIONGAME_ShowScore();
                    REACTIONGAME_ShowAvgTime();

                }
                else
                {
                    GameState=Is_Initial;
                    SEVEN_Clear(SEVEN_0);
                    SEVEN_Clear(SEVEN_1);
                    REACTIONGAME_TooSlowPressed();
                }
            };break;

             case Is_GameEnd:
                ;break;
        }
        return RC_SUCCESS;
}

/**
 * Increament the round turn.
 * @return Round
 */
uint8 REACTIONGAME_Round_Increament()
{
    Round++;
    if(Round <= MaxRound)
    {
    UART_Show_Round_Counter(Round);
    UART_Show_Prompt_Message();
    return Round;
    }
    return Round;
}

/**
 * Generats random time and enbles the ev_TimeOut event.
 * @return random number between 1 to 3 seconds.
 */
uint32 REACTIONGAME_RandTime()
{
    uint32 RandomTim=(rand()%3000)+1;
    return RandomTim;
}

/**
 * Generats random number of button and display on seven segment.
 * @return RC_SUCCESS
 */
RC_t REACTIONGAME_RandButtonSelect()
{
    SEVEN_id_t seven = SEVEN_0;
    uint8 RandButton=(rand()%2)+1;
     switch (RandButton)
    {
        case 1 : 
            RandomButton = BUTTON_1;
            seven = SEVEN_0;
            SEVEN_Set(seven,RandButton);
            seven = SEVEN_1;
            SEVEN_Set(seven,RandButton);
            break;
        case 2 : 
            RandomButton = BUTTON_2;
            seven = SEVEN_0;
            SEVEN_Set(seven,RandButton);
            seven = SEVEN_1;
            SEVEN_Set(seven,RandButton);
            break;
    }
    REACTIONGAME_CounterTicks_Reset();
    return RC_SUCCESS;
}

/**
 * Checks if the valid button is pressed.
 * @return RC_SUCCESS
 */
RC_t REACTIONGAME_CheckValidButton()
{
    uint32 roundtime=REACTIONGAME_CounterTicks_Read();
    if(PressedButton == RandomButton)
    {
        UART_Show_Valid_Button_Pressed();
        UART_Show_Round_Time(roundtime); 
        Time[Round]=roundtime;
        REACTIONGAME_IncreaseScore(); 
    }
    else
    {
        UART_Show_Invalid_Button_Pressed();
        Time[Round]=0;
        REACTIONGAME_DecreaseScore();
    }
    return RC_SUCCESS;
}

/**
 * Checks if player is too slow.
 * @return RC_SUCCESS
 */
RC_t REACTIONGAME_TooSlowPressed()
{
       UART_Show_TooSlow_Message();
       if(Score ==0)
        {
            Score=Score;
            Time[Round]=1000;
        }
       else
        {
          Score--;  
          Time[Round]=1000;
        }
    REACTIONGAME_Round_Increament();
    return RC_SUCCESS;
}

/**
 * Display the score on the console.
 * @return RC_SUCCESS
 */
RC_t REACTIONGAME_ShowScore()
{
    UART_Show_Score(Score);
    return RC_SUCCESS;
}

/**
 * Display average time after 10 rounds.
 * @return RC_SUCCESS
 */
RC_t REACTIONGAME_ShowAvgTime()
{
    uint32 sum=0;
    uint32 average=0;
    for(uint8 i = 0; i < MaxRound; i++)
    {
        sum += Time[i];
    }
    average = sum / MaxRound;
    UART_Show_Average_Time(average);
    return RC_SUCCESS;
}

/**
 * Increament the score.
 * @return RC_SUCCESS
 */
RC_t REACTIONGAME_IncreaseScore()
{
    Score++;
    REACTIONGAME_Round_Increament();
    return RC_SUCCESS;
}

/**
 * Decreament the score.
 * @return RC_SUCCESS
 */
RC_t REACTIONGAME_DecreaseScore()
{
    
    if(Score ==0)
    {
        Score=Score;
    }
    else
    {
      Score--;  
    }
    REACTIONGAME_Round_Increament();
    return RC_SUCCESS;
}

/**
 * Checks if the game is ended.
 * @return TRUE or FALSE
 */
boolean_t REACTIONGAME_CheckGameEnd()
{
    if (Round <= MaxRound )
    {
        return FALSE;
    }
    return TRUE;
}

/**
 * 1 ms Counter.
 * @return current counter value.
 */
uint32 REACTIONGAME_CounterTicks(Ticks_s_t ticks)
{
    static uint32 timer_ticks=0;
    switch(ticks)
    {
        case ReadTicks: return timer_ticks;break;
        case ResetTicks: timer_ticks=0;return timer_ticks;break;
        case IncreaseTicks:timer_ticks++;
        if(timer_ticks >1000)
        {
            SetEvent(tsk_Control, ev_TooSlow);
        };return timer_ticks;break; 
    }
    return timer_ticks;
}

/**
 * Reads the counter value.
 * @return current counter value.
 */
uint32 REACTIONGAME_CounterTicks_Read()
{
    return REACTIONGAME_CounterTicks(ReadTicks);
}

/**
 * Resets the counter value.
 * @return RC_SUCCESS
 */
RC_t REACTIONGAME_CounterTicks_Reset()
{
    REACTIONGAME_CounterTicks(ResetTicks);
    return RC_SUCCESS;
}

/**
 * Increament the counter value.
 * @return RC_SUCCESS
 */
RC_t REACTIONGAME_CounterTicks_Increase()
{
    REACTIONGAME_CounterTicks(IncreaseTicks);
    return RC_SUCCESS;
}
