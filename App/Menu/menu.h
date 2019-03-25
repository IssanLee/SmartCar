/***************************************************************************
* File:         menu.h
* Summary:      菜单任务
* Version:      V0.1
* Author:       Leopold & 山外[VCAN]
* Date:         2019/1/31
* Change Logs:
*       Date:   2019/1/31          
*       Version:V0.1         
*       Author: Leopold       
*       Notes:  修改自:山外[VCAN]                             
***************************************************************************/

#ifndef _MENU_H_
#define _MENU_H_

#include "common.h"
#include "LCD.h"
#include "KEY.h"

typedef  enum
{
    RESULT_NULL,
    RESULT_INIT,            //重新初始化
    RESULT_TO_MENU,         //上一层 菜单
} result_e;

typedef  result_e  ap_func(void);

typedef struct
{
    void    *   strbuff;      //字符串图案
    uint32      strnum;       //中文字符数目
    ap_func *   pfunc;        //执行函数

} menu_t;

#define VCAN_MENU(s,fc)        {(void *)(s),sizeof(s)/LCD_CH_SIZE,fc}         //菜单，第一个参数是 汉字字库 ，第二个是 执行函数


#define LCD_FSTR_CH_FUNC     LCD_FStr_CH

extern void        menu_init(Site_t site,menu_t *menu,uint8 tab,uint8 new_tab,uint8 max_tab);
extern result_e    menu_deal(Site_t site,menu_t *menu,uint8 tab,uint8 *new_tab,uint8 max_tab);
extern void        menu_display(Site_t  site,menu_t *menu,uint8 tab,uint8 new_tab,uint8 max_tab);



#endif  //_MENU_H_