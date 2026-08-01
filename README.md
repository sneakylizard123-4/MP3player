# MP3 Player

A portable MP3 player that plays music files off an SD card, picks up FM radio, and records audio through a MEMS microphone — all packed into a 50×70mm board with an OLED screen, an APA102 LED spectrum bar, and a battery that charges over USB-C.

The whole thing runs on an STM32H743 (480MHz Cortex-M7) with a high-fidelity CS43131 DAC driving the headphones. It's designed in KiCad 10 and programmed with PlatformIO.

## Why I made this

I wanted a tiny, standalone music player I could carry around and actually use — not just a phone-shaped black box. Modern phones have great audio but terrible UI, no physical buttons, no visible feedback while playing, and you can't hack on them. I wanted something with a screen, real buttons, a visual spectrum bar, and firmware I control end to end.

The STM32H743 gives me enough RAM and horsepower to buffer and decode audio smoothly while driving the display, LEDs, radio, and sensors all at once.

## Gallery

![3D render](images/penguin/pcb-render.png)

![PCB front](images/pcb/pcb-front.png) | ![PCB back](images/pcb/pcb-back.png)

![Root schematic](images/schematics/01-root.png)
![Power schematic](images/schematics/02-power.png)

## Features

- CS43131 headphone DAC with integrated amp
- Si4735 FM radio receiver (I2S audio out)
- ICS-43434 MEMS microphone for recording
- Micro SD card storage (4-bit SDMMC)
- OLED display with 3×3 button matrix + direct buttons
- APA102 RGB LED spectrum bar
- LSM6DSL IMU, BME680 environmental sensor, ISL29035 light sensor
- DS3231M RTC for timekeeping
- USB 2.0 (USB-C) with BQ24090DGQ battery charging
- TPS613222ADBV boost converter (battery → 5V)
- Ultra-low-noise TPS7A4701 LDO for the audio rail, TLV70233 for digital

## What the sensors are for

An MP3 player carries a set of sensors not because they're trendy, but because they make the device genuinely nicer to live with:

- **LSM6DSL (6-axis IMU)** — motion sensing for *gesture controls*: shake to skip a track, tilt to change volume, or flip the player face-down to mute. It also provides orientation so the UI knows which way is up, and the accelerometer data is used to make the spectrum bar react to how the device is being moved.

- **BME680 (temperature, humidity, pressure, VOC)** — keeps an eye on the environment the player is in. If the player gets too hot (pocket in summer, left in a car), it can warn you and throttle power-hungry features. It also reads barometric pressure so the player can act as a mini weather station on the OLED, and it's useful for dew-point / humidity-aware behavior. the VOC feature can warn the user about any potential hazards that are present in the air.

- **ISL29035 (ambient light sensor)** — *auto-brightness for the OLED display*. In bright sunlight it cranks the display up so you can read it; at night it dims so the screen doesn't blind you or drain the battery.

None of these affect playback directly, but together they turn the player into something that responds to its environment instead of just sitting there.

## Hardware design

### Power architecture

USB-C → USBLC6 ESD protection → BQ24090DGQ charger → Li-ion battery
Battery → TPS613222ADBV boost → 5V rail (LEDs)
Battery → TLV70233 → 3.3V digital rail
Battery → TPS7A4701 → 3.3V ultra-low-noise audio rail

The NTC thermistor monitors battery temperature during charging, and a charge LED shows charging state.

### Schematic sheets

| Sheet | File | Description | Key Components |
|-------|------|-------------|----------------|
| Root | MP3player.kicad_sch | Hierarchical sheet definitions | Mounting holes, power symbols |
| STM32 Core | stm32core.kicad_sch | MCU + onboard sensors | STM32H743, LSM6DSL, BME680, ISL29035, DS3231M |
| Power | power.kicad_sch | USB-C input, charger, regulators | BQ24090DGQ, TPS613222ADBV, TPS7A4701, TLV70233, USBLC6 |
| Audio | audio.kicad_sch | Headphone output | CS43131-CNZR |
| Storage | storage.kicad_sch | SD card slot | Micro SD (DM3) |
| OLED UI | oledui.kicad_sch | Display + button matrix | OLED, 13 buttons |
| Radio | radio.kicad_sch | FM/AM receiver | Si4735-D60-GU |
| Microphone | microphone.kicad_sch | I2S MEMS mic | ICS-43434 |
| LED Spectrum | sparkles.kicad_sch | APA102 LED strip | APA102-2020 |

### PCB

6-layer, 1.6mm, ~50×70mm. Top/bottom signal layers, two internal power planes (In1.Cu, In2.Cu), and two inner signal layers (In3.Cu, In4.Cu). Analog audio is kept separated from the digital/power sections; the CS43131 sits close to the headphone jack to keep analog traces short.

## Assembly

1. **Preheat** — 220°C reflow with the board on a stencil for both sides (top & bottom, separate stencils).
2. **Solder passives first** — all 0603 resistors/capacitors, then the ferrite bead and crystals.
3. **ICs** — place the STM32H743 (LQFP-100), CS43131, Si4735, and the power ICs (BQ24090, TPS613222ADBV, TPS7A4701, TLV70233). The LQFP-100 and VQFN-20 benefit from flux and a fine tip.
4. **Connectors** — USB-C, 3.5mm jack, micro SD slot, SMA antennas, pin headers (OLED, SWD), battery JST.
5. **Buttons & switches** — 17 tactile switches.
6. **Sensors & mic** — LSM6DSL, BME680, ISL29035, ICS-43434 (careful with reflow time on MEMS).
7. **LEDs** — 4× APA102-2020.
8. **Inspect** — check for bridges on the fine-pitch parts (TQFN-40 0.4mm pitch is the trickiest), confirm with flying-probe electrical test.
9. **Flash** — connect SWD and flash the firmware.
10. **Power up** — verify 3.3V rails, then check audio out on the headphone jack.

## Firmware

Built with [PlatformIO](https://platformio.org/) using the STM32 HAL (stm32cube framework).

### Prerequisites

- [PlatformIO CLI](https://docs.platformio.org/en/latest/core/installation.html) or VS Code extension
- ST-Link / CMSIS-DAP / J-Link for flashing

### Build & Flash

```bash
cd firmware
pio run              # build
pio run -t upload    # flash via SWD
```

The firmware skeleton initializes the 480MHz clock (PLL1 from the 8MHz HSE), PLL2 for audio clock generation, all four I2C buses, both SPI buses, the UART, and every GPIO (buttons, chip selects, control outputs, SD card detect).

## Bill of Materials

| Ref | Part | Qty | Package | Notes |
|-----|------|-----|---------|-------|
| U4 | STM32H743VIT6 | 1 | LQFP-100 | Main MCU |
| U9 | CS43131-CNZR | 1 | TQFN-40 | Audio DAC |
| U10 | Si4735-D60-GU | 1 | SSOP-24 | FM radio |
| MK1 | ICS-43434 | 1 | 3.5×2.65mm | MEMS mic |
| U6 | LSM6DSL | 1 | LGA-14 | IMU |
| U8 | BME680 | 1 | LGA-8 | Gas sensor |
| U5 | ISL29035 | 1 | DFN-6 | Light sensor |
| U7 | DS3231M | 1 | SOIC-16W | RTC |
| U11 | BQ24090DGQ | 1 | HVSSOP-10 | Battery charger |
| U12 | TPS613222ADBV | 1 | SOT-23-5 | Boost converter |
| U3 | TPS7A4701xRGW | 1 | VQFN-20 | Audio LDO |
| U2 | TLV70233 | 1 | SOT-23-5 | 3.3V regulator |
| U1 | USBLC6-2SC6 | 1 | SOT-23-6 | ESD protection |
| D4–D7 | APA102-2020 | 4 | 2.0×2.0mm | RGB LEDs |
| D1–D3 | ACDSV6-4448TI-G | 3 | SOT-363 | ESD arrays |
| D10 | BAT54W | 1 | SOT-323 | Schottky |
| D8–D9 | LED 0603 | 2 | 0603 | Status LEDs |
| J1 | USB-C (HRO TYPE-C-31-M-12) | 1 | — | USB input |
| J4 | 3.5mm jack PJ31060-I | 1 | — | Headphone |
| J5 | Micro SD (Hirose DM3D-SF) | 1 | — | SD card |
| J6–J7 | SMA (Amphenol 901-143) | 2 | — | Antenna |
| J8 | Pin header 1×08 | 1 | 2.54mm | OLED |
| J3 | Pin header 1×05 | 1 | 2.54mm | SWD debug |
| J2 | JST PH B2B-PH-K | 1 | 2.00mm | Battery |
| L1 | 450µH | 1 | 0805 | Radio antenna coil |
| L2 | 2.2µH | 1 | 0805 | Boost inductor |
| Y1 | 8MHz TXC AA08000002 | 1 | 5032 | HSE crystal |
| SW1–SW17 | Tactile (Panasonic EVQPUM/CK KMR2) | 17 | — | Buttons |
| TH1 | NTC 10k | 1 | 0603 | Battery temp |
| FB1 | 600Ω @ 100MHz | 1 | 0603 | Ferrite bead |
| R1–R26 | Resistors (various) | 26 | 0603 | — |
| C1–C62 | Capacitors (various) | 62 | 0603 | — |
| H1–H4 | Mounting hole M2 | 4 | — | Enclosure |

Full BOM with LCSC part numbers and prices: [`BOM.csv`](BOM.csv)

## Fabrication

**PCB** — JLCPCB (MP3player_Y33)

| Spec | Value |
|------|-------|
| Build Time | 5–6 days |
| Base Material | FR-4 TG155 |
| Layers | 6 |
| Dimensions | 50 × 70 mm |
| Qty | 5 |
| Thickness | 1.6 mm |
| PCB Color | Purple |
| Silkscreen | White (Ink-jet) |
| Via Covering | Epoxy filled & capped |
| Surface Finish | ENIG (1µ" gold) |
| Outer Copper | 1 oz |
| Inner Copper | 0.5 oz |
| Min via hole/diameter | 0.3 mm / 0.4–0.45 mm |
| Electrical Test | Flying probe, fully tested |
| 4-Wire Kelvin Test | Yes |
| Appearance | IPC Class 2 |
| Board Outline Tolerance | ±0.2 mm |
| Gross Weight | 0.17 kg |

**Stencil** — JLCPCB

| Spec | Value |
|------|-------|
| Build Time | 2–3 days |
| Dimension | 380 × 280 mm |
| Stencil Side | Top & Bottom (separate) |
| Stencil Qty | 2 |
| Thickness | Auto-selected |
| Polishing | Electropolishing |
| Nano-Coating | Yes |
| Engrave Text | Yes |
| Gross Weight | 0.17 kg |

## Known Issues

- KiCad crashes intermittently during layout, likely a Wayland rendering bug — save often!
- The 3×3 button matrix shares pins with direct buttons; debounce will be handled in firmware.
- Firmware drivers are still in progress — the skeleton initializes peripherals but application logic (playback, UI, recording) is not written yet.

## Credits

- All KiCad schematics/PCB and firmware written from scratch for this project
- STM32 HAL from STMicroelectronics, built via PlatformIO's ststm32 platform
- Symbols/footprints from the standard KiCad libraries

## Status

- [x] Schematic (8 sheets, complete)
- [x] PCB layout (6-layer, 1.6mm, ~50×70mm)
- [x] Firmware skeleton (clock config, GPIO, I2C, SPI, UART init)
- [ ] Firmware drivers (audio, SD, OLED, LEDs, sensors, radio, buttons)
- [ ] Firmware application (playback, UI, recording)
