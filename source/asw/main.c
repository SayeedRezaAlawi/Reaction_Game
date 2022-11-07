/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include "global.h"
#include "reactionGame.h"
#include "button.h"
#include "led.h"
#include "seven.h"



//ISR which will increment the systick counter every ms
ISR(systick_handler)
{
    CounterTick(cnt_systick); //application counter
}

int main()
{
    CyGlobalIntEnable; /* Enable global interrupts. */
   
    //Set systick period to 1 ms. Enable the INT and start it.
	EE_systick_set_period(MILLISECONDS_TO_TICKS(1, BCLK__BUS_CLK__HZ));
	EE_systick_enable_int();
	EE_systick_start();
   
    // Start Operating System
    StartOS(OSDEFAULTAPPMODE);
    for(;;)
    {
        asm("bkpt");   
    }

}

void unhandledException()
{
    //Ooops, something terrible happened....check the call stack to see how we got here...
    asm("bkpt");
}

/********************************************************************************
 * Task Definitions
 ********************************************************************************/

TASK(tsk_Init)
{
    
    //Init MCAL Drivers
    LED_Init();
    SEVEN_Init();
    
    //Init Complex Drivers and Applications
    REACTIONGAME_Init(0);
    
    //Reconfigure ISRs with OS parameters.
    //This line MUST be called after the hardware driver initialisation!
    EE_system_init();
    
    //Start the alarm with 100ms cycle time
    SetRelAlarm(alrm_1ms,1,1);
    SetRelAlarm(alrm_Glowingtime,1,0);
    ALARMCALLBACK(Counter_Ticks);
    SetRelAlarm(alrm_10ms,10,10);
    
    
    //Activate the remaining tasks
    ActivateTask(tsk_Control);
    ActivateTask(tsk_Background);
    
    TerminateTask();
    
}




/**
 * This task will c heck the input and based on this, "control" the output
 */

TASK(tsk_Control)
{
    
    EventMaskType ev = 0;
    while(1)
    {
        //Wait, read and clear the event
        WaitEvent(ev_Button | ev_TimeOut | ev_TooSlow);
        GetEvent(tsk_Control,&ev);
        ClearEvent(ev);

            if (ev & ev_Button)
            {
                REACTIONGAME_Run(ev_button);
            }
            
            if (ev & ev_TooSlow)
            {
                REACTIONGAME_Run(ev_tooslow);
            }
            
            if (ev & ev_TimeOut)
            {
                REACTIONGAME_Run(ev_timeout);
            }
    
    }
}

/**
 * This task will c heck the input and based on this, "control" the output
 */

TASK(tsk_LEDs_Fading)
{
    LED_fading();
    
    TerminateTask();
}



TASK(tsk_RGB_Glowing)
{
    
    //called every 10ms or so
    static uint32 tick = 0;
    tick++;
    LED_fader(tick);
    LED_Glower(tick);
    
    static uint8 glowing_seq=0;
    if(glowing_seq > 8)
    {
        glowing_seq=0;
        LED_RGB_Set(glowing_seq);
    }
    else
    {
        LED_RGB_Set(glowing_seq);
        glowing_seq++;
    }
    
    
    TerminateTask();
}


TASK(tsk_Background)
{
    while(1)
    {
        //do something with low prioroty
        asm("nop");
    }
}

ALARMCALLBACK (Counter_Ticks)
{
    REACTIONGAME_CounterTicks(IncreaseTicks);
}


/********************************************************************************
 * ISR Definitions
 ********************************************************************************/
    
ISR2(isr_Button)
{
    isr_Button_ClearPending();      
    if(Buttons_Status_Read() == 1)
    {
        PressedButton= BUTTON_1;
    }
    else
    {
       PressedButton= BUTTON_2; 
    }
    if (TRUE == BUTTON_IsPressed(BUTTON_1)) SetEvent(tsk_Control, ev_Button);
    if (TRUE == BUTTON_IsPressed(BUTTON_2)) SetEvent(tsk_Control, ev_Button);
}
/* [] END OF FILE */

























//TASK(tsk_Control)
//{
//    
//    EventMaskType ev = 0;
//    while(1)
//    {
//        //Wait, read and clear the event
////        | ev_TooSlow
//        WaitEvent(ev_Button | ev_RandTimeOut  | ev_EndGame);
//        GetEvent(tsk_Control,&ev);
//        ClearEvent(ev);
//        
//
//        switch(GameState)
//        {
//            case GameInitial: if (ev & ev_Button)
//            {
////                REACTIONGAME_Init();
//                uint32 randtime=REACTIONGAME_RandTime();
//                SetRelAlarm(Rand_Time,randtime,1);
//                GameState=GameRandTime;
//                
//            };break;
//            case GameRandTime: if (ev & ev_RandTimeOut)
//            {
//                REACTIONGAME_RandButtonSelect();
//                GameState=GamePressButton;
//            };break;
//            case GamePressButton: 
//            if(ev & ev_Button)
//            {
//                REACTIONGAME_CheckValidButton();
//                if(REACTIONGAME_CheckGameEnd() == TRUE)
//                {
//                    GameState=GameEnd;
//                    SEVEN_Clear(SEVEN_0);
//                    SEVEN_Clear(SEVEN_1);
//                    SetEvent(tsk_Control, ev_EndGame);
//                }
//                else
//                {   
//                    REACTIONGAME_ShowScore();
//                    GameState=GameInitial;
//                    SEVEN_Clear(SEVEN_0);
//                    SEVEN_Clear(SEVEN_1);
//                }
//                
//            }
//            if(REACTIONGAME_CounterTicks_Read() > 999)
//            {
//                GameState=GameInitial;
//                SEVEN_Clear(SEVEN_0);
//                SEVEN_Clear(SEVEN_1);
//                REACTIONGAME_TooSlowPressed();
////                SetEvent(tsk_Control, ev_TooSlow);
//            };break;
//             case GameEnd:if (ev & ev_EndGame)
//            {///////////////////I have to check why program is not ending after round max ended.
//                REACTIONGAME_ShowScore();
//                REACTIONGAME_ShowAvgTime();
////                REACTIONGAME_Init();
//                
//            };break;
//        }
//    
//    }
//}

































//TASK(tsk_Control)
//{
//    
//    EventMaskType ev = 0;
//    while(1)
//    {
//        //Wait, read and clear the event
//        WaitEvent(ev_Button | ev_RandTimeOut | ev_TooSlow | ev_EndGame);
//        GetEvent(tsk_Control,&ev);
//        ClearEvent(ev);
//
//            if (ev & ev_Button)
//            {
//                REACTIONGAME_Run(ev_button);
//            }
//            if (ev & ev_RandTimeOut)
//            {
//                REACTIONGAME_Run(ev_timeout);
//            }
////            if (ev & ev_TooSlow)
////            {
////                REACTIONGAME_Run(ev_endgame);
////            }
//            if (ev & ev_EndGame)
//            {
//                REACTIONGAME_Run(ev_endgame);
//            }
//    
//    }
//}
