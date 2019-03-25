/***************************************************************************
* File:         GPIO.h
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

#ifndef __GPIO_H__
#define __GPIO_H__
/********************************************************************/

//端口引脚枚举定义
typedef enum PortPinsEnum
{
    /*  PTA端口    */ 
    PTA0,  PTA1,  PTA2,  PTA3,  PTA4,  PTA5,  PTA6,  PTA7,  PTA8,  PTA9,  PTA10, PTA11, PTA12, PTA13, PTA14, PTA15,
    PTA16, PTA17, PTA18, PTA19, PTA20, PTA21, PTA22, PTA23, PTA24, PTA25, PTA26, PTA27, PTA28, PTA29, PTA30, PTA31,

    /*  PTB端口    */ 
    PTB0,  PTB1,  PTB2,  PTB3,  PTB4,  PTB5,  PTB6,  PTB7,  PTB8,  PTB9,  PTB10, PTB11, PTB12, PTB13, PTB14, PTB15,
    PTB16, PTB17, PTB18, PTB19, PTB20, PTB21, PTB22, PTB23, PTB24, PTB25, PTB26, PTB27, PTB28, PTB29, PTB30, PTB31,

    /*  PTC端口    */
    PTC0,  PTC1,  PTC2,  PTC3,  PTC4,  PTC5,  PTC6,  PTC7,  PTC8,  PTC9,  PTC10, PTC11, PTC12, PTC13, PTC14, PTC15,
    PTC16, PTC17, PTC18, PTC19, PTC20, PTC21, PTC22, PTC23, PTC24, PTC25, PTC26, PTC27, PTC28, PTC29, PTC30, PTC31,

    /*  PTD端口    */
    PTD0,  PTD1,  PTD2,  PTD3,  PTD4,  PTD5,  PTD6,  PTD7,  PTD8,  PTD9,  PTD10, PTD11, PTD12, PTD13, PTD14, PTD15,
    PTD16, PTD17, PTD18, PTD19, PTD20, PTD21, PTD22, PTD23, PTD24, PTD25, PTD26, PTD27, PTD28, PTD29, PTD30, PTD31,

    /*  PTE端口    */
    PTE0,  PTE1,  PTE2,  PTE3,  PTE4,  PTE5,  PTE6,  PTE7,  PTE8,  PTE9,  PTE10, PTE11, PTE12, PTE13, PTE14, PTE15,
    PTE16, PTE17, PTE18, PTE19, PTE20, PTE21, PTE22, PTE23, PTE24, PTE25, PTE26, PTE27, PTE28, PTE29, PTE30, PTE31,  
} PortPinsEnum_Type;


/*! 枚举端口模块 */
typedef enum {
    PTXA, PTXB, PTXC, PTXD, PTXE, PTX_MAX,
} PTX_e;

//根据以上的定义，可以得出：PTx = PTxn / 32 ; PTn = PTxn & 31
#define PTX(PTxn)           ((PTxn)>>5)
#define PTn(PTxn)           ((PTxn)&0x1f)
#define PORTX_BASE(PTxn)     PORTX[PTX(PTxn)]       //PORT模块的地址
#define GPIOX_BASE(PTxn)     GPIOX[PTX(PTxn)]       //GPIO模块的地址


//GPIO引脚掩码定义
typedef enum GpioPinsEnum
{
  GPIO_Pin0       =0x00000001,
  GPIO_Pin1       =0x00000002,
  GPIO_Pin2       =0x00000004,
  GPIO_Pin3       =0x00000008,
  GPIO_Pin4       =0x00000010,
  GPIO_Pin5       =0x00000020,
  GPIO_Pin6       =0x00000040,
  GPIO_Pin7       =0x00000080,
  GPIO_Pin8       =0x00000100,
  GPIO_Pin9       =0x00000200,
  GPIO_Pin10      =0x00000400,
  GPIO_Pin11      =0x00000800,
  GPIO_Pin12      =0x00001000,
  GPIO_Pin13      =0x00002000,
  GPIO_Pin14      =0x00004000,
  GPIO_Pin15      =0x00008000,
  GPIO_Pin16      =0x00010000,
  GPIO_Pin17      =0x00020000,
  GPIO_Pin18      =0x00040000,
  GPIO_Pin19      =0x00080000,
  GPIO_Pin20      =0x00100000,
  GPIO_Pin21      =0x00200000,
  GPIO_Pin22      =0x00400000,
  GPIO_Pin23      =0x00800000,
  GPIO_Pin24      =0x01000000,
  GPIO_Pin25      =0x02000000,
  GPIO_Pin26      =0x04000000,
  GPIO_Pin27      =0x08000000,
  GPIO_Pin28      =0x10000000,
  GPIO_Pin29      =0x20000000,
  GPIO_Pin30      =0x40000000,
  GPIO_Pin31      =0x80000000,
  GPIO_Pin0_7     =0x000000FF,
  GPIO_Pin8_15    =0x0000FF00,
  GPIO_Pin16_23   =0x00FF0000,
  GPIO_Pin24_31   =0xFF000000
} GpioPinsEnum_Type;


//GPIO位带操作宏定义
//Pin方向控制
#define DDR(x, n)       BITBAND_REG(x->PDDR, n)
#define DDRAn(n)        DDR(PTA, n)
#define DDRBn(n)        DDR(PTB, n)
#define DDRCn(n)        DDR(PTC, n)
#define DDRDn(n)        DDR(PTD, n)
#define DDREn(n)        DDR(PTE, n)
#define DDRA0           DDRAn(0)        //PortA方向控制
#define DDRA1           DDRAn(1)
#define DDRA2           DDRAn(2)
#define DDRA3           DDRAn(3)
#define DDRA4           DDRAn(4)
#define DDRA5           DDRAn(5)
#define DDRA6           DDRAn(6)
#define DDRA7           DDRAn(7)
#define DDRA8           DDRAn(8)
#define DDRA9           DDRAn(9)
#define DDRA10          DDRAn(10)
#define DDRA11          DDRAn(11)
#define DDRA12          DDRAn(12)
#define DDRA13          DDRAn(13)
#define DDRA14          DDRAn(14)
#define DDRA15          DDRAn(15)
#define DDRA16          DDRAn(16)
#define DDRA17          DDRAn(17)
#define DDRA18          DDRAn(18)
#define DDRA19          DDRAn(19)
#define DDRA24          DDRAn(24)
#define DDRA25          DDRAn(25)
#define DDRA26          DDRAn(26)
#define DDRA27          DDRAn(27)
#define DDRA28          DDRAn(28)
#define DDRA29          DDRAn(29)
#define DDRB0           DDRBn(0)        //PortB方向控制
#define DDRB1           DDRBn(1)
#define DDRB2           DDRBn(2)
#define DDRB3           DDRBn(3)
#define DDRB4           DDRBn(4)
#define DDRB5           DDRBn(5)
#define DDRB6           DDRBn(6)
#define DDRB7           DDRBn(7)
#define DDRB8           DDRBn(8)
#define DDRB9           DDRBn(9)
#define DDRB10          DDRBn(10)
#define DDRB11          DDRBn(11)
#define DDRB16          DDRBn(16)
#define DDRB17          DDRBn(17)
#define DDRB18          DDRBn(18)
#define DDRB19          DDRBn(19)
#define DDRB20          DDRBn(20)
#define DDRB21          DDRBn(21)
#define DDRB22          DDRBn(22)
#define DDRB23          DDRBn(23)
#define DDRC0           DDRCn(0)        //PortC方向控制
#define DDRC1           DDRCn(1)
#define DDRC2           DDRCn(2)
#define DDRC3           DDRCn(3)
#define DDRC4           DDRCn(4)
#define DDRC5           DDRCn(5)
#define DDRC6           DDRCn(6)
#define DDRC7           DDRCn(7)
#define DDRC8           DDRCn(8)
#define DDRC9           DDRCn(9)
#define DDRC10          DDRCn(10)
#define DDRC11          DDRCn(11)
#define DDRC12          DDRCn(12)
#define DDRC13          DDRCn(13)
#define DDRC14          DDRCn(14)
#define DDRC15          DDRCn(15)
#define DDRC16          DDRCn(16)
#define DDRC17          DDRCn(17)
#define DDRC18          DDRCn(18)
#define DDRC19          DDRCn(19)
#define DDRD0           DDRDn(0)        //PortD方向控制
#define DDRD1           DDRDn(1)
#define DDRD2           DDRDn(2)
#define DDRD3           DDRDn(3)
#define DDRD4           DDRDn(4)
#define DDRD5           DDRDn(5)
#define DDRD6           DDRDn(6)
#define DDRD7           DDRDn(7)
#define DDRD8           DDRDn(8)
#define DDRD9           DDRDn(9)
#define DDRD10          DDRDn(10)
#define DDRD11          DDRDn(11)
#define DDRD12          DDRDn(12)
#define DDRD13          DDRDn(13)
#define DDRD14          DDRDn(14)
#define DDRD15          DDRDn(15)
#define DDRE0           DDREn(0)        //PortE方向控制
#define DDRE1           DDREn(1)
#define DDRE2           DDREn(2)
#define DDRE3           DDREn(3)
#define DDRE4           DDREn(4)
#define DDRE5           DDREn(5)
#define DDRE6           DDREn(6)
#define DDRE7           DDREn(7)
#define DDRE8           DDREn(8)
#define DDRE9           DDREn(9)
#define DDRE10          DDREn(10)
#define DDRE11          DDREn(11)
#define DDRE12          DDREn(12)
#define DDRE24          DDREn(24)
#define DDRE25          DDREn(25)
#define DDRE26          DDREn(26)
#define DDRE27          DDREn(27)
#define DDRE28          DDREn(28)
//Pin输出
#define PTxn_O(x, n)    BITBAND_REG(x->PDOR, n)
#define PTAn_O(n)       PTxn_O(PTA, n)
#define PTBn_O(n)       PTxn_O(PTB, n)
#define PTCn_O(n)       PTxn_O(PTC, n)
#define PTDn_O(n)       PTxn_O(PTD, n)
#define PTEn_O(n)       PTxn_O(PTE, n)
#define PTA0_O          PTAn_O(0)        //PortA输出
#define PTA1_O          PTAn_O(1)
#define PTA2_O          PTAn_O(2)
#define PTA3_O          PTAn_O(3)
#define PTA4_O          PTAn_O(4)
#define PTA5_O          PTAn_O(5)
#define PTA6_O          PTAn_O(6)
#define PTA7_O          PTAn_O(7)
#define PTA8_O          PTAn_O(8)
#define PTA9_O          PTAn_O(9)
#define PTA10_O         PTAn_O(10)
#define PTA11_O         PTAn_O(11)
#define PTA12_O         PTAn_O(12)
#define PTA13_O         PTAn_O(13)
#define PTA14_O         PTAn_O(14)
#define PTA15_O         PTAn_O(15)
#define PTA16_O         PTAn_O(16)
#define PTA17_O         PTAn_O(17)
#define PTA18_O         PTAn_O(18)
#define PTA19_O         PTAn_O(19)
#define PTA24_O         PTAn_O(24)
#define PTA25_O         PTAn_O(25)
#define PTA26_O         PTAn_O(26)
#define PTA27_O         PTAn_O(27)
#define PTA28_O         PTAn_O(28)
#define PTA29_O         PTAn_O(29)
#define PTB0_O          PTBn_O(0)        //PortB输出
#define PTB1_O          PTBn_O(1)
#define PTB2_O          PTBn_O(2)
#define PTB3_O          PTBn_O(3)
#define PTB4_O          PTBn_O(4)
#define PTB5_O          PTBn_O(5)
#define PTB6_O          PTBn_O(6)
#define PTB7_O          PTBn_O(7)
#define PTB8_O          PTBn_O(8)
#define PTB9_O          PTBn_O(9)
#define PTB10_O         PTBn_O(10)
#define PTB11_O         PTBn_O(11)
#define PTB16_O         PTBn_O(16)
#define PTB17_O         PTBn_O(17)
#define PTB18_O         PTBn_O(18)
#define PTB19_O         PTBn_O(19)
#define PTB20_O         PTBn_O(20)
#define PTB21_O         PTBn_O(21)
#define PTB22_O         PTBn_O(22)
#define PTB23_O         PTBn_O(23)
#define PTC0_O          PTCn_O(0)        //PortC输出
#define PTC1_O          PTCn_O(1)
#define PTC2_O          PTCn_O(2)
#define PTC3_O          PTCn_O(3)
#define PTC4_O          PTCn_O(4)
#define PTC5_O          PTCn_O(5)
#define PTC6_O          PTCn_O(6)
#define PTC7_O          PTCn_O(7)
#define PTC8_O          PTCn_O(8)
#define PTC9_O          PTCn_O(9)
#define PTC10_O         PTCn_O(10)
#define PTC11_O         PTCn_O(11)
#define PTC12_O         PTCn_O(12)
#define PTC13_O         PTCn_O(13)
#define PTC14_O         PTCn_O(14)
#define PTC15_O         PTCn_O(15)
#define PTC16_O         PTCn_O(16)
#define PTC17_O         PTCn_O(17)
#define PTC18_O         PTCn_O(18)
#define PTC19_O         PTCn_O(19)
#define PTD0_O          PTDn_O(0)        //PortD输出
#define PTD1_O          PTDn_O(1)
#define PTD2_O          PTDn_O(2)
#define PTD3_O          PTDn_O(3)
#define PTD4_O          PTDn_O(4)
#define PTD5_O          PTDn_O(5)
#define PTD6_O          PTDn_O(6)
#define PTD7_O          PTDn_O(7)
#define PTD8_O          PTDn_O(8)
#define PTD9_O          PTDn_O(9)
#define PTD10_O         PTDn_O(10)
#define PTD11_O         PTDn_O(11)
#define PTD12_O         PTDn_O(12)
#define PTD13_O         PTDn_O(13)
#define PTD14_O         PTDn_O(14)
#define PTD15_O         PTDn_O(15)
#define PTE0_O          PTEn_O(0)        //PortE输出
#define PTE1_O          PTEn_O(1)
#define PTE2_O          PTEn_O(2)
#define PTE3_O          PTEn_O(3)
#define PTE4_O          PTEn_O(4)
#define PTE5_O          PTEn_O(5)
#define PTE6_O          PTEn_O(6)
#define PTE7_O          PTEn_O(7)
#define PTE8_O          PTEn_O(8)
#define PTE9_O          PTEn_O(9)
#define PTE10_O         PTEn_O(10)
#define PTE11_O         PTEn_O(11)
#define PTE12_O         PTEn_O(12)
#define PTE24_O         PTEn_O(24)
#define PTE25_O         PTEn_O(25)
#define PTE26_O         PTEn_O(26)
#define PTE27_O         PTEn_O(27)
#define PTE28_O         PTEn_O(28)
//Pin输入
#define PTxn_I(x, n)    BITBAND_REG(x->PDIR, n)
#define PTAn_I(n)       PTxn_I(PTA, n)
#define PTBn_I(n)       PTxn_I(PTB, n)
#define PTCn_I(n)       PTxn_I(PTC, n)
#define PTDn_I(n)       PTxn_I(PTD, n)
#define PTEn_I(n)       PTxn_I(PTE, n)
#define PTA0_I          PTAn_I(0)        //PortA输入
#define PTA1_I          PTAn_I(1)
#define PTA2_I          PTAn_I(2)
#define PTA3_I          PTAn_I(3)
#define PTA4_I          PTAn_I(4)
#define PTA5_I          PTAn_I(5)
#define PTA6_I          PTAn_I(6)
#define PTA7_I          PTAn_I(7)
#define PTA8_I          PTAn_I(8)
#define PTA9_I          PTAn_I(9)
#define PTA10_I         PTAn_I(10)
#define PTA11_I         PTAn_I(11)
#define PTA12_I         PTAn_I(12)
#define PTA13_I         PTAn_I(13)
#define PTA14_I         PTAn_I(14)
#define PTA15_I         PTAn_I(15)
#define PTA16_I         PTAn_I(16)
#define PTA17_I         PTAn_I(17)
#define PTA18_I         PTAn_I(18)
#define PTA19_I         PTAn_I(19)
#define PTA24_I         PTAn_I(24)
#define PTA25_I         PTAn_I(25)
#define PTA26_I         PTAn_I(26)
#define PTA27_I         PTAn_I(27)
#define PTA28_I         PTAn_I(28)
#define PTA29_I         PTAn_I(29)
#define PTB0_I          PTBn_I(0)        //PortB输入
#define PTB1_I          PTBn_I(1)
#define PTB2_I          PTBn_I(2)
#define PTB3_I          PTBn_I(3)
#define PTB4_I          PTBn_I(4)
#define PTB5_I          PTBn_I(5)
#define PTB6_I          PTBn_I(6)
#define PTB7_I          PTBn_I(7)
#define PTB8_I          PTBn_I(8)
#define PTB9_I          PTBn_I(9)
#define PTB10_I         PTBn_I(10)
#define PTB11_I         PTBn_I(11)
#define PTB16_I         PTBn_I(16)
#define PTB17_I         PTBn_I(17)
#define PTB18_I         PTBn_I(18)
#define PTB19_I         PTBn_I(19)
#define PTB20_I         PTBn_I(20)
#define PTB21_I         PTBn_I(21)
#define PTB22_I         PTBn_I(22)
#define PTB23_I         PTBn_I(23)
#define PTC0_I          PTCn_I(0)        //PortC输入
#define PTC1_I          PTCn_I(1)
#define PTC2_I          PTCn_I(2)
#define PTC3_I          PTCn_I(3)
#define PTC4_I          PTCn_I(4)
#define PTC5_I          PTCn_I(5)
#define PTC6_I          PTCn_I(6)
#define PTC7_I          PTCn_I(7)
#define PTC8_I          PTCn_I(8)
#define PTC9_I          PTCn_I(9)
#define PTC10_I         PTCn_I(10)
#define PTC11_I         PTCn_I(11)
#define PTC12_I         PTCn_I(12)
#define PTC13_I         PTCn_I(13)
#define PTC14_I         PTCn_I(14)
#define PTC15_I         PTCn_I(15)
#define PTC16_I         PTCn_I(16)
#define PTC17_I         PTCn_I(17)
#define PTC18_I         PTCn_I(18)
#define PTC19_I         PTCn_I(19)
#define PTD0_I          PTDn_I(0)        //PortD输入
#define PTD1_I          PTDn_I(1)
#define PTD2_I          PTDn_I(2)
#define PTD3_I          PTDn_I(3)
#define PTD4_I          PTDn_I(4)
#define PTD5_I          PTDn_I(5)
#define PTD6_I          PTDn_I(6)
#define PTD7_I          PTDn_I(7)
#define PTD8_I          PTDn_I(8)
#define PTD9_I          PTDn_I(9)
#define PTD10_I         PTDn_I(10)
#define PTD11_I         PTDn_I(11)
#define PTD12_I         PTDn_I(12)
#define PTD13_I         PTDn_I(13)
#define PTD14_I         PTDn_I(14)
#define PTD15_I         PTDn_I(15)
#define PTE0_I          PTEn_I(0)        //PortE输入
#define PTE1_I          PTEn_I(1)
#define PTE2_I          PTEn_I(2)
#define PTE3_I          PTEn_I(3)
#define PTE4_I          PTEn_I(4)
#define PTE5_I          PTEn_I(5)
#define PTE6_I          PTEn_I(6)
#define PTE7_I          PTEn_I(7)
#define PTE8_I          PTEn_I(8)
#define PTE9_I          PTEn_I(9)
#define PTE10_I         PTEn_I(10)
#define PTE11_I         PTEn_I(11)
#define PTE12_I         PTEn_I(12)
#define PTE24_I         PTEn_I(24)
#define PTE25_I         PTEn_I(25)
#define PTE26_I         PTEn_I(26)
#define PTE27_I         PTEn_I(27)
#define PTE28_I         PTEn_I(28)

//GPIO初始化参数值宏定义

//GPIO_InitTypeDef->GPIO_Dir
#define DIR_INPUT       0       //输入
#define DIR_OUTPUT      1       //输出

//GPIO_InitTypeDef->GPIO_Output
#define OUTPUT_L        0       //输出低电平
#define OUTPUT_H        1       //输出高电平

//GPIO_InitTypeDef->GPIO_PinControl
#define INPUT_PULL_DOWN (0|PORT_PCR_PE_MASK)                      //输入下拉
#define INPUT_PULL_UP   (PORT_PCR_PS_MASK|PORT_PCR_PE_MASK)       //输入上拉
#define INPUT_PULL_DIS  0                                       //禁用PULL
#define OUTPUT_SR_FAST  0                       //高压摆率
#define OUTPUT_SR_SLOW  PORT_PCR_SRE_MASK       //低压摆率
#define INPUT_PF_EN     PORT_PCR_PFE_MASK       //使能低通滤波器
#define INPUT_PF_DIS    0                       //禁用低通滤波器
#define OUTPUT_OD_EN    PORT_PCR_ODE_MASK       //输出开漏使能
#define OUTPUT_OD_DIS   0                       //输出开漏禁用
#define OUTPUT_DSH      PORT_PCR_DSE_MASK       //高驱动能力输出
#define OUTPUT_DSL      0                       //低驱动能力输出
#define IRQC_DIS        PORT_PCR_IRQC(0x00)     //禁用中断\请求
#define IRQC_DMARI      PORT_PCR_IRQC(0x01)     //上升沿产生DMA请求
#define IRQC_DMAFA      PORT_PCR_IRQC(0x02)     //下降沿产生DMA请求
#define IRQC_DMAET      PORT_PCR_IRQC(0x03)     //边沿产生DMA请求
#define IRQC_L          PORT_PCR_IRQC(0x08)     //低电平触发外部中断
#define IRQC_RI         PORT_PCR_IRQC(0x09)     //上升沿触发外部中断
#define IRQC_FA         PORT_PCR_IRQC(0x0A)     //下降沿触发外部中断
#define IRQC_ET         PORT_PCR_IRQC(0x0B)     //边沿触发外部中断
#define IRQC_H          PORT_PCR_IRQC(0x0C)     //高电平触发外部中断


/*! 枚举PORT 配置 */
typedef enum
{
    //中断方式和DMA请求方式，两者只能选其中一种（可以不选）
    //中断方式选择
    IRQ_ZERO     = 0x08 << PORT_PCR_IRQC_SHIFT,   //低电平触发
    IRQ_RISING   = 0x09 << PORT_PCR_IRQC_SHIFT,   //上升沿触发
    IRQ_FALLING  = 0x0A << PORT_PCR_IRQC_SHIFT,   //下降沿触发
    IRQ_EITHER   = 0x0B << PORT_PCR_IRQC_SHIFT,   //跳变沿触发
    IRQ_ONE      = 0x0C << PORT_PCR_IRQC_SHIFT,   //高电平触发

    //DMA请求选择
    DMA_RISING   = 0x01 << PORT_PCR_IRQC_SHIFT,   //上升沿触发
    DMA_FALLING  = 0x02 << PORT_PCR_IRQC_SHIFT,   //下降沿触发
    DMA_EITHER   = 0x03 << PORT_PCR_IRQC_SHIFT,   //跳变沿触发


    HDS          = 0x01 << PORT_PCR_DSE_SHIFT,    //输出高驱动能力
    ODO          = 0x01 << PORT_PCR_ODE_SHIFT,    //漏极输出
    PF           = 0x01 << PORT_PCR_PFE_SHIFT,    //带无源滤波器
    SSR          = 0x01 << PORT_PCR_SRE_SHIFT,    //输出慢变化率          Slow slew rate

    //下拉上拉选择
    PULLDOWN     = 0x02 << PORT_PCR_PS_SHIFT,     //下拉
    PULLUP       = 0x03 << PORT_PCR_PS_SHIFT,     //上拉

    //功能复用选择(如果不需要改变功能复用选择，保留原先的功能复用，直接选择ALT0 )
    //需要查 K60 Signal Multiplexing and Pin Assignments
    ALT0         = 0x00 << PORT_PCR_MUX_SHIFT,
    ALT1         = 0x01 << PORT_PCR_MUX_SHIFT,    //GPIO
    ALT2         = 0x02 << PORT_PCR_MUX_SHIFT,
    ALT3         = 0x03 << PORT_PCR_MUX_SHIFT,
    ALT4         = 0x04 << PORT_PCR_MUX_SHIFT,
    ALT5         = 0x05 << PORT_PCR_MUX_SHIFT,
    ALT6         = 0x06 << PORT_PCR_MUX_SHIFT,
    ALT7         = 0x07 << PORT_PCR_MUX_SHIFT,
} port_cfg;


//GPIO模块中断回调函数类型
typedef void (*GPIO_ISR_CALLBACK)(void);

//GPIO模块初始化结构体，用于配置GPIO各项参数
typedef struct
{  
  /*
    描述：选择PORTx
    取值：PTA、PTB、PTC、PTD、PTE
    初始化：必须初始化
  */
  GPIO_Type *GPIO_PTx;
  
  /*
    描述：选择要初始化的引脚
    取值：
      GPIO_Pin0~GPIO_Pin31、
      GPIO_Pin0_7、GPIO_Pin8_15、GPIO_Pin16_23、GPIO_Pin24_31
    初始化：必须初始化
  */
  uint32 GPIO_Pins;
  
  /*
    描述：配置引脚控制控制：Pull|压摆率|滤波|开漏|驱动力|中断
    取值：见宏定义
    初始化：不必须初始化，默认：无Pull|高压摆率|无滤波|无开漏|低驱动力|禁中断
  */
  uint32 GPIO_PinControl;
  
  /*
    描述： 选择GPIO的输入输出方向
    取值：
      输入-DIR_INPUT
      输出-DIR_OUTPUT
    初始化：必须初始化
  */
  uint8 GPIO_Dir;
  
  /*
    描述：选择GPIO的初始化时的输出
    取值：
      低电平-OUTPUT_L
      高电平-OUTPUT_H
    初始化：不必须初始化，默认为低电平
  */
  uint8 GPIO_Output;
  
  /*
    描述：外部中断回调函数
    取值：函数必须为无返回值,无参数(eg. void isr(void);)
    初始化：不必须初始化、如未初始化则不会触发中断
  */
  GPIO_ISR_CALLBACK GPIO_Isr; 
} GPIO_InitTypeDef;

//检测PORTX的引脚PINX是否产生外部中断
#define GPIO_IsPinxExt(PORTX, PINX)  (PORTX->ISFR&PINX)
//清除PORTX的中断标志
#define GPIO_ClearIntFlag(PORTX)     (PORTX->ISFR=0xFFFFFFFF)

void PORT_Init(PortPinsEnum_Type, uint32);
void PORT_Init_NoALT(PortPinsEnum_Type, uint32);

//GPIO通用初始化函数
uint8 GPIO_Init(GPIO_InitTypeDef);
//GPIO通用初始化函数(具体引脚)
void GPIO_Init_Pin(PortPinsEnum_Type, uint8, uint8);


//使能GPIO外部中断
uint8 GPIO_EnableIrq(GPIO_InitTypeDef);
//禁用GPIO外部中断
uint8 GPIO_DisableIrq(GPIO_InitTypeDef);
//设置GPIO端口0~31位的输出
void GPIO_Output(GPIO_Type *, uint32);
//设置GPIO端口一位的输出
void GPIO_Output_b(GPIO_Type *, uint32, uint8);
//设置GPIO端口8位的输出
void GPIO_Output_8b(GPIO_Type *, uint8, uint8);
//设置GPIO端口0~31的电平翻转
void GPIO_Toggle(GPIO_Type *, uint32);
//设置GPIO端口一位的翻转
void GPIO_Toggle_b(GPIO_Type *, uint8);
//设置GPIO端口8位的翻转
void GPIO_Toggle_8b(GPIO_Type *, uint8, uint8);
//取得GPIO口0~31位的数据
uint32 GPIO_Input(GPIO_Type *);
//取得GPIO口某一位的数据
uint8 GPIO_Input_b(GPIO_Type *, uint8);
//取得GPIO端口8位数据的输入
uint8 GPIO_Input_8b(GPIO_Type *, uint8);


void    GPIO_DDR   (PortPinsEnum_Type, uint8);                //设置引脚数据方向
void    GPIO_Set   (PortPinsEnum_Type,           uint8 data);    //设置引脚状态
void    GPIO_Turn  (PortPinsEnum_Type);                          //反转引脚状态
uint8   GPIO_Get   (PortPinsEnum_Type);                          //读取引脚状态

void GPIO_OUT_L(PortPinsEnum_Type);
void GPIO_OUT_H(PortPinsEnum_Type);


#endif /* __GPIO_H__ */
