#pragma once

// RFM69 Configuration and Features

/** Configuration
 *
 * RFM_CONFIG_COMPACT
 *     Drop unneeded code where possible.
 *
 * RFM_CONFIG_SSPIN
 *     Pin used to select the RFM69 (SPI SS).
 *
 */

#define RFM_CONFIG_COMPACT

#define RFM_CONFIG_SSPIN P1_3

/** Features
 *
 * RFM_FEATURE_TEMPERATURE
 *     Supplies rfm_temperature() to read the CMOS temperature sensor.
 */

#define RFM_FEATURE_TEMPERATURE
