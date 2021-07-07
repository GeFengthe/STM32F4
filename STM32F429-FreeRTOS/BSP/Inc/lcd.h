#ifndef __LCD_H
#define __LCD_H
#include "stm32f4xx.h"
#include "main.h"

//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	 0X841F //浅绿色
//#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)


//*--------------------LCD 相关指令---------------------
#define     LCD_READID                          0xD3            //读控制ID
#define     LCD_WCOLUMNADDR                     0x2A            //写入列地址
#define     LCD_WPAGEADDR                       0x2B            //写入页地址
#define     LCD_WGRAMADDR                       0x2C            //写入GRAM地址
#define     LCD_MADCLADDR                       0x36            //存储器访问指令，控制屏幕像素刷新方式
//------------------------------------------------------
//LCD重要参数集
typedef struct  
{		 	 
	u16 width;			//LCD 宽度
	u16 height;			//LCD 高度
	u16 id;				//LCD ID
	u8  dir;			//横屏还是竖屏控制：0，竖屏；1，横屏。	
	u16	wramcmd;		//开始写gram指令
	u16 setxcmd;		//设置x坐标指令
	u16 setycmd;		//设置y坐标指令 
}sky_lcd;

extern sky_lcd skylcd;              //管理LCD重要参数
//////////////////////////////////////////////////////////////////////////////////	 
//-----------------MCU屏 LCD端口定义---------------- 
#define	LCD_LED(n)          n?HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_RESET);             	//LCD背光	PB5	    
//LCD地址结构体
typedef struct
{
	vu16 LCD_REG;
	vu16 LCD_RAM;
} LCD_TypeDef;
//使用NOR/SRAM的 Bank1.sector1,地址位HADDR[27,26]=00 A18作为数据命令区分线 
//注意设置时STM32内部会右移一位对其!  			    
#define LCD_BASE        ((u32)(0x60000000 | 0x0007FFFE))
#define LCD             ((LCD_TypeDef *) LCD_BASE)
//////////////////////////////////////////////////////////////////////////////////

typedef enum{
    LCD_VERTICAL=0,
    LCD_LANDSCAPE
}LCD_DIR;

typedef enum{
    L2R_U2D=0,                      //从左到右,从上到下
    L2R_D2U,                        //从左到右,从下到上
    R2L_U2D,                        //从右到左,从上到下
    R2L_D2U,                        //从右到左,从下到上
    U2D_L2R,                        //从上到下,从左到右
    U2D_R2L,                        //从上到下,从右到左
    D2U_L2R,                        //从下到上,从左到右
    D2U_R2L                         //从下到上,从右到左
}SCAN_DIR;

#define DEFAULT_SCAN_DIR            L2R_U2D             //默认扫描方向


extern void LCD_Init(void);
extern void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p);
#endif