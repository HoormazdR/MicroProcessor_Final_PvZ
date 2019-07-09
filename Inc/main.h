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
  * COPYRIGHT(c) 2019 STMicroelectronics
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

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define DRDY_Pin GPIO_PIN_2
#define DRDY_GPIO_Port GPIOE
#define CS_I2C_SPI_Pin GPIO_PIN_3
#define CS_I2C_SPI_GPIO_Port GPIOE
#define MEMS_INT3_Pin GPIO_PIN_4
#define MEMS_INT3_GPIO_Port GPIOE
#define MEMS_INT4_Pin GPIO_PIN_5
#define MEMS_INT4_GPIO_Port GPIOE
#define MEMS_INT4_EXTI_IRQn EXTI9_5_IRQn
#define OSC32_IN_Pin GPIO_PIN_14
#define OSC32_IN_GPIO_Port GPIOC
#define OSC32_OUT_Pin GPIO_PIN_15
#define OSC32_OUT_GPIO_Port GPIOC
#define OSC_IN_Pin GPIO_PIN_0
#define OSC_IN_GPIO_Port GPIOF
#define OSC_OUT_Pin GPIO_PIN_1
#define OSC_OUT_GPIO_Port GPIOF
#define SevSeg_a3_Pin GPIO_PIN_0
#define SevSeg_a3_GPIO_Port GPIOC
#define SevSeg_a0_Pin GPIO_PIN_1
#define SevSeg_a0_GPIO_Port GPIOC
#define SevSeg_a1_Pin GPIO_PIN_2
#define SevSeg_a1_GPIO_Port GPIOC
#define SevSeg_a2_Pin GPIO_PIN_3
#define SevSeg_a2_GPIO_Port GPIOC
#define SSEG_i2_Pin GPIO_PIN_2
#define SSEG_i2_GPIO_Port GPIOF
#define B1_Pin GPIO_PIN_0
#define B1_GPIO_Port GPIOA
#define SSEG_i3_Pin GPIO_PIN_1
#define SSEG_i3_GPIO_Port GPIOA
#define SSEG_i1_Pin GPIO_PIN_2
#define SSEG_i1_GPIO_Port GPIOA
#define SSEG_i0_Pin GPIO_PIN_3
#define SSEG_i0_GPIO_Port GPIOA
#define SSEG_dot_Pin GPIO_PIN_4
#define SSEG_dot_GPIO_Port GPIOA
#define LD4_Pin GPIO_PIN_8
#define LD4_GPIO_Port GPIOE
#define LD3_Pin GPIO_PIN_9
#define LD3_GPIO_Port GPIOE
#define LD5_Pin GPIO_PIN_10
#define LD5_GPIO_Port GPIOE
#define LD7_Pin GPIO_PIN_11
#define LD7_GPIO_Port GPIOE
#define LD9_Pin GPIO_PIN_12
#define LD9_GPIO_Port GPIOE
#define LD10_Pin GPIO_PIN_13
#define LD10_GPIO_Port GPIOE
#define LD8_Pin GPIO_PIN_14
#define LD8_GPIO_Port GPIOE
#define LD6_Pin GPIO_PIN_15
#define LD6_GPIO_Port GPIOE
#define keypad_input_2nd_Pin GPIO_PIN_8
#define keypad_input_2nd_GPIO_Port GPIOA
#define keypad_input_2nd_EXTI_IRQn EXTI9_5_IRQn
#define keypad_input_4end_Pin GPIO_PIN_9
#define keypad_input_4end_GPIO_Port GPIOA
#define keypad_input_4end_EXTI_IRQn EXTI9_5_IRQn
#define keypad_input_1end_Pin GPIO_PIN_10
#define keypad_input_1end_GPIO_Port GPIOA
#define keypad_input_1end_EXTI_IRQn EXTI15_10_IRQn
#define DM_Pin GPIO_PIN_11
#define DM_GPIO_Port GPIOA
#define DP_Pin GPIO_PIN_12
#define DP_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define keypad_input_3nd_Pin GPIO_PIN_6
#define keypad_input_3nd_GPIO_Port GPIOF
#define keypad_input_3nd_EXTI_IRQn EXTI9_5_IRQn
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define I2C1_SCL_Pin GPIO_PIN_6
#define I2C1_SCL_GPIO_Port GPIOB
#define I2C1_SDA_Pin GPIO_PIN_7
#define I2C1_SDA_GPIO_Port GPIOB
#define MEMS_INT1_Pin GPIO_PIN_0
#define MEMS_INT1_GPIO_Port GPIOE
#define MEMS_INT2_Pin GPIO_PIN_1
#define MEMS_INT2_GPIO_Port GPIOE

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */

/*KEYPAD */

/*outs */
#define KEYPAD_COL1_PIN GPIO_PIN_4
#define KEYPAD_COL1_PORT GPIOC

#define KEYPAD_COL2_PIN GPIO_PIN_4
#define KEYPAD_COL2_PORT GPIOF

#define KEYPAD_COL3_PIN GPIO_PIN_6
#define KEYPAD_COL3_PORT GPIOA

#define KEYPAD_COL4_PIN GPIO_PIN_7
#define KEYPAD_COL4_PORT GPIOA


/*ins */
#define KEYPAD_ROW1_PIN GPIO_PIN_8
#define KEYPAD_ROW1_PORT GPIOA

#define KEYPAD_ROW2_PIN GPIO_PIN_9
#define KEYPAD_ROW2_PORT GPIOC


#define KEYPAD_ROW3_PIN GPIO_PIN_9
#define KEYPAD_ROW3_PORT GPIOA

#define KEYPAD_ROW4_PIN GPIO_PIN_10
#define KEYPAD_ROW4_PORT GPIOA

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
