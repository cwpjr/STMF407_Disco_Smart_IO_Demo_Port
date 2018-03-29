/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */
// STM32F407 Discovery Board test of Smart-IO board

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define FIX_G_S0_Pin GPIO_PIN_0
#define FIX_G_S0_GPIO_Port GPIOC
#define FIX_G_S1_Pin GPIO_PIN_1
#define FIX_G_S1_GPIO_Port GPIOC
#define FIX_G_S2_Pin GPIO_PIN_2
#define FIX_G_S2_GPIO_Port GPIOC
#define Smart_IO_SPI1_SCK_Pin GPIO_PIN_5
#define Smart_IO_SPI1_SCK_GPIO_Port GPIOA
#define Smart_IO_SPI1_MISO_Pin GPIO_PIN_6
#define Smart_IO_SPI1_MISO_GPIO_Port GPIOA
#define SMART_IO_SPI1_MOSI_Pin GPIO_PIN_7
#define SMART_IO_SPI1_MOSI_GPIO_Port GPIOA
#define FIX_ADC_Data_Ready_LOW_Pin GPIO_PIN_5
#define FIX_ADC_Data_Ready_LOW_GPIO_Port GPIOC
#define FIX_ADC_Reset_Pin GPIO_PIN_0
#define FIX_ADC_Reset_GPIO_Port GPIOB
#define FIX_ADC_PowerDown_Pin GPIO_PIN_1
#define FIX_ADC_PowerDown_GPIO_Port GPIOB
#define SPI_CS_ADC_Pin GPIO_PIN_12
#define SPI_CS_ADC_GPIO_Port GPIOD
#define SPI_CS_DAC_Pin GPIO_PIN_13
#define SPI_CS_DAC_GPIO_Port GPIOD
#define SPI_CS_Smart_IO_Pin GPIO_PIN_14
#define SPI_CS_Smart_IO_GPIO_Port GPIOD
#define Smart_IO_Reset_LOW_Pin GPIO_PIN_15
#define Smart_IO_Reset_LOW_GPIO_Port GPIOD
#define FIX_SW1_Pin GPIO_PIN_6
#define FIX_SW1_GPIO_Port GPIOC
#define FIX_SW2_Pin GPIO_PIN_7
#define FIX_SW2_GPIO_Port GPIOC
#define FIX_SW3_Pin GPIO_PIN_8
#define FIX_SW3_GPIO_Port GPIOC
#define FIX_SW4_Pin GPIO_PIN_9
#define FIX_SW4_GPIO_Port GPIOC
#define Smart_IO_Host_Irq_is_LOW_Pin GPIO_PIN_9
#define Smart_IO_Host_Irq_is_LOW_GPIO_Port GPIOA
#define SerialWireDebug_IO_Pin GPIO_PIN_13
#define SerialWireDebug_IO_GPIO_Port GPIOA
#define SerialWireDebug_CLK_Pin GPIO_PIN_14
#define SerialWireDebug_CLK_GPIO_Port GPIOA
#define UART3_TX_Pin GPIO_PIN_10
#define UART3_TX_GPIO_Port GPIOC
#define UART3_RX_Pin GPIO_PIN_11
#define UART3_RX_GPIO_Port GPIOC
#define SerialWireDebug_SWO_Pin GPIO_PIN_3
#define SerialWireDebug_SWO_GPIO_Port GPIOB

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
