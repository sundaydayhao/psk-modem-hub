# WinPSK Source 2.13

This repository contains the WinPSK 2.13 Windows client source code.

WinPSK is an MFC-based Windows application for PSK digital communication. It provides the graphical interface for receiving and transmitting text, configuring macros, displaying spectrum/waterfall views, tuning frequency, and managing user settings. The lower-level modulation/demodulation core is linked through `PSKCore.lib`.

## Project Contents

- `WinPSK.cpp`, `WinPSKView.cpp`, `MainFrm.cpp`: main application and UI logic
- `RcvView.cpp`, `RcvEditCtrl.cpp`: receive window and receive text control
- `XmitView.cpp`, `XmitEditCtrl.cpp`: transmit window and transmit text control
- `PlotData.cpp`, `PlotData.h`: spectrum and waterfall plotting
- `Settings.cpp`, `SetDlg.cpp`: application settings and setup dialog
- `MacroDlg.cpp`: macro setup dialog
- `PSKCore.lib`: external PSK core library used by the application
- `res/`: icons, bitmap toolbar, and resource support files

## Build Notes

The original source was written for Microsoft Visual C++ 6.0 and uses MFC.

Open `WinPSK.dsw` or `WinPSK.dsp` in Visual C++ 6.0 or a compatible environment. Debug builds may run significantly slower because compiler optimization is disabled.

## System Requirements

Original target environment:

- Windows 95/98/NT or compatible 32-bit Windows environment
- 133 MHz Pentium-class CPU or better
- Floating point processor required
- Windows-supported sound card

## Additional Notes

See `ReadMe.txt` for the original revision history and detailed source file descriptions.

The Chinese directory overview file contains a short summary of the directory and its role as a Windows front-end reference project.
