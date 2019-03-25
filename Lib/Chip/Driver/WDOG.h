/***************************************************************************
* File:         WDOG.h
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

#ifndef __WDOG_H__
#define __WDOG_H__

//看门狗初始化
uint8 WDOG_Init(uint32);

//看门狗使能
void WDOG_Enable(void);

//看门狗禁用
void WDOG_Disable(void);

//喂狗
void WDOG_Feed(void);

#endif /* __WDOG_H__ */