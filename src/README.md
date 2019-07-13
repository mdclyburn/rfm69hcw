RFM69HCW
========
A driver for the RFM69HCW wireless transceiver that aims to be both space-efficient and capable.

How to Use
----------
Create or copy the `rfm_configuration.h` file to your project.
Various features can be enabled or disabled with `#define`s.
This allows you to choose only the bits of code you need.
Parameters will need to be tweaked to your specific use case with `#define`s as well.

For example, placing `#define RFM_FEATURE_TEMPERATURE` in the `rfm_configuration.h` file includes code to work with the temperature sensor.
See the `rfm_configuration.h` file for a full listing of features.
