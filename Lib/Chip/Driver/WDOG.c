/***************************************************************************
* File:         WDOG.c
* Summary:      WDOG底层模块相关函数
* Version:      V0.1
* Author:       Leopold & 拉普兰德[LPLD]
* Date:         2019/1/27
* Change Logs:
*       Date:   2019/1/27          
*       Version:V0.1         
*       Author: Leopold       
*       Notes:  修改自:拉普兰德[LPLD]                                       
***************************************************************************/

#include "common.h"
#include "WDOG.h"

static void WDOG_Unlock(void);

/*
 * WDOG_Init
 * 看门狗初始化
 * 
 * 参数:
 *    period_ms--看门狗复位周期
 *      |__单位为毫秒
 *
 * 输出:
 *    0--配置错误
 *    1--配置成功
 */
uint8 WDOG_Init(uint32 period_ms)
{
  uint8 presc = 3;
  uint32 bus_khz = g_bus_clock / 1000;
  uint32 value;
  
  if (period_ms == 0)
  {
    return 0;
  }
    
  value = (period_ms * bus_khz) / (presc + 1);
    
  //先解锁寄存器
  WDOG_Unlock();
  //设置分频
  WDOG->PRESC = WDOG_PRESC_PRESCVAL(presc);
  //设置计时溢出值
  WDOG->TOVALH = (value&0xFFFF0000)>>16;
  WDOG->TOVALL = (value&0x0000FFFF)>>0;  
  //使能WDOG
  WDOG->STCTRLH |= WDOG_STCTRLH_WDOGEN_MASK;
  
  return 1;
}

/*
 * WDOG_Enable
 * 看门狗使能
 * 
 * 参数:无
 *
 * 输出:无
 */
void WDOG_Enable(void)
{
  //先解锁寄存器
  WDOG_Unlock();
  //使能WDOG
  WDOG->STCTRLH |= WDOG_STCTRLH_WDOGEN_MASK;
}

/*
 * WDOG_Disable
 * 看门狗禁用
 * 
 * 参数:无
 *
 * 输出:无
 */
void WDOG_Disable(void)
{
  //先解锁寄存器
  WDOG_Unlock();
  //禁用WDOG
  WDOG->STCTRLH &= ~(WDOG_STCTRLH_WDOGEN_MASK);
}

/*
 * WDOG_Feed
 * 喂狗
 * 
 * 参数:无
 *
 * 输出:无
 */
void WDOG_Feed(void)
{
  WDOG->REFRESH = 0xA602;
  WDOG->REFRESH = 0xB480;
}

/*
 * WDOG_Unlock
 * WDOG解锁函数，内部调用
 * 
 * 参数:无
 *
 * 输出:无
 */
static void WDOG_Unlock(void)
{
  //以下代码必须在20个时钟周期内完成，否则会复位
  DisableInterrupts;
  
  //写0xC520来解锁寄存器
  WDOG->UNLOCK = 0xC520;
  
  //接下来写0xD928完成解锁
  WDOG->UNLOCK = 0xD928;
  	
  EnableInterrupts;
}