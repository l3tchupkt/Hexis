# Hexis

**Hexis** is a professional, cross-platform hardware pentesting and firmware pentesting framework. It acts as the "Ghidra + Wireshark + Flashrom" of hardware security, providing an end-to-end pipeline from physical flash extraction to binary security analysis.

## Features

- **Modular Driver Architecture (C23):** Native C core engineered for extreme performance. Currently supports `CH341A`, architected to effortlessly expand to `FT2232H`, `Bus Pirate`, `J-Link`, etc.
- **Python Bindings (`cffi`):** Object-oriented Python API bridging natively to the core library.
- **Modern CLI:** Beautiful terminal application built with `Typer` and `Rich`.
- **Qt6 GUI Dashboard:** A sleek, dark-mode desktop frontend featuring live hardware metrics, memory viewers, and dockable control panels.
- **Firmware Analysis Engine:** Calculate Shannon entropy, parse ELF architectures, and scan for magic filesystem bytes (`SquashFS`, `uImage`, etc.).
- **Secret & Vulnerability Scanner:** Lightning-fast POSIX regex scanner identifying AWS keys, private certificates, and weak crypto markers directly inside firmware blobs.
- **Dynamic Plugin System:** Write extensions in C/C++ (`dlopen`) or Python to customize extraction logic.
- **Reporting:** Export audit findings seamlessly to Markdown and JSON formats.

## Building

Requires a Unix-like environment (Linux / macOS / WSL) with CMake, GCC/Clang, Qt6, and `libusb-1.0`.

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

## Quick Start (CLI)

```bash
# Detect connected chip
python3 -m hexis.cli detect --driver ch341a

# Read firmware
python3 -m hexis.cli read firmware.bin

# Launch the GUI
./build/gui/hexis-gui
```

## Architecture Overview

Hexis is split into three main layers:
1. `libhexis` (C23): The native shared object handling USB communication, chip databases, memory analysis, and plugins.
2. `hexis-gui` (C++20/Qt6): The graphical dashboard for interactive hardware analysis.
3. `hexis` (Python): High-level bindings and Typer CLI.

---
*Developed by l3tchupkt / letchu.geek@gmail.com*
