/***************************************************************************
* File:         L3G4.c
* Summary:      L3G4驱动
* Version:      V0.1
* Author:       Leopold
* Date:         2019/1/30
* Change Logs:
*       Date:   2019/1/30          
*       Version:V0.1         
*       Author: Leopold       
*       Notes:  
***************************************************************************/

#include "L3G4.h"

/*------------------------------------------------------------------------------
L3G4+向寄存器里面写数据
参数：
    reg：寄存器
    dat：所要写的值
备注：
 -----------------------------------------------------------------------------*/
static void L3G4_WriteReg(uint8 reg, uint8 dat)
{
    uint8 buff[2];
    buff[0] = reg;
    buff[1] = dat;
    spi_mosi(SPI2, SPI_PCS0, buff, NULL, 2); //发送buff里数据,并采集到 buff里
}


/*------------------------------------------------------------------------------
L3G4+读寄存器
参数：
    reg：寄存器
    dat：需要读取的数据的存放地址
    return:读到的值
返回：读到的数值
个人小理解：野火的spi_mosi(SPI2, SPIn_PCS0, buff, buff, 2);这个函数里面的buff
            还是作为临时参数比较合适
 -----------------------------------------------------------------------------*/
static uint8 L3G4_ReadReg(uint8 reg)
{
    uint8 buff[2];
    buff[0] = reg;                              //先发送寄存器
    spi_mosi(SPI2, SPI_PCS0, buff, buff, 2);    //发送buff数据，并保存到buff里
    return buff[1];                             //提取第二个数据
}


void L3G4_SPI_Init()
{
    L3G4_spi_init();

    L3G4_WriteReg(0x20, 0x0F);
    L3G4_WriteReg(0x22, 0x08);
    L3G4_WriteReg(0x23, 0x30);
    
    L3G4_WriteReg(0x20, 0x0F);
    L3G4_WriteReg(0x22, 0x08);
    L3G4_WriteReg(0x23, 0x30);
}

int16 L3G4_Read_X()
{
    uint8 X_L, X_H;

    int16 XHL;

    X_L = (uint8)(L3G4_ReadReg(0xA8));//0X28|0X80 = 0XA8

    X_H = (uint8)(L3G4_ReadReg(0xA9));

    XHL = (int16)((X_H<<8) | X_L);

    return (XHL);
}

int16 L3G4_Read_Y()
{
    uint8 Y_L, Y_H;

    int16 YHL;

    Y_L = (uint8)(L3G4_ReadReg(0xAA));//0X2A|0X80 = 0xAA

    Y_H = (uint8)(L3G4_ReadReg(0xAB));

    YHL = (int16)((Y_H<<8) | Y_L);

    return (YHL);
}

int16 L3G4_Read_Z()
{
    uint8 Z_L, Z_H;

    int16 ZHL;

    Z_L = (uint8)(L3G4_ReadReg(0xAC));//0X2C|0X80 = 0XAC

    Z_H = (uint8)(L3G4_ReadReg(0xAD));

    ZHL = (int16)((Z_H<<8) | Z_L);

    return (ZHL);
}




int16 Read_Angle_Speed()
{
    int16 temp;
    temp = L3G4_Read_Z();
    return temp;
}


