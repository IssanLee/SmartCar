/***************************************************************************
* File:         ST7735R.h
* Summary:      ST7735R驱动
* Version:      V0.1
* Author:       Leopold & 山外科技[VCAN]
* Date:         2019/1/30
* Change Logs:
*       Date:   2019/1/30          
*       Version:V0.1         
*       Author: Leopold       
*       Notes:  修改自:山外科技[VCAN]
***************************************************************************/


#ifndef _ST7735R_H_
#define _ST7735R_H_

#include "common.h"

/**需要配置start**/

#define ST7735R_DIR_DEFAULT     0       //定义默认方向即上下左右 (0~3)

#define ST7735R_H       128
#define ST7735R_W       128

//定义所用到的 IO管脚
#define	ST7735R_DIS     GPIO_Init(PTC11, DIR_OUTPUT, 1);        //禁用
#define	ST7735R_EN      GPIO_Init(PTC11, DIR_OUTPUT, 0);        //使能

#define ST7735R_RST     PTC18                                   //复位

#define ST7735R_WR      PTC17
#define ST7735R_RD      PTC12
#define ST7735R_CS      PTC19
#define ST7735R_RS      PTC15

#define ST7735R_PO(data)        GPIO_Output_8b(PTD, 0, data);
#define ST7735R_PI()            GPIO_Input_8b(PTD, 0);

/**需要配置end**/



/*********************** API接口 ******************************/
//提供API接口给LCD调用


//通过函数调用，速度太慢了，改成宏定义调用
#define LCD_WR_8DATA(data)      ST7735R_wr_data(data)       //写数据
#define LCD_WR_8CMD(cmd)        ST7735R_wr_cmd(cmd)         //命令


//#define     LCD_ST7735R_PDDR_OUT()      (DDRD_B0 = 0xff)
//#define     LCD_ST7735R_PDDR_IN()       (DDRD_B0 = 0)



#define     ST7735R_WR_OUT_H      GPIO_OUT_H(ST7735R_WR);
#define     ST7735R_RD_OUT_H      GPIO_OUT_H(ST7735R_RD);
#define     ST7735R_CS_OUT_H      GPIO_OUT_H(ST7735R_CS);
#define     ST7735R_RS_OUT_H      GPIO_OUT_H(ST7735R_RS);
#define     ST7735R_RST_OUT_H     GPIO_OUT_H(ST7735R_RST);

#define     ST7735R_WR_OUT_L      GPIO_OUT_L(ST7735R_WR);
#define     ST7735R_RD_OUT_L      GPIO_OUT_L(ST7735R_RD);
#define     ST7735R_CS_OUT_L      GPIO_OUT_L(ST7735R_CS);
#define     ST7735R_RS_OUT_L      GPIO_OUT_L(ST7735R_RS);
#define     ST7735R_RST_OUT_L     GPIO_OUT_L(ST7735R_RST);


#define ST7735R_DELAY()         DELAY_MS(10)
#define ST7735R_DELAY_MS(ms)    DELAY_MS(ms)





void     ST7735R_Init();
void     ST7735R_Dir(uint8 option);
void     ST7735R_Ptlon(Site_t site, Size_t size);
uint16   ST7735R_Get_H();
uint16   ST7735R_Get_W();
uint8    ST7735R_Get_Dir();


void ST7735R_wr_data(uint8 data);
void ST7735R_wr_cmd(uint8 cmd);

#define ST7735R_WR_8CMD(cmd)    do\
                            {\
                                ST7735R_RD_OUT_H;\
                                ST7735R_RS_OUT_L;\
                                ST7735R_CS_OUT_L;\
                                ST7735R_PO((uint8)cmd);\
                                ST7735R_WR_OUT_L;\
                                ST7735R_WR_OUT_H;\
                                ST7735R_CS_OUT_H;\
                            }while(0)   //LCD_WR=0;LCD_WR=1;产生一个上升沿

#define ST7735R_WR_8DATA(data)  do\
                            {\
                                ST7735R_RD_OUT_H;\
                                ST7735R_RS_OUT_H;\
                                ST7735R_CS_OUT_L;\
                                ST7735R_PO((uint8)data);\
                                ST7735R_WR_OUT_L;\
                                ST7735R_WR_OUT_H;\
                                ST7735R_CS_OUT_H;\
                            }while(0)   //LCD_WR=0;在这里写入数据到RAM


#define ST7735R_WR_DATA(data)       do{ST7735R_WR_8DATA((uint8)((data)>>8) );ST7735R_WR_8DATA( (uint8)(data) );}while(0)       //写数据



#endif
