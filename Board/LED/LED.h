/***************************************************************************
* File:         LED.h
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

#ifndef __LED_H__
#define __LED_H__


/*! 枚举LED编号 */
typedef enum
{
  LED0,
  LED1,
  LED2,
  LED3,
  LED_ALL,
} LED_e;

/*! 枚举LED亮灭状态 */
typedef enum LED_status
{
  LED_ON  =   0,            //灯亮(对应低电平)
  LED_OFF =   1             //灯暗(对应高电平)
} LED_status; 


extern void  LED_Init(LED_e);            //初始化LED端口

#endif  //__LED_H__