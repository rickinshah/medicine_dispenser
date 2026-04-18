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
#include "i2c.h"
#include "gpio.h"


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "pn532_stm32f4.h"
#include <string.h>   // for memcmp
#include "i2c-lcd.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
typedef struct {
    uint8_t uid[16];      // UID bytes (max 16 bytes, change if needed)
    uint8_t uid_len;      // Length of the UID
    char name[32];        // Name associated with the tag
    uint8_t tab1;         // Custom field tab1
    uint8_t tab2;         // Custom field tab2
} RFID_Data_t;


RFID_Data_t Data[3];



/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
 void MX_GPIO_Init(void);
 void MX_I2C1_Init(void);
 void MX_I2C2_Init(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void ConvA_Start(void)
{
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET);   // IN1 = 1

}

void ConvB_Start(void)
{
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET);   // IN1 = 1
}

void ConvA_Stop(void)
{
    HAL_GPIO_WritePin(GPIOE,
        GPIO_PIN_4,
        GPIO_PIN_RESET
    );
}

void ConvB_Stop(void)
{
    HAL_GPIO_WritePin(GPIOE,
        GPIO_PIN_5,
        GPIO_PIN_RESET
    );
}
/*
#define AUTH_UID_LENGTH 4
#define AUTH_UID_LENGTH_1 7
uint8_t auth_uid[AUTH_UID_LENGTH] = {0x89, 0xCE, 0xAB, 0x5A};
uint8_t auth_uid1[AUTH_UID_LENGTH_1] = {0x04, 0x5F, 0x3B, 0x42, 0xE3, 0x73, 0x80};
uint8_t auth_uid2[AUTH_UID_LENGTH_1] = {0x04, 0x11, 0x27, 0x42, 0xE3, 0x73, 0x81};
*/
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
Data[0] = (RFID_Data_t){ {0x89, 0xCE, 0xAB, 0x5A}, 4, "Shreesha", 1, 1 };
Data[1] = (RFID_Data_t){ {0x04, 0x5F, 0x3B, 0x42, 0xE3, 0x73, 0x80}, 7, "Rickin", 1, 0};
Data[2] = (RFID_Data_t){ {0x04, 0x6C, 0xA5, 0x5A, 0xE5, 0x73, 0x80}, 7, "Abhishek", 0, 1 };

 
/* USER CODE BEGIN 1 */
uint8_t buff[255];
	uint8_t uid[MIFARE_UID_MAX_LENGTH];
	int32_t uid_len = 0;

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

  /* USER CODE BEGIN 2 */

	lcd_init();
	 
		
		

 PN532 pn532;
  PN532_Init(&pn532);
  PN532_GetFirmwareVersion(&pn532, buff);
  if (PN532_GetFirmwareVersion(&pn532, buff) == PN532_STATUS_OK) {
	  HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);
	  HAL_Delay(500);

  } else {
    return -1;
  }
  PN532_SamConfiguration(&pn532);

	        HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);

						lcd_clear();
						HAL_Delay(500);
						lcd_send_cmd (0x80|0x01);
						lcd_send_string("MEDICINE DISPENCER");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	
	bool check = 0;
while (1)
{
    char name[32] = "";
    uid_len = PN532_ReadPassiveTarget(&pn532, uid, PN532_MIFARE_ISO14443A, 1000);

    if (uid_len == PN532_STATUS_ERROR) {
        lcd_send_cmd (0x80|0x14);
        lcd_send_string("Scan your Card");
        HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
        HAL_Delay(200);
    } 
    else 
    {
        check = 0;

        for(int i = 0; i < 3; i++) 
        {
            if(uid_len == Data[i].uid_len && memcmp(uid, Data[i].uid, Data[i].uid_len) == 0) 
            {
                check = 1;

                strcat(name, "Medicine, ");
                strcat(name, Data[i].name);

                for(int j = 0; j < Data[i].tab1; j++) {
                    ConvB_Start();
                    HAL_Delay(170);
                    ConvB_Stop();
                    HAL_Delay(100);
                }

                for(int j = 0; j < Data[i].tab2; j++) {
                    ConvA_Start();
                    HAL_Delay(170);
                    ConvA_Stop();
                    HAL_Delay(100);
                }

                lcd_send_cmd (0x80|0x14);
                lcd_send_string("Please collect your");
                lcd_send_cmd (0x80|0x54);
                lcd_send_string(name);

                HAL_Delay(1500);
                break;
            }
        }

    
        if (check == 0) {
            HAL_Delay(200);
            lcd_send_cmd(0x80 | 0x14);
            lcd_send_string("Card not registered");
            HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
            HAL_Delay(1500);
        }

        lcd_clear();
        HAL_Delay(500);
        lcd_send_cmd (0x80|0x01);
        lcd_send_string("MEDICINE DISPENCER");
    }
}
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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
#ifdef USE_FULL_ASSERT
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


