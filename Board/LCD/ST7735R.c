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

#include "ST7735R.h"

uint16  st7735r_h   = ST7735R_H;
uint16  st7735r_w   = ST7735R_W;
uint8   st7735r_dir = ST7735R_DIR_DEFAULT;// 显示方向



/********************************************************
 *  @brief      ST7735R初始化
 *  @since      v5.0
 ********************************************************/
void ST7735R_Init()
{
    uint8 n ;
    
    // 数据位(移植需更改,最好使用同一类PORT)
    for (n = 0; n < 8; n++)
    {
        GPIO_Init_Pin((PortPinsEnum_Type)(PTD0 + n), DIR_OUTPUT, 0);
    }
    
    GPIO_Init_Pin(ST7735R_WR, DIR_OUTPUT, 0);
    GPIO_Init_Pin(ST7735R_RD, DIR_OUTPUT, 1);
    GPIO_Init_Pin(ST7735R_CS, DIR_OUTPUT, 1);
    GPIO_Init_Pin(ST7735R_RS, DIR_OUTPUT, 0);
    
    GPIO_Init_Pin(ST7735R_RST, DIR_OUTPUT, 0);

    //HDS输出高驱动能力
    PORT_Init(ST7735R_WR , ALT1 | HDS);
    PORT_Init(ST7735R_RD , ALT1 | HDS);
    PORT_Init(ST7735R_CS , ALT1 | HDS);
    PORT_Init(ST7735R_RS , ALT1 | HDS);

    //初始化总线
    ST7735R_RST_OUT_L;
    ST7735R_DELAY();
    ST7735R_RST_OUT_H;
    ST7735R_DELAY_MS(500);      //给足上电时间

    ST7735R_WR_8CMD(0x11);      //Sleep out   退出睡眠模式
    ST7735R_DELAY_MS(120);      //Delay 120ms
    
    //------------------------------------ST7735R Frame Rate-----------------------------------------//
    /* Set the frame frequency of the full colors normal mode. */
    ST7735R_WR_8CMD(0xB1);     //In Normal Mode (Full Colors)    全屏模式
    ST7735R_WR_8DATA(0x05);    //设置 RTNA Set 1-line  Period  一行周期
    ST7735R_WR_8DATA(0x3A);    //设置 FPA: Front Porch
    ST7735R_WR_8DATA(0x3A);    //设置 BPA: Back Porch
    //Frame rate=fosc/((RTNA x 2 + 40) x (LINE + FPA + BPA +2))
    //其中 fosc = 850kHz

    /* Set the frame frequency of the Idle mode.  */
    ST7735R_WR_8CMD(0xB2);     //In Idle Mode (8-colors)
    ST7735R_WR_8DATA(0x05);
    ST7735R_WR_8DATA(0x3A);
    ST7735R_WR_8DATA(0x3A);

    /* Set the frame frequency of the Partial mode/ full colors. */
    ST7735R_WR_8CMD(0xB3);
    ST7735R_WR_8DATA(0x05);
    ST7735R_WR_8DATA(0x3A);
    ST7735R_WR_8DATA(0x3A);
    ST7735R_WR_8DATA(0x05);
    ST7735R_WR_8DATA(0x3A);
    ST7735R_WR_8DATA(0x3A);

    //------------------------------------End ST7735R Frame Rate-----------------------------------------//
    ST7735R_WR_8CMD(0xB4);   //Display Inversion Control  显示反转控制

    ST7735R_WR_8DATA(0x07);  //LCD_ST7735R_WR_8DATA(0x03); 原来为3，改为7
    // 低三位从高到低，分别为  full colors normal mode 、Idle mode 、
    // full Colors partial mode  的 点反转 或 列反转 ，1为列反转

    ST7735R_WR_8CMD(0xC0);   //Power Control Setting  电源控制设置
    ST7735R_WR_8DATA(0x28);
    ST7735R_WR_8DATA(0x08);
    ST7735R_WR_8DATA(0x84);
    ST7735R_WR_8CMD(0xC1);
    ST7735R_WR_8DATA(0XC0);
    ST7735R_WR_8CMD(0xC2);
    ST7735R_WR_8DATA(0x0C);
    ST7735R_WR_8DATA(0x00);
    ST7735R_WR_8CMD(0xC3);
    ST7735R_WR_8DATA(0x8C);
    ST7735R_WR_8DATA(0x2A);
    ST7735R_WR_8CMD(0xC4);
    ST7735R_WR_8DATA(0x8A);
    ST7735R_WR_8DATA(0xEE);
    //---------------------------------End ST7735R Power Sequence-------------------------------------//
    ST7735R_WR_8CMD(0xC5);   //  VCOM 电压配置
    ST7735R_WR_8DATA(0x0C); //  -0.725
    //------------------------------------ST7735R Gamma Sequence-----------------------------------------//
    ST7735R_WR_8CMD(0xE0);
    ST7735R_WR_8DATA(0x05);
    ST7735R_WR_8DATA(0x1A);
    ST7735R_WR_8DATA(0x0C);
    ST7735R_WR_8DATA(0x0E);
    ST7735R_WR_8DATA(0x3A);
    ST7735R_WR_8DATA(0x34);
    ST7735R_WR_8DATA(0x2D);
    ST7735R_WR_8DATA(0x2F);
    ST7735R_WR_8DATA(0x2D);
    ST7735R_WR_8DATA(0x2A);
    ST7735R_WR_8DATA(0x2F);
    ST7735R_WR_8DATA(0x3C);
    ST7735R_WR_8DATA(0x00);
    ST7735R_WR_8DATA(0x01);
    ST7735R_WR_8DATA(0x02);
    ST7735R_WR_8DATA(0x10);
    ST7735R_WR_8CMD(0xE1);
    ST7735R_WR_8DATA(0x04);
    ST7735R_WR_8DATA(0x1B);
    ST7735R_WR_8DATA(0x0D);
    ST7735R_WR_8DATA(0x0E);
    ST7735R_WR_8DATA(0x2D);
    ST7735R_WR_8DATA(0x29);
    ST7735R_WR_8DATA(0x24);
    ST7735R_WR_8DATA(0x29);
    ST7735R_WR_8DATA(0x28);
    ST7735R_WR_8DATA(0x26);
    ST7735R_WR_8DATA(0x31);
    ST7735R_WR_8DATA(0x3B);
    ST7735R_WR_8DATA(0x00);
    ST7735R_WR_8DATA(0x00);
    ST7735R_WR_8DATA(0x03);
    ST7735R_WR_8DATA(0x12);
    //------------------------------------End ST7735R Gamma Sequence-----------------------------------------//

    ST7735R_WR_8CMD(0x3A); //65k mode
    ST7735R_WR_8DATA(0x05);
    ST7735R_WR_8CMD(0x29); //开显示 Display on
    ST7735R_WR_8CMD(0x2c); //

    ST7735R_Dir(st7735r_dir);

}

void ST7735R_wr_data(uint8 data)//写数据
{
    ST7735R_RD_OUT_H;
    ST7735R_RS_OUT_H;
    ST7735R_CS_OUT_L;

    ST7735R_PO(data);//数据Out口D0-D7

    ST7735R_DELAY();

    ST7735R_WR_OUT_L;
    ST7735R_WR_OUT_H;
    ST7735R_CS_OUT_H;
}

void ST7735R_wr_cmd(uint8 cmd)//写命令
{
    ST7735R_RD_OUT_H;
    ST7735R_RS_OUT_L;
    ST7735R_CS_OUT_L;

    ST7735R_PO(cmd);

    ST7735R_DELAY();
    ST7735R_WR_OUT_L;
    ST7735R_WR_OUT_H;
    ST7735R_CS_OUT_H;
}

/********************************************************
 *  @brief      设置ST7735R GRAM指针扫描方向
 *  @param      option    方向选择（正看（焊接排线在下面） 为 0 ，90度为1，180度为2，270度为2）
 *  @since      v5.0
 *  ??????????? MX 列地址顺序   ???????
 *  ??????????? MY 行地址顺序   ??????? 
 *  ??????????? MV 行列交换     ???????
 *  ??????????? ML 垂直刷新顺序 ???????
 ********************************************************/
void ST7735R_Dir(uint8 option)
{
    ASSERT(option < 4);

    st7735r_dir = option;
    switch(option)
    {
    case 0:
        ST7735R_WR_8CMD(0x36); // 内存数据访问控制  MX 列地址顺序, MY 行地址顺序 ，MV 行列交换 ，ML 垂直刷新顺序 ,RGB  RGB-BGA顺序
        ST7735R_WR_8DATA(0xc8);//0xc8  0xA8 0x08 0x68

        ST7735R_WR_8CMD(0x2a);
        ST7735R_WR_8DATA(0x00);
        ST7735R_WR_8DATA(0x00);
        ST7735R_WR_8DATA(0x00);
        ST7735R_WR_8DATA(128);

        ST7735R_WR_8CMD(0x2B);
        ST7735R_WR_8DATA(0x00);
        ST7735R_WR_8DATA(50);
        ST7735R_WR_8DATA(0x00);
        ST7735R_WR_8DATA(128+50);

        st7735r_h   = ST7735R_H;
        st7735r_w   = ST7735R_W;
        break;
    case 1:
        ST7735R_WR_8CMD(0x36); // 内存数据访问控制  MX 列地址顺序, MY 行地址顺序 ，MV 行列交换 ，ML 垂直刷新顺序 ,RGB  RGB-BGA顺序
        ST7735R_WR_8DATA(0xA8);//0xc8  0xA8 0x08 0x68

        ST7735R_WR_8CMD(0x2a);
        ST7735R_WR_8DATA(0x00);
        ST7735R_WR_8DATA(0x03);
        ST7735R_WR_8DATA(0x00);
        ST7735R_WR_8DATA(0x82);

        ST7735R_WR_8CMD(0x2B);
        ST7735R_WR_8DATA(0x00);
        ST7735R_WR_8DATA(0x02);
        ST7735R_WR_8DATA(0x00);
        ST7735R_WR_8DATA(0x81);

        st7735r_h   = ST7735R_W;
        st7735r_w   = ST7735R_H;
        break;
    case 2:
        ST7735R_WR_8CMD(0x36); // 内存数据访问控制  MX 列地址顺序, MY 行地址顺序 ，MV 行列交换 ，ML 垂直刷新顺序 ,RGB  RGB-BGA顺序
        ST7735R_WR_8DATA(0x08);//0xc8  0xA8 0x08 0x68

        ST7735R_WR_8CMD(0x2a);
        ST7735R_WR_8DATA(0x00);
        ST7735R_WR_8DATA(0x02);
        ST7735R_WR_8DATA(0x00);
        ST7735R_WR_8DATA(0x81);

        ST7735R_WR_8CMD(0x2B);
        ST7735R_WR_8DATA(0x00);
        ST7735R_WR_8DATA(0x03);
        ST7735R_WR_8DATA(0x00);
        ST7735R_WR_8DATA(0x82);

        st7735r_h   = ST7735R_H;
        st7735r_w   = ST7735R_W;
        break;
    case 3:
        ST7735R_WR_8CMD(0x36); // 内存数据访问控制  MX 列地址顺序, MY 行地址顺序 ，MV 行列交换 ，ML 垂直刷新顺序 ,RGB  RGB-BGA顺序
        ST7735R_WR_8DATA(0x68);//0xc8  0xA8 0x08 0x68

        ST7735R_WR_8CMD(0x2a);
        ST7735R_WR_8DATA(0x00);
        ST7735R_WR_8DATA(0x03);
        ST7735R_WR_8DATA(0x00);
        ST7735R_WR_8DATA(0x82);

        ST7735R_WR_8CMD(0x2B);
        ST7735R_WR_8DATA(0x00);
        ST7735R_WR_8DATA(0x00);
        ST7735R_WR_8DATA(0x00);
        ST7735R_WR_8DATA(128);

        st7735r_h   = ST7735R_W;
        st7735r_w   = ST7735R_H;
        break;
    default:
        break;
    }

}

/*!
 *  @brief      设置 ST7735R 开窗
 *  @param      site        左上角坐标位置
 *  @param      size        开窗大小
 *  @since      v5.0
 */
void ST7735R_ptlon(Site_t site, Size_t size)
{

    if(st7735r_dir&0x01)
    {
        site.X += 32;     //液晶需要偏移一下，避免四周看不到的行
        //site.y += 2;
    }
    else
    {
        //site.x += 2;     //液晶需要偏移一下，避免四周看不到的行
        site.Y += 32;
    }

    ST7735R_WR_8CMD(0x2a);   //Partial Mode On  局部模式
    ST7735R_WR_8DATA((uint8)(site.X >> 8)); //高8位 Sx
    ST7735R_WR_8DATA((uint8)site.X);   //低8位 Sx
    ST7735R_WR_8DATA((uint8)((site.X + size.W - 1) >> 8));
    ST7735R_WR_8DATA((uint8)(site.X + size.W - 1));

    ST7735R_WR_8CMD(0x2B);//Row Address Set  行地址设置
    ST7735R_WR_8DATA((uint8)(site.Y >> 8));
    ST7735R_WR_8DATA((uint8)site.Y);
    ST7735R_WR_8DATA((uint8)((site.Y + size.H - 1) >> 8));
    ST7735R_WR_8DATA((uint8)(site.Y + size.H - 1));

}

/*!
 *  @brief      获取 ST7735R 高度
 *  @return     ILI9341 高度
 *  @since      v5.0
 */
uint16 ST7735R_Get_H()
{
    return st7735r_h;
}

/*!
 *  @brief      获取 ST7735R 宽度
 *  @return     ST7735R 宽度
 *  @since      v5.0
 */
uint16 ST7735R_Get_W()
{
    return st7735r_w;
}

/*!
 *  @brief      获取 ST7735R 显示方向
 *  @return     ST7735R 方向
 *  @since      v5.0
 */
uint8 ST7735R_Get_Dir()
{
    return st7735r_dir;
}




