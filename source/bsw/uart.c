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

#include "project.h"
#include "uart.h"
#include <stdlib.h> 


/*****************************************************************************/
/* Local pre-processor symbols/macros ('#define')                            */
/*****************************************************************************/

/*****************************************************************************/
/* Global variable definitions (declared in header file with 'extern')       */
/*****************************************************************************/

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
 * Sends and diplays the current round value on the console.
 * @param uint8 round current round value
 * @return RC_SUCCESS if function was executed as exepected, other error code in case of error
 */
RC_t UART_Show_Round_Counter(uint8 round)
{
        char arr[2] ="" ;
        itoa(round,arr,10);
        UART_LOG_Start();
        UART_LOG_PutStringConst(" Reaction test program round ");
        UART_LOG_PutString(arr);
        UART_LOG_PutStringConst("\n");
        return RC_SUCCESS;
}

/**
 * Sends and diplays prompt message on the console.
 * @return RC_SUCCESS if function was executed as exepected, other error code in case of error
 */
RC_t UART_Show_Prompt_Message()
{   
    UART_LOG_Start();
    UART_LOG_PutStringConst(" press one of the two buttons to start... \n \n");
    return RC_SUCCESS;
}

/**
 * Sends and diplays the current score value on the console.
 * @param uint8 score current socre value
 * @return RC_SUCCESS if function was executed as exepected, other error code in case of error
 */
RC_t UART_Show_Score(uint8 score)
{
        uint8 realscore=score;
        char arr[3] ="" ;
        itoa(realscore,arr,10);
        UART_LOG_Start();
        UART_LOG_PutStringConst(" Final Score is: ");
        UART_LOG_PutString(arr);
        UART_LOG_PutStringConst("\n");
        return RC_SUCCESS;
}

/**
 * Sends and diplays the average time value on the console.
 * @param uint8 avgtime Average time of all 10 rounds.
 * @return RC_SUCCESS if function was executed as exepected, other error code in case of error
 */
RC_t UART_Show_Average_Time(uint32 avgtime)
{
        uint32 time=avgtime;
        char arr[20] ="" ;
        itoa(time,arr,10);
        UART_LOG_Start();
        UART_LOG_PutStringConst(" Average time in ms: ");
        UART_LOG_PutString(arr);
        UART_LOG_PutStringConst("\n"); 
        UART_LOG_PutStringConst(" ****************************************************** \n");
        return RC_SUCCESS;
}

/**
 * Sends and diplays each round time after valid button pressed on the console.
 * @param uint8 roundtime current round time value.
 * @return RC_SUCCESS if function was executed as exepected, other error code in case of error
 */
RC_t UART_Show_Round_Time(uint32 roundtime)
{
    uint32 time=roundtime;
        char arr[20] ="" ;
        itoa(time,arr,10);
        UART_LOG_Start();
        UART_LOG_PutStringConst(" Reaction time in ms: ");
        UART_LOG_PutString(arr);
        UART_LOG_PutStringConst("\n \n");
        UART_LOG_PutStringConst(" ======================================================= \n");
        return RC_SUCCESS;
}

/**
 * Sends and diplays the invalid buttom pressed message on the console.
 * @return RC_SUCCESS if function was executed as exepected, other error code in case of error
 */
RC_t UART_Show_Invalid_Button_Pressed()
{
    UART_LOG_Start();
    UART_LOG_PutStringConst(" Oops - incorrect button pressed \n");
    UART_LOG_PutStringConst(" \n");
    UART_LOG_PutStringConst(" ======================================================= \n");
    return RC_SUCCESS;
}

/**
 * Sends and diplays the valid buttom pressed message on the console.
 * @return RC_SUCCESS if function was executed as exepected, other error code in case of error
 */
RC_t UART_Show_Valid_Button_Pressed()
{
    UART_LOG_Start();
    UART_LOG_PutStringConst(" Great - correct button pressed \n");
    return RC_SUCCESS;
}

/**
 * Sends and diplays the too slow message on the console.
 * @return RC_SUCCESS if function was executed as exepected, other error code in case of error
 */
RC_t UART_Show_TooSlow_Message()
{
    UART_LOG_Start();
    UART_LOG_PutStringConst(" Too Slow| \n");
    UART_LOG_PutStringConst(" \n");
    UART_LOG_PutStringConst(" ======================================================= \n");
    return RC_SUCCESS;
}

