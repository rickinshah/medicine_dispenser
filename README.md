# Medicine Dispenser

## Overview

This project implements an embedded medicine dispensing system using an STM32 microcontroller. The system dispenses medicine only when valid RFID tag is detected, enabling controlled and authorized access

It is built using STM32 HAL drivers and configured via STM32CubeMX.

## Features

- Motor-controlled dispensing mechanism
- User feedback via buzzer/LED
- Embedded control using STM32 HAL
- Modular design for adding sensors or automation later

## Internal Layout

<img width="250" height="250" alt="internals" src="https://github.com/user-attachments/assets/83d027c9-231a-4afb-88fc-beb3661b06eb" />

## Demo

https://github.com/user-attachments/assets/5336d19a-5b55-49a7-8e7b-d9053d2fd31e

## Project Structure

```
├── Core                        
│   ├── Inc                     # Header files
│   └── Src                     # Application source code
├── Drivers
│   ├── CMSIS                   # ARM core + device headers
│   └── STM32F4xx_HAL_Driver    # HAL drivers
├── MDK-ARM                     # Keil Project files
└── Medicine_Disp.ioc           # STM32CubeMX configuration
```

## Hardware Requirements

- STM32F4 series microcontroller
- Motor driver
- Gear motor
- PN532 NFC Module
- RFID tag/card
- 20x4 LCD Display
- Power Supply

## Software Requirements

- STM32CubeMX
- Keil MDK
- STM32 HAL drivers


