# RISC-V based Virtual Prototype (VP)

### Key features of our VP:

 - RV32GC and RV64GC core support (i.e. RV32IMAFDC and RV64IMAFDC)
 - Implemented in SystemC TLM-2.0
 - SW debug capabilities (GDB RSP interface) with Eclipse
 - FreeRTOS support
 - Generic and configurable bus
 - CLINT and PLIC-based interrupt controller + additional peripherals
 - Instruction-based timing model + annotated TLM 2.0 transaction delays
 - Peripherals, e.g. display, flash controller, preliminary ethernet
 - Example configuration for the SiFive HiFive1 board available
 - Zephyr operating system support
 - Support for simulation of multi-core platforms
 - Machine-, Supervisor- and User-mode (including user traps) privilege levels and CSRs
 - Virtual memory support (Sv32, Sv39, Sv48)

For related information, e.g. verification, please visit http://www.systemc-verification.org/ or contact <riscv@systemc-verification.org>. 
We accept pull requests and in general contributions are very welcome. 

In the following we provide build instructions and how to compile and run software on the VP.


#### 1) Build the RISC-V GNU Toolchain:

(Cross-)Compiling the software examples, in order to run them on the VP, requires the RISC-V GNU toolchain to be available in PATH. Several standard packages are required to build the toolchain. On Ubuntu the required packages can be installed as follows:

```bash
sudo apt-get install autoconf automake autotools-dev curl libmpc-dev libmpfr-dev libgmp-dev gawk build-essential bison flex texinfo gperf libtool patchutils bc zlib1g-dev libexpat-dev libboost-iostreams-dev
```
On Ubuntu 20, install these:
```bash
sudo apt-get install autoconf automake autotools-dev curl libmpc-dev libmpfr-dev libgmp-dev gawk build-essential bison flex texinfo libgoogle-perftools-dev libtool patchutils bc zlib1g-dev libexpat-dev libboost-iostreams-dev libboost-program-options-dev libboost-log-dev qt5-default
```

On Fedora, following actions are required:
```bash
sudo dnf install autoconf automake curl libmpc-devel mpfr-devel gmp-devel gawk bison flex texinfo gperf libtool patchutils bc zlib-devel expat-devel cmake boost-devel
sudo dnf groupinstall "C Development Tools and Libraries"
#optional debuginfo
sudo dnf debuginfo-install boost-iostreams boost-program-options boost-regex bzip2-libs glibc libgcc libicu libstdc++ zlib
```

For more information on prerequisites for the RISC-V GNU toolchain visit https://github.com/riscv/riscv-gnu-toolchain. With the packages installed, the toolchain can be build as follows:

```bash
# in some source folder
git clone https://github.com/riscv/riscv-gnu-toolchain.git
cd riscv-gnu-toolchain
git submodule update --init --recursive # this may take a while
./configure --prefix=$(pwd)/../riscv-gnu-toolchain-dist-rv32imac-ilp32 --with-arch=rv32imac --with-abi=ilp32
make -j$(nproc)
```

For additional configurations and options for the RISC-V GNU toolchain visit https://github.com/riscv/riscv-gnu-toolchain.

If wanted, move the `riscv-gnu-toolchain-dist-rv32imac-ilp32` folder to your `/opt/` folder and add it to your path in your `~/.bashrc`
(e.g. `PATH=$PATH:/opt/riscv-gnu-toolchain-dist-rv32imac-ilp32/bin`)

#### 2) Build this RISC-V Virtual Prototype:

In the root folder, type `make`. This script does the following for you:

> i) Checkout required git submodules:
>
>```bash
>git submodule update --init vp/src/core/common/gdb-mc/libgdb/mpc
>```
>
>ii) in *vp/dependencies* folder (will download and compile SystemC, and build a local version of the softfloat library):
>
>```bash
>./build_systemc_233.sh
>./build_softfloat.sh
>```
>
>
>iii) in *vp* folder (requires the *boost* C++ library):
> 
>```bash
>mkdir build
>cd build
>cmake ..
>make install
>```

#### 3) Compile and run some Software:

In *sw*:

```bash
cd simple-sensor    # can be replaced with different example
make                # (requires RISC-V GNU toolchain in PATH)
make sim            # (requires *riscv-vp*, i.e. *vp/build/bin/riscv-vp*, executable in PATH)
```

Please note, if *make* is called without the *install* argument in step 2, then the *riscv-vp* executable is available in *vp/build/src/platform/basic/riscv-vp*.


#### 4) Optional Makefile:

The toplevel Makefile can alternatively be used to build the VP including its dependencies (i.e. step 2 in this README), from the toplevel folder call:

```bash
make
```

This will also copy the VP binaries into the *vp/build/bin* folder.

#### FAQ

**Q:** How do I exit the VP?

**A:** All VPs use the input TTY in raw mode and forward all control
characters to the guest. For this reason, one cannot use Ctrl-c to exit
the VP. Instead, press Ctrl-a to enter command mode and press Ctrl-x to
exit the VP.

**Q:** How do I emit a Ctrl-a control character?

**A:** Enter control mode using Ctrl-a and press Ctrl-a again to send a
literal Ctrl-a control character to the guest.

#### Acknowledgements:

This work was supported in part by the German Federal Ministry of Education and Research (BMBF) within the project CONFIRM under contract no. 16ES0565 and within the project SATiSFy under contract no. 16KIS0821K and within the project VerSys under contract no. 01IW19001 and within the project Scale4Edge under contract no. 16ME0127, and by the University of Bremen’s graduate school SyDe, funded by the German Excellence Initiative.
