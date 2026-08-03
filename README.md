# Hexis

**Hexis** is a professional, cross-platform hardware pentesting and firmware pentesting framework. It acts as the "Ghidra + Wireshark + Flashrom" of hardware security, providing an end-to-end pipeline from physical flash extraction to binary security analysis.

## Features

- **Modular Driver Architecture (C23):** Native C core engineered for extreme performance with capability-based driver routing and transactional hooks (`CAP_SPI`, `CAP_UART`).
- **Dual-Format Project System:** Store sessions as `.hexisproj` directories with Content-Addressed Storage (CAS) for firmware deduplication, or export as portable `.hexis` zip archives.
- **Session Replay Engine:** Logs all read/write/erase interactions to recreate automated hardware pentesting sessions.
- **Knowledge Graph & Virtual Filesystem:** Maps extracted firmware chunks into a hierarchical VFS and outputs findings to a native graph database structure.
- **Event Bus & Job System:** Completely asynchronous Pub/Sub backend ensuring the GUI remains perfectly responsive during massive memory dumps.
- **Firmware Analysis Engine:** Calculate Shannon entropy, parse ELF architectures, and scan for magic filesystem bytes (`SquashFS`, `uImage`, etc.).
- **Secret & Vulnerability Scanner:** Lightning-fast POSIX regex scanner identifying AWS keys, private certificates, and weak crypto markers directly inside firmware blobs.

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
