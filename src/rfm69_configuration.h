#pragma once

// RFM69 Configuration and Features

/** Configuration
 *
 * RFM_CONFIG_COMPACT
 *     Drop unneeded code where possible.
 * RFM_CONFIG_ENCRYPTIONKEY
 *     16-byte length encryption key used to encrypt payloads.
 * RFM_CONFIG_SSPIN
 *     Pin used to select the RFM69 (SPI SS).
 */

#define RFM_CONFIG_COMPACT

#define RFM_CONFIG_ENCRYPTIONKEY { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

#define RFM_CONFIG_SSPIN P1_3

/** Features
 *
 * RFM_FEATURE_ENCRYPTION
 *     Use 128-bit AES to encrypt message packets.
 *     The encryption key should be specified in RFM_CONFIG_ENCRYPTIONKEY.
 * RFM_FEATURE_TEMPERATURE
 *     Supplies rfm_temperature() to read the CMOS temperature sensor.
 */

#define RFM_FEATURE_ENCRYPTION

#define RFM_FEATURE_TEMPERATURE

// ===== Sanity Checks =========================================================
// =============================================================================

/* An encryption key must be supplied when encryption is enabled.
 */
#ifdef RFM_FEATURE_ENCRYPTION
#ifndef RFM_CONFIG_ENCRYPTIONKEY
#error To use packet encryption: specify an encryption key up to 16 bytes in length with RFM_CONFIG_ENCRYPTIONKEY.
#endif
#endif
