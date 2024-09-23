#include "uart.h"


UART_HandleTypeDef usart1_hander;
UART_HandleTypeDef usart2_hander;
UART_HandleTypeDef usart3_hander;

uart_rx_data_type uart1_message;
uart_rx_data_type uart2_message;
uart_rx_data_type uart3_message;


//串口发送缓存区
__align(8) uint8_t USART2_TX_BUF[256]; 	
__align(8) uint8_t USART3_TX_BUF[256];

//串口2,printf 函数
void u2_printf(char* fmt,...)
{  
	uint16_t i,j; 
	va_list ap; 
	va_start(ap,fmt);
	vsprintf((char*)USART2_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART2_TX_BUF);		//此次发送数据的长度
	for(j=0;j<i;j++)							//循环发送数据
	{
		while((USART2->SR&0X40)==0);			//循环发送,直到发送完毕   
		USART2->DR=USART2_TX_BUF[j];  
	}
}


//串口3,printf 函数
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

void uart1_init(uint32_t baud) {
    usart1_hander.Instance = USART1;
    usart1_hander.Init.BaudRate = 115200;
    usart1_hander.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    usart1_hander.Init.Mode = UART_MODE_TX_RX;
    usart1_hander.Init.Parity = UART_PARITY_NONE;
    usart1_hander.Init.StopBits = UART_STOPBITS_1;
    usart1_hander.Init.WordLength = UART_WORDLENGTH_8B;
    HAL_UART_Init(&usart1_hander);
}

void uart2_init(uint32_t baud) {
    usart2_hander.Instance = USART2;
    usart2_hander.Init.BaudRate = 115200;
    usart2_hander.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    usart2_hander.Init.Mode = UART_MODE_TX_RX;
    usart2_hander.Init.Parity = UART_PARITY_NONE;
    usart2_hander.Init.StopBits = UART_STOPBITS_1;
    usart2_hander.Init.WordLength = UART_WORDLENGTH_8B;
    HAL_UART_Init(&usart2_hander);
}

void uart3_init(uint32_t baud) {
    usart3_hander.Instance = USART3;
    usart3_hander.Init.BaudRate = 115200;
    usart3_hander.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    usart3_hander.Init.Mode = UART_MODE_TX_RX;
    usart3_hander.Init.Parity = UART_PARITY_NONE;
    usart3_hander.Init.StopBits = UART_STOPBITS_1;
    usart3_hander.Init.WordLength = UART_WORDLENGTH_8B;
    HAL_UART_Init(&usart3_hander);
}



void HAL_UART_MspInit(UART_HandleTypeDef *huart) {
    GPIO_InitTypeDef gpio_hander;

    if(huart->Instance == USART1) {

        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_USART1_CLK_ENABLE();
        __HAL_RCC_AFIO_CLK_ENABLE();

        gpio_hander.Mode = GPIO_MODE_AF_PP;
        gpio_hander.Pin = USART1_TX_PIN;
        gpio_hander.Pull = GPIO_NOPULL;
        gpio_hander.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(USART1_TX_PORT, &gpio_hander);

        gpio_hander.Mode = GPIO_MODE_AF_INPUT;
        gpio_hander.Pin = USART1_RX_PIN;
        HAL_GPIO_Init(USART1_RX_PORT, &gpio_hander);

        __HAL_UART_ENABLE_IT(huart, UART_IT_RXNE);
        __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
        HAL_NVIC_EnableIRQ(USART1_IRQn);
        HAL_NVIC_SetPriority(USART1_IRQn, 3, 3);
    }
    else if(huart->Instance == USART2) {

        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_USART2_CLK_ENABLE();
        __HAL_RCC_AFIO_CLK_ENABLE();

        gpio_hander.Mode = GPIO_MODE_AF_PP;
        gpio_hander.Pin = USART2_TX_PIN;
        gpio_hander.Pull = GPIO_NOPULL;
        gpio_hander.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(USART2_TX_PORT, &gpio_hander);

        gpio_hander.Mode = GPIO_MODE_AF_INPUT;
        gpio_hander.Pin = USART2_RX_PIN;
        HAL_GPIO_Init(USART2_RX_PORT, &gpio_hander);

        __HAL_UART_ENABLE_IT(huart, UART_IT_RXNE);
        __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
        HAL_NVIC_EnableIRQ(USART2_IRQn);
        HAL_NVIC_SetPriority(USART2_IRQn, 3, 2);
    }
    else if(huart->Instance == USART3) {

        __HAL_RCC_GPIOB_CLK_ENABLE();
        __HAL_RCC_USART3_CLK_ENABLE();
        __HAL_RCC_AFIO_CLK_ENABLE();

        gpio_hander.Mode = GPIO_MODE_AF_PP;
        gpio_hander.Pin = USART3_TX_PIN;
        gpio_hander.Pull = GPIO_NOPULL;
        gpio_hander.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(USART3_TX_PORT, &gpio_hander);

        gpio_hander.Mode = GPIO_MODE_AF_INPUT;
        gpio_hander.Pin = USART3_RX_PIN;
        HAL_GPIO_Init(USART3_RX_PORT, &gpio_hander);

        __HAL_UART_ENABLE_IT(huart, UART_IT_RXNE);
        __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
        HAL_NVIC_EnableIRQ(USART3_IRQn);
        HAL_NVIC_SetPriority(USART3_IRQn, 3, 1);
    }

}

void USART1_IRQHandler(void) {
    uint8_t receive;
    if(__HAL_UART_GET_FLAG(&usart1_hander, UART_FLAG_RXNE) == SET) {
        HAL_UART_Receive(&usart1_hander, &receive, 1, 1000);
        uart1_message.rx_buf[uart1_message.rx_len] = receive;
        uart1_message.rx_len++;
        if(uart1_message.rx_len >= 256) uart1_message.rx_len = 0;
        __HAL_UART_CLEAR_FLAG(&usart1_hander, UART_FLAG_RXNE);
    }

    if(__HAL_UART_GET_FLAG(&usart1_hander, UART_FLAG_IDLE) == SET) {
        uart1_message.rx_buf[uart1_message.rx_len] = '\0';
        uart1_message.rx_finsh = true;
        __HAL_UART_CLEAR_IDLEFLAG(&usart1_hander);
    }
}

void USART2_IRQHandler(void) {
    uint8_t receive;
    if(__HAL_UART_GET_FLAG(&usart2_hander, UART_FLAG_RXNE) == SET) {
        HAL_UART_Receive(&usart2_hander, &receive, 1, 1000);
        uart2_message.rx_buf[uart2_message.rx_len] = receive;
        uart2_message.rx_len++;
        if(uart2_message.rx_len >= 256) uart2_message.rx_len = 0;
        __HAL_UART_CLEAR_FLAG(&usart2_hander, UART_FLAG_RXNE);
    }

    if(__HAL_UART_GET_FLAG(&usart2_hander, UART_FLAG_IDLE) == SET) {
        uart2_message.rx_buf[uart2_message.rx_len] = '\0';
        uart2_message.rx_finsh = true;
        __HAL_UART_CLEAR_IDLEFLAG(&usart2_hander);
    }
}

void USART3_IRQHandler(void) {
    uint8_t receive;
    if(__HAL_UART_GET_FLAG(&usart3_hander, UART_FLAG_RXNE) == SET) {
        HAL_UART_Receive(&usart3_hander, &receive, 1, 1000);
        uart3_message.rx_buf[uart3_message.rx_len] = receive;
        uart3_message.rx_len++;
        if(uart3_message.rx_len >= 256) uart3_message.rx_len = 0;
        __HAL_UART_CLEAR_FLAG(&usart3_hander, UART_FLAG_RXNE);
    }

    if(__HAL_UART_GET_FLAG(&usart3_hander, UART_FLAG_IDLE) == SET) {
        uart3_message.rx_buf[uart3_message.rx_len] = '\0';
        uart3_message.rx_finsh = true;
        __HAL_UART_CLEAR_IDLEFLAG(&usart3_hander);
    }
}

void uart1_clear_message(void)
{
    uart1_message.rx_len = 0;
    memset(uart1_message.rx_buf, 0, sizeof(uart1_message.rx_buf));
    uart1_message.rx_finsh = false;
}

void uart2_clear_message(void)
{
    uart2_message.rx_len = 0;
    memset(uart2_message.rx_buf, 0, sizeof(uart2_message.rx_buf));
    uart2_message.rx_finsh = false;
}

void uart3_clear_message(void)
{
    uart3_message.rx_len = 0;
    memset(uart3_message.rx_buf, 0, sizeof(uart3_message.rx_buf));
    uart3_message.rx_finsh = false;
}
