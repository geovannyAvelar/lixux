# Lixux

Lixux is an attempt to create a simple kernel focused on RISC-V instruction set.

## Installation

You need to install [RISC-V toolchain](https://github.com/riscv-collab/riscv-gnu-toolchain), [QEMU](https://www.qemu.org/download/) and GDB multiarch (it is available on Ubuntu and Debian as *gdb-multiarch* package) in order to run this project.

To install Lixux, follow these steps:

1. Clone the repository:
    ```bash
    git clone https://github.com/geovannyAvelar/lixux.git
    ```
2. Navigate to the project directory:
    ```bash
    cd lixux
    ```
3. Run with QEMU:
    ```bash
    make qemu
    ```

**NOTE**: Port 1234/TCP must be available to run the debbuger.

4. Run GDB:
    ```bash
    make debug
    ```
