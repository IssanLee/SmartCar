/***************************************************************************
* File:         LED.c
* Summary:      LED
* Version:      V0.1
* Author:       Leopold
* Date:         2019/1/27
* Change Logs:
*       Date:   2019/1/27          
*       Version:V0.1         
*       Author: Leopold       
*       Notes:  first version                                        
***************************************************************************/

#include "common.h"
#include "LED.h"


/*
 * 定义LED 编号对应的管脚
 */
PortPinsEnum_Type LED_PTxn[LED_ALL] = {PTB20, PTB21, PTB22, PTB23};


/*!
 *  @brief      初始化LED端口
 *  @param      LED_e    LED编号
 *  @since      v5.0
 *  Sample usage:       led_init (LED0);    //初始化 LED0
 */
void LED_Init(LED_e ledn)
{
    if(ledn < LED_ALL)
    {
        GPIO_Init_Pin(LED_PTxn[ledn], DIR_OUTPUT, LED_OFF);
    }
    else
    {
        ledn = LED_ALL;
        while(ledn--)
        {
            GPIO_Init_Pin(LED_PTxn[ledn], DIR_OUTPUT, LED_OFF);
        }
    }
}

