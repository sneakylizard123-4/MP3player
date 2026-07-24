#include "stm32h7xx_hal.h"
#include "pins.h"
#include "config.h"

extern "C" {
extern UART_HandleTypeDef huart_pmm;
extern I2C_HandleTypeDef hi2c_sensors;
extern I2C_HandleTypeDef hi2c_audio;
extern I2C_HandleTypeDef hi2c_pwr;
extern I2C_HandleTypeDef hi2c_rtc;
extern I2C_HandleTypeDef hi2c_radio;
extern SPI_HandleTypeDef hspi_oled;
extern SPI_HandleTypeDef hspi_sensors;
extern SD_HandleTypeDef hsd1;

void SystemClock_Config(void);
void GPIO_Init(void);
void Error_Handler(void);
}

static void MX_I2C_Init(I2C_HandleTypeDef *hi2c, I2C_TypeDef *instance, uint32_t timing_val)
{
    hi2c->Instance = instance;
    hi2c->Init.Timing = timing_val;
    hi2c->Init.OwnAddress1 = 0;
    hi2c->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(hi2c) != HAL_OK) {
        Error_Handler();
    }
}

static void MX_SPI_Init(SPI_HandleTypeDef *hspi, SPI_TypeDef *instance)
{
    hspi->Instance = instance;
    hspi->Init.Mode = SPI_MODE_MASTER;
    hspi->Init.Direction = SPI_DIRECTION_2LINES;
    hspi->Init.DataSize = SPI_DATASIZE_8BIT;
    hspi->Init.CLKPolarity = SPI_POLARITY_HIGH;
    hspi->Init.CLKPhase = SPI_PHASE_2EDGE;
    hspi->Init.NSS = SPI_NSS_SOFT;
    hspi->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
    hspi->Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi->Init.TIMode = SPI_TIMODE_DISABLE;
    hspi->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi->Init.CRCPolynomial = 0x0;
    hspi->Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
    hspi->Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
    if (HAL_SPI_Init(hspi) != HAL_OK) {
        Error_Handler();
    }
}

static void MX_UART_Init(UART_HandleTypeDef *huart, USART_TypeDef *instance)
{
    huart->Instance = instance;
    huart->Init.BaudRate = cfg::UART_PMM_BAUD;
    huart->Init.WordLength = UART_WORDLENGTH_8B;
    huart->Init.StopBits = UART_STOPBITS_1;
    huart->Init.Parity = UART_PARITY_NONE;
    huart->Init.Mode = UART_MODE_TX_RX;
    huart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart->Init.OverSampling = UART_OVERSAMPLING_16;
    huart->Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart->Init.ClockPrescaler = UART_PRESCALER_DIV1;
    if (HAL_UART_Init(huart) != HAL_OK) {
        Error_Handler();
    }
}

extern "C" void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {};

    HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);
    while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_LSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.LSEState = RCC_LSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 4;
    RCC_OscInitStruct.PLL.PLLN = 240;
    RCC_OscInitStruct.PLL.PLLP = 1;
    RCC_OscInitStruct.PLL.PLLQ = 10;
    RCC_OscInitStruct.PLL.PLLR = 2;
    RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
    RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
    RCC_OscInitStruct.PLL.PLLFRACN = 0;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2
                                | RCC_CLOCKTYPE_D1PCLK1 | RCC_CLOCKTYPE_D3PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
    RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
    RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK) {
        Error_Handler();
    }

    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART3 | RCC_PERIPHCLK_I2C1
                                       | RCC_PERIPHCLK_I2C2 | RCC_PERIPHCLK_I2C3
                                       | RCC_PERIPHCLK_SAI1 | RCC_PERIPHCLK_SAI23
                                       | RCC_PERIPHCLK_SDMMC | RCC_PERIPHCLK_SPI123
                                       | RCC_PERIPHCLK_SPI45 | RCC_PERIPHCLK_USART1;

    PeriphClkInit.Sai1ClockSelection = RCC_SAI1CLKSOURCE_PLL2;
    PeriphClkInit.Sai23ClockSelection = RCC_SAI2CLKSOURCE_PLL2;
    PeriphClkInit.Usart16ClockSelection = RCC_USART16CLKSOURCE_D2PCLK2;
    PeriphClkInit.I2c123ClockSelection = RCC_I2C123CLKSOURCE_D2PCLK1;
    PeriphClkInit.I2c4ClockSelection = RCC_I2C4CLKSOURCE_D3PCLK1;
    PeriphClkInit.SdmmcClockSelection = RCC_SDMMCCLKSOURCE_PLL;
    PeriphClkInit.Spi123ClockSelection = RCC_SPI123CLKSOURCE_PLL2;
    PeriphClkInit.Spi45ClockSelection = RCC_SPI45CLKSOURCE_D2PCLK1;

    PeriphClkInit.PLL2.PLL2M = 4;
    PeriphClkInit.PLL2.PLL2N = 198;
    PeriphClkInit.PLL2.PLL2P = 5;
    PeriphClkInit.PLL2.PLL2Q = 2;
    PeriphClkInit.PLL2.PLL2R = 2;
    PeriphClkInit.PLL2.PLL2RGE = RCC_PLL2VCIRANGE_2;
    PeriphClkInit.PLL2.PLL2VCOSEL = RCC_PLL2VCOWIDE;
    PeriphClkInit.PLL2.PLL2FRACN = 0;

    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
        Error_Handler();
    }
}

extern "C" void GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {};

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();

    GPIO_InitStruct.Pin = BTN_POWER_Pin | BTN_VOL_UP_Pin | BTN_VOL_DOWN_Pin
                        | BTN_LOCK_Pin | BTN_RECORD_Pin | MIC_CHANNEL_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = BTN_MATRIX_COL1_Pin | BTN_MATRIX_COL2_Pin | BTN_MATRIX_COL3_Pin
                        | BTN_MATRIX_ROW1_Pin | BTN_MATRIX_ROW2_Pin;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = BTN_BOOT0_Pin | BTN_USER_Pin | BTN_MATRIX_ROW3_Pin
                        | IMU_INT2_Pin | LIGHT_INT_Pin;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = OLED_CS_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(OLED_CS_Port, &GPIO_InitStruct);
    HAL_GPIO_WritePin(OLED_CS_Port, OLED_CS_Pin, GPIO_PIN_SET);

    GPIO_InitStruct.Pin = OLED_DC_Pin;
    HAL_GPIO_Init(OLED_DC_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = OLED_RST_Pin;
    HAL_GPIO_Init(OLED_RST_Port, &GPIO_InitStruct);
    HAL_GPIO_WritePin(OLED_RST_Port, OLED_RST_Pin, GPIO_PIN_SET);

    GPIO_InitStruct.Pin = OLED_BL_Pin;
    HAL_GPIO_Init(OLED_BL_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = IMU_CS_Pin;
    HAL_GPIO_Init(IMU_CS_Port, &GPIO_InitStruct);
    HAL_GPIO_WritePin(IMU_CS_Port, IMU_CS_Pin, GPIO_PIN_SET);

    GPIO_InitStruct.Pin = GAS_CS_Pin;
    HAL_GPIO_Init(GAS_CS_Port, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GAS_CS_Port, GAS_CS_Pin, GPIO_PIN_SET);

    GPIO_InitStruct.Pin = AUDIO_RESET_Pin;
    HAL_GPIO_Init(AUDIO_RESET_Port, &GPIO_InitStruct);
    HAL_GPIO_WritePin(AUDIO_RESET_Port, AUDIO_RESET_Pin, GPIO_PIN_SET);

    GPIO_InitStruct.Pin = PMM_SHDN_Pin | PMM_FORCE_Pin;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = RADIO_RST_Pin;
    HAL_GPIO_Init(RADIO_RST_Port, &GPIO_InitStruct);
    HAL_GPIO_WritePin(RADIO_RST_Port, RADIO_RST_Pin, GPIO_PIN_SET);

    GPIO_InitStruct.Pin = SDMMC_DET_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(SDMMC_DET_Port, &GPIO_InitStruct);
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    GPIO_Init();

    MX_I2C_Init(&hi2c_sensors, I2C1, 0x10C0ECFF);
    MX_I2C_Init(&hi2c_audio, I2C4, 0x10C0ECFF);
    MX_I2C_Init(&hi2c_pwr, I2C2, 0x307075B1);
    MX_I2C_Init(&hi2c_rtc, I2C3, 0x10C0ECFF);

    MX_SPI_Init(&hspi_oled, SPI4);
    MX_SPI_Init(&hspi_sensors, SPI1);

    MX_UART_Init(&huart_pmm, USART3);

    while (true) {
        HAL_Delay(100);
    }
}
