# NUE-PSK Source

NUE-PSK Digital Modem firmware source for the Microchip dsPIC33FJ128MC706.

This repository contains the original MPLAB project files, C and assembly
sources, linker scripts, bootloader sources, and generated HEX/COF/MAP outputs
for the NUE-PSK digital modem firmware.

## Project Info

- Target MCU: Microchip dsPIC33FJ128MC706
- Main project: `Modem.mcp`
- Workspace: `NUEPSK.mcw`
- Firmware version noted in source: 2.33
- Version note: `Version2_33c.txt`
- Primary firmware output: `output/Modem.hex`
- Bootloader project: `dsPIC_Boot/BootLoader.mcp`

## Directory Layout

- `C Source/` - modem firmware C source files
- `C Header/` - firmware headers and lookup tables
- `Assembler Source/` - assembly source files
- `Assembler Include/` - dsPIC include files
- `Linker Script/` - application linker script
- `output/` - generated application build outputs
- `dsPIC_Boot/` - bootloader source and build outputs
- `bootloaderlow/` - low bootloader project and outputs

## Build Notes

The project is an MPLAB 8 era project and is configured through `.mcp` and
`.mcw` files. It expects the Microchip C30 toolchain and dsPIC33 support files.

Typical workflow:

1. Open `NUEPSK.mcw` or `Modem.mcp` in MPLAB IDE.
2. Confirm the selected device is `dsPIC33FJ128MC706`.
3. Confirm the C30 compiler and linker paths match the local installation.
4. Build the modem project.
5. Use `output/Modem.hex` as the application firmware image.

The checked-in project files reference historical paths such as
`C:\nuepsk_source\...`; adjust MPLAB project paths if the repository is cloned
elsewhere.

## Notes

The source history and inline changelog are mainly documented in
`C Source/NUE_PSK_main_1.c`.

