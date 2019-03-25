/***************************************************************************
* File:         L3G4.h
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
#ifndef __L3G4_H__
#define __L3G4_H__


#include "common.h"

/*------------------------------------------------------------------------------
    接线：B20-->CS=====CS0
          B21-->SCL====SCK
          B22-->SDA====SIN
          B23-->SDO====SOUT
------------------------------------------------------------------------------*/

#define SPI_SPEED 10000000////不知道取多大合适

#define L3G4_SPIX (SPI2)
#define L3G4_MOSI ()
#define L3G4_MISO ()
#define L3G4_SCK  ()
#define L3G4_PCS0 (SPI_PCS0)



extern void L3G4_SPI_Init();

int16 L3G4_Read_X();
int16 L3G4_Read_Y();
int16 L3G4_Read_Z();

int16 Read_Angle_Speed();

#endif 
