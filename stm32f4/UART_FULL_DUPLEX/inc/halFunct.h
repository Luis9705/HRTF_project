#include "stm32f4xx.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_adc.h"


void sendBlockDMA(void);
void schmitt_trigger(uint16_t level, uint16_t hyst);
void convert_16bits_to_8bits(__IO uint16_t *data_16bits ,uint8_t *data_8bits, uint16_t size_16bits);
void convert_8bits_to_16bits(__IO uint8_t *data_8bits , uint16_t *data_16bits, uint16_t size_16bits);
void format_Tx_Block_Mono(uint8_t *data_input, __IO uint8_t *tx_block, uint16_t data_size);
void format_Tx_Block_Stereo(uint8_t *data_input_right,uint8_t *data_input_left, __IO uint8_t *tx_block, uint16_t data_size);
void TIM2_Config(void);
void RCC_Configuration(void);
void GPIO_Configuration(void);
void UART_Configuration(void);
void DMA_Tx_Configuration(__IO uint8_t * buffer1,__IO uint8_t * buffer2, uint16_t size);
void NVIC_DMA_Tx_Configuration(void);
void DMA_Rx_Configuration(__IO uint8_t * buffer1, __IO uint8_t * buffer2, uint16_t size);
void NVIC_DMA_Rx_Configuration(void);

