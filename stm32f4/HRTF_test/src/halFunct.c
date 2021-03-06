
#include <halFunct.h>


#define BAUD_RATE 1540000//921600//1500000

void RCC_Configuration(void)
{
	/* --------------------------- System Clocks Configuration -----------------*/
	/* UART4 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	/* UART5 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);

	/* GPIOA clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	/* DMA1 clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
}

/**************************************************************************************/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/*-------------------------- GPIO Configuration ----------------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	/* Connect USART pins to AF */
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_UART4);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_UART4);


}

void UART_Configuration(void)
{

	USART_InitTypeDef USART_InitStructure;
	/* USARTx configuration ------------------------------------------------------*/
	/* USARTx configured as follow:
	- BaudRate = 1800000 baud
	- Word Length = 8 Bits
	- One Stop Bit
	- No parity
	- Hardware flow control disabled (RTS and CTS signals)
	- Receive and transmit enabled
	*/
	USART_InitStructure.USART_BaudRate = BAUD_RATE;//921600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART4, &USART_InitStructure);
	USART_Cmd(UART4, ENABLE);


	/* USARTx configuration ------------------------------------------------------*/
	/* USARTx configured as follow:
	- BaudRate = 1800000 baud
	- Word Length = 8 Bits
	- One Stop Bit
	- No parity
	- Hardware flow control disabled (RTS and CTS signals)
	- Receive and transmit enabled
	*/
	USART_InitStructure.USART_BaudRate = BAUD_RATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx;
	USART_Init(UART5, &USART_InitStructure);
	USART_Cmd(UART5, ENABLE);


}

void DMA_Tx_Configuration(__IO uint8_t * buffer1,__IO uint8_t * buffer2, uint16_t size)
{
	DMA_InitTypeDef DMA_InitStructure;
	DMA_DeInit(DMA1_Stream7);
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral; // Receive
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)buffer1;
	DMA_InitStructure.DMA_BufferSize = (uint16_t)size; //sizeof(Buffer);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(UART5->DR);
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA1_Stream7, &DMA_InitStructure);
	/* Enable the USART Rx DMA request */
	//USART_DMACmd(UART5, USART_DMAReq_Tx, ENABLE);
	/* Enable DMA Stream Half Transfer and Transfer Complete interrupt */
	DMA_ITConfig(DMA1_Stream7, DMA_IT_TC, ENABLE);
	//DMA_ITConfig(DMA1_Stream7, DMA_IT_HT, ENABLE);
	/* Enable the DMA RX Stream */
	//DMA_Cmd(DMA1_Stream7, ENABLE);

	//Double buffer
	DMA_MemoryTargetConfig(DMA1_Stream7, (uint32_t)buffer2,DMA_Memory_1);
	//DMA_DoubleBufferModeCmd(DMA1_Stream7, ENABLE);

}

void NVIC_DMA_Tx_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	/* Configure the Priority Group to 2 bits */
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	/* Enable the UART4 RX DMA Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void DMA_Rx_Configuration(__IO uint8_t * buffer1, __IO uint8_t * buffer2, uint16_t size){

	DMA_InitTypeDef DMA_InitStructure;
	DMA_DeInit(DMA1_Stream2);
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory; // Receive
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)buffer1;
	DMA_InitStructure.DMA_BufferSize = (uint16_t)size; //sizeof(Buffer);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(UART4->DR);
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA1_Stream2, &DMA_InitStructure);
	/* Enable the USART Rx DMA request */
	USART_DMACmd(UART4, USART_DMAReq_Rx, ENABLE);
	/* Enable DMA Stream Half Transfer and Transfer Complete interrupt */
	DMA_ITConfig(DMA1_Stream2, DMA_IT_TC, ENABLE);
	//DMA_ITConfig(DMA1_Stream2, DMA_IT_HT, ENABLE);

	//Double buffer
	DMA_MemoryTargetConfig(DMA1_Stream2, (uint32_t)buffer2,DMA_Memory_1);
	DMA_DoubleBufferModeCmd(DMA1_Stream2, ENABLE);

	/* Enable the DMA RX Stream */
	DMA_Cmd(DMA1_Stream2, ENABLE);

}
void NVIC_DMA_Rx_Configuration(void){
	NVIC_InitTypeDef NVIC_InitStructure;
	/* Configure the Priority Group to 2 bits */
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	/* Enable the UART4 RX DMA Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}





void convert_16bits_to_8bits(__IO uint16_t *data_16bits ,uint8_t *data_8bits, uint16_t size_16bits){
	int j;
	for(j=0;j<size_16bits;j++){
		data_8bits[j*2+1]=data_16bits[j]&0xFF;
		data_8bits[j*2]=data_16bits[j]>>8;
	}
}

void convert_8bits_to_16bits(__IO uint8_t *data_8bits , uint16_t *data_16bits, uint16_t size_16bits){
	int j;
	uint16_t lower, upper;
	for(j=0;j<size_16bits*2;j+=2){
		//data_16bits[j] = ((((int16_t)data_16bits[j])<<8) | (((int16_t)data_8bits[j*2+1])&0x00FF));
		lower = (data_8bits[j+1]&0xff);
		upper = (((uint16_t)data_8bits[j])<<8);
		data_16bits[j/2] =  upper| lower;
	}
}

void format_Tx_Block_Mono(uint8_t *data_input, __IO uint8_t *tx_block, uint16_t data_size){
	char * header = "#st";
	char * terminator = "\r\n";
	int j = 0;
	while(header[j]){
		tx_block[j] = header[j];
		j++;
	}

	for(j=0;j<data_size;j++){
		tx_block[j+3]=data_input[j];
	}

	j = 0;
	while(terminator[j]){
		tx_block[data_size + 3 + j] = terminator[j];
		j++;
	}

}

void format_Tx_Block_Stereo(uint8_t *data_input_right,uint8_t *data_input_left, __IO uint8_t *tx_block, uint16_t data_size){

	char * header = "#st";
	char * terminator = "\r\n";

	int j = 0;
	while(header[j]){
		tx_block[j] = header[j];
		j++;
	}

	for(j=0;j<data_size;j++){
		tx_block[j+3]=data_input_right[j];
	}

	for(j=0;j<data_size;j++){
		tx_block[j+3+data_size]=data_input_left[j];
	}

	j = 0;
	while(terminator[j]){
		tx_block[data_size*2 + 3 + j] = terminator[j];
		j++;
	}
}



void sendBlockDMA(void){

  /* Enable DMA USART TX Stream */
//	DMA_Cmd(DMA1_Stream7,DISABLE);
//	DMA1_Stream7->M0AR=(uint32_t)data;
//	DMA1_Stream7->NDTR=size;
    DMA_Cmd(DMA1_Stream7,ENABLE);
	/* Enable USART DMA TX Requsts */
	USART_DMACmd(UART5, USART_DMAReq_Tx, ENABLE);



}

void schmitt_trigger(uint16_t level, uint16_t hyst){
uint16_t val=0;
while(val<level+hyst){
	while(ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC) == RESET);
	val=ADC_GetConversionValue(ADC3);
}
while(val>level){
	while(ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC) == RESET);
	val=ADC_GetConversionValue(ADC3);
}
while(val>level-hyst){
	while(ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC) == RESET);
	val=ADC_GetConversionValue(ADC3);
}
}

void TIM2_Config(void)
{
  /* TIM6CLK = HCLK / 4 = SystemCoreClock /4 */

  TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
  /* TIM6 Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  /* Time base configuration */
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Period = 0x2F;
  TIM_TimeBaseStructure.TIM_Prescaler = TIM_ICPSC_DIV1;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV4;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  /* TIM6 TRGO selection */
  TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);
  TIM_GenerateEvent(TIM2, TIM_EventSource_Trigger);

  /* TIM6 enable counter */
 }


uint16_t ADC_getValue(void){
	ADC_SoftwareStartConv(ADC3);
	while (ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC) == RESET) {}
	return ADC_GetConversionValue(ADC3);

}

 uint16_t get_angle(void){
	 uint16_t conv;
	 conv = ADC_getValue();


	 return ((conv * 36) / 4095);
 }
/**
  * @brief  ADC3 channel13 with DMA configuration
  * @param  None
  * @retval None
  */
void ADC_Config(void)
{
  ADC_InitTypeDef       ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  GPIO_InitTypeDef      GPIO_InitStructure;

  /* Enable ADC3, DMA2 and GPIO clocks ****************************************/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);


  /* Configure ADC3 Channel13 pin as analog input ******************************/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /* ADC Common Init **********************************************************/
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);

  /* ADC3 Init ****************************************************************/
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_TRGO;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC3, &ADC_InitStructure);

  /* ADC3 regular channel13 configuration *************************************/
  ADC_RegularChannelConfig(ADC3, ADC_Channel_13, 1, ADC_SampleTime_3Cycles);

  /* Enable ADC3 */
  ADC_Cmd(ADC3, ENABLE);

}

