# Hexis

<div align="center">
  <h3>Universal Hardware Hacking & Firmware Analysis Framework</h3>
  <i>The "Ghidra + Wireshark + Flashrom" of hardware security.</i>
</div>

---

## ⚡ Features

* **Modular Driver Architecture (C23):** Native C core engineered for extreme performance with capability-based driver routing and transactional hooks (`CAP_SPI`, `CAP_UART`).
* **Dual-Format Project System:** Store sessions as `.hexisproj` directories with Content-Addressed Storage (CAS) for firmware deduplication, or export as portable `.hexis` zip archives.
* **Session Replay Engine:** Logs all read/write/erase interactions to recreate automated hardware pentesting sessions.
* **Knowledge Graph & Virtual Filesystem:** Maps extracted firmware chunks into a hierarchical VFS and outputs findings to a native graph database structure.
* **Event Bus & Job System:** Completely asynchronous Pub/Sub backend ensuring the GUI remains perfectly responsive during massive memory dumps.
* **Firmware Analysis Engine:** Calculate Shannon entropy, parse ELF architectures, and scan for magic filesystem bytes (`SquashFS`, `uImage`, etc.).
* **Secret & Vulnerability Scanner:** Lightning-fast POSIX regex scanner identifying AWS keys, private certificates, and weak crypto markers directly inside firmware blobs.

## 🛠️ Installation

We strongly recommend installing Hexis directly from PyPI (packaged as `hexiskit`):

```bash
pip install hexiskit
```

*Note: Pre-compiled binary wheels are published for most environments via GitHub Actions. If you are building from source locally on Windows, you must use `vcpkg` to provide the `libusb-1.0` dependency.*

## 💻 CLI Commands

The `hexiskit` Python package comes with a built-in command line interface powered by Typer.

### Basic Usage

You can invoke the CLI using:
```bash
python3 -m hexis.cli [COMMAND] [OPTIONS]
```

### Hardware Interaction Commands

| Command | Description | Example |
|---|---|---|
| `detect` | Automatically probe and detect the connected flash memory chip via the programmer driver. | `python3 -m hexis.cli detect --driver ch341a` |
| `read` | Dump the entire contents of the flash memory into a local binary file. | `python3 -m hexis.cli read dump.bin --driver ch341a` |
| `write` | Write a local binary file into the target flash memory. | `python3 -m hexis.cli write patched.bin --driver ch341a` |
| `erase` | Completely erase the connected flash memory chip. | `python3 -m hexis.cli erase` |
| `verify` | Read the flash memory and compare it byte-by-byte against a local file. | `python3 -m hexis.cli verify original.bin` |
| `backup` | Safely backup the firmware into a structured dump. | `python3 -m hexis.cli backup backup.bin` |

*(Note: The `--driver` option defaults to `ch341a` for all hardware interaction commands).*

### Firmware Analysis Commands (In-Development)

* `analyze [filename]`: Analyze firmware file for architecture, entropy, and strings.
* `extract [filename]`: Extract underlying filesystems from raw firmware blobs.
* `secrets [filename]`: Scan firmware for hardcoded secrets, AWS keys, and private certificates.

## 🖥️ Graphical Interface (GUI)

Hexis also features a lightning-fast native C++20 / Qt6 dashboard for interactive hardware analysis.

### Launching the GUI
If you compiled the project from source, you can launch the GUI directly from the build directory:
```bash
./build/gui/hexis-gui
```

## 🏗️ Architecture

Hexis is split into three main layers to guarantee both peak performance and scriptability:
1. **`libhexis` (C23):** The highly-optimized native shared object handling USB communication, chip databases, memory analysis, and plugins.
2. **`hexis-gui` (C++20/Qt6):** The graphical dashboard providing hex editors, firmware explorers, and visual feedback for operations.
3. **`hexis` (Python 3.8+):** High-level CFFI bindings and the Typer CLI (packaged on PyPI as `hexiskit`).

## 🧱 Building from Source

To compile the native C core and Qt6 GUI locally, you need a Unix-like environment (Linux / macOS / WSL) or a Windows environment configured with `vcpkg`.

**Requirements:** CMake (3.25+), GCC/Clang (or MSVC), Qt6, and `libusb-1.0`.

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

---
*Developed by l3tchupkt / letchu.geek@gmail.com*
