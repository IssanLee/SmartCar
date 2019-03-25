/***************************************************************************
* File:         LCD.h
* Summary:      LCD驱动
* Version:      V0.1
* Author:       Leopold & 山外科技[VCAN]
* Date:         2019/1/30
* Change Logs:
*       Date:   2019/1/30          
*       Version:V0.1         
*       Author: Leopold                       n
*       Notes:  修改自:山外科技[VCAN]
***************************************************************************/

#ifndef _LCD_H_
#define _LCD_H_

#include "common.h"
#include "ST7735R.h"
#include "ASCII.h"


#define LCD_EN_W        8
#define LCD_EN_H        16

#define LCD_CH_H        16
#define LCD_CH_W        16

#define LCD_CH_SIZE (((LCD_CH_W + 7) / 8) * LCD_CH_H)           //汉字占用空间大小

#define LCD_INIT                ST7735R_Init()                  //初始化
#define LCD_H                   ST7735R_Get_H()                 //高
#define LCD_W                   ST7735R_Get_W()                 //宽

#define LCD_PTLON(site,size)    ST7735R_Ptlon(site, size)       //开窗
#define LCD_RAMWR()             LCD_WR_CMD(0x2C)                //写模式
#define LCD_WR_DATA(data)       do{ST7735R_WR_8DATA((uint8)((data)>>8) );ST7735R_WR_8DATA( (uint8)(data) );}while(0)       //写数据
#define LCD_WR_CMD(cmd)         ST7735R_WR_8CMD(cmd)            //命令
#define LCD_SET_DIR(opt)        ST7735R_Dir(opt)                //方向

#define LCD_DIR                 ST7735R_Get_Dir()               //获取方向


#define LCD_FCOLOUR     LCD_BLUE        //定义字体颜色
#define LCD_BCOLOUR     LCD_WHITE       //定义背景颜色



void LCD_Init(void);                    //LCD初始化

/******************************************
                LCD绘画
*******************************************/
void LCD_point(Site_t, uint16 rgb565);                                  //画点
void LCD_points(Site_t *site, uint32 point_num, uint16 rgb565);         //画一堆点

void LCD_rectangle(Site_t, Size_t, uint16 rgb565);                      //画矩形
void LCD_cross(Site_t, uint16 len, uint16 rgb565);                      //画十字形

void LCD_char(Site_t, uint8 ascii, uint16 Color, uint16 bkColor);       //显示8*16字符
void LCD_str(Site_t, uint8 *Str, uint16 Color, uint16 bkColor);         //显示8*16字符串
void LCD_num(Site_t, uint32 num, uint16 Color, uint16 bkColor);         //显示数字

void LCD_clear(uint16 rgb565);                                          //清屏

#define MAX_NUM_BIT 5                                                   //数字的最大位数（用于清掉多余的屏幕残留数字）
#define LCD_num_C(site, num, color, bkColor)        LCD_NUM(site, num, MAX_NUM_BIT, color, bkColor)  

void LCD_NUM(Site_t site, int32 num, uint8 max_num_bit, uint16 Color, uint16 bkColor);
void LCD_num_BC(Site_t, uint32 num, uint8 max_num_bit,  uint16 Color, uint16 bkColor);  //显示数字（清掉多余的屏幕残留数字）
void LCD_Img_gray(Site_t site, Size_t size, uint8 *img);                        //显示灰度图像
void LCD_Img_gray_Z(Site_t site, Size_t size, uint8 *img, Size_t imgsize);      //显示灰度图像(可缩放)

#define BINARY_BGCOLOR  LCD_WHITE       //定义二值化图像背景颜色
#define BINARY_COLOR    LCD_BLACK       //定义二值化图像前景颜色

void LCD_Img_Binary(Site_t site, Size_t size, uint8 *img);                      //显示二值化图像
void LCD_Img_Binary_Z(Site_t site, Size_t size, uint8 *img, Size_t imgsize);    //显示二值化图像(可缩放)
void LCD_wave(Site_t site, Size_t size, uint8 *img, uint8 maxval, uint16 Color, uint16 bkColor);    //波形显示，带背景颜色（maxval 最大值表示超过此值的数都当作最大值处理）
void LCD_wave_display(Site_t site, Size_t size, uint8 *img, uint8 maxval, uint16 Color);            //波形显示，不带背景颜色

void LCD_char_CHI(Site_t site, const uint8 str[], uint16 Color, uint16 bkColor);
void LCD_str_CHI(Site_t site, const uint8 *str, uint16 Color, uint16 bkColor);
void LCD_Print(Site_t site, const uint8 *str, uint16 Color, uint16 bkColor);

//下面的显示 函数，是调用 flash 里的字符数据显示汉字
void LCD_FChar_CH(Site_t site, const uint8 *strbuf, uint16 Color, uint16 bkColor);
void LCD_FStr_CH(Site_t site, const uint8 *strbuf, uint16 num, uint16 Color, uint16 bkColor);

#define LCD_FSTR_CH(site,strbuf,Color,bkColor)   LCD_FStr_CH (site,strbuf,sizeof(strbuf)/LCD_CH_SIZE,Color,bkColor)



/****************************************************
* 提取RGB的3元素，rgb565必须是16位
* RGB三基色的处理
* 可以忽略，不去考虑，在ST7735S技术手册和轻松玩转中有说明
****************************************************/
#define RGB565_R(rgb565)                ((uint8)(((uint16)(rgb565)>>11) &0x1F))
#define RGB565_G(rgb565)                ((uint8)(((uint16)(rgb565)>> 5) &0x3F))
#define RGB565_B(rgb565)                ((uint8)( (uint16)(rgb565)      &0x1F))

#define GRAY_2_RGB565(gray)             ((uint16)((((uint8)(gray)>>3)<<11)|(((uint8)(gray)>>2)<<5)|((uint8)(gray)>>3)))

#define RGB565_2_GRAY(rgb565)           ((uint8)(((RGB565_R(rgb565)*235+RGB565_G(rgb565)*613+RGB565_B(rgb565)*625)+1)>>8))  //  31*235+63*613+31*625+1  = 255*256

#define RGB565_H(rgb565)                ((uint8)(((uint16)(rgb565))>>8))
#define RGB565_L(rgb565)                (uint8)(rgb565))

#define RGB24_RGB565(R,G,B)             ((uint16)((((uint8)(R)>>3)<<11)|(((uint8)(G)>>2)<<5)|((uint8)(B)>>3)))
#define RGB555_RGB565(rgb555)           (((((uint16)(rgb555)) & (uint16)(~0x1F)) << 1) |  (((uint16)(rgb555)) & 0x1F))
#define BGR555_RGB565(bgr555)           ((((uint16)(bgr555)<<11) & (uint16)(0x1F<<11)) | (((uint16)(bgr555)) & (uint16)(0x1F<<5)) | (((uint16)(bgr555)>>10) & (uint16)(0x1F)))


/**************************************
              常用颜色
***************************************/
#define LCD_BLACK       0X0000          //黑色
#define LCD_RED         0XF800          //红色
#define LCD_GREEN       0X07E0          //绿色
#define LCD_BLUE        0X001F          //蓝色
#define LCD_GBLUE       0X07FF          //淡蓝
#define LCD_BRED        0XF81F          //紫色
#define LCD_GRED        0XFFE0          //灰色
#define LCD_YELLOW      0xFFE0          //黄色
#define LCD_WHITE       0XFFFF          //白色

#endif //_LCD_H_
