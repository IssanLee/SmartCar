/***************************************************************************
* File:         KEY.h
* Summary:      KEY
* Version:      V0.1
* Author:       Leopold
* Date:         2019/1/27
* Change Logs:
*       Date:   2019/1/27          
*       Version:V0.1         
*       Author: Leopold       
*       Notes:  first version                                        
***************************************************************************/
#ifndef __KEY_H__
#define __KEY_H__

//下面是定义按键的时间，单位为 ： 10ms（中断时间）
#define KEY_DOWN_TIME           1       //消抖确认按下时间
#define KEY_HOLD_TIME           50      //长按hold确认时间，最多253，否则需要修改 keytime 的类型
                                        //如果按键一直按下去，则每隔 KEY_HOLD_TIME - KEY_DOWN_TIME 时间会发送一个 KEY_HOLD 消息

//定义按键消息FIFO大小
#define KEY_MSG_FIFO_SIZE       20      //最多 255，否则需要修改key_msg_front/key_msg_rear类型

//按键端口的枚举
typedef enum
{
    KEY_U,         //上
    KEY_D,         //下
    KEY_L,         //左
    KEY_R,         //右
    KEY_A,         //取消
    KEY_B,         //选择
    //KEY_START,   //开始
    //KEY_STOP,    //停止

    KEY_ALL,
} KEY_e;

//拨码按键枚举定义  PTA7 、PTA6 、PTA8 、PTA9 、PTA10、PTA11、PTA12、PTA13
typedef enum
{
    DSW_0,
    DSW_1,
    DSW_2,
    DSW_3,
    DSW_4,
    DSW_5,
    DSW_6,
    DSW_7,
    
    DSW_ALL,
  
} DSW_e;

//key状态枚举定义
typedef enum
{
    KEY_DOWN  =   0,         //按键按下时对应电平
    KEY_UP    =   1,         //按键弹起时对应电平

    KEY_HOLD,               //长按按键(用于定时按键扫描)

} KEY_STATUS_e;


//DSW状态枚举定义
typedef enum
{
    DSW_OFF  =   0,         //拨码关闭时对应电平
    DSW_ON   =   1,         //拨码开启时对应电平

} DSW_STATUS_e;


//按键消息结构体
typedef struct
{
    KEY_e           key;        //按键编号
    KEY_STATUS_e    status;     //按键状态
} KEY_MSG_t;

void            KEY_DSW_Init();                //初始化按键和拨码
void            DSW_Init(DSW_e dsw);
void            KEY_Init(KEY_e key);            // KEY初始化函数(key 小于 KEY_MAX 时初始化 对应端口，否则初始化全部端口)
KEY_STATUS_e    key_get(KEY_e key);             //检测key状态（不带延时消抖）
KEY_STATUS_e    key_check(KEY_e key);           //检测key状态（带延时消抖）

DSW_STATUS_e dsw_get(DSW_e dsw);  //检测dsw_key状态（不带延时消抖）
DSW_STATUS_e dsw_check(DSW_e dsw); //检测dsw_key状态（带延时消抖）   
//定时扫描按键
uint8   get_key_msg(KEY_MSG_t *keymsg);         //获取按键消息，返回1表示有按键消息，0为无按键消息
void    key_IRQHandler(void);                   //需要定时扫描的中断服务函数（定时时间为10ms）


#endif  //__KEY_H__