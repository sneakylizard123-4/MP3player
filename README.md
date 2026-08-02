# MP3 Player

A portable music player built around an STM32H743 that plays audio from a microSD card, receives FM radio, and supports audio recording through an I2S MEMS microphone. The entire design fits on a 50×70mm PCB with an OLED display, APA102 RGB LEDs, physical buttons, and USB-C battery charging.

The system uses a high-quality CS43131 DAC for headphone output and is designed completely from scratch in KiCad 10. Firmware is developed using PlatformIO with the STM32 HAL framework.

## Why I made this

I wanted to build a standalone music player that I could actually use, rather than another device where the hardware and software are completely closed.

Modern phones usually have excellent audio hardware, but they are difficult to customize, provide limited physical interaction, and hide their insides. I wanted a device with real buttons, a display, visible feedback while playing music, and firmware that I could modify from the hardware level up.

This project combines the parts of electronics that I enjoy: PCB design, embedded firmware, and hardware interfaces. The goal is to create a platform where I can experiment with different features and learn how the entire system works without potentially breaking my phone.

I hchose the STM32H743 because its processing capability and memory are suitable for handling audio buffering, display updates, sensor communication, and multiple peripherals simultaneously.

## Gallery

![3D render](images/penguin/pcb-render.png)

![PCB front](images/pcb/pcb-front.png) | ![PCB back](images/pcb/pcb-back.png)

![Root schematic](images/schematics/01-root.png)

![Power schematic](images/schematics/02-power.png)

## Features

### Audio

* CS43131 high-resolution headphone DAC
* Dedicated low-noise audio power rail
* 3.5mm headphone output
* Si4735 AM/FM radio receiver
* ICS-43434 I2S MEMS microphone for recording

### Storage and interface

* MicroSD card storage using 4-bit SDMMC
* OLED display
* 17 physical buttons:

  * 3×3 button matrix
  * Direct control buttons
* APA102 RGB LEDs

### Sensors

* LSM6DSL 6-axis IMU
* BME680 environmental sensor
* ISL29035 ambient light sensor
* DS3231M real-time clock

### Power

* USB-C charging
* BQ24090DGQ Li-ion charger
* TPS613222ADBV boost converter
* Separate digital and low-noise audio power rails

### Main controller

* STM32H743VIT6
* ARM Cortex-M7 @ 480MHz
* Designed in KiCad 10
* Firmware developed using PlatformIO

## What the sensors are used for

The sensors are not required for basic audio playback, but they allow the device to interact with its environment and provide additional functionality.

### LSM6DSL IMU

The LSM6DSL provides motion and orientation data.

Possible uses include:

* gesture-based controls
* detecting device orientation for UI rotation
* motion-based visual effects for the spectrum LEDs

For example, the device could detect a shake gesture to skip tracks or a flip gesture to mute audio.

### BME680 environmental sensor

The BME680 measures:

* temperature
* humidity
* pressure
* VOC levels

The data can be displayed on the OLED as additional environmental information. The VOC measurement can also be used for tracking air-quality trends.

### ISL29035 ambient light sensor

The ISL29035 measures surrounding light levels and allows automatic display brightness adjustment.

In bright environments the display can increase brightness, while in dark environments it can reduce brightness to improve battery life and comfort.

### DS3231M RTC

The DS3231M provides accurate timekeeping while the device is powered off. It can be used for:

* clock display
* timestamps for recordings
* future alarm or scheduling features

The sensors allow the player to become more than just an audio playback device by providing environmental awareness and additional user interaction possibilities.

# Hardware Design

## Power Architecture

The power system is designed around a single-cell Li-ion battery with separate power paths for digital electronics, LEDs, and sensitive analog audio circuitry.

Power flow:

```
USB-C
  |
  +-- USBLC6 ESD protection
  |
  +-- BQ24090DGQ Li-ion charger
          |
          +-- Li-ion battery
                  |
                  +-- TPS613222ADBV boost converter
                  |       |
                  |       +-- 5V rail (LEDs and peripherals)
                  |
                  +-- TLV70233 LDO
                  |       |
                  |       +-- 3.3V digital rail
                  |
                  +-- Audio supply regulation
                          |
                          +-- TPS7A4701 low-noise LDO
                                  |
                                  +-- audio rail
```

The BQ24090 handles USB charging and battery management. A battery NTC thermistor is included for temperature monitoring during charging, and dedicated LEDs indicate charging status.

The digital and analog power sections are separated to reduce noise coupling into the audio circuitry. The CS43131 DAC is supplied from a dedicated low-noise regulator to improve audio performance.

## Schematic Organization

The design is split into hierarchical KiCad schematic sheets:

| Sheet        | File                 | Description                                  | Main Components                                        |
| ------------ | -------------------- | -------------------------------------------- | ------------------------------------------------------ |
| Root         | MP3player.kicad_sch  | Hierarchical connections and system overview | Power symbols, connectors, mounting holes              |
| STM32 Core   | stm32core.kicad_sch  | Main MCU and sensor interfaces               | STM32H743, LSM6DSL, BME680, ISL29035, DS3231M          |
| Power        | power.kicad_sch      | Charging and regulation                      | BQ24090DGQ, TPS613222ADBV, TPS7A4701, TLV70233, USBLC6 |
| Audio        | audio.kicad_sch      | Headphone audio path                         | CS43131-CNZR                                           |
| Storage      | storage.kicad_sch    | Removable storage                            | MicroSD socket                                         |
| OLED UI      | oledui.kicad_sch     | Display and buttons                          | OLED, button matrix, GPIO controls                     |
| Radio        | radio.kicad_sch      | AM/FM receiver                               | Si4735-D60-GU                                          |
| Microphone   | microphone.kicad_sch | Audio input                                  | ICS-43434 MEMS microphone                              |
| LED Spectrum | sparkles.kicad_sch   | Visual audio display                         | APA102-2020 LEDs                                       |

## PCB Design

The PCB is a six-layer, 1.6mm board measuring approximately 50×70mm.

Stackup:

* Top layer: component placement and high-speed signals
* Inner layers: power distribution and signal routing
* Bottom layer: component placement and routing

The board uses dedicated internal planes to improve power distribution and reduce interference.

Analog audio traces are kept separate from noisy digital signals and switching regulators. The CS43131 DAC is placed close to the headphone connector to minimize the length of sensitive analog traces.

High-current paths around the battery charger and boost converter are kept short, with attention paid to switching loop area and return paths.

# Assembly

The PCB is designed for standard SMT assembly, with a mixture of hand-assemblable and fine-pitch components.

## Assembly Process

1. Apply solder paste using the SMT stencil.
2. Place passive/small components:

   * 0603 resistors and capacitors
   * ferrite beads
   * crystals
3. Place bigger components:

   * STM32H743 (LQFP-100)
   * CS43131 DAC
   * Si4735 radio receiver
   * power management ICs
   * APA102 LEDs
   * Sensors
4. Reflow time.
5. Inspect fine-pitch components for solder bridges.
6. Install connectors:

   * USB-C
   * 3.5mm headphone jack
   * microSD socket
   * SMA connectors
   * battery connector
   * debug headers
7. Install user-interface components:

   * 17 tactile switches
   * OLED connector

8. Perform electrical checks:

   * continuity testing
   * short-circuit checks
   * power rail verification
9. Flash initial firmware through SWD.

## Firmware Development

Firmware is developed using PlatformIO with the STM32 HAL framework.

The initial firmware framework handles:

* STM32H743 clock configuration
* PLL setup
* GPIO initialization
* I2C buses
* SPI buses
* UART communication
* SD card interface
* peripheral control pins

The firmware project provides the foundation for future application features:

* audio playback
* radio control
* OLED interface
* button handling
* recording
* sensor integration
* LED spectrum visualization

## Build and Flash

Requirements:

* PlatformIO CLI or VS Code extension
* ST-Link, CMSIS-DAP, or J-Link debugger

Build:

```bash
cd firmware
pio run
```

Flash:

```bash
pio run -t upload
```

The device is programmed through the SWD debug interface.

# Bill of Materials

| Ref      | Part                 | Qty | Package   | Description                         |
| -------- | -------------------- | --- | --------- | ----------------------------------- |
| U4       | STM32H743VIT6        | 1   | LQFP-100  | Main microcontroller                |
| U9       | CS43131-CNZR         | 1   | QFN-40    | High-quality audio DAC              |
| U10      | Si4735-D60-GU        | 1   | SSOP-24   | FM radio receiver                   |
| MK1      | ICS-43434            | 1   | LGA       | I2S MEMS microphone                 |
| U6       | LSM6DSL              | 1   | LGA-14    | 6-axis IMU                          |
| U8       | BME680               | 1   | LGA-8     | Environmental sensor                |
| U5       | ISL29035             | 1   | DFN-6     | Ambient light sensor                |
| U7       | DS3231M              | 1   | SOIC-16W  | Real-time clock                     |
| U11      | BQ24090DGQ           | 1   | HVSSOP-10 | Li-ion charger and power management |
| U12      | TPS613222ADBV        | 1   | SOT-23-5  | Boost converter                     |
| U3       | TPS7A4701xRGW        | 1   | VQFN-20   | Low-noise audio regulator           |
| U2       | TLV70233             | 1   | SOT-23-5  | 3.3V digital regulator              |
| U1       | USBLC6-2SC6          | 1   | SOT-23-6  | USB ESD protection                  |
| D4-D7    | APA102-2020          | 4   | 2×2mm     | RGB spectrum LEDs                   |
| D1-D3    | ACDSV6-4448TI-G      | 3   | SOT-363   | Additional ESD protection           |
| D10      | BAT54W               | 1   | SOT-323   | Schottky diode                      |
| D8-D9    | 0603 LEDs            | 2   | 0603      | Status indicators                   |
| J1       | HRO TYPE-C-31-M-12   | 1   | USB-C     | Charging and USB interface          |
| J4       | PJ31060-I            | 1   | —         | 3.5mm headphone jack                |
| J5       | Hirose DM3D-SF       | 1   | —         | MicroSD socket                      |
| J6-J7    | Amphenol 901-143     | 2   | SMA       | Antenna connectors                  |
| J2       | JST PH B2B-PH-K      | 1   | 2.0mm     | Battery connector                   |
| J3       | SWD header           | 1   | 2.54mm    | Debug connector                     |
| J8       | OLED header          | 1   | 2.54mm    | Display connector                   |
| L1       | 450µH                | 1   | 0805      | Radio antenna matching              |
| L2       | 2.2µH                | 1   | 0805      | Boost converter inductor            |
| Y1       | 8MHz TXC AA08000002  | 1   | 5032      | MCU crystal                         |
| SW1-SW17 | Tactile switches     | 17  | —         | User controls                       |
| TH1      | 10k NTC              | 1   | 0603      | Battery temperature monitoring      |
| FB1      | 600Ω @100MHz ferrite | 1   | 0603      | Noise filtering                     |

The complete BOM, including supplier part numbers and pricing, is available in:

```
BOM.csv
```

# Fabrication

## PCB

Manufactured through JLCPCB.

| Specification  | Value         |
| -------------- | ------------- |
| Layers         | 6             |
| Dimensions     | 50×70mm       |
| Thickness      | 1.6mm         |
| Material       | FR-4 TG155    |
| Surface finish | ENIG          |
| Outer copper   | 1oz           |
| Inner copper   | 0.5oz         |
| PCB color      | Purple        |
| Silkscreen     | White         |

## Stencil

Manufactured through JLCPCB.

| Specification         | Value                     |
| --------------------- | ------------------------- |
| Quantity              | 2                         |
| Sides                 | Top and bottom            |
| Manufacturing process | Laser-cut stainless steel |
| Finish                | Electropolished           |
| Coating               | Nano-coating              |

# Known Issues and Future Work

The current hardware design is complete but there are some issues.

## Hardware

* Audio performance has not yet been measured with final firmware.
* Battery runtime measurements still need to be performed.
* RF performance and antenna tuning require testing.
* Thermal behavior under high load needs validation.
* The final enclosure design is not done.

## Firmware

Current firmware provides the hardware initialization layer, but application features are still being developed.

Remaining work:

* [ ] Audio playback pipeline
* [ ] SD card filesystem support
* [ ] Audio decoding
* [ ] OLED user interface
* [ ] Button handling and debounce
* [ ] Radio control interface
* [ ] Recording system
* [ ] Sensor drivers
* [ ] APA102 spectrum visualization
* [ ] Battery monitoring

# Credits

* PCB design, schematics, and firmware written from scratch.
* STM32 HAL provided by STMicroelectronics.
* PlatformIO used for firmware development.
* KiCad libraries used for standard symbols and footprints.

# Project Status

## Hardware

* [x] System architecture
* [x] Schematic design
* [x] PCB layout
* [x] Manufacturing files
* [x] BOM generation

## Firmware

* [x] PlatformIO project setup
* [x] Clock configuration
* [x] GPIO initialization
* [x] Peripheral initialization
* [ ] Audio playback
* [ ] User interface
* [ ] Recording
* [ ] Sensor integration
* [ ] Radio features

The project's hardware is done and all that remains is firmware development.
