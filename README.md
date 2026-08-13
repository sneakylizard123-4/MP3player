# MP3 Player

A pocket-sized music player I designed and built from scratch. It plays audio off a microSD card, tunes into FM radio, and can record through a MEMS microphone — all on one 50×70mm PCB with an OLED screen, RGB LEDs, and seventeen physical buttons.

It's built around an STM32H743 running at 480MHz, and the audio comes out through a CS43131 DAC. The whole thing was designed in KiCad 10 and the firmware is C++ written with PlatformIO on ST's HAL.

## Why I built this

I wanted a music player that was actually *mine*. Phones sound great and all, but you can't change anything about them — the hardware is sealed up, there's no satisfying button to press, and the software only does what the manufacturer lets it do. I wanted real buttons, a screen, lights that react to the music, and firmware I could rewrite from scratch whenever I felt like it.

It also turned out to be the perfect excuse to practice the stuff I actually enjoy: PCB design, embedded firmware, and getting hardware interfaces to talk to each other.

I went with the STM32H743 because it has the RAM to buffer audio properly, enough peripherals for everything I wanted to add, and 480MHz of headroom so I never have to worry about it being slow.

## Pictures

![3D render](images/penguin/pcb-render.png)

![PCB front](images/pcb/pcb-front.png) | ![PCB back](images/pcb/pcb-back.png)

![Root schematic](images/schematics/01-root.png)

![Power schematic](images/schematics/02-power.png)

## Features

**Audio**

* CS43131 high-res headphone DAC with a built-in amp
* Dedicated low-noise power rail just for the audio stuff
* 3.5mm headphone output
* Si4735 AM/FM radio receiver
* ICS-43434 I2S microphone for recording

**Storage & controls**

* microSD card over 4-bit SDMMC
* OLED display
* 17 buttons: a 3×3 matrix for navigation plus direct controls
* APA102 RGB LEDs

**Sensors**

* LSM6DSL 6-axis IMU
* BME680 temperature / humidity / pressure / gas sensor
* ISL29035 ambient light sensor
* DS3231M real-time clock

**Power**

* USB-C charging via a BQ24090DGQ Li-ion charger
* TPS613222ADBV boost converter for the 5V rail
* Separate digital and low-noise analog rails

## What the sensors actually do

None of these are needed to play music, but they're what turn this from "a box that plays audio" into something that notices the world around it:

* **LSM6DSL IMU** — motion and orientation. You could shake it to skip a track, flip it to mute, rotate the UI, or drive the LEDs off the beat.
* **BME680** — temperature, humidity, pressure, and VOC levels. Nice for showing environmental data on the OLED, and it can track air quality over time.
* **ISL29035** — how bright it is outside. Auto-adjusts the OLED brightness so it stays readable in daylight and saves battery at night.
* **DS3231M RTC** — keeps accurate time even when the player is off, for a clock on the display, recording timestamps, and maybe alarms later.

# Hardware

## Power

A single Li-ion cell powers everything, with separate rails for the digital side, the LEDs, and the analog audio. The charger (BQ24090DGQ) handles USB input, battery management, and has an NTC thermistor on the pack for temperature monitoring plus charge status LEDs. The boost converter (TPS613222ADBV) steps the battery up to 5V for the LEDs and peripherals, a TLV70233 LDO makes the 3.3V digital rail, and a TPS7A4701 low-noise LDO feeds the audio.

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
                  +-- TPS7A4701 low-noise LDO
                          |
                          +-- audio rail
```

The digital and audio rails are deliberately kept apart so switching noise from the boost converter doesn't end up in your headphones. The DAC gets its own clean regulator for the same reason.

## Schematic

The design is split across hierarchical KiCad sheets:

| Sheet        | File                  | Description                                  | Main Components                                        |
| ------------ | --------------------- | -------------------------------------------- | ------------------------------------------------------ |
| Root         | MP3player.kicad_sch   | Connections and system overview              | Power symbols, connectors, mounting holes              |
| STM32 Core   | stm32core.kicad_sch   | MCU and sensors                              | STM32H743, LSM6DSL, BME680, ISL29035, DS3231M          |
| Power        | power.kicad_sch       | Charging and regulation                      | BQ24090DGQ, TPS613222ADBV, TPS7A4701, TLV70233, USBLC6 |
| Audio        | audio.kicad_sch       | Headphone audio path                         | CS43131-CNZR                                           |
| Storage      | storage.kicad_sch     | Removable storage                            | MicroSD socket                                         |
| OLED UI      | oledui.kicad_sch      | Display and buttons                          | OLED, button matrix, GPIO controls                     |
| Radio        | radio.kicad_sch       | AM/FM receiver                               | Si4735-D60-GU                                          |
| Microphone   | microphone.kicad_sch  | Audio input                                  | ICS-43434 MEMS microphone                              |
| LED Spectrum | ledspectrum.kicad_sch | Visual audio display                         | APA102-2020 LEDs                                       |

## PCB

Six layers, 1.6mm thick, about 50×70mm. Internal power planes keep the distribution clean, and I paid attention to keeping the analog audio away from the digital and switching sections. The DAC sits right next to the headphone jack so the analog traces stay short, and the high-current paths around the charger and boost converter are kept tight with proper return paths.

# Assembly

Designed for standard SMT assembly. Here's how it goes together:

1. Apply solder paste with the stencil.
2. Place the small stuff — 0603 resistors and caps, ferrite beads, crystals.
3. Place the big ICs: STM32H743 (LQFP-100), CS43131, Si4735, power management, APA102 LEDs, sensors.
4. Reflow.
5. Check the fine-pitch parts for solder bridges.
6. Install connectors: USB-C, headphone jack, microSD socket, SMA antenna connectors, battery connector, debug headers.
7. Install the user-facing stuff: 17 tactile switches and the OLED connector.
8. Electrical checks — continuity, shorts, and verifying the power rails.
9. Flash the initial firmware over SWD.

## Firmware

The firmware is in `firmware/`, built with PlatformIO on the STM32 HAL. Right now it sets up the hardware: 480MHz clock from the 8MHz crystal via PLL, all the GPIO, the I2C/SPI/UART buses, the SD card interface, and the control pins. The fun part — actual playback, the UI, radio control, recording, and the LED effects — is still being written.

### Build and flash

You'll need PlatformIO (CLI or the VS Code extension) and an ST-Link, CMSIS-DAP, or J-Link debugger.

```bash
cd firmware
pio run
```

To flash over SWD:

```bash
pio run -t upload
```

# Bill of Materials

| Ref      | Part                 | Qty | Package   | Description                         |
| -------- | -------------------- | --- | --------- | ----------------------------------- |
| U4       | STM32H743VIT6        | 1   | LQFP-100  | Main microcontroller                |
| U9       | CS43131-CNZR         | 1   | QFN-40    | Audio DAC                           |
| U10      | Si4735-D60-GU        | 1   | SSOP-24   | FM radio receiver                   |
| MK1      | ICS-43434            | 1   | LGA       | I2S MEMS microphone                 |
| U6       | LSM6DSL              | 1   | LGA-14    | 6-axis IMU                          |
| U8       | BME680               | 1   | LGA-8     | Environmental sensor                |
| U5       | ISL29035             | 1   | DFN-6     | Ambient light sensor                |
| U7       | DS3231M              | 1   | SOIC-16W  | Real-time clock                     |
| U11      | BQ24090DGQ           | 1   | HVSSOP-10 | Li-ion charger                      |
| U12      | TPS613222ADBV        | 1   | SOT-23-5  | Boost converter                     |
| U3       | TPS7A4701xRGW        | 1   | VQFN-20   | Low-noise audio regulator           |
| U2       | TLV70233             | 1   | SOT-23-5  | 3.3V digital regulator              |
| U1       | USBLC6-2SC6          | 1   | SOT-23-6  | USB ESD protection                  |
| D4-D7    | APA102-2020          | 4   | 2×2mm     | RGB LEDs                            |
| D1-D3    | ACDSV6-4448TI-G      | 3   | SOT-363   | Button ESD protection               |
| D10      | BAT54W               | 1   | SOT-323   | Schottky diode                      |
| D8-D9    | 0603 LEDs            | 2   | 0603      | Status LEDs                         |
| J1       | HRO TYPE-C-31-M-12   | 1   | USB-C     | Charging and USB                    |
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

`BOM.csv` has the full BOM with supplier part numbers, links, and pricing.

# Fabrication

Boards and stencils are made through JLCPCB.

PCB specs:

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

Stencil specs:

| Specification         | Value                     |
| --------------------- | ------------------------- |
| Quantity              | 2                         |
| Sides                 | Top and bottom            |
| Manufacturing process | Laser-cut stainless steel |
| Finish                | Electropolished           |
| Coating               | Nano-coating              |

# Known Issues

* Audio performance hasn't been measured with the final firmware yet.
* Battery runtime still needs to be measured.
* RF performance and antenna tuning need real testing.
* Thermal behavior under load hasn't been validated.
* The enclosure design isn't done.

The firmware currently only does hardware init. Remaining features:

* [ ] Audio playback pipeline
* [ ] SD card filesystem support
* [ ] Audio decoding
* [ ] OLED user interface
* [ ] Button handling and debounce
* [ ] Radio control
* [ ] Recording
* [ ] Sensor drivers
* [ ] LED spectrum visualization
* [ ] Battery monitoring

# Credits

* Board, schematics, and firmware designed and written from scratch by me.
* STM32 HAL by STMicroelectronics.
* PlatformIO for firmware builds.
* KiCad libraries for standard symbols and footprints.
