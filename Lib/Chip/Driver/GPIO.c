/***************************************************************************
* File:         GPIO.c
* Summary:      GPIO底层模块相关函数
* Version:      V0.1
* Author:       Leopold & 拉普兰德[LPLD]
* Date:         2019/1/27
* Change Logs:
*       Date:   2019/1/27          
*       Version:V0.1         
*       Author: Leopold       
*       Notes:  修改自:拉普兰德[LPLD]
***************************************************************************/

#include "common.h"
#include "GPIO.h"


//用户自定义中断服务函数数组
GPIO_ISR_CALLBACK GPIO_ISR[5];

PORT_Type* PORTX[PTX_MAX] = {PORTA, PORTB, PORTC, PORTD, PORTE};
GPIO_Type* GPIOX[PTX_MAX] = {PTA, PTB, PTC, PTD, PTE};
/*
 * GPIO_Init
 * GPIO通用初始化函数
 * 
 * 参数:
 *    gpio_init_structure--GPIO初始化结构体,具体定义见GPIO_InitTypeDef
 *
 * 输出:
 *    0--配置错误
 *    1--配置成功
 */
uint8 GPIO_Init(GPIO_InitTypeDef gpio_init_structure)
{
  uint8 i;
  PORT_Type *portx;
  uint32 pcr = PORT_PCR_MUX(1)| gpio_init_structure.GPIO_PinControl; 
  GPIO_Type *ptx = gpio_init_structure.GPIO_PTx;
  uint32 pins = gpio_init_structure.GPIO_Pins;
  uint8 dir = gpio_init_structure.GPIO_Dir;
  uint8 output = gpio_init_structure.GPIO_Output;
  GPIO_ISR_CALLBACK isr_func = gpio_init_structure.GPIO_Isr;
  
  //参数检查
  ASSERT( ptx <= PTE);                  //判断端口
  ASSERT( dir <= 1 );                   //判断方向
  ASSERT( output <= 1 );                //判断输出电平状态
  
  if(ptx == PTA)
  {
    portx = PORTA;  
    i = 0;
  }
  else if(ptx == PTB) 
  {
    portx = PORTB;
    i = 1;
  }
  else if(ptx == PTC) 
  {
    portx = PORTC;
    i = 2;
  }
  else if(ptx == PTD) 
  {
    portx = PORTD;
    i = 3; 
  }
  else if(ptx == PTE) 
  {
    portx = PORTE;
    i = 4;
  }
  else
    return 0;
  
  //输入或输出设置
  if(dir==DIR_OUTPUT)
  {
    ptx->PDDR |= pins;
    //设置初始输出
    if(output==OUTPUT_H)
    {
      ptx->PSOR = pins; 
    }
    else
    {
      ptx->PCOR = pins;
    }
  }
  else
  { 
    ptx->PDDR &= ~(pins);
  }
  
  //配置所选引脚的控制寄存器
  for(uint8 i=0; i<32; i++)
  {
    if(pins&(1ul<<i))
    {
      portx->PCR[i] = pcr;
    }
  }

  if(isr_func!=NULL)
    GPIO_ISR[i] = isr_func;
  
  return 1;
}


/*!
 *  @brief      PORT初始化
 *  @param      PortPinsEnum_Type    端口
 *  @param      cfg     端口属性配置，如触发选项和上拉下拉选项
 *  @since      v5.0
 *  @note       与port_init_NoALT不同的是，此函数需要配置 MUX 复用功能，
                否则 MUX = ALT0
 *  Sample usage:       PORT_Init (PTA8, IRQ_RISING | PF | ALT1 | PULLUP );    //初始化 PTA8 管脚，上升沿触发中断，带无源滤波器，复用功能为GPIO ，上拉电阻
 */
void PORT_Init(PortPinsEnum_Type ptxn, uint32 cfg)
{
    SIM->SCGC5 |= (SIM_SCGC5_PORTA_MASK << PTX(ptxn));          //开启PORTx端口
    
    ((PORT_Type *)(PORTX_BASE(ptxn)))->ISFR = (1 << PTn(ptxn)); // 清空标志位

    ((PORT_Type *)(PORTX_BASE(ptxn)))->PCR[PTn(ptxn)] = cfg;    // 复用功能 , 确定触发模式 ,开启上拉或下拉电阻                           
}

/*!
 *  @brief      PORT初始化
 *  @param      PTxn    端口
 *  @param      cfg     端口属性配置，如触发选项和上拉下拉选项
 *  @since      v5.0
 *  @note       与port_init不同的是，此函数不需要配置 MUX 复用功能（即使配置了也不生效），
                MUX 保留 为原先寄存器配置的值
 *  Sample usage:       port_init_NoALT (PTA8, IRQ_RISING | PF | PULLUP );    //初始化 PTA8 管脚，上升沿触发中断，带无源滤波器，保留原先复用功能，上拉电阻
 */
void  PORT_Init_NoALT(PortPinsEnum_Type ptxn, uint32 cfg)
{
    SIM->SCGC5 |= (SIM_SCGC5_PORTA_MASK << PTX(ptxn));          //开启PORTx端口

    ((PORT_Type *)(PORTX_BASE(ptxn)))->ISFR = (1 << PTn(ptxn)); // 清空标志位
    
    //清空cfg里的MUX，加载寄存器里的MUX
    cfg &= ~PORT_PCR_MUX_MASK;                      //清了MUX 字段（即不需要配置ALT，保持原来的ALT）
    cfg |=  (((PORT_Type *)(PORTX_BASE(ptxn)))->PCR[PTn(ptxn)] & PORT_PCR_MUX_MASK);    //读取寄存器里配置的 MUX

    ((PORT_Type *)(PORTX_BASE(ptxn)))->PCR[PTn(ptxn)] = cfg;    // 复用功能 , 确定触发模式 ,开启上拉或下拉电阻                           
}

/*!
 *  @brief      初始化gpio
 *  @param      PTxn    端口
 *  @param      cfg     引脚方向,0=输入,1=输出
 *  @param      data    输出初始状态,0=低电平,1=高电平 （对输入无效）
 *  @since      v5.0
 *  Sample usage:       gpio_init (PTA8, DIR_INPUT, 0);    //初始化 PTA8 管脚为输入
 */
void GPIO_Init_Pin(PortPinsEnum_Type ptxn, uint8 cfg, uint8 data)
{
    //复用管脚为GPIO功能
    PORT_Init(ptxn, ALT1);

    //端口方向控制输入还是输出
    if (cfg == DIR_INPUT)
    {
        //设置端口方向为输入
        ((GPIO_Type *)(GPIOX_BASE(ptxn)))->PDDR &= ~(1 << PTn(ptxn));           // GPIO PDDR 管脚号 清0，即对应管脚配置为端口方向输入
    }
    else
    {
        //设置端口方向为输出
        ((GPIO_Type *)(GPIOX_BASE(ptxn)))->PDDR |= (1 << PTn(ptxn));            // GPIO PDDR 管脚号 置1，即对应管脚配置为端口方向输出

        //端口输出数据
        if(data == 0)
        {
            ((GPIO_Type *)(GPIOX_BASE(ptxn)))->PDOR &= ~(1 << PTn(ptxn));       // GPIO PDOR 管脚号 清0，即对应管脚配置为端口输出低电平
        }
        else
        {
            ((GPIO_Type *)(GPIOX_BASE(ptxn)))->PDOR |= (1 << PTn(ptxn));        // GPIO PDOR 管脚号 置1，即对应管脚配置为端口输出高电平
        }
    }
}


/*
 * GPIO_EnableIrq
 * 使能GPIO外部中断
 * 
 * 参数:
 *    gpio_init_structure--GPIO初始化结构体,具体定义见GPIO_InitTypeDef
 *
 * 输出:
 *    0--配置错误
 *    1--配置成功
 *
 */
uint8 GPIO_EnableIrq(GPIO_InitTypeDef gpio_init_structure)
{
  uint8 i;
  GPIO_Type *ptx = gpio_init_structure.GPIO_PTx;
  
  //参数检查
  ASSERT( ptx <= PTE);                  //判断端口
  
  if(ptx == PTA)
    i = 0;
  else if(ptx == PTB) 
    i = 1;
  else if(ptx == PTC) 
    i = 2;
  else if(ptx == PTD) 
    i = 3;
  else
    i = 4;

  enable_irq((IRQn_Type)(i + PORTA_IRQn));
  
  return 1;
}

/*
 * GPIO_DisableIrq
 * 禁用GPIO外部中断
 * 
 * 参数:
 *    gpio_init_structure--GPIO初始化结构体,具体定义见GPIO_InitTypeDef
 *
 * 输出:
 *    0--配置错误
 *    1--配置成功
 *
 */
uint8 GPIO_DisableIrq(GPIO_InitTypeDef gpio_init_structure)
{
  uint8 i;
  GPIO_Type *ptx = gpio_init_structure.GPIO_PTx;
  
  //参数检查
  ASSERT( ptx <= PTE);                  //判断端口
  
  if(ptx == PTA)
    i = 0;
  else if(ptx == PTB) 
    i = 1;
  else if(ptx == PTC) 
    i = 2;
  else if(ptx == PTD) 
    i = 3;
  else
    i = 4;
  
  disable_irq((IRQn_Type)(i + PORTA_IRQn));
  
  return 1;
}

/*
 * GPIO_Ouptut
 * 设置GPIO端口0~31位的输出
 * 
 * 参数:
 *    ptx--端口号
 *      |__PTA        --Port A
 *      |__PTB        --Port B
 *      |__PTC        --Port C
 *      |__PTD        --Port D
 *      |__PTE        --Port E
 *    data32--输出数据
 *      |__0x00000000~0xFFFFFFFF--低到高代表GPIO口的第0~31位数据
 *
 * 输出:
 *
 */
__INLINE void GPIO_Output(GPIO_Type *ptx, uint32 data32)
{ 
  ptx->PDOR = data32;
}

/*
 * GPIO_Ouptut_b
 * 设置GPIO端口一位的输出
 * 
 * 参数:
 *    ptx--端口号
 *      |__PTA        --Port A
 *      |__PTB        --Port B
 *      |__PTC        --Port C
 *      |__PTD        --Port D
 *      |__PTE        --Port E
 *    lsb_num--端口引脚位数编号
 *      |__0~31       --GPIO口的第0~31位
 *    data1--输出数据
 *      |__0          --输出低电平
 *      |__1          --输出高电平
 *
 * 输出:
 *
 */
__INLINE void GPIO_Output_b(GPIO_Type *ptx, uint32 lsb_num, uint8 data1)
{
  ptx->PDOR = (ptx->PDOR & ~(0x01L<<lsb_num)) | (uint32)data1<<lsb_num;
}

/*
 * GPIO_Ouptut_8b
 * 设置GPIO端口8位的输出
 * 
 * 参数:
 *    ptx--端口号
 *      |__PTA        --Port A
 *      |__PTB        --Port B
 *      |__PTC        --Port C
 *      |__PTD        --Port D
 *      |__PTE        --Port E
 *    lsb_num--代表8个端口引脚的最低位数编号
 *      |__0~24       --GPIO口的第0~24位
 *    data8--输出数据
 *      |__0x00~0xFF--代表GPIO口输出的8位数据
 *
 * 输出:
 *
 */
__INLINE void GPIO_Output_8b(GPIO_Type *ptx, uint8 lsb_num, uint8 data8)
{
  ptx->PDOR = (ptx->PDOR & ~(0xFFL<<lsb_num)) | (uint32)data8<<lsb_num;
}

/*
 * GPIO_Toggle
 * 设置GPIO端口0~31的电平翻转
 * 
 * 参数:
 *    ptx--端口号
 *      |__PTA        --Port A
 *      |__PTB        --Port B
 *      |__PTC        --Port C
 *      |__PTD        --Port D
 *      |__PTE        --Port E
 *    data32--翻转数据
 *      |__0x00000000~0xFFFFFFFF--低到高代表GPIO口的第0~31位的翻转，1为反转，0为保持不变。
 *
 * 输出:
 *
 */
__INLINE void GPIO_Toggle(GPIO_Type *ptx, uint32 data32)
{  
  ptx->PTOR = data32;
}

/*
 * GPIO_Toggle_b
 * 设置GPIO端口一位的翻转
 * 
 * 参数:
 *    ptx--端口号
 *      |__PTA        --Port A
 *      |__PTB        --Port B
 *      |__PTC        --Port C
 *      |__PTD        --Port D
 *      |__PTE        --Port E
 *    lsb_num--端口引脚位数编号
 *      |__0~31       --GPIO口的第0~31位
 *
 * 输出:
 *
 */
__INLINE void GPIO_Toggle_b(GPIO_Type *ptx, uint8 lsb_num)
{  
  ptx->PTOR = 0x01L<<lsb_num;
}

/*
 * GPIO_Toggle_8b
 * 设置GPIO端口8位的翻转
 * 
 * 参数:
 *    ptx--端口号
 *      |__PTA        --Port A
 *      |__PTB        --Port B
 *      |__PTC        --Port C
 *      |__PTD        --Port D
 *      |__PTE        --Port E
 *    lsb_num--代表8个端口引脚的最低位数编号
 *      |__0~24       --GPIO口的第0~24位
 *    data8--输出数据
 *      |__0x00~0xFF--代表GPIO口输出的8位数据
 *
 * 输出:
 *
 */
__INLINE void GPIO_Toggle_8b(GPIO_Type *ptx, uint8 lsb_num, uint8 data8)
{
  ptx->PTOR = (uint32)data8<<lsb_num;
}

/*
 * GPIO_Input
 * 取得GPIO口0~31位的数据
 * 
 * 参数:
 *    ptx--端口号
 *      |__PTA        --Port A
 *      |__PTB        --Port B
 *      |__PTC        --Port C
 *      |__PTD        --Port D
 *      |__PTE        --Port E
 *
 * 输出:
 *    指定GPIO口的32位输入
 *
 */
__INLINE uint32 GPIO_Input(GPIO_Type *ptx)
{
  return ptx->PDIR;
}

/*
 * GPIO_Input_b
 * 取得GPIO口某一位的数据
 * 
 * 参数:
 *    ptx--端口号
 *      |__PTA        --Port A
 *      |__PTB        --Port B
 *      |__PTC        --Port C
 *      |__PTD        --Port D
 *      |__PTE        --Port E
 *    lsb_num--端口引脚位数编号
 *      |__0~31       --GPIO口的第0~31位
 *
 * 输出:
 *    指定GPIO口的指定位数的电平
 *
 */
__INLINE uint8 GPIO_Input_b(GPIO_Type *ptx, uint8 lsb_num)
{  
  return (uint8)((ptx->PDIR>>lsb_num)&0x01L);
}

/*
 * GPIO_Input_8b
 * 取得GPIO端口8位数据的输入
 * 
 * 参数:
 *    ptx--端口号
 *      |__PTA        --Port A
 *      |__PTB        --Port B
 *      |__PTC        --Port C
 *      |__PTD        --Port D
 *      |__PTE        --Port E
 *    lsb_num--代表8个端口引脚的最低位数编号
 *      |__0~24       --GPIO口的第0~24位
 *
 * 输出:
 *    指定GPIO口的8位数据的电平
 *
 */
__INLINE uint8 GPIO_Input_8b(GPIO_Type *ptx, uint8 lsb_num)
{
  return (uint8)((ptx->PDIR>>lsb_num)&0xFFL);
}


/*!
 *  @brief      设置引脚数据方向
 *  @param      PTxn    端口
 *  @param      cfg     引脚方向,0=输入,1=输出
 *  @since      v5.0
 *  Sample usage:       gpio_ddr (PTA8, GPI);    //设置 PTA8 管脚为输入
 */
void GPIO_DDR(PortPinsEnum_Type ptxn, uint8 cfg)
{
    //端口方向控制输入还是输出
    if (cfg == DIR_INPUT)
    {
        //设置端口方向为输入
        ((GPIO_Type *)(GPIOX_BASE(ptxn)))->PDDR &= ~(1 << PTn(ptxn));           // GPIO PDDR 管脚号 清0，即对应管脚配置为端口方向输入
    }
    else
    {
        //设置端口方向为输出
        ((GPIO_Type *)(GPIOX_BASE(ptxn)))->PDDR |= (1 << PTn(ptxn));            // GPIO PDDR 管脚号 置1，即对应管脚配置为端口方向输出
    }
}

/*!
 *  @brief      设置引脚状态
 *  @param      PTxn    端口
 *  @param      data    输出初始状态,0=低电平,1=高电平 （对输入无效）
 *  @since      v5.0
 *  @warning    务必保证数据方向为输出（DEBUG模式下，有断言进行检测）
 *  Sample usage:       gpio_set (PTA8, 1);    // PTA8 管脚 输出 1
 */
void GPIO_Set (PortPinsEnum_Type ptxn, uint8 data)
{
    ASSERT( BIT_GET( ((GPIO_Type *)(GPIOX_BASE(ptxn)))->PDDR, PTn(ptxn)) == DIR_OUTPUT ); // 断言，检测 输出方向是否为输出
                                                                            // 获取 GPIO PDDR 管脚号 ，比较是否为输出

    //端口输出数据
    if(data == 0)
    {
        ((GPIO_Type *)(GPIOX_BASE(ptxn)))->PDDR &= ~(1 << PTn(ptxn));   // GPIO PDOR 管脚号 清0，即对应管脚配置为端口输出低电平
    }
    else
    {
        ((GPIO_Type *)(GPIOX_BASE(ptxn)))->PDDR  |= (1 << PTn(ptxn));   // GPIO PDOR 管脚号 置1，即对应管脚配置为端口输出高电平
    }
}


/*!
 *  @brief      反转引脚状态
 *  @param      PTxn    端口
 *  @since      v5.0
 *  @warning    务必保证数据方向为输出（DEBUG模式下，有断言进行检测）
 *  Sample usage:       gpio_turn (PTA8);    // PTA8 管脚 输出 反转
 */
void GPIO_Turn(PortPinsEnum_Type ptxn)
{
    ASSERT( BIT_GET( ((GPIO_Type *)(GPIOX_BASE(ptxn)))->PDDR, PTn(ptxn)) == DIR_OUTPUT ); // 断言，检测 输出方向是否为输出
                                                                            // 获取 GPIO PDDR 管脚号 ，比较是否为输出

    ((GPIO_Type *)(GPIOX_BASE(ptxn)))->PTOR = 1 << (PTn(ptxn ));              // GPIO PTOR ptxn 置1，其他清0 ，即对应管脚配置为端口输出反转，其他位不变
                                                                            // 此处不能用 BIT_SET 这个宏来置1 ，因为必须保证其他位 不变，其他位直接清0即可
}

/*!
 *  @brief      读取引脚输入状态
 *  @param      PTxn    端口
 *  @return     管脚的状态，1为高电平，0为低电平
 *  @since      v5.0
 *  @warning    务必保证数据方向为输入（DEBUG模式下，有断言进行检测）
 *  Sample usage:       uint8 pta8_data = gpio_get (PTA8);    // 获取 PTA8 管脚 输入电平
 */
uint8 GPIO_Get(PortPinsEnum_Type ptxn)
{
    ASSERT( BIT_GET( ((GPIO_Type *)(GPIOX_BASE(ptxn)))->PDDR, PTn(ptxn)) == DIR_INPUT ); // 断言，检测 输出方向是否为输入
                                                                            // 获取 GPIO PDDR 管脚号 ，比较是否为输入
    return ((((GPIO_Type *)(GPIOX_BASE(ptxn)))->PDIR >> PTn(ptxn )) & 0x01);        // 获取 GPIO PDIR ptxn 状态，即读取管脚输入电平
}


__INLINE void GPIO_OUT_L(PortPinsEnum_Type ptxn)
{ 
    ((GPIO_Type *)(GPIOX_BASE(ptxn)))->PDOR &= ~(1 << PTn(ptxn));
}

__INLINE void GPIO_OUT_H(PortPinsEnum_Type ptxn) 
{
    ((GPIO_Type *)(GPIOX_BASE(ptxn)))->PDOR &= ~(1 << PTn(ptxn));
}

/*
 * PORTA--PORTE中断处理函数
 * 与启动文件startup_K60.s中的中断向量表关联
 * 用户无需修改，程序自动进入对应通道中断函数
 */
void PORTA_IRQHandler(void)
{
#if (UCOS_II > 0u)
  OS_CPU_SR  cpu_sr = 0u;
  OS_ENTER_CRITICAL(); //告知系统此时已经进入了中断服务子函数
  OSIntEnter();
  OS_EXIT_CRITICAL();
#endif
  
  //调用用户自定义中断服务
  GPIO_ISR[0](); 
  PORTA->ISFR =0xFFFFFFFF;
  
#if (UCOS_II > 0u)
  OSIntExit();          //告知系统此时即将离开中断服务子函数
#endif
}//           ;87:  PORT A interrupt

void PORTB_IRQHandler(void)
{
#if (UCOS_II > 0u)
  OS_CPU_SR  cpu_sr = 0u;
  OS_ENTER_CRITICAL(); //告知系统此时已经进入了中断服务子函数
  OSIntEnter();
  OS_EXIT_CRITICAL();
#endif
  
  //调用用户自定义中断服务
  GPIO_ISR[1](); 
  PORTB->ISFR =0xFFFFFFFF;
  
#if (UCOS_II > 0u)
  OSIntExit();          //告知系统此时即将离开中断服务子函数
#endif
}//           ;88:  PORT B interrupt

void PORTC_IRQHandler(void)
{
#if (UCOS_II > 0u)
  OS_CPU_SR  cpu_sr = 0u;
  OS_ENTER_CRITICAL(); //告知系统此时已经进入了中断服务子函数
  OSIntEnter();
  OS_EXIT_CRITICAL();
#endif
  
  //调用用户自定义中断服务
  GPIO_ISR[2](); 
  PORTC->ISFR =0xFFFFFFFF;
  
#if (UCOS_II > 0u)
  OSIntExit();          //告知系统此时即将离开中断服务子函数
#endif
}//           ;89:  PORT C interrupt

void PORTD_IRQHandler(void)
{
#if (UCOS_II > 0u)
  OS_CPU_SR  cpu_sr = 0u;
  OS_ENTER_CRITICAL(); //告知系统此时已经进入了中断服务子函数
  OSIntEnter();
  OS_EXIT_CRITICAL();
#endif
  
  //调用用户自定义中断服务
  GPIO_ISR[3](); 
  PORTD->ISFR =0xFFFFFFFF;
  
#if (UCOS_II > 0u)
  OSIntExit();          //告知系统此时即将离开中断服务子函数
#endif
}//           ;90:  PORT D interrupt

void PORTE_IRQHandler(void)
{
#if (UCOS_II > 0u)
  OS_CPU_SR  cpu_sr = 0u;
  OS_ENTER_CRITICAL(); //告知系统此时已经进入了中断服务子函数
  OSIntEnter();
  OS_EXIT_CRITICAL();
#endif
  
  //调用用户自定义中断服务
  GPIO_ISR[4](); 
  PORTE->ISFR =0xFFFFFFFF;
  
#if (UCOS_II > 0u)
  OSIntExit();          //告知系统此时即将离开中断服务子函数
#endif
}//           ;91:  PORT E interrupt