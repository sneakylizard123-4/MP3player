# MP3 Player Firmware

PlatformIO firmware for an STM32H743VIT6-based portable MP3 player.

## Hardware

| Component | Part | Interface |
|-----------|------|-----------|
| MCU | STM32H743VIT6 (Cortex-M7, 480MHz, 2MB Flash, 1MB RAM) | — |
| Audio DAC | CS43131-CNZR | I2S + I2C |
| FM Radio | Si4735-D60-GU | I2S + I2C |
| Microphone | ICS-43434 | I2S |
| IMU | LSM6DSL | SPI |
| Gas Sensor | BME680 | SPI |
| Light Sensor | ISL29035 | I2C |
| RTC | DS3231M | I2C |
| OLED Display | SPI (via Conn_01x08) | SPI |
| SD Card | Micro SD (4-bit SDMMC) | SDMMC |
| LEDs | 70x APA102-2020 | SPI |
| Power Mgmt | PMM (STM32-based module) | UART + I2C |
| USB | USB 2.0 (USB-C) | USB FS |

## Build

```bash
cd firmware
pio run
```

Output: `.pio/build/stm32h743/firmware.bin`

## Flash

```bash
pio run -t upload
```

Requires ST-Link, CMSIS-DAP, or J-Link debugger connected via SWD.

## Project Structure

```
firmware/
├── platformio.ini                  # Build config
├── include/
│   ├── stm32h7xx_hal_conf.h       # HAL module selection
│   ├── pins.h                      # All GPIO assignments from schematic
│   └── config.h                    # Clock, bus speed, and system constants
└── src/
    ├── main.cpp                    # Clock config, peripheral init, main loop
    └── msp.cpp                     # HAL MSP callbacks (pin muxing)
```

## Pin Mapping

### Buttons (direct, active low)
| Pin | Signal |
|-----|--------|
| PA0 | BTN_POWER |
| PA1 | BTN_VOL_UP |
| PA2 | BTN_VOL_DOWN |
| PA3 | BTN_LOCK |
| PA4 | BTN_RECORD |
| PC1 | BTN_USER |
| PC0 | BTN_BOOT0 |

### Button Matrix (3x3)
| Pin | Signal |
|-----|--------|
| PB0 | COL1 |
| PB1 | COL2 |
| PB2 | COL3 |
| PB3 | ROW1 |
| PB4 | ROW2 |
| PC0 | ROW3 |

### OLED Display (SPI4)
| Pin | Signal |
|-----|--------|
| PA5 | OLED_SCK |
| PA7 | OLED_MOSI |
| PB6 | OLED_CS |
| PB7 | OLED_DC |
| PA6 | OLED_RST |
| PB5 | OLED_BL |

### Audio DAC (CS43131 — SAI1 + I2C4)
| Pin | Signal |
|-----|--------|
| PE1 | I2S_MCLK |
| PE11 | I2S_LRCLK |
| PE12 | I2S_BCLK |
| PE14 | I2S_SDOUT |
| PE0 | AUDIO_SDA |
| PE1 | AUDIO_SCL |
| PD3 | AUDIO_INT |
| PD4 | AUDIO_RESET |

### FM Radio (Si4735 — SAI2 + I2C)
| Pin | Signal |
|-----|--------|
| PB10 | RADIO_I2S_BCLK |
| PB12 | RADIO_I2S_LRCLK |
| PB15 | RADIO_I2S_DATA |
| PD12 | RADIO_SCL |
| PD13 | RADIO_SDA |
| PD14 | RADIO_RST |
| PD11 | RADIO_IRQ |

### Microphone (ICS-43434 — I2S)
| Pin | Signal |
|-----|--------|
| PE2 | MIC_DATA |
| PE3 | MIC_LRCLK |
| PE4 | MIC_BCLK |
| PA15 | MIC_CHANNEL |

### Sensors (SPI1 + I2C1)
| Pin | Signal |
|-----|--------|
| PE13 | SENSORS_SPI_SCK |
| PE5 | SENSORS_SPI_MISO |
| PE6 | SENSORS_SPI_MOSI |
| PE8 | IMU_CS |
| PE10 | GAS_CS |
| PE9 | IMU_INT1 |
| PC2 | IMU_INT2 |
| PC3 | LIGHT_INT |
| PD0 | SENSORS_I2C_SCL |
| PD1 | SENSORS_I2C_SDA |

### SD Card (SDMMC1)
| Pin | Signal |
|-----|--------|
| PC12 | SDMMC_CLK |
| PC13 | SDMMC_CMD |
| PC8 | SDMMC_D0 |
| PC9 | SDMMC_D1 |
| PC10 | SDMMC_D2 |
| PC11 | SDMMC_D3 |
| PC7 | SDMMC_DET |

### LED Strip (APA102)
| Pin | Signal |
|-----|--------|
| PB11 | LED_SCK |
| PB13 | LED_MOSI |

### Power Management (PMM)
| Pin | Signal |
|-----|--------|
| PD8 | PMM_TX (USART3) |
| PD9 | PMM_RX (USART3) |
| PD7 | PMM_FORCE |
| PD10 | PMM_SHDN |
| PD15 | PMM_INIT |
| PB8 | PWR_I2C_SCL |
| PB9 | PWR_I2C_SDA |

### USB
| Pin | Signal |
|-----|--------|
| PA11 | USB_DM |
| PA12 | USB_DP |

### Clock
| Pin | Signal |
|-----|--------|
| PH0 | HSE_IN (8MHz crystal) |
| PH1 | HSE_OUT |
| PC14 | LSE_IN (32.768kHz crystal) |
| PC15 | LSE_OUT |

### Debug (SWD)
| Pin | Signal |
|-----|--------|
| PA13 | SWDIO |
| PA14 | SWCLK |

## Clock Tree

- **SYSCLK**: 480MHz (8MHz HSE / 4 * 240)
- **AHB**: 240MHz (/2)
- **APB1/APB2**: 120MHz (/2)
- **SDMMC**: 48MHz (from PLL1Q)
- **PLL2**: Audio clock source (SAI1, SAI2)
- **USB**: 48MHz (from PLL1Q)
