/***************************************************************************
* File:         UART.h
* Summary:      UART底层模块相关函数
* Version:      V0.1
* Author:       Leopold & 拉普兰德[LPLD] 
* Date:         2019/1/27
* Change Logs:
*       Date:   2019/1/27          
*       Version:V0.1         
*       Author: Leopold       
*       Notes:  修改自:拉普兰德[LPLD]                                       
***************************************************************************/

#ifndef __UART_H__
#define __UART_H__

//UART模块FIFO设置
typedef struct 
{
  boolean FIFO_Enable;
  boolean FIFO_BufFlow_IntEnable;
  uint8   FIFO_WaterMark;
} UART_FIFO_Config_T;

//UART模块中断回调函数类型
typedef void (*UART_ISR_CALLBACK)(void);

//UART模块初始化结构体，用于配置UART各项参数
typedef struct
{
  /*
    描述：选择UARTx
    取值：UART0~UART5
    初始化：不必须初始化，默认值UART5
  */
  UART_Type *UART_Uartx;        
      
  /*
    描述：设置UARTx波特率
    推荐取值：4800、9600、19200、38400、115200
    初始化：不必须初始化，默认值9600
  */
  uint32 UART_BaudRate;  
  
  /*
    描述：发送引脚，每个UARTx对应不同的TxPin
    取值：
      UART0-PTA2、PTA14、*PTB17
      UART1-PTE0、*PTC4
      UART2-*PTD3
      UART3-PTE4、PTB11、*PTC17
      UART4-PTE24、*PTC15
      UART5-PTD9、*PTE8
    初始化：不必须初始化、默认值为带*的引脚
  */
  PortPinsEnum_Type UART_TxPin;   
  
  /*
    描述：接收引脚，每个UARTx对应不同的RxPin
    取值：
      UART0-PTA1、PTA15、*PTB16
      UART1-PTE1、*PTC3
      UART2-*PTD2
      UART3-PTE5、PTB10、*PTC16
      UART4-PTE25、*PTC14
      UART5-PTD8、*PTE9
    初始化：不必须初始化、默认值为带*的引脚
  */
  PortPinsEnum_Type UART_RxPin; 
  
  /*
    描述：使能接收中断
    取值：
      TRUE-使能
      FALSE-禁用
    初始化：不必须初始化，默认值FALSE
  */
  boolean UART_RxIntEnable;
  
  /*
    描述：使能发送中断
    取值：
      TRUE-使能
      FALSE-禁用
    初始化：不必须初始化，默认值FALSE
  */
  boolean UART_TxIntEnable;
  
  /*
    描述：使能发送DMA请求
    取值：
      TRUE-使能
      FALSE-禁用
    初始化：不必须初始化，默认值FALSE
  */
  boolean UART_TxDMAEnable;
  
  /*
    描述：使能接收DMA请求
    取值：
      TRUE-使能
      FALSE-禁用
    初始化：不必须初始化，默认值FALSE
  */
  boolean UART_RxDMAEnable;
  
  /*
    描述：配置发送FIFO
    取值：
      FIFO_Enable （使能FIFO）
        TRUE-使能
        FALSE-禁用
      FIFO_BufFlow_IntEnable（使能FIFO buffer overflow 或者 underflow 中断 ）
        TRUE-使能
        FALSE-禁用
      FIFO_WaterMark （设置 FIFO waterMark，不能超过FIFO Size）
        0 -- FIFO Size
    初始化：
      FIFO_Enable，默认值FALSE
      FIFO_BufFlow_IntEnable，默认值FALSE
      FIFO_WaterMark，默认值0
  */
  UART_FIFO_Config_T TxFIFO;
  
  /*
    描述：配置接收FIFO
    取值：
      FIFO_Enable （使能FIFO）
        TRUE-使能
        FALSE-禁用
      FIFO_BufFlow_IntEnable（使能FIFO buffer overflow 或者 underflow 中断 ）
        TRUE-使能
        FALSE-禁用
      FIFO_WaterMark （设置 FIFO waterMark，不能超过FIFO Size）
        0 -- FIFO Size
    初始化：
      FIFO_Enable，默认值FALSE
      FIFO_BufFlow_IntEnable，默认值FALSE
      FIFO_WaterMark，默认值0
  */
  UART_FIFO_Config_T RxFIFO;
  
  /*
    描述：接收中断回调函数
    取值：函数必须为无返回值,无参数(eg. void isr(void);)
    初始化：不必须初始化、如未初始化则不会触发中断
  */
  UART_ISR_CALLBACK UART_RxIsr; 
  
  /*
    描述：发送中断回调函数
    取值：函数必须为无返回值,无参数(eg. void isr(void);)
    初始化：不必须初始化、如未初始化则不会触发中断
  */
  UART_ISR_CALLBACK UART_TxIsr; 
} UART_InitTypeDef;


//初始化UART通道、波特率、发送接收引脚
void UART_Init(UART_InitTypeDef);

//UART获取一个字符
int8 UART_GetChar(UART_Type *);

//检查是否接收到字符
int32 UART_GetChar_Present(UART_Type *);

//UART发送一个字符
void UART_PutChar(UART_Type *, int8);

//UART发送字符数组
void UART_PutCharArr(UART_Type *, int8*, int32);

//UART中断使能
void UART_EnableIrq(UART_InitTypeDef);

//UART中断禁用
void UART_DisableIrq(UART_InitTypeDef);

//UART清空接收FIFO
void UART_RxFIFO_Flush(UART_Type *uartx);

//UART清空发送FIFO
void UART_TxFIFO_Flush(UART_Type *uartx);
#endif  /* __UART_H__ */