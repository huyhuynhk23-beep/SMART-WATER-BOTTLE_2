/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "i2c_lcd.h"
#include <stdio.h>   // để dùng sprintf
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define DS1307_I2C_ADDR 	0x68
#define DS1307_REG_SECOND 	0x00
#define DS1307_REG_MINUTE 	0x01
#define DS1307_REG_HOUR  	0x02
#define DS1307_REG_DOW    	0x03
#define DS1307_REG_DATE   	0x04
#define DS1307_REG_MONTH  	0x05
#define DS1307_REG_YEAR   	0x06
#define DS1307_REG_CONTROL 	0x07
#define DS1307_REG_UTC_HR	0x08
#define DS1307_REG_UTC_MIN	0x09
#define DS1307_REG_CENT    	0x10
#define DS1307_TIMEOUT		1000


typedef struct {
	uint8_t	sec;
	uint8_t min;
	uint8_t hour;
	uint8_t dow;
	uint8_t date;
	uint8_t month;
	uint16_t year;
}DS1307_STRUCT;

DS1307_STRUCT	ds1307;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;

TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */
I2C_LCD_HandleTypeDef lcd1;

int32_t last_weight_mg = 0;     // khối lượng lần đo trước (mg)
float   daily_total_ml = 0;     // tổng lượng nước trong ngày (ml)
uint8_t prev_date = 0;          // lưu ngày trước đó để phát hiện sang ngày mới
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2C2_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */
uint8_t DS1307_DecodeBCD(uint8_t bin);
uint8_t DS1307_EncodeBCD(uint8_t dec);
void DS1307_SetClockHalt(uint8_t halt);
void DS1307_SetRegByte(uint8_t regAddr, uint8_t val);
void DS1307_SetTimeZone(int8_t hr, uint8_t min);
uint8_t DS1307_GetClockHalt(void);
uint8_t DS1307_GetRegByte(uint8_t regAddr);
void DS1307_config();
void DS1307_gettime();
void DS1307_settime(uint8_t sec,uint8_t min,uint8_t hour_24mode,uint8_t dayOfWeek,uint8_t date,uint8_t month, uint16_t year);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define DT_PIN GPIO_PIN_8
#define DT_PORT GPIOB
#define SCK_PIN GPIO_PIN_9
#define SCK_PORT GPIOB

uint32_t tare = 7831281;
float knownOriginal = 110000;  // in milli gram
float knownHX711 = 42386;
int weight;

void microDelay(uint16_t delay)
{
  __HAL_TIM_SET_COUNTER(&htim2, 0);
  while (__HAL_TIM_GET_COUNTER(&htim2) < delay);
}

int32_t getHX711(void)
{
  uint32_t data = 0;
  uint32_t startTime = HAL_GetTick();
  while(HAL_GPIO_ReadPin(DT_PORT, DT_PIN) == GPIO_PIN_SET)
  {
    if(HAL_GetTick() - startTime > 200)
      return 0;
  }
  for(int8_t len=0; len<24 ; len++)
  {
    HAL_GPIO_WritePin(SCK_PORT, SCK_PIN, GPIO_PIN_SET);
    microDelay(1);
    data = data << 1;
    HAL_GPIO_WritePin(SCK_PORT, SCK_PIN, GPIO_PIN_RESET);
    microDelay(1);
    if(HAL_GPIO_ReadPin(DT_PORT, DT_PIN) == GPIO_PIN_SET)
      data ++;
  }
  data = data ^ 0x800000;
  HAL_GPIO_WritePin(SCK_PORT, SCK_PIN, GPIO_PIN_SET);
  microDelay(1);
  HAL_GPIO_WritePin(SCK_PORT, SCK_PIN, GPIO_PIN_RESET);
  microDelay(1);
  return data;
}

int weigh()
{
  int32_t  total = 0;
  int32_t  samples = 50;
  int milligram;
  float coefficient;
  for(uint16_t i=0 ; i<samples ; i++)
  {
      total += getHX711();
  }
  int32_t average = (int32_t)(total / samples);
  coefficient = knownOriginal / knownHX711;
  milligram = (int)(average-tare)*coefficient;
  return milligram;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  lcd1.hi2c = &hi2c1;
  lcd1.address = 0x4E;
  lcd_init(&lcd1);

  lcd_clear(&lcd1);
  lcd_puts(&lcd1, "STM32 I2C LCD");
  lcd_gotoxy(&lcd1, 0, 1);
  lcd_puts(&lcd1, "Library Demo");
  DS1307_config();
 // DS1307_settime(0, 59, 23, 4,29, 2, 2024); // 23:59:30 t4, 29/2/2024

  HAL_TIM_Base_Start(&htim2);
  HAL_GPIO_WritePin(SCK_PORT, SCK_PIN, GPIO_PIN_SET);
  HAL_Delay(10);
  HAL_GPIO_WritePin(SCK_PORT, SCK_PIN, GPIO_PIN_RESET);
  HAL_Delay(10);

  // Đọc lần đầu để lấy mốc ban đầu
    DS1307_gettime();
    weight = weigh();          // mg
    last_weight_mg = weight;   // lưu mốc ban đầu
    prev_date = ds1307.date;   // lưu ngày hiện tại
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  // 1. Đọc thời gian từ RTC
	      DS1307_gettime();

	      // 2. Đọc loadcell (mg)
	      int32_t current_weight_mg = weigh();

	      // 3. Kiểm tra sang ngày mới -> reset tổng
	      if (ds1307.date != prev_date)
	      {
	          daily_total_ml = 0;        // reset tổng nước trong ngày
	          prev_date = ds1307.date;   // cập nhật lại ngày
	      }

	      // 4. Tính lượng nước đã uống (dựa vào giảm khối lượng bình)
	      int32_t diff = last_weight_mg - current_weight_mg;
	      if (diff > 0)
	      {
	          // diff > 0: nước trong bình giảm -> đã uống thêm
	          daily_total_ml += (float)diff / 1000.0f; // mg -> ml
	      }
	      // cập nhật lại mốc
	      last_weight_mg = current_weight_mg;

	      // 5. Thể tích hiện tại trong bình (ml) và tổng trong ngày (ml)
	      int current_volume_ml = current_weight_mg / 1000; // mg -> ml
	      int daily_ml          = (int)daily_total_ml;

	      // 6. Hiển thị lên LCD
	      lcd_clear(&lcd1);

	      char line1[20];
	      char line2[20];

	      // Dòng 1: hh:mm:ss (RTC)
	      sprintf(line1, "%02d:%02d:%02d",
	              ds1307.hour, ds1307.min, ds1307.sec);

	      // Dòng 2: binh: Xxxml tong: Yyyml
	      // Lưu ý LCD 16x2, nếu số lớn có thể bị tràn, bạn có thể rút gọn format lại
	      sprintf(line2, "B:%3d T:%3dml", current_volume_ml, daily_ml);

	      lcd_gotoxy(&lcd1, 0, 0);
	      lcd_puts(&lcd1, line1);

	      lcd_gotoxy(&lcd1, 0, 1);
	      lcd_puts(&lcd1, line2);

	      // Delay 1 giây -> RTC nhảy từng giây, LCD update theo
	      HAL_Delay(100);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 400000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 400000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 71;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 65535;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pin : PB8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void DS1307_gettime(){
	uint16_t cen;
	ds1307.sec=DS1307_DecodeBCD(DS1307_GetRegByte(DS1307_REG_SECOND) & 0x7f);
	ds1307.min=DS1307_DecodeBCD(DS1307_GetRegByte(DS1307_REG_MINUTE));
	ds1307.hour=DS1307_DecodeBCD(DS1307_GetRegByte(DS1307_REG_HOUR) & 0x3f);
	ds1307.dow=DS1307_DecodeBCD(DS1307_GetRegByte(DS1307_REG_DOW));
	ds1307.date=DS1307_DecodeBCD(DS1307_GetRegByte(DS1307_REG_DATE));
	ds1307.month=DS1307_DecodeBCD(DS1307_GetRegByte(DS1307_REG_MONTH));
	cen = DS1307_GetRegByte(DS1307_REG_CENT) * 100;
	ds1307.year=DS1307_DecodeBCD(DS1307_GetRegByte(DS1307_REG_YEAR)) + cen;

}
void DS1307_SetRegByte(uint8_t regAddr, uint8_t val) {
	uint8_t bytes[2] = { regAddr, val };
	HAL_I2C_Master_Transmit(&hi2c2, DS1307_I2C_ADDR << 1, bytes, 2, DS1307_TIMEOUT);
}
uint8_t DS1307_GetClockHalt(void) {
	return (DS1307_GetRegByte(DS1307_REG_SECOND) & 0x80) >> 7;
}

void DS1307_settime(uint8_t sec,uint8_t min,uint8_t hour_24mode,uint8_t dayOfWeek,uint8_t date,uint8_t month, uint16_t year)
{
	uint8_t ch = DS1307_GetClockHalt() << 7;

	DS1307_SetRegByte(DS1307_REG_SECOND, DS1307_EncodeBCD(sec | DS1307_GetClockHalt()));
	DS1307_SetRegByte(DS1307_REG_MINUTE, DS1307_EncodeBCD(min));
	DS1307_SetRegByte(DS1307_REG_HOUR, DS1307_EncodeBCD(hour_24mode & 0x3f));//hour_24mode Hour in 24h format, 0 to 23.
	DS1307_SetRegByte(DS1307_REG_DOW, DS1307_EncodeBCD(dayOfWeek));//dayOfWeek Days since last Sunday, 0 to 6.
	DS1307_SetRegByte(DS1307_REG_DATE, DS1307_EncodeBCD(date));//date Day of month, 1 to 31.
	DS1307_SetRegByte(DS1307_REG_MONTH, DS1307_EncodeBCD(month));//month Month, 1 to 12.
	DS1307_SetRegByte(DS1307_REG_CENT, year / 100);
	DS1307_SetRegByte(DS1307_REG_YEAR, DS1307_EncodeBCD(year % 100));//2000 to 2099.

}
uint8_t DS1307_GetRegByte(uint8_t regAddr) {
	uint8_t val;
	HAL_I2C_Master_Transmit(&hi2c2, DS1307_I2C_ADDR << 1, &regAddr, 1, DS1307_TIMEOUT);
	HAL_I2C_Master_Receive(&hi2c2, DS1307_I2C_ADDR << 1, &val, 1, DS1307_TIMEOUT);
	return val;
}
void DS1307_SetClockHalt(uint8_t halt) {
	uint8_t ch = (halt ? 1 << 7 : 0);
	DS1307_SetRegByte(DS1307_REG_SECOND, ch | (DS1307_GetRegByte(DS1307_REG_SECOND) & 0x7f));
}
/**
 * @brief Sets UTC offset.
 * @note  UTC offset is not updated automatically.
 * @param hr UTC hour offset, -12 to 12.
 * @param min UTC minute offset, 0 to 59.
 */
void DS1307_SetTimeZone(int8_t hr, uint8_t min) {
	DS1307_SetRegByte(DS1307_REG_UTC_HR, hr);
	DS1307_SetRegByte(DS1307_REG_UTC_MIN, min);
}
void DS1307_config(){
	DS1307_SetClockHalt(0);
	DS1307_SetTimeZone(+8, 00);
}
uint8_t DS1307_DecodeBCD(uint8_t bin) {
	return (((bin & 0xf0) >> 4) * 10) + (bin & 0x0f);
}

uint8_t DS1307_EncodeBCD(uint8_t dec) {
	return (dec % 10 + ((dec / 10) << 4));
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
