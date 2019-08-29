RFM69HCW
========
A driver for the RFM69HCW wireless transceiver that aims to be both space-efficient and capable.

How to Use
----------
Configuration is located in the project Makefile in the `src` directory.

|Variable|Usage|
|:------:|:----|
|`MSP430PATH`| Path to the MSP430 toolchain (contains the `bin` and `lib` directories).|
|`MSP430MCU`|MCU to build for. Acceptable values are those accepted by GCC's `--mcu=` option.|
|`MDLPATH`|Path to the MDL wrapper library source.|
