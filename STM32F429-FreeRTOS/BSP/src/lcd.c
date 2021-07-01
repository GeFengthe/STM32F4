#include "lcd.h"
#include "delay.h"

SRAM_HandleTypeDef  SRAM_Handler;                           //SRAM句柄
/**
 * SRAM底层驱动,时钟使能，引脚分配
 * 
*/
//LCD 管理结构体
//默认为竖屏
sky_lcd skylcd;

//写寄存器函数
//regval:寄存器值
void LCD_WR_REG(vu16 regval)
{   
	regval=regval;		//使用-O2优化的时候,必须插入的延时
	LCD->LCD_REG=regval;//写入要写的寄存器序号	 
}
//写LCD数据
//data:要写入的值
void LCD_WR_DATA(vu16 data)
{	  
	data=data;			//使用-O2优化的时候,必须插入的延时
	LCD->LCD_RAM=data;		 
}

//读LCD数据
//返回值:读到的值
u16 LCD_RD_DATA(void)
{
	vu16 ram;			//防止被优化
	ram=LCD->LCD_RAM;	
	return ram;	 
}

//写寄存器
//LCD_Reg:寄存器地址
//LCD_RegValue:要写入的数据
void LCD_WriteReg(u16 LCD_Reg,u16 LCD_RegValue)
{	
	LCD->LCD_REG = LCD_Reg;		//写入要写的寄存器序号	 
	LCD->LCD_RAM = LCD_RegValue;//写入数据	    		 
}

//读寄存器
//LCD_Reg:寄存器地址
//返回值:读到的数据
u16 LCD_ReadReg(u16 LCD_Reg)
{										   
	LCD_WR_REG(LCD_Reg);		//写入要读的寄存器序号
	delay_us(5);		  
	return LCD_RD_DATA();		//返回读到的值
}   
//开始写GRAM
void LCD_WriteRAM_Prepare(void)
{
 	LCD->LCD_REG =skylcd.wramcmd;	  
}	 
//LCD写GRAM
//RGB_Code:颜色值
void LCD_WriteRAM(u16 RGB_Code)
{							    
	LCD->LCD_RAM = RGB_Code;//写十六位GRAM
}

//从ILI93xx读出的数据为GBR格式，而我们写入的时候为RGB格式。
//通过该函数转换
//c:GBR格式的颜色值
//返回值：RGB格式的颜色值
u16 LCD_BGR2RGB(u16 c)
{
	u16  r,g,b,rgb;   
	b=(c>>0)&0x1f;
	g=(c>>5)&0x3f; 
	r=(c>>11)&0x1f;	 
	rgb=(b<<11)+(g<<5)+(r<<0);		 
	return(rgb);
}

//当mdk -O1时间优化时需要设置
//延时i
void opt_delay(u8 i)
{
	while(i--);
}

static void LCD_Display_Dir(u8 dir)
{
    skylcd.dir =dir;
    skylcd.wramcmd =LCD_WGRAMADDR;
    skylcd.setxcmd =LCD_WCOLUMNADDR;
    skylcd.setycmd =LCD_WPAGEADDR; 
    if(dir ==LCD_VERTICAL)
    {
        skylcd.width =240;
        skylcd.height =320;
   
    }else{
        skylcd.width =320;
        skylcd.height =240;
    }
    LCD_Scan_Dir(DFT_SCAN_DIR);	//默认扫描方向
}

static void sky_IL9341LCD_init(void)
{
    LCD_WR_REG(0xCF);  
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0xC1); 
	LCD_WR_DATA(0X30); 
	LCD_WR_REG(0xED);  
	LCD_WR_DATA(0x64); 
	LCD_WR_DATA(0x03); 
	LCD_WR_DATA(0X12); 
	LCD_WR_DATA(0X81); 
	LCD_WR_REG(0xE8);  
	LCD_WR_DATA(0x85); 
	LCD_WR_DATA(0x10); 
	LCD_WR_DATA(0x7A); 
	LCD_WR_REG(0xCB);  
	LCD_WR_DATA(0x39); 
	LCD_WR_DATA(0x2C); 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x34); 
	LCD_WR_DATA(0x02); 
	LCD_WR_REG(0xF7);  
	LCD_WR_DATA(0x20); 
	LCD_WR_REG(0xEA);  
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x00); 
	LCD_WR_REG(0xC0);    //Power control 
	LCD_WR_DATA(0x1B);   //VRH[5:0] 
	LCD_WR_REG(0xC1);    //Power control 
	LCD_WR_DATA(0x01);   //SAP[2:0];BT[3:0] 
	LCD_WR_REG(0xC5);    //VCM control 
	LCD_WR_DATA(0x30); 	 //3F
	LCD_WR_DATA(0x30); 	 //3C
	LCD_WR_REG(0xC7);    //VCM control2 
	LCD_WR_DATA(0XB7); 
	LCD_WR_REG(0x36);    // Memory Access Control 
	LCD_WR_DATA(0x48); 
	LCD_WR_REG(0x3A);   
	LCD_WR_DATA(0x55); 
	LCD_WR_REG(0xB1);   
	LCD_WR_DATA(0x00);   
	LCD_WR_DATA(0x1A); 
	LCD_WR_REG(0xB6);    // Display Function Control 
	LCD_WR_DATA(0x0A); 
	LCD_WR_DATA(0xA2); 
	LCD_WR_REG(0xF2);    // 3Gamma Function Disable 
	LCD_WR_DATA(0x00); 
	LCD_WR_REG(0x26);    //Gamma curve selected 
	LCD_WR_DATA(0x01); 
	LCD_WR_REG(0xE0);    //Set Gamma 
	LCD_WR_DATA(0x0F); 
	LCD_WR_DATA(0x2A); 
	LCD_WR_DATA(0x28); 
	LCD_WR_DATA(0x08); 
	LCD_WR_DATA(0x0E); 
	LCD_WR_DATA(0x08); 
	LCD_WR_DATA(0x54); 
	LCD_WR_DATA(0XA9); 
	LCD_WR_DATA(0x43); 
	LCD_WR_DATA(0x0A); 
	LCD_WR_DATA(0x0F); 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x00); 		 
	LCD_WR_REG(0XE1);    //Set Gamma 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x15); 
	LCD_WR_DATA(0x17); 
	LCD_WR_DATA(0x07); 
	LCD_WR_DATA(0x11); 
	LCD_WR_DATA(0x06); 
	LCD_WR_DATA(0x2B); 
	LCD_WR_DATA(0x56); 
	LCD_WR_DATA(0x3C); 
	LCD_WR_DATA(0x05); 
	LCD_WR_DATA(0x10); 
	LCD_WR_DATA(0x0F); 
	LCD_WR_DATA(0x3F); 
	LCD_WR_DATA(0x3F); 
	LCD_WR_DATA(0x0F); 
	LCD_WR_REG(0x2B); 
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x01);
	LCD_WR_DATA(0x3f);
	LCD_WR_REG(0x2A); 
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xef);	 
	LCD_WR_REG(0x11); //Exit Sleep
	delay_ms(120);
	LCD_WR_REG(0x29); //display on	
}

void HAL_SRAM_MspInit(SRAM_HandleTypeDef *hsram)
{
    GPIO_InitTypeDef    GPIO_InitStruct;

    __HAL_RCC_FMC_CLK_ENABLE();                                         //使能FMC时钟
    __HAL_RCC_GPIOD_CLK_ENABLE();                                       //使能GPIOD时钟
    __HAL_RCC_GPIOE_CLK_ENABLE();                                       //使能GPIOE时钟

    //初始化PD0,1,4,5,7,8,9,10,13,14,15
    GPIO_InitStruct.Pin =GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|\
                         GPIO_PIN_10|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
    GPIO_InitStruct.Mode =GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull =GPIO_PULLUP;
    GPIO_InitStruct.Speed =GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate =GPIO_AF12_FMC;           //复用FMC功能
    HAL_GPIO_Init(GPIOD,&GPIO_InitStruct);

    //初始化PE7,8,9,10,11,12,13,14,15
    GPIO_InitStruct.Pin =GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|\
                         GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
    HAL_GPIO_Init(GPIOD,&GPIO_InitStruct);
}

void LCD_Init(void)
{
    GPIO_InitTypeDef    GPIO_InitStruct;
    FMC_NORSRAM_TimingTypeDef   FMC_ReadWriteTim;
    FMC_NORSRAM_TimingTypeDef   FMC_WriteTim;

    __HAL_RCC_GPIOB_CLK_ENABLE();                                                   //开启GPIOB时钟
    GPIO_InitStruct.Pin =GPIO_PIN_5;
    GPIO_InitStruct.Mode =GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull =GPIO_PULLUP;
    GPIO_InitStruct.Speed =GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB,&GPIO_InitStruct);

    SRAM_Handler.Instance =FMC_NORSRAM_DEVICE;
    SRAM_Handler.Extended =FMC_NORSRAM_EXTENDED_DEVICE;

    SRAM_Handler.Init.NSBank =FMC_NORSRAM_BANK1;                                //使用NE1
    SRAM_Handler.Init.DataAddressMux =FMC_DATA_ADDRESS_MUX_DISABLE;             //地址/数据线不复用
    SRAM_Handler.Init.MemoryType = FMC_MEMORY_TYPE_SRAM;                        //SRAM
    SRAM_Handler.Init.MemoryDataWidth =FMC_NORSRAM_MEM_BUS_WIDTH_16;            //16位数据宽度
    SRAM_Handler.Init.BurstAccessMode =FMC_BURST_ACCESS_MODE_DISABLE;           //是否使用突发访问，仅对同步突发存储器有效，此处未用到。
    SRAM_Handler.Init.WaitSignalPolarity=FMC_WAIT_SIGNAL_POLARITY_LOW;          //等待信号的极性,仅在突发模式访问下有用
	SRAM_Handler.Init.WaitSignalActive=FMC_WAIT_TIMING_BEFORE_WS;                //存储器是在等待周期之前的一个时钟周期还是等待周期期间使能NWAIT
	SRAM_Handler.Init.WriteOperation=FMC_WRITE_OPERATION_ENABLE;                //存储器写使能
	SRAM_Handler.Init.WaitSignal=FMC_WAIT_SIGNAL_DISABLE;                       //等待使能位,此处未用到
	SRAM_Handler.Init.ExtendedMode=FMC_EXTENDED_MODE_ENABLE;                    //读写使用不同的时序
	SRAM_Handler.Init.AsynchronousWait=FMC_ASYNCHRONOUS_WAIT_DISABLE;           //是否使能同步传输模式下的等待信号,此处未用到
	SRAM_Handler.Init.WriteBurst=FMC_WRITE_BURST_DISABLE;                       //禁止突发写
	SRAM_Handler.Init.ContinuousClock=FMC_CONTINUOUS_CLOCK_SYNC_ASYNC;
    
    //FMC读时序控制寄存器
	FMC_ReadWriteTim.AddressSetupTime=0x0F;        //地址建立时间(ADDSET)为15个HCLK 1/180M*15=5.5ns*15=82.5ns
	FMC_ReadWriteTim.AddressHoldTime=0x00;
	FMC_ReadWriteTim.DataSetupTime=0x46;           //数据保存时间(DATAST)为70个HCLK	=5.5*70=385ns
	FMC_ReadWriteTim.AccessMode=FMC_ACCESS_MODE_A; //模式A
	//FMC写时序控制寄存器
	FMC_WriteTim.AddressSetupTime=0x0F;            //地址建立时间(ADDSET)为15个HCLK=82.5ns
	FMC_WriteTim.AddressHoldTime=0x00;
	FMC_WriteTim.DataSetupTime=0x0F;               //数据保存时间(DATAST)为5.5ns*15个HCLK=82.5ns
	FMC_WriteTim.AccessMode=FMC_ACCESS_MODE_A;     //模式A
	HAL_SRAM_Init(&SRAM_Handler,&FMC_ReadWriteTim,&FMC_WriteTim);		
	delay_ms(50); // delay 50 ms 
    
    //尝试9341 ID的读取
    LCD_WR_REG(LCD_READID);                 
    skylcd.id =LCD_RD_DATA();
    skylcd.id =LCD_RD_DATA();
    skylcd.id =LCD_RD_DATA();
    
    skylcd.id <<=8;
    skylcd.id |=LCD_RD_DATA();
    
    if(skylcd.id !=0x9341)
    {
        printf("LCD ERROR");
        return ;
    }
    sky_IL9341LCD_init();
    	//重新配置写时序控制寄存器的时序   	 							    
	FMC_Bank1E->BWTR[0]&=~(0XF<<0);	//地址建立时间(ADDSET)清零 	 
	FMC_Bank1E->BWTR[0]&=~(0XF<<8);	//数据保存时间清零
	FMC_Bank1E->BWTR[0]|=4<<0;	 	//地址建立时间(ADDSET)为4个HCLK =21ns  	 
	FMC_Bank1E->BWTR[0]|=4<<8; 		//数据保存时间(DATAST)为5.2ns*4个HCLK=21ns
    
    LCD_Display_Dir(0);		//默认为竖屏
	LCD_LED=1;				//点亮背光
	LCD_Clear(WHITE);
    
}