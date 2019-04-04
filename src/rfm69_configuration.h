#pragma once

// RFM69 Configuration and Features

/** Configuration
 *
 * RFM_CONFIG_BITRATE
 *     Rate of data transmission.
 *     Defaults to 4.8kbps.
 *       1.2kbps    0x682B
 *       2.4kbps    0x3415
 *       4.8kbps    0x1A0B
 *       9.6kbps    0x0D05
 *      19.2kbps    0x0683
 *      38.4kbps    0x0341
 *      76.8kbps    0x01A1
 *     153.6kbps    0x00D0
 * RFM_CONFIG_COMPACT
 *     Drop unneeded code where possible.
 * RFM_CONFIG_ENCRYPTIONKEY
 *     16-byte length encryption key used to encrypt payloads.
 * RFM_CONFIG_PACKETFIXED
 *     Packet size is set ahead of time.
 *     The size of packets should be specified in RFM_CONFIG_PACKETSIZE.
 * RFM_CONFIG_PACKETSIZE
 *     Bytes contained in each packet.
 *     When using variable-length packets, this value becomes the max received payload size.
 *     If not specified, it is set to the maximum size allowable.
 * RFM_CONFIG_PACKETVARIABLE
 *     Packet size can vary.
 * RFM_CONFIG_PINRESET
 *     Pin used to reset the RFM69.
 * RFM_CONFIG_PINSS
 *     Pin used to select the RFM69 (SPI SS).
 * RFM_CONFIG_SYNCWORD
 *     Sync word used to filter packets on.
 *     This value must be an array of length RFM_CONFIG_SYNCWORDLENGTH + 1.
 * RFM_CONFIG_SYNCWORDLENGTH
 *     Additional bytes to the sync word length (up to 7 additional bytes)
 *     For example, if set to 5 then the sync word will be 6 bytes long.
 */

#define RFM_CONFIG_COMPACT

#define RFM_CONFIG_ENCRYPTIONKEY { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

// #define RFM_CONFIG_PACKETFIXED

// #define RFM_CONFIG_PACKETSIZE 32

#define RFM_CONFIG_PACKETVARIABLE

#define RFM_CONFIG_PINRESET 9

#define RFM_CONFIG_PINSS 10

#define RFM_CONFIG_SYNCWORD { 0xAC, 0xDC, 0xFF, 0x06 }

#define RFM_CONFIG_SYNCWORDLENGTH 3

/** Features
 *
 * RFM_FEATURE_ADDRESSING
 *     Not implemented.
 * RFM_FEATURE_CRC
 *     Not implemented.
 * RFM_FEATURE_ENCRYPTION (~ +160 bytes)
 *     Use 128-bit AES to encrypt message packets.
 *     The encryption key should be specified in RFM_CONFIG_ENCRYPTIONKEY.
 * RFM_FEATURE_LISTEN
 *     Listen for messages when in standby mode.
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

// #define RFM_FEATURE_LISTEN

#define RFM_FEATURE_RESET

#define RFM_FEATURE_SYNCWORD

#define RFM_FEATURE_TEMPERATURE

// #define RFM_DEBUG

// ===== Autoconfiguration =====================================================
// =============================================================================

/* RFM_CONFIG_PACKETVARIABLE: set max payload received size
 */
#ifdef RFM_CONFIG_PACKETVARIABLE

#ifndef RFM_CONFIG_PACKETSIZE

// Max receive size is 255, regardless of addressing.
#ifndef RFM_FEATURE_ENCRYPTION
#define RFM_CONFIG_PACKETSIZE 255
#endif

// Max receive size is 65 bytes without addressing: length byte + message (1 + 64)
// Max receive size is 50 bytes with addressing: length byte + address byte + message (1 + 1 + 48)
#ifdef RFM_FEATURE_ENCRYPTION
#ifndef RFM_FEATURE_ADDRESSING
#define RFM_CONFIG_PACKETSIZE 65
#endif
#else
#define RFM_CONFIG_PACKETSIZE 50
#endif

#endif

#endif

// ===== Sanity Checks =========================================================
// =============================================================================

/* RFM_CONFIG_BITRATE: Defaults to 4.8kbps.
 */
#ifndef RFM_CONFIG_BITRATE

#define RFM_CONFIG_BITRATE 0x1A0B

#endif

/* RFM_FEATURE_ENCRYPTION: An encryption key must be supplied when encryption is enabled.
 */
#ifdef RFM_FEATURE_ENCRYPTION

#ifndef RFM_CONFIG_ENCRYPTIONKEY
#error To use packet encryption: specify an encryption key up to 16 bytes in length with RFM_CONFIG_ENCRYPTIONKEY.
#endif

#endif

/* RFM_CONFIG_PACKETFIXED:
 * - RFM_CONFIG_PACKETVARIABLE is not also defined (more generally covers both being defined).
 * - RFM_CONFIG_PACKETSIZE is defined.
 */
#ifdef RFM_CONFIG_PACKETFIXED

#ifdef RFM_CONFIG_PACKETVARIABLE
#error Both fixed- and variable-size packets cannot be used at the same time.
#endif

#endif

/* RFM_CONFIG_PACKETVARIABLE: check the maximum received payload size.
 */
#ifdef RFM_CONFIG_PACKETVARIABLE

// Max receive size is 255, regardless of addressing.
#ifndef RFM_FEATURE_ENCRYPTION
#if RFM_CONFIG_PACKETSIZE > 255
#error Packet size cannot be greater than 255 bytes.
#endif
#endif

// Max receive size is 65 bytes without addressing: length byte + message (1 + 64)
// Max receive size is 50 bytes with addressing: length byte + address byte + message (1 + 1 + 48)
#ifdef RFM_FEATURE_ENCRYPTION
#ifdef RFM_FEATURE_ADDRESSING

#if RFM_CONFIG_PACKETSIZE > 50
#error Packet size cannot be greater than 50 bytes.
#endif

#else // not using addressing

#if RFM_CONFIG_PACKETSIZE > 65
#error Packet size cannot be greater than 65 bytes.
#endif

#endif // addressing
#endif // encryption

#endif

/* RFM_CONFIG_PACKETFIXED, RFM_CONFIG_PACKETVARIABLE: a packet mode is set
 */
#ifndef RFM_CONFIG_PACKETFIXED
#ifndef RFM_CONFIG_PACKETVARIABLE
#error One of either fixed or variable-length packets must be specified with RFM_CONFIG_PACKETFIXED or RFM_CONFIG_PACKETVARIABLE.
#endif
#endif

#ifndef RFM_CONFIG_PACKETSIZE
#error To use fixed packet size: the packet size must be defined with RFM_CONFIG_PACKETSIZE.
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

/* RFM_FEATURE_RESET: RFM_CONFIG_PINRESET must be supplied.
 */
#ifdef RFM_FEATURE_RESET

#ifndef RFM_CONFIG_PINRESET
#error To use the reset pin: a pin number must be specified with RFM_CONFIG_PINRESET.
#endif

#endif
