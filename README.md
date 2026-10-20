# stm32f407-clickerV2-soes

Firmware for the MikroElektronika Clicker 2 for STM32 board, based on an STM32F407VG and a LAN9252
EtherCAT controller. The project uses CMake as the common build system for
Visual Studio Code, STM32CubeIDE, and command-line builds.

## Project structure

```text
.
├── CMakeLists.txt                 # Main build configuration
├── CMakePresets.json              # Debug, Release, RelWithDebInfo, MinSizeRel
├── Makefile                       # Build, flash, and deploy shortcuts
├── cmake/
│   ├── gcc-arm-none-eabi.cmake     # GNU Arm toolchain
│   ├── soes.cmake                  # SOES, uc_test, and build metadata integration
│   └── stm32cubemx/CMakeLists.txt  # Board and HAL source configuration
├── Core/
│   ├── Inc/                       # Board headers
│   ├── Src/                       # Initialization and interrupt handlers
│   └── Startup/                   # STM32F407 startup assembly
├── Drivers/                       # STM32 HAL and CMSIS
├── external/
│   ├── soes/                      # EtherCAT stack submodule
│   └── uc_test/                   # Application, CAN code, and shared helpers
│       └── scripts/               # Linux CAN setup and RTR test scripts
├── .vscode/                       # VS Code build and debug configuration
├── .project / .cproject           # STM32CubeIDE project metadata
├── stm32f407-clickerV2-soes.ioc    # CubeMX board configuration
├── stm32f407-clickerV2-soes.launch # STM32CubeIDE debug launch
├── STM32F407VGTX_FLASH.ld          # Partitioned flash linker script
├── STM32F407VGTX_RAM.ld            # RAM linker script
└── build/<preset>/                # Generated firmware and build metadata
```

## Memory layout

The active `STM32F407VGTX_FLASH.ld` linker script defines these regions:

| Region | Start address | Size | Purpose |
| --- | --- | --- | --- |
| `CCMRAM` | `0x10000000` | 64 KiB | Core-coupled RAM |
| `RAM` | `0x20000000` | 128 KiB | Data, heap, and stack |
| `FLASH_BLD` | `0x08000000` | 48 KiB | Bootloader |
| `FLASH_PAR` | `0x0800C000` | 16 KiB | Persistent parameters (`.params`) |
| `FLASH_CAL` | `0x08010000` | 64 KiB | Calibration (`.calib`) |
| `FLASH_APP` | `0x08020000` | 128 KiB | Application vectors and code |
| `FLASH_NXT` | `0x08040000` | 256 KiB | Reserved next-image region |

The STM32F407VG has 1 MiB of flash; the remaining 512 KiB starting at
`0x08080000` is not assigned by this linker script. The `.params` and `.calib`
sections are `NOLOAD`, so their contents are not included in the firmware image.
The application binary must be programmed at `0x08020000`.

## Clone the repository

Clone the project together with its SOES and `uc-test` submodules:

```sh
git clone --recurse-submodules https://github.com/alessiomargan/stm32f407-clickerV2-soes.git
cd stm32f407-clickerV2-soes
```

If the repository was cloned without `--recurse-submodules`, initialize the
submodules afterward:

```sh
git submodule update --init --recursive
```

Both submodules use HTTPS GitHub URLs.

## Prerequisites

The common build requires:

- Git
- CMake 3.22 or newer
- Ninja
- GNU Arm Embedded toolchain (`arm-none-eabi-gcc`)

Optional programming and debugging tools include:

- An ST-LINK probe
- STM32CubeProgrammer
- OpenOCD or `stlink-tools`

Verify the basic tools with:

```sh
cmake --version
ninja --version
arm-none-eabi-gcc --version
```

## Command-line build

Configure and build the Debug preset:

```sh
cmake --preset Debug
cmake --build --preset Debug
```

Perform a clean rebuild with:

```sh
cmake --build --preset Debug --clean-first
```

Other available presets are `Release`, `RelWithDebInfo`, and `MinSizeRel`.
For example:

```sh
cmake --preset Release
cmake --build --preset Release
```

Debug artifacts are written to:

```text
build/Debug/stm32f407-clickerV2-soes.elf
build/Debug/stm32f407-clickerV2-soes.hex
build/Debug/stm32f407-clickerV2-soes.bin
```

The application is linked to run from address `0x08020000`.

## Makefile shortcuts

The repository includes a convenience `Makefile` around the Debug CMake preset.
Running `make` configures the project and performs a clean Debug build:

```sh
make
```

The available targets are:

```sh
make configure       # Configure the Debug CMake preset
make build           # Configure and perform a clean Debug build
make bin             # Build and verify that stm32f407-clickerV2-soes.bin was generated
make size            # Build and print the ELF section sizes
make flash           # Build and program the BIN at 0x08020000 with st-flash
make flash-openocd   # Build and program the ELF with OpenOCD
make deploy          # Copy the BIN to the local EtherCAT master firmware path
make clean           # Remove the complete build directory
```

By default, `make deploy` copies the firmware to:

```text
~/.ecat_master/firmware/st_lan9252.bin
```

Override the destination when necessary:

```sh
make deploy ECAT_FIRMWARE_DIR=/path/to/firmware
```

## Visual Studio Code setup

1. Open the repository root in Visual Studio Code.
2. Install **STM32CubeIDE for Visual Studio Code** by STMicroelectronics. A
   dedicated VS Code profile can be used to keep the STM32 extensions isolated
   from unrelated workspaces.
3. Allow the extension and CMake integration to discover the project.
4. Select the `Debug` configure preset and the `Debug` build preset when
   prompted.
5. Run **CMake: Configure**, followed by **CMake: Build** from the Command
   Palette.

The default `Ctrl+Shift+B` task also builds the selected CMake directory. The
clean and rebuild tasks are available through **Tasks: Run Task**.

For ST-LINK debugging, first configure the project so that
`${command:cmake.launchTargetPath}` resolves to `build/Debug/stm32f407-clickerV2-soes.elf`.
Then start the **Debug Microcontroller - ST-Link** launch configuration. If
required by the local installation, update `.vscode/launch.json` with the MCU,
SVD, ST-LINK server, and toolchain paths.

## STM32CubeIDE setup

STM32CubeIDE 1.13 or newer supports CMake projects. This repository already
contains the Eclipse project metadata and can be imported as follows:

1. Open STM32CubeIDE and select a workspace outside this repository.
2. Select **File > Import > General > Existing Projects into Workspace**.
3. Choose the cloned `stm32f407-clickerV2-soes` directory as the root directory.
4. Do not select **Copy projects into workspace**.
5. Finish the import and select the `stm32f407-clickerV2-soes` project.
6. Select **Project > Build Configurations > Set Active > Debug**.
7. Open **Project Properties > C/C++ Build > CMake Settings** and verify:
   - Source directory: the repository root (`.`)
   - Build directory: `build/Debug`
   - Generator: `Ninja`
   - CMake build type: `Debug`
   - Preset: `Debug`
   - Toolchain file: `cmake/gcc-arm-none-eabi.cmake`
8. Run **CMake > Configure** from the project context menu.
9. Select **Project > Build Project**.

To force a complete rebuild, select **Project > Clean**, run
**CMake > Configure**, and then build the project again.

The checked-in `stm32f407-clickerV2-soes` debug launch configuration must load:

```text
build/Debug/stm32f407-clickerV2-soes.elf
```

Verify this under **Run > Debug Configurations > STM32 C/C++ Application**
before programming the target.

## Using both IDEs

VS Code and the STM32CubeIDE Debug configuration intentionally share the same
CMake preset, toolchain file, build directory, and resulting firmware:

```text
build/Debug/stm32f407-clickerV2-soes.bin
```

Do not use the legacy `Debug/stm32f407-clickerV2-soes.elf` or `Debug/stm32f407-clickerV2-soes.bin` files.
They were produced by the previous STM32CubeIDE managed-make configuration.

The generated firmware contains build time, host, user, Git revision, and dirty
working-tree information. Consequently, two clean builds performed at different
times are not expected to be byte-for-byte identical, even when their build
configuration is otherwise the same.

## Flashing

When using `st-flash`, program the binary at the application address:

```sh
st-flash write build/Debug/stm32f407-clickerV2-soes.bin 0x08020000
```

Alternatively, program `build/Debug/stm32f407-clickerV2-soes.elf` through STM32CubeIDE,
STM32CubeProgrammer, or the configured VS Code debug launch.

## Updating submodules

The parent repository records exact submodule commits. To fetch the configured
tracking branches and review new revisions:

```sh
git submodule update --remote --recursive
git status
```

After testing an update, commit the changed submodule pointers in this parent
repository. Changes made inside a submodule must first be committed and pushed
from that submodule repository.

## Troubleshooting

- If CMake cannot find the compiler, ensure the directory containing
  `arm-none-eabi-gcc` is present in `PATH` before starting the IDE.
- If sources under `external/` are missing, run
  `git submodule update --init --recursive`.
- If CMake configuration is stale, remove the applicable `build/<preset>`
  directory and configure it again.
- Use only `cmake/gcc-arm-none-eabi.cmake` for this project. Do not configure a
  second toolchain file for the same build directory.

## Board and related bootloader

This application targets the MikroElektronika Clicker 2 for STM32 with an
STM32F407VG, using SPI3 for LAN9252 EtherCAT, TIM7, UART4, and CAN1.
The corresponding bootloader is
[bl-stm32f407](https://github.com/alessiomargan/bl-stm32f407).

Board configuration is stored in `stm32f407-clickerV2-soes.ioc`.
The `external/uc_test/scripts/can_rtr_test.sh` helper sends CAN RTR requests from Linux using
`can-utils`; run it with `--help` for usage.

## Linux CAN helpers

The CAN scripts are maintained in the `uc-test` submodule and shared across
firmware projects. Initialize the submodules, then run these commands on the
Linux CAN host:

```sh
./external/uc_test/scripts/setup_can0.sh
./external/uc_test/scripts/can_rtr_test.sh can0 test
```

The setup script requests sudo privileges to configure the existing `can0`
interface at 250000 bit/s. The RTR test requires `can-utils`.
See `external/uc_test/scripts/README.md` for load testing and traffic monitoring.
The `uc_test` tracking branch is `stm32f4xx`; the parent repository pins its exact
commit. Update the shared scripts in that submodule rather than copying them
into the firmware repository.
