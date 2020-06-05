/**
  ******************************************************************************
  * @file    ADC_DMA/main.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   This example describes how to use the ADC and DMA to transfer 
  *          continuously converted data from ADC to memory.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "halFunct.h"
#include "coefficients.h"
#include "arm_math.h"


/** @addtogroup STM32F429I_DISCOVERY_Examples
  * @{
  */

/** @addtogroup ADC_DMA
  * @{
  */


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/ 

#define SENDING_BLOCKS 2
#define BUFFER_SIZE_16BITS	640
#define BUFFER_SIZE_8BITS	BUFFER_SIZE_16BITS*2

#define NUM_COEFF       NCOEFF

__IO uint8_t uartRxBuffer_8bits1[BUFFER_SIZE_8BITS];
__IO uint8_t uartRxBuffer_8bits2[BUFFER_SIZE_8BITS];

__IO uint8_t * uartRxBuffer_8bits[2] = {uartRxBuffer_8bits1 , uartRxBuffer_8bits2};

__IO uint8_t uartTxBuffer_8bits1[BUFFER_SIZE_8BITS*SENDING_BLOCKS + 5];
__IO uint8_t uartTxBuffer_8bits2[BUFFER_SIZE_8BITS*SENDING_BLOCKS + 5];

__IO uint8_t * uartTxBuffer_8bits[2] = {uartTxBuffer_8bits1 , uartTxBuffer_8bits1};

uint16_t data_in_buffer_16bits[BUFFER_SIZE_16BITS];
uint16_t data_out_buffer_16bits[BUFFER_SIZE_16BITS];

uint8_t data_out_buffer_8bits_right[BUFFER_SIZE_8BITS];
uint8_t data_out_buffer_8bits_left[BUFFER_SIZE_8BITS];

float32_t data_in_buffer_float[BUFFER_SIZE_16BITS];
float32_t data_out_buffer_float[BUFFER_SIZE_16BITS];

arm_fir_instance_f32 fir_struct_right_channel = {0};
arm_fir_instance_f32 fir_struct_left_channel = {0};



float fir_states_right_channel [BUFFER_SIZE_16BITS + NUM_COEFF - 1];
float fir_states_left_channel [BUFFER_SIZE_16BITS + NUM_COEFF - 1];

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* You can monitor the converted value by adding the variable "uhADC3ConvertedValue" 
   to the debugger watch window */


__IO uint8_t send_dma = 0, memory_full = 0, current_memory_in = 1, current_memory_out = 0;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/





int main(void)
{

	RCC_Configuration();
	NVIC_DMA_Tx_Configuration();
	NVIC_DMA_Rx_Configuration();
	GPIO_Configuration();
	UART_Configuration();
	DMA_Tx_Configuration(uartTxBuffer_8bits1, uartTxBuffer_8bits2, BUFFER_SIZE_8BITS*SENDING_BLOCKS +5);
	DMA_Rx_Configuration(uartRxBuffer_8bits1, uartRxBuffer_8bits2, BUFFER_SIZE_8BITS);
	ADC_Config();

	arm_fir_init_f32(&fir_struct_right_channel, NUM_COEFF, (float32_t *)&right[0], &fir_states_right_channel[0], BUFFER_SIZE_16BITS);
	arm_fir_init_f32(&fir_struct_left_channel, NUM_COEFF, (float32_t *)&left[0], &fir_states_left_channel[0], BUFFER_SIZE_16BITS);

	uint16_t angle;
	uint16_t index;

	while (1)
	  {


		while(memory_full==0){}
		memory_full = 0;

		angle = get_angle();
		index = angle * NCOEFF;

		/*************************** FORMATTING INPUT DATA ******************************/

		convert_8bits_to_16bits(uartRxBuffer_8bits[current_memory_in], data_in_buffer_16bits, BUFFER_SIZE_16BITS);

		arm_q15_to_float (( q15_t *)data_in_buffer_16bits, data_in_buffer_float, BUFFER_SIZE_16BITS);

		/*************************** RIGHT CHANNEL PROCESSING ******************************/

		fir_struct_right_channel.pCoeffs = (float32_t *)&right[index];

		arm_fir_f32	(	&fir_struct_right_channel,
				data_in_buffer_float,
				data_out_buffer_float,
				BUFFER_SIZE_16BITS
		);

		arm_float_to_q15 (data_out_buffer_float, (q15_t *)data_out_buffer_16bits, BUFFER_SIZE_16BITS);

		convert_16bits_to_8bits(data_out_buffer_16bits ,data_out_buffer_8bits_right, BUFFER_SIZE_16BITS);

		/*************************** LEFT CHANNEL PROCESSING ******************************/

	//	for(i=0;i<BUFFER_SIZE_16BITS;i++){
	//		data_out_buffer_16bits[i] = data_in_buffer_16bits[i];
	//	}

		fir_struct_left_channel.pCoeffs = (float32_t *)&left[index];

		arm_fir_f32	(	&fir_struct_left_channel,
				data_in_buffer_float,
				data_out_buffer_float,
				BUFFER_SIZE_16BITS
		);

		arm_float_to_q15 (data_out_buffer_float, (q15_t *)data_out_buffer_16bits, BUFFER_SIZE_16BITS);

		convert_16bits_to_8bits(data_out_buffer_16bits ,data_out_buffer_8bits_left, BUFFER_SIZE_16BITS);


		/*************************** SENDING OUTPUT DATA ******************************/

		if(SENDING_BLOCKS == 1){
			format_Tx_Block_Mono(data_out_buffer_8bits_right, uartTxBuffer_8bits1, BUFFER_SIZE_8BITS);
		}else{
			format_Tx_Block_Stereo(data_out_buffer_8bits_right, data_out_buffer_8bits_left, uartTxBuffer_8bits1, BUFFER_SIZE_8BITS);
		}


		while(send_dma==1){}
		send_dma = 1;
		sendBlockDMA();

	  }
}



#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
