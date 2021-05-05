#include "lan8720.h"

ETH_HandleTypeDef   ETH_Handler;                        //以太网句柄
ETH_DMADescTypeDef  *DMARxDscrTab;                      //以太网DMA接收描述符数据结构指针
ETH_DMADescTypeDef  *DMATxDscrTab;                      //以太网DMA发送描述符数据结构指针 
uint8_t *Rx_Buff;                                       //以太网底层驱动接收buffer指针
uint8_t *Tx_Buff;                                       //以太网底层驱动发送buffer指针

//关闭所有中断(但是不包括fault和NMI中断)
__asm void INTX_DISABLE(void)
{
	CPSID   I
	BX      LR	  
}

uint8_t LAN8720_Init(void)
{      
    uint8_t macaddress[6];
    
    INTX_DISABLE();                         //关闭所有中断，复位过程不能被打断！
    PCF8574_WriteBit(ETH_RESET_IO,1);       //硬件复位
    delay_ms(100);
    PCF8574_WriteBit(ETH_RESET_IO,0);       //复位结束
    delay_ms(100);
    INTX_ENABLE();                          //开启所有中断  

    macaddress[0]=lwipdev.mac[0]; 
	macaddress[1]=lwipdev.mac[1]; 
	macaddress[2]=lwipdev.mac[2];
	macaddress[3]=lwipdev.mac[3];   
	macaddress[4]=lwipdev.mac[4];
	macaddress[5]=lwipdev.mac[5];
        
	ETH_Handler.Instance=ETH;
    ETH_Handler.Init.AutoNegotiation=ETH_AUTONEGOTIATION_ENABLE;//使能自协商模式 
    ETH_Handler.Init.Speed=ETH_SPEED_100M;//速度100M,如果开启了自协商模式，此配置就无效
    ETH_Handler.Init.DuplexMode=ETH_MODE_FULLDUPLEX;//全双工模式，如果开启了自协商模式，此配置就无效
    ETH_Handler.Init.PhyAddress=LAN8720_PHY_ADDRESS;//LAN8720地址  
    ETH_Handler.Init.MACAddr=macaddress;            //MAC地址  
    ETH_Handler.Init.RxMode=ETH_RXINTERRUPT_MODE;   //中断接收模式 
    ETH_Handler.Init.ChecksumMode=ETH_CHECKSUM_BY_HARDWARE;//硬件帧校验  
    ETH_Handler.Init.MediaInterface=ETH_MEDIA_INTERFACE_RMII;//RMII接口  
    if(HAL_ETH_Init(&ETH_Handler)==HAL_OK)
    {
        return 0;   //成功
    }
    else return 1;  //失败
}


/**
 * ETH底层驱动，时钟使能，引脚配置
 * 
*/

void HAL_ETH_MspInit(ETH_HandleTypeDef *heth)
{
    GPIO_InitTypeDef    GPIO_InitStruct;
    __HAL_RCC_ETH_CLK_ENABLE();                         //开启ETH时钟
    __HAL_RCC_GPIOA_CLK_ENABLE();                       //开启GPIOA时钟
    __HAL_RCC_GPIOB_CLK_ENABLE();                       //开启GPIOB时钟
    __HAL_RCC_GPIOC_CLK_ENABLE();                       //开启GPIOC时钟
    __HAL_RCC_GPIOG_CLK_ENABLE();                       //开启GPIOG时钟

    /**网络引脚设置RMII接口
     * ETH_MDIO------------>PA2
     * ETH_MDC------------->PC1
     * ETH_RMII_REF_CLK---->PA1
     * ETH_RMII_CRS_DV----->PA7
     * ETH_RMII_RXD0------->PC4
     * ETH_RMII_RXD1------->PC5
     * ETH_RMII_TX_EN------>PB11
     * ETH_RMII_TXD0------->PG13
     * ETH_RMII_TXD1------->PG14
     * ETH_RESET----------->PCF8274扩展IO
    */
   //PA1,2,7
   GPIO_InitStruct.Pin =GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_7;
   GPIO_InitStruct.Mode =GPIO_MODE_AF_PP;                           //推挽复用
   GPIO_InitStruct.Pull =GPIO_NOPULL;                               //不带上下拉
   GPIO_InitStruct.Speed =GPIO_SPEED_HIGH;                          //高速
   GPIO_InitStruct.Alternate =GPIO_AF11_ETH;                        //复用为ETH功能
   HAL_GPIO_Init(GPIOA,&GPIO_InitStruct);                           //初始化

   //PB11
   GPIO_InitStruct.Pin =GPIO_PIN_11;
   HAL_GPIO_Init(GPIOB,&GPIO_InitStruct);

   //PC1,4,5
   GPIO_InitStruct.Pin =GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5;
   HAL_GPIO_Init(GPIOC,&GPIO_InitStruct);                           //初始化

   //PG13,14
   GPIO_InitStruct.Pin =GPIO_PIN_13|GPIO_PIN_14;                    //PG13,14
   HAL_GPIO_Init(GPIOG,&GPIO_InitStruct);                           

   HAL_NVIC_SetPriority(ETH_IRQn,1,0);
   HAL_NVIC_EnableIRQ(ETH_IRQn);

}

/**
 * 读取PHY寄存器值
*/
uint32_t LAN8720_ReadPHY(uint16_t reg)
{
    uint32_t regval;
    HAL_ETH_ReadPHYRegister(&ETH_Handler,reg,&regval);
    return regval;
}
/**
 * 向LAN8720指定寄存器写入值
 * reg:要写入的寄存器
 * valueL要写入的值
*/
void LAN8720_WritePHY(uint16_t reg,uint16_t value)
{
    uint32_t temp =value;
    HAL_ETH_ReadPHYRegister(&ETH_Handler,reg,&temp);
}

/**
 * 得到8720 的速度模式
 * 返回值:
 * 001:10M半双工
 * 101:10M全双工
 * 010:100M半双工
*/
uint8_t LAN8720_Get_Speed(void)
{
    uint8_t speed;
    speed =((LAN8720_ReadPHY(31)&0x1C) >>2);
    return speed;
}

//中断服务函数
void ETH_IRQHandler(void)
{
    while(ETH_GetRxPktSize(ETH_Handler.RxDesc))
    {
        lwip_pkt_hanle();                                                           //处理以太网数据，即将数据提交给LWIP
    }
    __HAL_ETH_DMA_CLEAR_IT(&ETH_Handler,ETH_DMA_IT_NIS);                            //清除DMA中断标志位
    __HAL_ETH_DMA_CLEAR_IT(&ETH_Handler,ETH_DMA_IT_R);                             //清除DMA接收中断标志位
}


/**
 * 获取接收到的帧长度
 * DMARxDesc:接收DMA描述符
 * 返回值：接收到的帧长度
*/
uint32_t ETH_GetRxPktSize(ETH_DMADescTypeDef *DMARxDscr)
{
    uint32_t frameLength =0;
    if(((DMARxDscr->Status &ETH_DMARXDESC_OWN) ==(uint32_t) RESET) &&
       ((DMARxDscr->Status &ETH_DMARXDESC_ES) ==(uint32_t) RESET)&&
       ((DMARxDscr->Status &ETH_DMARXDESC_LS) ==(uint32_t) RESET))
       {
           frameLength =((DMARxDscr->Status &ETH_DMARXDESC_FL) >>ETH_DMARXDESC_FRAME_LENGTHSHIFT);
       }
       return frameLength;
}
uint8_t ETH_Mem_Malloc(void)
{
    DMARxDscrTab =mymalloc(ETH_RXBUFNB*sizeof(ETH_DMADescTypeDef));
    DMATxDscrTab =mymalloc(ETH_TXBUFNB*sizeof(ETH_DMADescTypeDef));
    Rx_Buff =mymalloc(ETH_RX_BUF_SIZE*ETH_RXBUFNB);
    Tx_Buff =mymalloc(ETH_TX_BUF_SIZE*ETH_TXBUFNB);
    if(!(uint32_t)&DMARxDscrTab||!(uint32_t)&DMATxDscrTab||!(uint32_t)&Rx_Buff||!(uint32_t)&Tx_Buff)
    {
       ETH_Mem_Free();
       return 1;
    }
    return 0;
}



//释放ETH 底层驱动申请的内存
void ETH_Mem_Free(void)
{
    myfree(DMARxDscrTab);               //释放内存
    myfree(DMATxDscrTab);               //释放内存
    myfree(Rx_Buff);
    myfree(Tx_Buff);
}
