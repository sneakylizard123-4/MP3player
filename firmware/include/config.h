#pragma once

#include <cstdint>

namespace cfg {
    // System
    constexpr uint32_t SYSCLK_FREQ_HZ = 480000000U;
    constexpr uint32_t APB1_FREQ_HZ    = 120000000U;
    constexpr uint32_t APB2_FREQ_HZ    = 120000000U;

    // I2C bus speeds
    constexpr uint32_t I2C_SENSORS_FREQ  = 400000U;
    constexpr uint32_t I2C_AUDIO_FREQ    = 400000U;
    constexpr uint32_t I2C_RADIO_FREQ    = 400000U;
    constexpr uint32_t I2C_RTC_FREQ      = 400000U;
    constexpr uint32_t I2C_PWR_FREQ      = 100000U;

    // SPI frequencies
    constexpr uint32_t SPI_OLED_FREQ     = 40000000U;
    constexpr uint32_t SPI_SENSORS_FREQ  = 10000000U;

    // UART
    constexpr uint32_t UART_PMM_BAUD    = 115200U;

    // Audio
    constexpr uint32_t AUDIO_SAMPLE_RATE = 44100U;
    constexpr uint32_t I2S_MCLK_FREQ     = 11289600U; // 256 * 44100

    // LED strip
    constexpr uint16_t NUM_LEDS_SPARKLES  = 35;
    constexpr uint16_t NUM_LEDS_SPECTRUM  = 35;
    constexpr uint16_t NUM_LEDS_TOTAL     = NUM_LEDS_SPARKLES + NUM_LEDS_SPECTRUM;

    // Button scan
    constexpr uint32_t BTN_DEBOUNCE_MS    = 30;
    constexpr uint32_t BTN_SCAN_INTERVAL_MS = 5;

    // PMM I2C address (STM32-based PMM module)
    constexpr uint8_t PMM_I2C_ADDR = 0x55;
}
