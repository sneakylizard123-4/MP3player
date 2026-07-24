#include "stm32h7xx_hal.h"
#include "pins.h"
#include "config.h"

extern "C" {

UART_HandleTypeDef huart_pmm;
I2C_HandleTypeDef hi2c_sensors;
I2C_HandleTypeDef hi2c_audio;
I2C_HandleTypeDef hi2c_pwr;
I2C_HandleTypeDef hi2c_rtc;
I2C_HandleTypeDef hi2c_radio;
SPI_HandleTypeDef hspi_oled;
SPI_HandleTypeDef hspi_sensors;
SD_HandleTypeDef hsd1;

void Error_Handler(void)
{
    __disable_irq();
    while (true) {}
}

void HAL_MspInit(void)
{
    __HAL_RCC_SYSCFG_CLK_ENABLE();
}

void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
    GPIO_InitTypeDef GPIO_InitStruct = {};
    if (hi2c->Instance == I2C1) {
        __HAL_RCC_I2C1_CLK_ENABLE();
        __HAL_RCC_GPIOD_CLK_ENABLE();
        GPIO_InitStruct.Pin = SENSORS_I2C_SCL_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
        HAL_GPIO_Init(SENSORS_I2C_SCL_Port, &GPIO_InitStruct);
        GPIO_InitStruct.Pin = SENSORS_I2C_SDA_Pin;
        HAL_GPIO_Init(SENSORS_I2C_SDA_Port, &GPIO_InitStruct);
    } else if (hi2c->Instance == I2C2) {
        __HAL_RCC_I2C2_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        GPIO_InitStruct.Pin = PWR_I2C_SCL_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF4_I2C2;
        HAL_GPIO_Init(PWR_I2C_SCL_Port, &GPIO_InitStruct);
        GPIO_InitStruct.Pin = PWR_I2C_SDA_Pin;
        HAL_GPIO_Init(PWR_I2C_SDA_Port, &GPIO_InitStruct);
    } else if (hi2c->Instance == I2C3) {
        __HAL_RCC_I2C3_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        GPIO_InitStruct.Pin = RTC_SCL_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF4_I2C3;
        HAL_GPIO_Init(RTC_SCL_Port, &GPIO_InitStruct);
        GPIO_InitStruct.Pin = RTC_SDA_Pin;
        HAL_GPIO_Init(RTC_SDA_Port, &GPIO_InitStruct);
    } else if (hi2c->Instance == I2C4) {
        __HAL_RCC_I2C4_CLK_ENABLE();
        __HAL_RCC_GPIOE_CLK_ENABLE();
        GPIO_InitStruct.Pin = AUDIO_SCL_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF4_I2C4;
        HAL_GPIO_Init(AUDIO_SCL_Port, &GPIO_InitStruct);
        GPIO_InitStruct.Pin = AUDIO_SDA_Pin;
        HAL_GPIO_Init(AUDIO_SDA_Port, &GPIO_InitStruct);
    }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef *hi2c)
{
    if (hi2c->Instance == I2C1) {
        __HAL_RCC_I2C1_CLK_DISABLE();
        HAL_GPIO_DeInit(SENSORS_I2C_SCL_Port, SENSORS_I2C_SCL_Pin);
        HAL_GPIO_DeInit(SENSORS_I2C_SDA_Port, SENSORS_I2C_SDA_Pin);
    } else if (hi2c->Instance == I2C2) {
        __HAL_RCC_I2C2_CLK_DISABLE();
        HAL_GPIO_DeInit(PWR_I2C_SCL_Port, PWR_I2C_SCL_Pin);
        HAL_GPIO_DeInit(PWR_I2C_SDA_Port, PWR_I2C_SDA_Pin);
    } else if (hi2c->Instance == I2C3) {
        __HAL_RCC_I2C3_CLK_DISABLE();
        HAL_GPIO_DeInit(RTC_SCL_Port, RTC_SCL_Pin);
        HAL_GPIO_DeInit(RTC_SDA_Port, RTC_SDA_Pin);
    } else if (hi2c->Instance == I2C4) {
        __HAL_RCC_I2C4_CLK_DISABLE();
        HAL_GPIO_DeInit(AUDIO_SCL_Port, AUDIO_SCL_Pin);
        HAL_GPIO_DeInit(AUDIO_SDA_Port, AUDIO_SDA_Pin);
    }
}

void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef GPIO_InitStruct = {};
    if (hspi->Instance == SPI4) {
        __HAL_RCC_SPI4_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        GPIO_InitStruct.Pin = OLED_SCK_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI4;
        HAL_GPIO_Init(OLED_SCK_Port, &GPIO_InitStruct);
        GPIO_InitStruct.Pin = OLED_MOSI_Pin;
        HAL_GPIO_Init(OLED_MOSI_Port, &GPIO_InitStruct);
    } else if (hspi->Instance == SPI1) {
        __HAL_RCC_SPI1_CLK_ENABLE();
        __HAL_RCC_GPIOE_CLK_ENABLE();
        GPIO_InitStruct.Pin = SENSORS_SPI_SCK_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
        HAL_GPIO_Init(SENSORS_SPI_SCK_Port, &GPIO_InitStruct);
        GPIO_InitStruct.Pin = SENSORS_SPI_MOSI_Pin;
        HAL_GPIO_Init(SENSORS_SPI_MOSI_Port, &GPIO_InitStruct);
        GPIO_InitStruct.Pin = SENSORS_SPI_MISO_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        HAL_GPIO_Init(SENSORS_SPI_MISO_Port, &GPIO_InitStruct);
    }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi)
{
    if (hspi->Instance == SPI4) {
        __HAL_RCC_SPI4_CLK_DISABLE();
        HAL_GPIO_DeInit(OLED_SCK_Port, OLED_SCK_Pin);
        HAL_GPIO_DeInit(OLED_MOSI_Port, OLED_MOSI_Pin);
    } else if (hspi->Instance == SPI1) {
        __HAL_RCC_SPI1_CLK_DISABLE();
        HAL_GPIO_DeInit(SENSORS_SPI_SCK_Port, SENSORS_SPI_SCK_Pin);
        HAL_GPIO_DeInit(SENSORS_SPI_MOSI_Port, SENSORS_SPI_MOSI_Pin);
        HAL_GPIO_DeInit(SENSORS_SPI_MISO_Port, SENSORS_SPI_MISO_Pin);
    }
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef GPIO_InitStruct = {};
    if (huart->Instance == USART3) {
        __HAL_RCC_USART3_CLK_ENABLE();
        __HAL_RCC_GPIOD_CLK_ENABLE();
        GPIO_InitStruct.Pin = PMM_TX_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
        HAL_GPIO_Init(PMM_TX_Port, &GPIO_InitStruct);
        GPIO_InitStruct.Pin = PMM_RX_Pin;
        HAL_GPIO_Init(PMM_RX_Port, &GPIO_InitStruct);
    }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART3) {
        __HAL_RCC_USART3_CLK_DISABLE();
        HAL_GPIO_DeInit(PMM_TX_Port, PMM_TX_Pin);
        HAL_GPIO_DeInit(PMM_RX_Port, PMM_RX_Pin);
    }
}

} // extern "C"
