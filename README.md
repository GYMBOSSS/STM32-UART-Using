# STM32-UART-Registers

UART communication for STM32F103RB (Nucleo-F103RB), written with direct register access — no HAL, no CubeMX code. Built with GCC + CMake, flashed with OpenOCD.

This is my second learning project: I wanted to understand how the Clock Tree (HSE, PLL, Bus Prescalers) works, how to configure GPIO pins for Alternate Functions (AF), and how to interact with the USART peripheral (calculating BRR, writing to CR1, and sending data via DR) at the lowest level.

## Hardware

- Board: NUCLEO-F103RB
- Interface: USART2 via the on-board ST-LINK Virtual COM Port
- Pins: PA2 (USART2_TX), PA3 (USART2_RX)
- System Clock: 72 MHz (HSE 8 MHz -> PLL x9)
- APB1 Bus: 36 MHz (USART2 is connected here)

## Requirements

- [ARM GNU Toolchain](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads) (`arm-none-eabi-gcc` in PATH)
- CMake ≥ 3.22
- GNU Make (e.g. via MinGW)
- [OpenOCD](https://openocd.org/) with ST-LINK support
- ST-LINK USB driver (Windows)
- **Serial Terminal** (Tera Term, PuTTY, or VS Code Serial Monitor)

## Build

```bash
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
