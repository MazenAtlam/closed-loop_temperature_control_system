# Auto-Cooler: Closed-Loop Temperature Control System

This repository contains the bare-metal C implementation of the Auto-Cooler, a closed-loop temperature control system built for the STM32F4 microcontroller.

The system reads ambient temperature via an LM35 analog sensor, dynamically adjusts the speed of a 5V/12V DC fan using PWM, and provides real-time system feedback on a parallel LCD1602 display. The entire system is governed by a strict Mealy State Machine and is written entirely from scratch without the use of the STM32 Hardware Abstraction Layer (HAL).

## Hardware Configuration

- **Microcontroller:** STM32F401RE (Configurable for other STM32F4xx devices)

- **LM35 Temperature Sensor:** Connected to `PA0` (ADC1 Channel 0)

- **DC Fan (via NPN/Darlington Transistor):** Connected to `PA6` (TIM3 Channel 1 PWM)

- **LCD1602 (4-bit mode):**

  - RS -> `PB0`
  - EN -> `PB1`
  - D4 to D7 -> `PB2` to `PB5`

- **Status LEDs:**
  - IDLE State LED -> `PA2`
  - COOLING State LED -> `PA3`
  - OVERHEAT Alarm LED -> `PA4`

## Prerequisites

Before building the project, ensure your development environment is set up with the following:

- ARM GNU toolchain (`arm-none-eabi-*`) installed.
- CMake (version 3.30 or higher).
- Ninja (recommended generator) or an IDE like CLion.

## Quick Start & Build Instructions

Follow these steps to configure your environment and build the firmware binaries.

### 1. Configure Toolchain Path

Edit the `cmake/ArmToolchain.cmake` file to point to your local ARM GCC installation.
Set the `ARM_DIR` variable to the folder *just above* the `bin` directory.

```cmake
set(ARM_DIR  "C:/Path/To/Your/arm-gnu-toolchain")
```

### 2. Select Target Device (Optional)

By default, the project targets the `STM32F401xE`. If you are using a different STM32F4 board, edit `cmake/Device.cmake` to match your specific hardware.

```cmake
set(DEVICE STM32F401xE)
```

### 3. Configure and Build

Open your terminal at the project root directory and generate the build files using CMake, then compile the project:

```powershell
cmake -S . -B build -G Ninja
cmake --build build
```

### 4. Build Outputs

After a successful build, the compiled binaries and map files will be located in the `build/` directory:

- `stm32-template.elf`
- `stm32-template.hex`
- `stm32-template.bin`
- `stm32-template.map`

Flash the `.hex` or `.elf` file to your microcontroller using STM32CubeProgrammer, ST-Link Utility, or directly within your Proteus simulation.

## Project Structure

This project utilizes a modular, bare-metal folder structure. All drivers are written from the ground up:

- `Adc/`: Analog-to-Digital Converter driver (configured for one-shot mode and triggered by software to prevent CPU starvation).
- `Gpio/`: General-Purpose Input/Output driver.
- `Lcd/`: Custom LCD1602 driver operating in 4-bit parallel mode.
- `Pwm/`: Pulse-Width Modulation driver utilizing General-Purpose Timers.
- `Rcc/`: Reset and Clock Control driver for peripheral clock gating and HSI initialization.
- `Timer/`: Hardware timer driver (TIM2) used for precise, non-blocking delays and ADC polling pacing.
- `src/main.c`: Contains the core Mealy State Machine and application logic.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
