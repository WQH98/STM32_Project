#include "lan8720.h"

ETH_HandleTypeDef ETH_Handler; 

#if defined ( __ICCARM__ ) /*!< IAR Compiler */
  #pragma data_alignment=4   
#endif
__ALIGN_BEGIN ETH_DMADescTypeDef  DMARxDscrTab[ETH_RXBUFNB] __ALIGN_END;/* Ethernet Rx MA Descriptor */

#if defined ( __ICCARM__ ) /*!< IAR Compiler */
  #pragma data_alignment=4   
#endif
__ALIGN_BEGIN ETH_DMADescTypeDef  DMATxDscrTab[ETH_TXBUFNB] __ALIGN_END;/* Ethernet Tx DMA Descriptor */

#if defined ( __ICCARM__ ) /*!< IAR Compiler */
  #pragma data_alignment=4   
#endif
__ALIGN_BEGIN uint8_t Rx_Buff[ETH_RXBUFNB][ETH_RX_BUF_SIZE] __ALIGN_END; /* Ethernet Receive Buffer */

#if defined ( __ICCARM__ ) /*!< IAR Compiler */
  #pragma data_alignment=4   
#endif
__ALIGN_BEGIN uint8_t Tx_Buff[ETH_TXBUFNB][ETH_TX_BUF_SIZE] __ALIGN_END; /* Ethernet Transmit Buffer */


HAL_StatusTypeDef lan8720_init(void)
{
	HAL_StatusTypeDef ret;
	
	HAL_ETH_DeInit(&ETH_Handler);
	
	uint8_t macaddress[6];
    
    pcf8574_write_bit(ETH_RESET_IO,1);       //硬件复位
    delay_ms(100);
    pcf8574_write_bit(ETH_RESET_IO,0);       //复位结束
    delay_ms(100);
    
	ETH->DMABMR |= ETH_DMABMR_SR;
	
    macaddress[0] = 02; 
	macaddress[1] = 00; 
	macaddress[2] = 00;
	macaddress[3] = 00;
	macaddress[4] = 00;
	macaddress[5] = 00;
        
	ETH_Handler.Instance = ETH;
    ETH_Handler.Init.AutoNegotiation = ETH_AUTONEGOTIATION_ENABLE;//使能自协商模式 
    ETH_Handler.Init.Speed = ETH_SPEED_100M;          //速度100M,如果开启了自协商模式，此配置就无效
    ETH_Handler.Init.DuplexMode = ETH_MODE_FULLDUPLEX;//全双工模式，如果开启了自协商模式，此配置就无效
    ETH_Handler.Init.PhyAddress = LAN8720_PHY_ADDRESS;//LAN8720地址  
    ETH_Handler.Init.MACAddr = macaddress;            //MAC地址  
    ETH_Handler.Init.RxMode = ETH_RXINTERRUPT_MODE;   //中断接收模式 
    ETH_Handler.Init.ChecksumMode = ETH_CHECKSUM_BY_HARDWARE;//硬件帧校验  
    ETH_Handler.Init.MediaInterface = ETH_MEDIA_INTERFACE_RMII;//RMII接口  
    ret = HAL_ETH_Init(&ETH_Handler);
	if(ret == HAL_OK)
    {
        printf("eth hardware init success...\r\n");
    }
    else
	{
		printf("eth hardware init faild...\r\n");
	}
	
	/* Initialize Tx Descriptors list: Chain Mode */
	HAL_ETH_DMATxDescListInit(&ETH_Handler, DMATxDscrTab, &Tx_Buff[0][0], ETH_TXBUFNB);
	 
	/* Initialize Rx Descriptors list: Chain Mode  */
	HAL_ETH_DMARxDescListInit(&ETH_Handler, DMARxDscrTab, &Rx_Buff[0][0], ETH_RXBUFNB);
	
	HAL_ETH_Start(&ETH_Handler);
	
	return ret;
}

void HAL_ETH_MspInit(ETH_HandleTypeDef* ethHandle)
{
	 GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_ETH_CLK_ENABLE();             //开启ETH时钟
    __HAL_RCC_GPIOA_CLK_ENABLE();			//开启GPIOA时钟
	__HAL_RCC_GPIOB_CLK_ENABLE();			//开启GPIOB时钟
    __HAL_RCC_GPIOC_CLK_ENABLE();			//开启GPIOC时钟
    __HAL_RCC_GPIOG_CLK_ENABLE();			//开启GPIOG时钟
    
    /*网络引脚设置 RMII接口 
    ETH_MDIO -------------------------> PA2
    ETH_MDC --------------------------> PC1
    ETH_RMII_REF_CLK------------------> PA1
    ETH_RMII_CRS_DV ------------------> PA7
    ETH_RMII_RXD0 --------------------> PC4
    ETH_RMII_RXD1 --------------------> PC5
    ETH_RMII_TX_EN -------------------> PB11
    ETH_RMII_TXD0 --------------------> PG13
    ETH_RMII_TXD1 --------------------> PG14
    ETH_RESET-------------------------> PCF8574扩展IO*/
    
    //PA1,2,7
    GPIO_Initure.Pin=GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_7; 
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;          //推挽复用
    GPIO_Initure.Pull=GPIO_NOPULL;              //不带上下拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;         //高速
    GPIO_Initure.Alternate=GPIO_AF11_ETH;       //复用为ETH功能
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);         //初始化
    
    //PB11
    GPIO_Initure.Pin=GPIO_PIN_11;               //PB11
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);         //始化
    
    //PC1,4,5
    GPIO_Initure.Pin=GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5; //PC1,4,5
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);         //初始化
	
    //PG13,14
    GPIO_Initure.Pin=GPIO_PIN_13|GPIO_PIN_14;   //PG13,14
    HAL_GPIO_Init(GPIOG,&GPIO_Initure);         //初始化
	
	HAL_NVIC_SetPriority(ETH_IRQn,1,0);         //网络中断优先级应该高一点
    HAL_NVIC_EnableIRQ(ETH_IRQn);
}

