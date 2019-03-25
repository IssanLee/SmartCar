/***************************************************************************
* File:         common.h
* Summary:      公共头文件
* Version:      V0.1
* Author:       Leopold
* Date:         2019/1/19
* Change Logs:
*       Date:   2019/1/19          
*       Version:V0.1         
*       Author: Leopold       
*       Notes:  修改自:拉普兰德
***************************************************************************/

#ifndef _COMMON_H_
#define _COMMON_H_

/***********************************************************************/
/*
 * 基础数据类型定义
 */
typedef unsigned char	        uint8;  /*  8 bits */
typedef unsigned short int      uint16; /* 16 bits */
typedef unsigned long int	uint32; /* 32 bits */

typedef signed char		int8;   /*  8 bits */
typedef short int	        int16;  /* 16 bits */
typedef int		        int32;  /* 32 bits */

typedef volatile int8		vint8;  /*  8 bits */
typedef volatile int16  	vint16; /* 16 bits */
typedef volatile int32	        vint32; /* 32 bits */

typedef volatile uint8	        vuint8;  /*  8 bits */
typedef volatile uint16	        vuint16; /* 16 bits */
typedef volatile uint32	        vuint32; /* 32 bits */

typedef float   		float32; /*  32 bits */
typedef double   		float64; /*  64 bits */

typedef unsigned char           boolean;      /* 8-bit*/

/*
 * 其他定义(false、true、null)
 */
#ifdef	FALSE
#undef	FALSE
#endif
#define FALSE	0

#ifdef	TRUE
#undef	TRUE
#endif
#define	TRUE	1

#ifndef NULL
#define NULL    0
#endif

/********************************************************************/

/*
 * RAMFUNC关键字定义，用于告诉编译器函数在RAM中运行
 */
#ifndef __IAR_SYSTEMS_ICC__
  #warning OSKineits仅支持IAR编译器[http://www.lpld.cn]
  #define RAMFUNC 
#else
  #define RAMFUNC __ramfunc
#endif

/*
 * 打印调试信息定义
 */
#define DEBUG_PRINT

/*
 * 使用断言判断参数合法性
 */
#define DEBUG_ASSERT
   
/* 
 * 包含开发平台指定的头文件
 * 如果提示"未定义K60开发平台"，需在工程选项中添加定义
 */
#if (defined(DEFAULT_K60))
 #include "k60_Config.h"
#else
  #error "未定义K60开发平台"
#endif
   
/* 
 * 包含CPU指定的头文件
 * CPU型号应该在"开发平台指定的头文件"中定义
 */
#if (defined(CPU_MK60DZ10))
  #include "MK60DZ10.h"
#elif (defined(CPU_MK60D10))
  #include "MK60D10.h"
#elif (defined(CPU_MK60F12))
  #include "MK60F12.h"
#elif (defined(CPU_MK60F15))
  #include "MK60F15.h"
#endif


/* 
 * 包含通用功能函数
 */
#include "assert.h"
#include "io.h"
#include "uif.h"
#include "stdlib.h"
#include "relocate.h"
#if (defined(__IAR_SYSTEMS_ICC__))
	#include "intrinsics.h"
#endif

/* 
 * 包含拉普兰德K60固件库头文件
 */
#include "Drivers.h"

#define DELAY()         SYSTICK_DelayMs(500)
#define DELAY_MS(ms)    SYSTICK_DelayMs(ms)
#define DELAY_US(us)    SYSTICK_DelayUs(us)

/* 
 * 包含实时系统头文件
 */
#if (UCOS_II > 0u)
#include  <ucos_ii.h>
#endif

/* 
 * 包含文件系统头文件
 */
#if (USE_FATFS > 0u)
#include  <ff.h>
#endif

/***********************************************************************/
/* 
 * 中断相关函数定义
 */
//使能全局中断宏定义
#define EnableInterrupts __enable_irq()

//禁用全局中断宏定义
#define DisableInterrupts  __disable_irq()

//使能指定中断向量号的中断
#define enable_irq(IRQn)    NVIC_EnableIRQ(IRQn)

//禁用指定中断向量号的中断
#define disable_irq(IRQn)    NVIC_DisableIRQ(IRQn)

//更改中断向量表地址，写VTOR寄存器
#define write_vtor(vector_addr) SCB->VTOR = (uint32_t)vector_addr;
/***********************************************************************/

/****************************************************
              @brief 变量的位清0和置1
 ****************************************************/
#define BIT_CLEAN(var, n)        (var) &= ~(1<<(n))   //变量var 的n位（即第n+1位）清0
#define BIT_SET(var, n)          (var) |=  (1<<(n))   //变量var 的n位（即第n+1位）置1
#define BIT_GET(var, n)          (((var)>>(n))&0x01)  //读取变量var 的n位（即第n+1位）


/****************************************************
                  定义矩形坐标结构体
 ****************************************************/
typedef struct
{
    uint16 X;
    uint16 Y;
} Site_t;


/****************************************************
                 定义矩形大小结构体
 ****************************************************/
typedef struct
{
    uint16 W;       //宽
    uint16 H;       //高
} Size_t;
   
   

/* 
 * 用户入口函数声明
 */
void main(void);


/********************************************************************/

#endif /* _COMMON_H_ */
