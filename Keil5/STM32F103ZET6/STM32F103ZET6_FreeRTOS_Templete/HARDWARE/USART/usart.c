#include "usart.h"

UART_HandleTypeDef USART1_Handler;
UART_HandleTypeDef USART3_Handler;

uint16_t USART1_RX_STA = 0;
uint16_t USART3_RX_STA = 0;
char USART3_RX_STA_SIGN = 0;

uint8_t USART1_RX_BUF[200];
uint8_t USART3_RX_BUF[200];

//串口发送缓存区 	
__align(8) uint8_t USART3_TX_BUF[200];

#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40) == 0);//循环发送,直到发送完毕   
    USART1->DR = (uint8_t)ch;      
	return ch;
}
#endif 

void USART1_Init(uint32_t baud_rate)
{
    USART1_Handler.Instance = USART1;
    USART1_Handler.Init.BaudRate = baud_rate;
    USART1_Handler.Init.WordLength = UART_WORDLENGTH_8B;
    USART1_Handler.Init.StopBits = UART_STOPBITS_1;
    USART1_Handler.Init.Parity = UART_PARITY_NONE;
    USART1_Handler.Init.Mode = UART_MODE_TX_RX;
    HAL_UART_Init(&USART1_Handler);
}

void USART3_Init(uint32_t baud_rate)
{
    USART3_Handler.Instance = USART3;
    USART3_Handler.Init.BaudRate = baud_rate;
    USART3_Handler.Init.WordLength = UART_WORDLENGTH_8B;
    USART3_Handler.Init.StopBits = UART_STOPBITS_1;
    USART3_Handler.Init.Parity = UART_PARITY_NONE;
    USART3_Handler.Init.Mode = UART_MODE_TX_RX;
    HAL_UART_Init(&USART3_Handler);
}


void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1) 
    {
        GPIO_InitTypeDef GPIOA_Handler;
        __HAL_RCC_USART1_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_AFIO_CLK_ENABLE();

        GPIOA_Handler.Mode = GPIO_MODE_AF_PP;
        GPIOA_Handler.Pin = GPIO_PIN_9;
        GPIOA_Handler.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOA, &GPIOA_Handler);

        GPIOA_Handler.Mode = GPIO_MODE_AF_INPUT;
        GPIOA_Handler.Pin = GPIO_PIN_10;
        GPIOA_Handler.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOA, &GPIOA_Handler);

        __HAL_UART_ENABLE_IT(huart, UART_IT_RXNE);
        HAL_NVIC_EnableIRQ(USART1_IRQn);
        HAL_NVIC_SetPriority(USART1_IRQn, 3, 3);
    }

    if(huart->Instance == USART3) 
    {
        GPIO_InitTypeDef GPIOB_Handler;
        __HAL_RCC_USART3_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        __HAL_RCC_AFIO_CLK_ENABLE();

        GPIOB_Handler.Mode = GPIO_MODE_AF_PP;
        GPIOB_Handler.Pin = GPIO_PIN_10;
        GPIOB_Handler.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOB, &GPIOB_Handler);

        GPIOB_Handler.Mode = GPIO_MODE_AF_INPUT;
        GPIOB_Handler.Pin = GPIO_PIN_11;
        GPIOB_Handler.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOB, &GPIOB_Handler);

        __HAL_UART_ENABLE_IT(huart, UART_IT_RXNE);
		__HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
        HAL_NVIC_EnableIRQ(USART3_IRQn);
        HAL_NVIC_SetPriority(USART3_IRQn, 3, 3);
    }
}

//串口3,printf 函数
//确保一次发送数据不超过USART3_MAX_SEND_LEN字节
void u3_printf(char* fmt,...)
{  
	uint16_t i,j; 
	va_list ap; 
	va_start(ap,fmt);
	vsprintf((char*)USART3_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART3_TX_BUF);		//此次发送数据的长度
	for(j=0;j<i;j++)							//循环发送数据
	{
		while((USART3->SR&0X40)==0);			//循环发送,直到发送完毕   
		USART3->DR=USART3_TX_BUF[j];  
	}
}

void USART1_IRQHandler(void)
{
    uint8_t receive;
    if(__HAL_UART_GET_FLAG(&USART1_Handler, UART_FLAG_RXNE) == SET)
    {
        HAL_UART_Receive(&USART1_Handler, &receive, 1, 1000);
        if((USART1_RX_STA&0x8000) == 0)
        {
            if((USART1_RX_STA&0x4000)) 
            {
                if(receive != 0x0A) USART1_RX_STA = 0;
                else USART1_RX_STA |= 0x8000;
            }
            else
            {
                if(receive == 0x0D) USART1_RX_STA |= 0x4000;
                else
                {
                    USART1_RX_BUF[USART1_RX_STA] = receive;
                    USART1_RX_STA++;
                    if(USART1_RX_STA > 199) USART1_RX_STA = 0;
                }
            }
        }
    }
}


#if 0
void USART3_IRQHandler(void)
{
    uint8_t receive;
    if(__HAL_UART_GET_FLAG(&USART3_Handler, UART_FLAG_RXNE) == SET)
    {
        HAL_UART_Receive(&USART3_Handler, &receive, 1, 1000);
        if((USART3_RX_STA&0x8000) == 0)
        {
            if((USART3_RX_STA&0x4000)) 
            {
                if(receive != 0x0A) USART3_RX_STA = 0;
                else USART3_RX_STA |= 0x8000;
            }
            else
            {
                if(receive == 0x0D) USART3_RX_STA |= 0x4000;
                else
                {
                    USART3_RX_BUF[USART3_RX_STA] = receive;
                    USART3_RX_STA++;
                    if(USART3_RX_STA > 199) USART3_RX_STA = 0;
                }
            }
        }
		//__HAL_UART_CLEAR_FLAG(&USART3_Handler, UART_FLAG_RXNE);
    }
	if(__HAL_UART_GET_FLAG(&USART3_Handler, UART_FLAG_IDLE) == SET) 
	{
        USART3_RX_STA_SIGN = 1;
		//printf("UART_FLAG_IDLE\r\n");
        __HAL_UART_CLEAR_IDLEFLAG(&USART3_Handler);
	}
}
#endif

void USART3_IRQHandler(void)
{
    uint8_t receive;
    if(__HAL_UART_GET_FLAG(&USART3_Handler, UART_FLAG_RXNE) == SET)
    {
        HAL_UART_Receive(&USART3_Handler, &receive, 1, 1000);
        USART3_RX_BUF[USART3_RX_STA] = receive;
		USART3_RX_STA++;
		if(USART3_RX_STA > 199) USART3_RX_STA = 0;
		__HAL_UART_CLEAR_FLAG(&USART3_Handler, UART_FLAG_RXNE);
    }
	if(__HAL_UART_GET_FLAG(&USART3_Handler, UART_FLAG_IDLE) == SET) 
	{
        USART3_RX_STA_SIGN = 1;
        //HAL_ResumeTick();
        __HAL_UART_CLEAR_IDLEFLAG(&USART3_Handler);
	}
}
