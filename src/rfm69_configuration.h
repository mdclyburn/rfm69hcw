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
 * RFM_CONFIG_RESETPIN
 *     Pin used to reset the RFM69.
 * RFM_CONFIG_SYNCWORD
 *     Sync word used to filter packets on.
 *     This value must be an array of length RFM_CONFIG_SYNCWORDLENGTH + 1.
 * RFM_CONFIG_SYNCWORDLENGTH
 *     Additional bytes to the sync word length (up to 7 additional bytes)
 *     For example, if set to 5 then the sync word will be 6 bytes long.
 */

#define RFM_CONFIG_COMPACT

#define RFM_CONFIG_ENCRYPTIONKEY { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

#define RFM_CONFIG_SSPIN 10

#define RFM_CONFIG_RESETPIN 9

#define RFM_CONFIG_SYNCWORD { 0xAC, 0xDC, 0xFF, 0x06 }

#define RFM_CONFIG_SYNCWORDLENGTH 3

/** Features
 *
 * RFM_FEATURE_ENCRYPTION (~ +160 bytes)
 *     Use 128-bit AES to encrypt message packets.
 *     The encryption key should be specified in RFM_CONFIG_ENCRYPTIONKEY.
 * RFM_FEATURE_SYNCWORD (~ +276 bytes, 4-byte sync word)
 *     Filter incoming packets by ensuring they match the sync word.
 *     The sync word is given by RFM_CONFIG_SYNCWORD.
 *     RFM_CONFIG_SYNCWORDLENGTH must also be specified.
 * RFM_FEATURE_RESET
 *     Enable resetting the RFM69 with the RESET pin.
 * RFM_FEATURE_TEMPERATURE
 *     Supplies rfm_temperature() to read the CMOS temperature sensor.
 */

#define RFM_FEATURE_ENCRYPTION

#define RFM_FEATURE_RESET

#define RFM_FEATURE_SYNCWORD

#define RFM_FEATURE_TEMPERATURE

// #define RFM_DEBUG

// ===== Sanity Checks =========================================================
// =============================================================================

/* RFM_FEATURE_ENCRYPTION: An encryption key must be supplied when encryption is enabled.
 */
#ifdef RFM_FEATURE_ENCRYPTION

#ifndef RFM_CONFIG_ENCRYPTIONKEY
#error To use packet encryption: specify an encryption key up to 16 bytes in length with RFM_CONFIG_ENCRYPTIONKEY.
#endif

#endif

/* RFM_FEATURE_SYNCWORD: RFM_CONFIG_SYNCWORD and RFM_CONFIG_SYNCWORDLENGTH must be supplied.
 */
#ifdef RFM_FEATURE_SYNCWORD

#ifndef RFM_CONFIG_SYNCWORD
#error To use sync word: a sync word must be specified with RFM_CONFIG_SYNCWORD (remember to set the length also!).
#endif

#ifndef RFM_CONFIG_SYNCWORDLENGTH
#error To use sync word: the length of the sync word length must be specified with RFM_CONFIG_SYNCWORDLENGTH (length should match array length minus one).
#endif

#endif

/* RFM_FEATURE_RESET: RFM_CONFIG_RESETPIN must be supplied.
 */
#ifdef RFM_FEATURE_RESET

#ifndef RFM_CONFIG_RESETPIN
#error To use the reset pin: a pin number must be specified with RFM_CONFIG_RESETPIN.
#endif

#endif
