RFM69HCW
========
A driver for the RFM69HCW wireless transceiver that aims to be both space-efficient and capable.

Building
--------
Currently, the library is compiled with GCC 9.1.0 with C++17.

Source code is in `src`.
To simplify code and reduce binary size, some configuration is provided in `rfm-config.mk`.
Edit the values in this file before compiling the project.

Configuration
-------------
For `src/Makefile`:

|Variable|Usage|
|:------:|:---:|
|`MSP430PATH`| Path to the MSP430 toolchain (contains the `bin` and `lib` directories).|
|`MSP430MCU`|MCU to build for. Acceptable values are those accepted by GCC's `-mmcu=` option.|
|`MDLMSP430PATH`|Path to the MDL wrapper library source.|

For `src/rfm-config.mk`:

|Variable|Usage|
|:------:|:---:|
|`RFM_BITRATE`|Transmission bit rate. See the RFM69HCW documentation for the correct values.|
|`RFM_ENCRYPTIONKEY`|256-bit AES key to encrypt packets with.|
|`RFM_NODEADDRESS`|8-bit address the radio will filter packets on.|
|`RFM_PINRESET`|Pin used to reset the RFM69HCW.|
|`RFM_PINSS`|Pin used for SPI slave select.|
|`RFM_SYNCWORD`|Sync word used to filter packets on.|
|`RFM_SYNCWORDLENGTH`|Byte length of `RFM_SYNCWORD` minus one.|
|`RFM_UCMODULE`|USCI module the RFM69HCW is connected to.|

License
-------
This library is licensed under the 3-clause BSD License.
See LICENSE for more information.
