#pragma once

#include "stm32h7xx_hal.h"

// ─── Buttons (direct GPIO, active low) ─────────────────────────────
#define BTN_POWER_Pin       GPIO_PIN_0
#define BTN_POWER_Port      GPIOA
#define BTN_VOL_UP_Pin      GPIO_PIN_1
#define BTN_VOL_UP_Port     GPIOA
#define BTN_VOL_DOWN_Pin    GPIO_PIN_2
#define BTN_VOL_DOWN_Port   GPIOA
#define BTN_LOCK_Pin        GPIO_PIN_3
#define BTN_LOCK_Port       GPIOA
#define BTN_RECORD_Pin      GPIO_PIN_4
#define BTN_RECORD_Port     GPIOA
#define BTN_USER_Pin        GPIO_PIN_1
#define BTN_USER_Port       GPIOC
#define BTN_BOOT0_Pin       GPIO_PIN_0
#define BTN_BOOT0_Port      GPIOC

// ─── Button Matrix ─────────────────────────────────────────────────
#define BTN_MATRIX_ROW1_Pin GPIO_PIN_3
#define BTN_MATRIX_ROW1_Port GPIOB
#define BTN_MATRIX_ROW2_Pin GPIO_PIN_4
#define BTN_MATRIX_ROW2_Port GPIOB
#define BTN_MATRIX_ROW3_Pin GPIO_PIN_0
#define BTN_MATRIX_ROW3_Port GPIOC
#define BTN_MATRIX_COL1_Pin GPIO_PIN_0
#define BTN_MATRIX_COL1_Port GPIOB
#define BTN_MATRIX_COL2_Pin GPIO_PIN_1
#define BTN_MATRIX_COL2_Port GPIOB
#define BTN_MATRIX_COL3_Pin GPIO_PIN_2
#define BTN_MATRIX_COL3_Port GPIOB

// ─── OLED Display (SPI) ───────────────────────────────────────────
#define OLED_SCK_Pin        GPIO_PIN_5
#define OLED_SCK_Port       GPIOA
#define OLED_MOSI_Pin       GPIO_PIN_7
#define OLED_MOSI_Port      GPIOA
#define OLED_CS_Pin         GPIO_PIN_6
#define OLED_CS_Port        GPIOB
#define OLED_DC_Pin         GPIO_PIN_7
#define OLED_DC_Port        GPIOB
#define OLED_RST_Pin        GPIO_PIN_6
#define OLED_RST_Port       GPIOA
#define OLED_BL_Pin         GPIO_PIN_5
#define OLED_BL_Port        GPIOB

// ─── I2S Audio (CS43131 DAC) ───────────────────────────────────────
#define I2S_MCLK_Pin        GPIO_PIN_1
#define I2S_MCLK_Port       GPIOE
#define I2S_SDOUT_Pin       GPIO_PIN_14
#define I2S_SDOUT_Port      GPIOE
#define I2S_BCLK_Pin        GPIO_PIN_12
#define I2S_BCLK_Port       GPIOE
#define I2S_LRCLK_Pin       GPIO_PIN_11
#define I2S_LRCLK_Port      GPIOE
#define AUDIO_RESET_Pin     GPIO_PIN_4
#define AUDIO_RESET_Port    GPIOD
#define AUDIO_INT_Pin       GPIO_PIN_3
#define AUDIO_INT_Port      GPIOD

// ─── I2C: Audio DAC control ────────────────────────────────────────
#define AUDIO_SCL_Pin       GPIO_PIN_1
#define AUDIO_SCL_Port      GPIOE
#define AUDIO_SDA_Pin       GPIO_PIN_0
#define AUDIO_SDA_Port      GPIOE

// ─── I2C: Sensors (LSM6DSL, ISL29035, BME680) ────────────────────
#define SENSORS_I2C_SCL_Pin  GPIO_PIN_0
#define SENSORS_I2C_SCL_Port GPIOD
#define SENSORS_I2C_SDA_Pin  GPIO_PIN_1
#define SENSORS_I2C_SDA_Port GPIOD

// ─── SPI: Sensors (LSM6DSL, BME680) ───────────────────────────────
#define SENSORS_SPI_SCK_Pin  GPIO_PIN_13
#define SENSORS_SPI_SCK_Port GPIOE
#define SENSORS_SPI_MISO_Pin GPIO_PIN_5
#define SENSORS_SPI_MISO_Port GPIOE
#define SENSORS_SPI_MOSI_Pin GPIO_PIN_6
#define SENSORS_SPI_MOSI_Port GPIOE
#define IMU_CS_Pin           GPIO_PIN_8
#define IMU_CS_Port          GPIOE
#define GAS_CS_Pin           GPIO_PIN_10
#define GAS_CS_Port          GPIOE
#define IMU_INT1_Pin         GPIO_PIN_9
#define IMU_INT1_Port        GPIOE
#define IMU_INT2_Pin         GPIO_PIN_2
#define IMU_INT2_Port        GPIOC
#define LIGHT_INT_Pin        GPIO_PIN_3
#define LIGHT_INT_Port       GPIOC

// ─── I2S: Microphone (ICS-43434) ──────────────────────────────────
#define MIC_DATA_Pin        GPIO_PIN_2
#define MIC_DATA_Port       GPIOE
#define MIC_LRCLK_Pin       GPIO_PIN_3
#define MIC_LRCLK_Port      GPIOE
#define MIC_BCLK_Pin        GPIO_PIN_4
#define MIC_BCLK_Port       GPIOE
#define MIC_CHANNEL_Pin     GPIO_PIN_15
#define MIC_CHANNEL_Port    GPIOA

// ─── I2S: Radio (Si4735) ──────────────────────────────────────────
#define RADIO_I2S_BCLK_Pin  GPIO_PIN_10
#define RADIO_I2S_BCLK_Port GPIOB
#define RADIO_I2S_LRCLK_Pin GPIO_PIN_12
#define RADIO_I2S_LRCLK_Port GPIOB
#define RADIO_I2S_DATA_Pin  GPIO_PIN_15
#define RADIO_I2S_DATA_Port GPIOB
#define RADIO_SCL_Pin       GPIO_PIN_12
#define RADIO_SCL_Port      GPIOD
#define RADIO_SDA_Pin       GPIO_PIN_13
#define RADIO_SDA_Port      GPIOD
#define RADIO_RST_Pin       GPIO_PIN_14
#define RADIO_RST_Port      GPIOD
#define RADIO_IRQ_Pin       GPIO_PIN_11
#define RADIO_IRQ_Port      GPIOD
#define RADIO_GP1_Pin       GPIO_PIN_14
#define RADIO_GP1_Port      GPIOB

// ─── LED Strip (APA102) ───────────────────────────────────────────
#define LED_SCK_Pin         GPIO_PIN_11
#define LED_SCK_Port        GPIOB
#define LED_MOSI_Pin        GPIO_PIN_13
#define LED_MOSI_Port       GPIOB

// ─── SD Card (SDMMC) ──────────────────────────────────────────────
#define SDMMC_CLK_Pin       GPIO_PIN_12
#define SDMMC_CLK_Port      GPIOC
#define SDMMC_CMD_Pin       GPIO_PIN_13
#define SDMMC_CMD_Port      GPIOC
#define SDMMC_D0_Pin        GPIO_PIN_8
#define SDMMC_D0_Port       GPIOC
#define SDMMC_D1_Pin        GPIO_PIN_9
#define SDMMC_D1_Port       GPIOC
#define SDMMC_D2_Pin        GPIO_PIN_10
#define SDMMC_D2_Port       GPIOC
#define SDMMC_D3_Pin        GPIO_PIN_11
#define SDMMC_D3_Port       GPIOC
#define SDMMC_DET_Pin       GPIO_PIN_7
#define SDMMC_DET_Port      GPIOC

// ─── USB ───────────────────────────────────────────────────────────
#define USB_DM_Pin          GPIO_PIN_11
#define USB_DM_Port         GPIOA
#define USB_DP_Pin          GPIO_PIN_12
#define USB_DP_Port         GPIOA

// ─── RTC (DS3231M) ────────────────────────────────────────────────
#define RTC_SCL_Pin         GPIO_PIN_8
#define RTC_SCL_Port        GPIOA
#define RTC_SDA_Pin         GPIO_PIN_9
#define RTC_SDA_Port        GPIOA
#define RTC_INT_Pin         GPIO_PIN_10
#define RTC_INT_Port        GPIOA

// ─── Power Management (PMM) ───────────────────────────────────────
#define PMM_TX_Pin          GPIO_PIN_8
#define PMM_TX_Port         GPIOD
#define PMM_RX_Pin          GPIO_PIN_9
#define PMM_RX_Port         GPIOD
#define PMM_SHDN_Pin        GPIO_PIN_10
#define PMM_SHDN_Port       GPIOD
#define PMM_FORCE_Pin       GPIO_PIN_7
#define PMM_FORCE_Port      GPIOD
#define PMM_INIT_Pin        GPIO_PIN_15
#define PMM_INIT_Port       GPIOD
#define PWR_I2C_SCL_Pin     GPIO_PIN_8
#define PWR_I2C_SCL_Port    GPIOB
#define PWR_I2C_SDA_Pin     GPIO_PIN_9
#define PWR_I2C_SDA_Port    GPIOB

// ─── Debug (SWD) ──────────────────────────────────────────────────
#define SWDIO_Pin           GPIO_PIN_13
#define SWDIO_Port          GPIOA
#define SWCLK_Pin           GPIO_PIN_14
#define SWCLK_Port          GPIOA
