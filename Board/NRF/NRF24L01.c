/***************************************************************************
* File:         NRF24L01.c
* Summary:      NRF24L01设备驱动程序      
* Version:      V0.1
* Author:       Leopold & 拉普兰德[LPLD]
* Date:         2019/1/31
* Change Logs:
*       Date:   2019/1/31          
*       Version:V0.1         
*       Author: Leopold       
*       Notes:  修改自:拉普兰德[LPLD]                             
***************************************************************************/

#include "NRF24L01.h"

static uint8 NRF24L01_WriteBuf(uint8 reg,uint8 *pbuf ,uint8 len);
static uint8 NRF24L01_ReadBuf(uint8 reg,uint8 *pbuf,uint8 len);
static uint8 NRF24L01_WriteReg(uint8 reg,uint8 value);
static uint8 NRF24L01_ReadReg(uint8 reg);
static uint8 NRF24L01_CheckID(void);
static void NRF24L01_DelayUs(uint16 t);

uint8 TX_ADDRESS[NRF24L01_TX_ADR_LEN]={0x34,0x43,0x10,0x10,0x02}; //发送地址
uint8 RX_ADDRESS[NRF24L01_RX_ADR_LEN]={0x34,0x43,0x10,0x10,0x02}; //接收地址(本机地址,仅用于接收模式)
//NRF24L01复位之后用于检测
// NRF24L01_RX_ADDR_P0的复位值是否为0xE7E7E7E7
// NRF24L01_RX_ADDR_P1的复位值是否为0xC2C2C2C2
// NRF24L01_TX_ADDR的复位值是否为0xE7E7E7E7
uint8 NRF24L01_RX_ADDR_P0_RESET_BUF[5];
uint8 NRF24L01_RX_ADDR_P1_RESET_BUF[5];
uint8 NRF24L01_TX_ADDR_RESET_BUF[5];

SPI_InitTypeDef spi_init_param;
GPIO_InitTypeDef ce_io_param;
/*
 * LPLD_Nrf24L01_Init
 * Nrf24L01初始化函数，配置SPI模块，选择Nrf24L01 CE引脚
 * 
 * 参数:
 *    无
 *
 * 输出:
 *    1 配置成功
 */
uint8 NRF24L01_Init(void)
{
  //配置SPI
  spi_init_param.SPI_SPIx = NRF24L01_SPIX;
  spi_init_param.SPI_SckDivider = SPI_SCK_DIV_32;//1.56Mhz
  spi_init_param.SPI_Pcs0Pin = NRF24L01_PCS0;
  spi_init_param.SPI_SckPin  = NRF24L01_SCK;
  spi_init_param.SPI_MosiPin = NRF24L01_MOSI;
  spi_init_param.SPI_MisoPin = NRF24L01_MISO;
  
  spi_init_param.SPI_TxCompleteIntIsr = NULL;
  spi_init_param.SPI_QueueEndIntIsr = NULL;
  spi_init_param.SPI_TxFIFO_FillIntIsr = NULL;
  spi_init_param.SPI_TxFIFO_UnderflowIntIsr = NULL;
  spi_init_param.SPI_RxFIFO_DrainIntIsr = NULL;
  spi_init_param.SPI_RxFIFO_OverflowIntIsr = NULL;
  //配置CE
  ce_io_param.GPIO_PTx  = NRF24L01_CE_PTX;
  ce_io_param.GPIO_Pins = NRF24L01_CE_IOX;
  ce_io_param.GPIO_Dir  = DIR_OUTPUT;
  ce_io_param.GPIO_Output = OUTPUT_L;

  GPIO_Init(ce_io_param);
  SPI_Init(spi_init_param);
  
  return 1;
}

/*
 * LPLD_Nrf24L01_TxModeInit
 * Nrf24L01发送模式初始化
 * 
 * 参数:
 *    无
 *
 * 输出:
 *    Nrf24L01寄存器返回状态
 */
uint8 NRF24L01_TxModeInit(void)
{
  uint8 Temp;
  
  NRF24L01_CE_L;  
  // 芯片上电复位后
  // 分别读取NRF24L01_RX_ADDR_P0、NRF24L01_RX_ADDR_P1、NRF24L01_TX_ADDR的值
  // 查看以下数据
  // NRF24L01_RX_ADDR_P0的复位值是否为0xE7E7E7E7
  // NRF24L01_RX_ADDR_P1的复位值是否为0xC2C2C2C2
  // NRF24L01_TX_ADDR的复位值是否为0xE7E7E7E7
  // 如果正确证明SPI读写没有问题，反之亦然
  NRF24L01_CheckID();
    
  Temp = NRF24L01_WriteReg(NRF24L01_FLUSE_TX,0xFF);
  
  Temp = NRF24L01_WriteBuf(NRF24L01_WRITE_REG + NRF24L01_TX_ADDR,TX_ADDRESS,NRF24L01_TX_ADR_LEN);
  
  Temp = NRF24L01_WriteReg(NRF24L01_WRITE_REG + NRF24L01_EN_AA, 0x01);       //频道0自动,ACK应答允许
  Temp = NRF24L01_WriteReg(NRF24L01_WRITE_REG + NRF24L01_EN_RXADDR, 0x01);   //允许接收地址只有频道0      
  Temp = NRF24L01_WriteReg(NRF24L01_WRITE_REG + NRF24L01_SETUP_RETR, 0x1a);  //设置自动重发时间和次数：500us + 86us, 10 retrans...
  Temp = NRF24L01_WriteReg(NRF24L01_WRITE_REG + NRF24L01_RF_CH, 40);         //设置信道工作为2.4GHZ，收发必须一致
  Temp = NRF24L01_WriteReg(NRF24L01_WRITE_REG + NRF24L01_RF_SETUP, 0x0F);    //设置发射速率为1MHZ，发射功率为最大值0dB
  Temp = NRF24L01_WriteReg(NRF24L01_WRITE_REG + NRF24L01_CONFIG, 0x0E);      //使能CRC校验，使能TX，并且上电
  
  NRF24L01_CE_H;
  
  NRF24L01_DelayUs(2000);//等待Nrf24L01发送模式初始化完毕
  
  return Temp;
}

/*
 * LPLD_Nrf24L01_RxModeInit
 * Nrf24L01接收模式初始化
 * 
 * 参数:
 *    无
 *
 * 输出:
 *    Nrf24L01寄存器返回状态
 */
uint8 NRF24L01_RxModeInit(void) 
{ 
  uint8 Temp;
  
  NRF24L01_CE_L; 
  // 芯片上电复位后
  // 分别读取NRF24L01_RX_ADDR_P0、NRF24L01_RX_ADDR_P1、NRF24L01_TX_ADDR的值
  // 查看以下数据
  // NRF24L01_RX_ADDR_P0的复位值是否为0xE7E7E7E7
  // NRF24L01_RX_ADDR_P1的复位值是否为0xC2C2C2C2
  // NRF24L01_TX_ADDR的复位值是否为0xE7E7E7E7
  // 如果正确证明SPI读写没有问题，反之亦然
  NRF24L01_CheckID();
 
  Temp = NRF24L01_WriteBuf(NRF24L01_WRITE_REG + NRF24L01_RX_ADDR_P0, RX_ADDRESS, NRF24L01_RX_ADR_LEN); 
  Temp = NRF24L01_WriteReg(NRF24L01_WRITE_REG + NRF24L01_RX_PW_P0, NRF24L01_PLOAD_LEN);
  
  Temp = NRF24L01_WriteReg(NRF24L01_WRITE_REG + NRF24L01_EN_AA, 0x01);      //频道0自动,ACK应答允许      
  Temp = NRF24L01_WriteReg(NRF24L01_WRITE_REG + NRF24L01_EN_RXADDR, 0x01);  //允许接收地址只有频道0 
  Temp = NRF24L01_WriteReg(NRF24L01_WRITE_REG + NRF24L01_SETUP_RETR, 0x1a); // 500us + 86us, 10 retrans... 	
  Temp = NRF24L01_WriteReg(NRF24L01_WRITE_REG + NRF24L01_RF_CH, 40);        //设置信道工作为2.4GHZ，收发必须一致  
  Temp = NRF24L01_WriteReg(NRF24L01_WRITE_REG + NRF24L01_RF_SETUP, 0x0F);   //设置发射速率为1MHZ，发射功率为最大值0dB
  Temp = NRF24L01_WriteReg(NRF24L01_WRITE_REG + NRF24L01_CONFIG, 0x0F);     //使能CRC校验，使能RX，并且上电 
  
  NRF24L01_CE_H;  
  
  NRF24L01_DelayUs(2000);//等待Nrf24L01接收模式初始化完毕
  
  return Temp;
}

/*
 * LPLD_Nrf24L01_SendFrame
 * Nrf24L01发送数据帧
 * 
 * 参数:
 *    *data--指向用户发送数据缓冲区
 *    len--发送数据长度，以字节为单位
 *
 * 输出:
 *    1 发送成功
 *    0 发送失败
 */
uint8 NRF24L01_SendFrame(uint8 *data, uint8 len)
{
  uint8 Status;
  
  NRF24L01_CE_L; //拉低CE，让Nrf24l01进入Standby-I模式
  NRF24L01_WriteBuf(NRF24L01_WRITE_REG + NRF24L01_RX_ADDR_P0, TX_ADDRESS, NRF24L01_RX_ADR_LEN); // 装载接收端地址
  NRF24L01_WriteBuf(NRF24L01_WR_TX_PLOAD, data, len); 			   // 装载数据	
  NRF24L01_CE_H;  
  
  while (1)
  {
    Status = NRF24L01_ReadReg(NRF24L01_STATUS);           //读取状态寄存器
    NRF24L01_WriteReg(NRF24L01_WRITE_REG + NRF24L01_STATUS, Status);  //清空状态奇存器
    if (Status & NRF24L01_STATUS_TX_DS)                //如果发送完毕，并接收到ACK，该标志位置位
    {
      NRF24L01_WriteReg(NRF24L01_FLUSE_TX,0xFF); //清空发送缓冲区
      Status = 1;
      break;
    }
    else if (Status & NRF24L01_STATUS_MAX_RT)//如果达到多次发送的最大值，且期间没有接收到ACK
    { 
      Status = 0;
      break;
    }
  }
  return Status;
}

/*
 * LPLD_Nrf24L01_RecvFrame
 * Nrf24L01接收数据帧
 * 
 * 参数:
 *    *data--指向用户接收数据缓冲区
 *    len--发送数据长度，以字节为单位
 *
 * 输出:
 *    无
 */
void NRF24L01_RecvFrame(uint8 *data, uint8 len)
{
  uint8 Status;
  Status = NRF24L01_ReadReg(NRF24L01_STATUS);                  
  NRF24L01_ReadBuf(NRF24L01_RD_RX_PLOAD,data,len);
  NRF24L01_WriteReg(NRF24L01_WRITE_REG+NRF24L01_STATUS,Status);
  Status = 0;
}

/*
* LPLD_Nrf24L01_CheckID   
* 用于检查复位后的NRF24L01_RX_ADDR_P1、NRF24L01_RX_ADDR_P0、NRF24L01_TX_ADDR的值是否正确
* 此值用于反映SPI总线是否配置正确
*
* 输出:
*     Nrf24L01寄存器返回状态
*/
static uint8 NRF24L01_CheckID(void)
{
  uint8 Temp;
  Temp = NRF24L01_ReadBuf(NRF24L01_READ_REG + NRF24L01_RX_ADDR_P1, NRF24L01_RX_ADDR_P1_RESET_BUF,5);
  asm("nop");
  Temp = NRF24L01_ReadBuf(NRF24L01_READ_REG + NRF24L01_TX_ADDR, NRF24L01_TX_ADDR_RESET_BUF,5);
  asm("nop");
  Temp = NRF24L01_ReadBuf(NRF24L01_READ_REG + NRF24L01_RX_ADDR_P0, NRF24L01_RX_ADDR_P0_RESET_BUF,5);
  asm("nop"); 
  return Temp;
}

/*
* LPLD_Nrf24L01_WriteBuf
* 向nrf24l01的缓冲区中写入数据，并返回寄存器的状态
* 参数:   
*     reg--nrf24l01寄存器
*     *pbuf--指向存储写入数据的首地址
*     len--存取区的长度，单位字节
* 返回值: 
*     返回当前寄存器的状态
*/
static uint8 NRF24L01_WriteBuf(uint8 reg,uint8 *pbuf ,uint8 len)
{
  uint8 status,i;
  
  status = SPI_Master_WriteRead(NRF24L01_SPIX, reg, SPI_PCS0, SPI_PCS_ASSERTED); //帧发送完毕后PCS保持低电平
  
  for (i = 0; i < len; i++) 
  {       
    if (i == (len-1))
    {
        SPI_Master_WriteRead(NRF24L01_SPIX,*pbuf++,SPI_PCS0,SPI_PCS_INACTIVE);//帧发送完毕后PCS变成高电平
    }
    else
    {
        SPI_Master_WriteRead(NRF24L01_SPIX,*pbuf++,SPI_PCS0,SPI_PCS_ASSERTED);
    }
  }
  return status;
} 

/*
* LPLD_Nrf24L01_ReadBuf
* 向nrf24l01的缓冲区中读取数据，并返回寄存器的状态
* 参数:
*     reg--nrf24l01寄存器
*     *pBUF--指向存储读出数据的首地址
*     len--存取区的长度，单位字节
* 返回值：
*     返回当前寄存器的状态
*/
static uint8 NRF24L01_ReadBuf(uint8 reg,uint8 *pbuf,uint8 len)
{
  uint8 Status,i;
  
  Status = SPI_Master_WriteRead(NRF24L01_SPIX,reg,SPI_PCS0,SPI_PCS_ASSERTED); 
  
  for ( i = 0; i < len; i++)
  {
    if (i == (len-1))
    {
      pbuf[i] = SPI_Master_WriteRead(NRF24L01_SPIX,0xFF,SPI_PCS0,SPI_PCS_INACTIVE);
    }
    else
    {
      pbuf[i] = SPI_Master_WriteRead(NRF24L01_SPIX,0xFF,SPI_PCS0,SPI_PCS_ASSERTED);
    }
  }
  return Status ;
}

/*
* LPLD_Nrf24L01_WriteReg
* 向nrf24l01寄存器中写入数据，并返回寄存器的状态/向nrf24l01写入指令
* 参数:
*     reg--nrf24l01寄存器
*     value--寄存器的数值
* 返回值: 
*     返回当前寄存器的状态
*/
static uint8 NRF24L01_WriteReg(uint8 reg,uint8 value)
{
  uint8 Status;
  Status = SPI_Master_WriteRead(NRF24L01_SPIX,reg,SPI_PCS0,SPI_PCS_ASSERTED);    
  SPI_Master_WriteRead(NRF24L01_SPIX,value,SPI_PCS0,SPI_PCS_INACTIVE);  
  return Status;
}

/*
* LPLD_Nrf24L01_ReadReg
* 向nrf24l01寄存器中写入数据，并返回寄存器的状态/向nrf24l01写入指令
* 参数:
*     reg--nrf24l01寄存器
* 返回值: 
*     返回当前寄存器的数值
*/
static uint8 NRF24L01_ReadReg(uint8 reg)
{
  uint8 temp;
  SPI_Master_WriteRead(NRF24L01_SPIX,reg,SPI_PCS0,SPI_PCS_ASSERTED);  
  temp = SPI_Master_WriteRead(NRF24L01_SPIX,0xFF,SPI_PCS0,SPI_PCS_INACTIVE); 
  return temp;
}

/*
* Nrf24L01_DelayUs
* 用于延时 最小10us
* 参数:   
*     t 延时
* 返回值: 
*     无      
*/
static void NRF24L01_DelayUs(uint16 t)
{
  uint16 i;
  for (i = 0; i < t; i++)
  {
      asm("nop");
  }
}
