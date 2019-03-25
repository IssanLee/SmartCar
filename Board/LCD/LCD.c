/***************************************************************************
* File:         LCD.h
* Summary:      LCD驱动
* Version:      V0.1
* Author:       Leopold & 山外科技[VCAN]
* Date:         2019/1/30
* Change Logs:
*       Date:   2019/1/30          
*       Version:V0.1         
*       Author: Leopold       
*       Notes:  修改自:山外科技[VCAN]
***************************************************************************/
#include "LCD.h"


/***********************************
 *  @brief      LCD初始化
 *  @since      v5.0
 **********************************/
void LCD_Init(void)
{
    Site_t site = {0, 0};                   //定义坐标结构体
    Size_t size ;                           //定义矩形大小结构体
    
    LCD_INIT;                               //初始化所使用的LCD芯片

    //LCD_SET_DIR(LCD_DIR + 1);
    //由于初始化的时候进行 屏幕方向 选择，因而初始化完成后，才获取宽高
    size.W = LCD_W;
    size.H = LCD_H;

    LCD_rectangle(site, size, LCD_BCOLOUR);     //初始化背景
}


/******************************************************
 *  @brief      显示实心矩形
 *  @param      site    左上角坐标
 *  @param      size    矩形大小
 *  @param      rgb565  颜色
 *  @since      v5.0
 *  Sample usage:        Site_t site = {10,20};   //x = 10 ,y = 20
                        Size_t size = {50,60};  // W = 50 ,H = 60
                        LCD_rectangle(site, size, RED);
 ******************************************************/
void LCD_rectangle(Site_t site, Size_t size, uint16 rgb565)
{
    uint32 n, temp;
    LCD_PTLON(site, size);              //开窗
    temp = (uint32)size.W * size.H;
    LCD_RAMWR();                        //写内存
    for(n = 0; n < temp; n++)
    {
        LCD_WR_DATA( rgb565 );          //写数据
    }
}


/*************************************
                清屏
*************************************/
void LCD_clear(uint16 rgb565)     
{
    Site_t site = {0, 0};
    Size_t size;
    size.H = LCD_H;
    size.W = LCD_W;
    LCD_rectangle(site, size, rgb565);
}

/*************************************
             画十字形
*************************************/
void LCD_cross(Site_t site,uint16 len,uint16 Color)
{
    //画两条直线
    Site_t sitetmp;
    Size_t size;
    int16  stmp,etmp;
    uint16 w = LCD_W, h = LCD_H;

    ASSERT((site.X<LCD_W) && (site.Y<LCD_H));       //十字线的中心不能超出液晶范围

    stmp = site.X - len / 2;                        //计算水平线的x轴起点和结束点，限制不超出液晶范围
    if (stmp < 0) stmp = 0;
    etmp = site.X + len / 2;
    if (etmp >= w) etmp = w - 1;

    sitetmp.X = stmp;
    sitetmp.Y = site.Y;
    size.W = etmp - stmp +1;
    size.H = 1;
    LCD_rectangle(sitetmp, size, Color);

    stmp = site.Y - len / 2;                          //计算水平线的x轴起点和结束点，限制不超出液晶范围
    if (stmp < 0) stmp = 0;
    etmp = site.Y + len / 2;
    if (etmp >= h) etmp= h - 1;

    sitetmp.X = site.X;
    sitetmp.Y = stmp;
    size.W = 1;
    size.H = etmp - stmp + 1;
    LCD_rectangle(sitetmp, size, Color);
}

/*************************************************************
 *  @brief      画点
 *  @param      site    左上角坐标
 *  @param      rgb565  颜色
 *  @since      v5.0
 *  Sample usage:        Site_t site = {10,20};   //x = 10 ,y = 20
                        LCD_point(site, RED);
 *************************************************************/
void LCD_point(Site_t site, uint16 rgb565)
{
    Size_t size = {1, 1};
    LCD_PTLON(site, size);
    LCD_RAMWR();                        //写内存
    LCD_WR_DATA(rgb565);                //写数据
}

/************************************************************
 *  @brief      画一堆点
 *  @param      site        点坐标数组
 *  @param      point_num   点的数量
 *  @param      rgb565      颜色
 *  @since      v5.0
 *  Sample usage:        Site_t site[3] = {{10,20},{11,21},{12,22}};   //3个点，坐标分别是  {10,20},{11,21},{12,22}
                        LCD_points(site,3, RED);
 ************************************************************/
void LCD_points(Site_t *site, uint32 point_num, uint16 rgb565)                 //画一堆点
{
    while (point_num--)
    {
        LCD_point(site[point_num],rgb565);                 //画点
    }
}

/************************************************************
 *  @brief      显示字符
 *  @param      site    左上角坐标
 *  @param      ascii   字符
 *  @param      Color   字体颜色
 *  @param      bkColor 背景颜色
 *  @since      v5.0
 *  Sample usage:        Site_t site = {10,20};   //x = 10 ,y = 20
                        LCD_char(site,'0', BLUE,RED);
 ************************************************************/
void LCD_char(Site_t site, uint8 ascii, uint16 Color, uint16 bkColor)
{
#define MAX_CHAR_POSX (LCD_W-8)
#define MAX_CHAR_POSY (LCD_H-16)

    uint8 temp, t, pos;
    Size_t size = {8, 16};

    if(site.X > MAX_CHAR_POSX || site.Y > MAX_CHAR_POSY)
    {
        return;
    }

    LCD_PTLON(site, size);

    LCD_RAMWR();                    //写内存

    for (pos = 0; pos < 16; pos++)
    {
        temp = ascii_8x16[((ascii-0x20)*16)+pos];

        for (t = 0; t < 8; t++)
        {
            if (temp & 0x80)
            {
                LCD_WR_DATA(Color);
            }
            else
            {
                LCD_WR_DATA(bkColor);
            }
            temp <<= 1;
        }
    }
    return;
#undef MAX_CHAR_POSX
#undef MAX_CHAR_POSY
}

/************************************************************
 *  @brief      显示字符串
 *  @param      site    左上角坐标
 *  @param      Str     字符串地址
 *  @param      Color   字体颜色
 *  @param      bkColor 背景颜色
 *  @since      v5.0
 *  Sample usage:        Site_t site = {10,20};   //x = 10 ,y = 20
                        LCD_str(site,"www.vcan123.com", BLUE,RED);
 ************************************************************/
void LCD_str(Site_t site, uint8 *Str, uint16 Color, uint16 bkColor)
{
#define MAX_CHAR_POSX (LCD_W-8)
#define MAX_CHAR_POSY (LCD_H-16)
    while(*Str != '\0')
    {
        if(site.X > MAX_CHAR_POSX )
        {
            //换行
            site.X = 0;
            site.Y += 16;
        }
        if(site.Y > MAX_CHAR_POSY )
        {
            //一屏
            site.Y = 0;
            site.X = 0;
        }

        LCD_char(site, *Str, Color, bkColor);
        site.X += 8;
        Str ++ ;
    }
#undef MAX_CHAR_POSX
#undef MAX_CHAR_POSY
}

/************************************************************
 *  @brief      显示数字
 *  @param      site    左上角坐标
 *  @param      num     数字
 *  @param      Color   字体颜色
 *  @param      bkColor 背景颜色
 *  @since      v5.0
 *  Sample usage:        Site_t site = {10,20};   //x = 10 ,y = 20
                        LCD_num(site,123, BLUE,RED);
 ************************************************************/
void LCD_num(Site_t site, uint32 num, uint16 Color, uint16 bkColor)
{
    uint32 res = num;
    uint8 t = 0;
    Site_t sitetemp;
    sitetemp.Y = site.Y;

    if (num == 0)
    {
        LCD_char(site, '0', Color, bkColor);
        return;
    }
    while (res)  /*得到数字长度t*/
    {
        res /= 10;
        t++;
    }

    while (num)
    {
        sitetemp.X = site.X + (8 * (t--) - 8);
        LCD_char(sitetemp, (num % 10) + '0', Color, bkColor);
        num /= 10 ;
    }
}

/************************************************************
 *  @brief      显示数字（清空多余的位）
 *  @param      site            左上角坐标
 *  @param      num             数字
 *  @param      max_num_bit     最大的位数
 *  @param      Color           字体颜色
 *  @param      bkColor         背景颜色
 *  @since      v5.0
 *  Sample usage:        Site_t site = {10,20};   //x = 10 ,y = 20
                        LCD_num_BC(site,123,5, BLUE,RED);
 ************************************************************/
void LCD_num_BC(Site_t site, uint32 num, uint8 max_num_bit, uint16 Color, uint16 bkColor)
{
    uint32 res = num;
    uint8 t = 0;
    Site_t sitetemp;
    Size_t size;

    sitetemp.Y = site.Y;

    if (num == 0)
    {
        LCD_char(site, '0', Color, bkColor);

        site.X += 8;
        size.H  = 16;
        size.W  = 8 * (max_num_bit - 1);
        LCD_rectangle(site, size, bkColor);

        return;
    }
    while (res)            /*得到数字长度t*/
    {
        res /= 10;
        t++;
    }
    if (t >= max_num_bit)    //限制最大长度
    {
        t = max_num_bit;
    }

    res = t;

    while (t != 0)
    {
        sitetemp.X = site.X + (8 * (t--) - 8);
        LCD_char(sitetemp, (num % 10) + '0', Color, bkColor);
        num /= 10 ;
    }

    if (res != max_num_bit)
    {
        size.W = 8 * (max_num_bit - res);
        site.X += (8 * res);
        size.H  = 16;
        LCD_rectangle(site, size, bkColor);
    }
}
/*!
 *  @brief      显示数字（清空多余的位）
 *  @param      site            左上角坐标
 *  @param      num             数字
 *  @param      max_num_bit     最大的位数
 *  @param      Color           字体颜色
 *  @param      bkColor         背景颜色
 *  @since      v5.0
*  Sample usage:        Site_t site = {10,20};   //x = 10 ,y = 20
                        LCD_num_BC(site,123,5, BLUE,RED);
 */
void LCD_NUM(Site_t site, int32 num, uint8 max_num_bit, uint16 Color, uint16 bkColor)
{
    uint32 res;
    uint8 t = 0;
    Site_t sitetemp;
    Size_t size;

    sitetemp.Y = site.Y;

    if (num == 0)
    {
        LCD_char(site, '0', Color, bkColor);

        site.X += 8;
        size.H  = 16;
        size.W  = 8 * (max_num_bit - 1);
        LCD_rectangle(site, size, bkColor);

        return;
    }
    if (num < 0)
    {
        res = -num;
        t++;
    }
    else
    {
        res = num;
    }
    while (res)            /*得到数字长度t*/
    {
        res /= 10;
        t++;
    }
    if (t >= max_num_bit)    //限制最大长度
    {
        t = max_num_bit;
    }
    if (t != max_num_bit)
    {
        sitetemp.X = site.X + (8 * t);
        size.W = 8 * (max_num_bit - t);
        size.H  = 16;
        LCD_rectangle(sitetemp, size, bkColor);
    }
    if (num < 0)
    {
        num = -num;
        LCD_char(site, '-', Color, bkColor);
        while (t != 1)
        {
            sitetemp.X = site.X + (8 * (t--) - 8);
            LCD_char(sitetemp, (num % 10) + '0', Color, bkColor);
            num /= 10 ;
        }
    }
    else
    {
        while (t != 0)
        {
            sitetemp.X = site.X + (8 * (t--) - 8);
            LCD_char(sitetemp, (num % 10) + '0', Color, bkColor);
            num /= 10 ;
        }
    }
}
/************************************************************
 *  @brief      灰度图像显示
 *  @param      site            左上角坐标
 *  @param      size            显示图像大小
 *  @param      img             图像地址
 *  @since      v5.0
 *  Sample usage:       Site_t site = {10,20};      //x = 10 ,y = 20
                        Size_t size = {320,240};    //W = 320,H = 240
                        LCD_Img_gray(site, size, img);
 ************************************************************/
void LCD_Img_gray(Site_t site, Size_t size, uint8 *img)
{
    uint32     total = (size.H * size.W);
    uint16     imgtemp;
    uint8     *pimg = (uint8 *)img;

    LCD_PTLON(site, size);                      //开窗
    LCD_RAMWR();                                //写内存

    while (total--)
    {
        imgtemp     = (uint16) * (pimg++);
        imgtemp = GRAY_2_RGB565(imgtemp);
        LCD_WR_DATA( imgtemp );               //写图像数据
    }
}

/************************************************************
 *  @brief      缩放灰度图像显示
 *  @param      site            左上角坐标
 *  @param      size            显示图像大小
 *  @param      img             图像地址
 *  @param      imgsize         图像大小
 *  @since      v5.0
 *  Sample usage:       Site_t site = {10,20};          //x = 10 ,y = 20
                        Size_t size = {80,60};          //W = 80,H = 60
                        Size_t imgsize = {320,240};     //W = 320,H = 240
                        LCD_Img_gray_Z(site, size, img,imgsize);
 ************************************************************/
void LCD_Img_gray_Z(Site_t site, Size_t size, uint8 *img, Size_t imgsize)
{

    uint32 temp, tempY;
    uint16 x, y;
    uint16 X, Y;
    uint8 *pimg = (uint8 *)img;
    uint16 rgb;

    LCD_PTLON(site, size);                      //开窗

    LCD_RAMWR();                                //写内存

    for (y = 0; y < size.H; y++)
    {
        Y = ( (  y * imgsize.H   ) / size.H) ;
        tempY = Y * imgsize.W ;

        for (x = 0; x < size.W; x++)
        {
            X = ( x * imgsize.W  ) / size.W ;
            temp = tempY + X;
            rgb = GRAY_2_RGB565(pimg[temp]);    //
            LCD_WR_DATA(rgb);
        }
    }
}


/************************************************************
 *  @brief      二值化图像显示
 *  @param      site            左上角坐标
 *  @param      size            显示图像大小
 *  @param      img             图像地址
 *  @since      v5.0
 *  Sample usage:       Site_t site = {10,20};      //x = 10 ,y = 20
                        Size_t size = {320,240};    //W = 320,H = 240
                        LCD_Img_Binary(site, size, img);
 ************************************************************/
void LCD_Img_Binary(Site_t site, Size_t size, uint8 *img)
{
    uint32     total = (size.H * size.W) / 8;
    uint8     imgtemp;
    uint8       bitindex;
    uint8     *pimg = (uint8 *)img;

    LCD_PTLON(site, size);                      //开窗

    LCD_RAMWR();                                //写内存

    while (total--)
    {
        imgtemp     = *(pimg++);
        bitindex    = 8;
        while (bitindex--)
        {
            if (imgtemp & (0x01 << bitindex))
            {
                LCD_WR_DATA(BINARY_COLOR);      //写图像数据
            }
            else
            {
                LCD_WR_DATA(BINARY_BGCOLOR);      //写图像数据
            }
        }
    }
}
//显示二值化图像(可缩放)
void LCD_Img_Binary_Z(Site_t site, Size_t size, uint8 *img, Size_t imgsize)
{

    uint32 temp, tempY;
    uint16 x, y;
    uint16 X, Y;
    uint8 *pimg = (uint8 *)img;

    LCD_PTLON(site, size);                      //开窗

    LCD_RAMWR();                                //写内存

    for (y = 0; y < size.H; y++)
    {
        Y = ( (  y * imgsize.H  ) / size.H) ;
        tempY = Y * imgsize.W ;

        for (x = 0; x < size.W; x++)
        {
            X = (( x * imgsize.W  ) / size.W) ;
            temp = tempY + X;
            if ( (pimg[temp>>3] & (1 << (7 - (temp & 0x07))) ) == 0  )
            {
                LCD_WR_DATA( BINARY_BGCOLOR );        //写图像数据
            }
            else
            {
                LCD_WR_DATA( BINARY_COLOR );      //写图像数据
            }
        }
    }
}


//波形显示，带背景颜色（maxval 最大值表示超过此值的数都当作最大值处理）
void LCD_wave(Site_t site, Size_t size, uint8 *img, uint8 maxval, uint16 Color, uint16 bkColor)
{

    uint8 h;
    uint16 y = site.Y + size.H - 1;

    LCD_rectangle(site, size, bkColor);
    site.X += size.W;
    img  += (size.W - 1);
    while (size.W --)
    {
        if (*img >= maxval )
        {
            h = size.H - 1;
        }
        else
        {
            h = ((*img )* size.H )/maxval;
        }
        site.Y = y - h ;
        site.X--;
        img --;
        LCD_point(site, Color);
    }
}


//波形显示，不带背景颜色
void LCD_wave_display(Site_t site,Size_t size,uint8 *img,uint8 maxval,uint16 Color)
{
    uint8 h;
    uint16 y = site.Y + size.H - 1;

    site.X += size.W;
    img  += (size.W - 1);
    while (size.W --)
    {
        if(*img >= maxval )
        {
            h = size.H - 1;
        }
        else
        {
            h = ((*img )* size.H) / maxval;
        }
        site.Y = y - h ;
        site.X--;
        img --;
        LCD_point(site, Color);
    }
}

/********************************************************************
 * 函数名：  LCD_char_CHI
 * 描述  ：  显示单个汉字字符
 * 输入  :
 *	    str[]:   汉字 
 *	    Color:   字体颜色
 *          bkColor: 背景颜色
 * 输出  ： 无
 * 举例  ： Site_t site={0,0};
            LCD_Char_CH(site,"好",RED,BLACK);
 * 备注	 ：受龙邱OLED汉字显示启发，结合龙邱和山外代码改编 汉字库仅1000个常用汉字
 * 日期  ：2016,1,29
************************************************************************/
void LCD_char_CHI (Site_t site,const uint8 str[],uint16 Color,uint16 bkColor)
{
    uint8 i, j;
    uint8 tmp_char=0, ii=0;
    uint16 n=0, m=1;
    while(Chinese[n] > 127)//根据龙邱改编
    {
        if(Chinese[n] == str[ii])
        {
            if(Chinese[n + 1] == str[ii + 1])
            {
                m = n * 16;
                break;
            }
        }
        n += 2;			
     }

    const Size_t size   = {LCD_CH_W, LCD_CH_H};

    LCD_PTLON(site, size);              //开窗
    LCD_RAMWR();                        //写内存

    for (i=0; i < LCD_CH_SIZE; i++)
    {
        tmp_char = Chinese_Code[m++];    //第一次：m+1 错了

        for(j=0;j<8;j++)
        {
            if(tmp_char & (0x80))
                LCD_WR_DATA( Color );
            else
                LCD_WR_DATA( bkColor );
            tmp_char <<= 1;
        }
    }
}

/********************************************************************
 * 函数名：  LCD_str_CHI
 * 描述  ：  显示16*16汉字字符串
 * 输入  :
 *	    str[]:   汉字 
 *	    Color:   字体颜色
 *          bkColor: 背景颜色
 * 输出  ： 无
 * 举例  ： Site_t site={0,0};
            LCD_str_CHI(site,"你好",RED,BLACK);
 * 备注	 ：受龙邱OLED汉字显示启发，结合龙邱和山外代码改编汉字库仅1000个常用汉字
 * 日期  ：2016,1,29
************************************************************************/
void LCD_str_CHI (Site_t site, const uint8 *str, uint16 Color, uint16 bkColor)		
{
    while (*str != '\0')
    {
        if (site.X > (LCD_W-16))
            {	
             /*换行*/
            site.X = 0;
            site.Y += LCD_CH_H;
        }
        if (site.Y > (LCD_H-LCD_CH_W))
        {
             /*重新归零*/
             site.Y = 0;
             site.X = 0;
        }
        LCD_char_CHI(site, str, Color, bkColor);
        str += 2 ;
        site.X += LCD_CH_W ;	
    }
}

/********************************************************************
 * 函数名：  LCD_Print
 * 描述  ：  显示16*16汉字英文混合字符串
 * 输入  :
 *	    str[]:   汉字 
 *	    Color:   字体颜色
 *          bkColor: 背景颜色
 * 输出  ： 无
 * 举例  ： Site_t site={0,0};
            LCD_str_CHI(site,"你好Tesla!",RED,BLACK);
 * 备注	 ：受龙邱OLED汉字显示启发，结合龙邱和山外代码改编
           汉字库仅1000个常用汉字
           英文为8*16的ASCII码
 * 日期  ：2016，1，29
************************************************************************/
void LCD_Print(Site_t site, const uint8 *str, uint16 Color, uint16 bkColor)
{
    while (*str != '\0')
    {
        if (*str < 0x80)       //英文
        {
            if (site.X > (LCD_W-LCD_EN_W))
            {	
                 /*换行*/
                site.X =0;
                site.Y += LCD_EN_H;
            }
            if (site.Y > (LCD_H-LCD_EN_H))
            {
                 /*重新归零*/
                 site.Y =0;
                 site.X =0;
            }
            LCD_char(site, *str, Color, bkColor);
            str += 1 ;
            site.X += LCD_EN_W ;	
        }
        else
        {
            if (site.X > (LCD_W-16))
            {	
                /*换行*/
                site.X = 0;
                site.Y += LCD_CH_H;
            }
            if(site.Y > (LCD_H-LCD_CH_W))
            {
                /*重新归零*/
                site.Y = 0;
                site.X = 0;
            }
            LCD_char_CHI(site, str, Color, bkColor);
            str += 2 ;
            site.X += LCD_CH_W ;
        }
    }  
 }



void LCD_FChar_CH (Site_t site, const uint8 *str, uint16 Color, uint16 bkColor)
{
    uint8 i,j;
    //uint8 *pbuf = str;
    uint8 tmp_char=0;

    const Size_t size   = {LCD_CH_W, LCD_CH_H};
    uint8 l = 0;

    LCD_PTLON(site, size);              //开窗
    LCD_RAMWR();                        //写内存

    for (i=0; i < LCD_CH_SIZE; i++)
    {
        tmp_char = *str++;

        for (j=0; j<8; j++)
        {
            if(tmp_char & (0x80))
            {
                LCD_WR_DATA( Color );
            }
            else
            {
                LCD_WR_DATA( bkColor );
            }
            tmp_char <<= 1;
            
            l++;
            if (l >= LCD_CH_W)
            {
                l = 0;
                break;
            }
        }
    }
}

void LCD_FStr_CH (Site_t site, const uint8 *str, uint16 num, uint16 Color, uint16 bkColor)
{
    while (num--)
    {
        if (site.X > (LCD_W-16))
        {
             /*换行*/
            site.X = 0;
            site.Y += LCD_CH_H;
        }
        if (site.Y > (LCD_H-LCD_CH_W))
        {
             /*重新归零*/
             site.Y = 0;
             site.X = 0;
        }
        LCD_FChar_CH(site, str, Color, bkColor);
        str += LCD_CH_SIZE ;
        site.X += LCD_CH_W ;
    }
}




