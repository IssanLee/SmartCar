/***************************************************************************
* File:         KEY.c
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

#include "common.h"
#include "KEY.h"


/*
 * 定义 KEY 编号对应的管脚
 */

/*   上、下、左、右、取消、选择、开始、停止*/
//PTXn_e KEY_PTxn[KEY_MAX] = {PTD10, PTD14, PTD11, PTD12, PTD7, PTD13, PTC14, PTC15};//山外

PortPinsEnum_Type KEY_PTxn[KEY_ALL] = {PTC13, PTD8, PTE1, PTC16, PTC14, PTD9};// PTB14, PTB15};//V2主板

PortPinsEnum_Type DSW_PTxn[DSW_ALL] = {PTA7, PTA6, PTA8, PTA9, PTA10, PTA11, PTA12, PTA13};


void KEY_DSW_Init()
{
    KEY_Init(KEY_ALL);
    DSW_Init(DSW_ALL);
}

/**************************************************************************************
 *  @brief      初始化key端口(key 小于 KEY_MAX 时初始化 对应端口，否则初始化全部端口)
 *  @param      KEY_e    KEY编号
 *  Sample usage:       key_init (KEY_U);    //初始化 KEY_U
***************************************************************************************/
void KEY_Init(KEY_e key)
{
    if(key < KEY_ALL)
    {
        GPIO_Init_Pin(KEY_PTxn[key], DIR_INPUT, 0);
        PORT_Init_NoALT(KEY_PTxn[key], PULLUP);         //保持复用不变，仅仅改变配置选项
    }
    else
    {
        key = KEY_ALL;

        //初始化全部 按键
        while(key--)
        {
            GPIO_Init_Pin(KEY_PTxn[key], DIR_INPUT, 0);
            PORT_Init_NoALT(KEY_PTxn[key], PULLUP);         //保持复用不变，仅仅改变配置选项
        }

    }
}

/**************************************************************************************
                              初始化dsw_key端口
***************************************************************************************/
void DSW_Init(DSW_e dsw)
{
    while(dsw--)
    {
        GPIO_Init_Pin(DSW_PTxn[dsw], DIR_INPUT, 0);
        PORT_Init_NoALT(DSW_PTxn[dsw], PULLUP);         //保持复用不变，仅仅改变配置选项
    }
}

/****************************************************************************************
                           获取key状态（不带延时消抖）
****************************************************************************************/
KEY_STATUS_e KEY_Get(KEY_e key)
{
    if(GPIO_Get(KEY_PTxn[key]) == KEY_DOWN)
    {
        return KEY_DOWN;
    }
    return KEY_UP;
}


/****************************************************************************************
                            检测key状态（带延时消抖）
****************************************************************************************/
KEY_STATUS_e KEY_Check(KEY_e key)
{
    if(KEY_Get(key) == KEY_DOWN)
    {
        DELAY_MS(10);
        if( KEY_Get(key) == KEY_DOWN)
        {
            return KEY_DOWN;
        }
    }
    return KEY_UP;
}

/*********************  如下代码是实现按键定时扫描，发送消息到FIFO  ********************/
/*
 * 定义按键消息FIFO状态
 */
typedef enum
{
    KEY_MSG_EMPTY,      //没有按键消息
    KEY_MSG_NORMAL,     //正常，有按键消息，但不满
    KEY_MSG_FULL,       //按键消息满
} key_msg_e;

/*
 * 定义按键消息FIFO相关的变量
 */
KEY_MSG_t           key_msg[KEY_MSG_FIFO_SIZE];             //按键消息FIFO
volatile uint8      key_msg_front = 0, key_msg_rear = 0;    //接收FIFO的指针
volatile uint8      key_msg_flag = KEY_MSG_EMPTY;           //按键消息FIFO状态


/*!
 *  @brief      发送按键消息到FIFO
 *  @param      KEY_MSG_t       按键消息
 *  @since      v5.0
 *  Sample usage:
                    KEY_MSG_t *keymsg;
                    keymsg.key      = KEY_U;
                    keymsg.status   = KEY_HOLD;
                    send_key_msg(keymsg);                   //发送
 */
void send_key_msg(KEY_MSG_t keymsg)
{
    uint8 tmp;
    //保存在FIFO里
    if(key_msg_flag == KEY_MSG_FULL)
    {
        //满了直接不处理
        return ;
    }
    key_msg[key_msg_rear].key = keymsg.key;
    key_msg[key_msg_rear].status = keymsg.status;

    key_msg_rear++;

    if(key_msg_rear >= KEY_MSG_FIFO_SIZE)
    {
        key_msg_rear = 0;                       //重头开始
    }

    tmp = key_msg_rear;
    if(tmp == key_msg_front)                   //追到屁股了，满了
    {
        key_msg_flag = KEY_MSG_FULL;
    }
    else
    {
        key_msg_flag = KEY_MSG_NORMAL;
    }
}


/*!
 *  @brief      从FIFO里获取按键消息
 *  @param      KEY_MSG_t       按键消息
 *  @return     是否获取按键消息（1为获取成功，0为没获取到按键消息）
 *  @since      v5.0
 *  Sample usage:
                    KEY_MSG_t keymsg;
                    if(get_key_msg(&keymsg) == 1)
                    {
                        printf("\n按下按键KEY%d,类型为%d（0为按下，1为弹起，2为长按）",keymsg.key,keymsg.status);
                    }
 */
uint8 get_key_msg(KEY_MSG_t *keymsg)
{
    uint8 tmp;

    if(key_msg_flag == KEY_MSG_EMPTY)               //按键消息FIFO为空，直接返回0
    {
        return 0;
    }

    keymsg->key = key_msg[key_msg_front].key;       //从FIFO队首中获取按键值
    keymsg->status = key_msg[key_msg_front].status; //从FIFO队首中获取按键类型

    key_msg_front++;                                //FIFO队首指针加1，指向下一个消息

    if(key_msg_front >= KEY_MSG_FIFO_SIZE)          //FIFO指针队首溢出则从0开始计数
    {
        key_msg_front = 0;                          //重头开始计数（循环利用数组）
    }

    tmp = key_msg_rear;
    if(key_msg_front == tmp)                        //比较队首和队尾是否一样，一样则表示FIFO已空了
    {
        key_msg_flag = KEY_MSG_EMPTY;
    }
    else
    {
        key_msg_flag = KEY_MSG_NORMAL;
    }

    return 1;
}

/*!
 *  @brief      定时检测key状态
 *  @since      v5.0
 *  @note       此函数需要放入 定时中断服务函数里，定时10ms执行一次
 */
void key_IRQHandler(void)
{

    KEY_e   keynum;
    static uint8 keytime[KEY_ALL];                          //静态数组，保存各数组按下时间

    KEY_MSG_t keymsg;                                       //按键消息

    for(keynum = (KEY_e)0 ; keynum < KEY_ALL; keynum ++)    //每个按键轮询
    {
        if(key_get(keynum) == KEY_DOWN)                     //判断按键是否按下
        {
            keytime[keynum]++;                              //按下时间累加

            if(keytime[keynum] <= KEY_DOWN_TIME)            //判断时间是否没超过消抖确认按下时间
            {
                continue;                                   //没达到，则继续等待
            }
            else if(keytime[keynum] == KEY_DOWN_TIME + 1 )  //判断时间是否为消抖确认按下时间
            {
                //确认按键按下
                keymsg.key = keynum;
                keymsg.status = KEY_DOWN;
                send_key_msg(keymsg);                       //把按键值和按键类型发送消息到FIFO
            }
            else if(keytime[keynum] <= KEY_HOLD_TIME)       //是否没超过长按HOLD按键确认时间
            {
                continue;                                   //没超过，则继续等待
            }
            else if(keytime[keynum]  == KEY_HOLD_TIME + 1)  //是否为长按hold确认时间
            {
                //确认长按HOLD
                keymsg.key = keynum;
                keymsg.status = KEY_HOLD;
                send_key_msg(keymsg);                       //发送
                keytime[keynum] = KEY_DOWN_TIME + 1;
            }
            else
            {
                keytime[keynum] = KEY_DOWN_TIME + 1;        //继续重复检测 hold 状态
            }
        }
        else
        {
            if(keytime[keynum] > KEY_DOWN_TIME)             //如果确认过按下按键
            {
                keymsg.key = keynum;
                keymsg.status = KEY_UP;
                send_key_msg(keymsg);                       //发送按键弹起消息
            }

            keytime[keynum] = 0;                            //时间累计清0
        }
    }
}





