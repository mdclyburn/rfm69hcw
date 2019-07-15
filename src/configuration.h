#pragma once

// ===== Autoconfiguration =====================================================
// =============================================================================

/* RFM_CONFIG_BITRATE: Defaults to 4.8kbps.
 */
#ifndef RFM_CONFIG_BITRATE
  #define RFM_CONFIG_BITRATE 0x1A0B
#endif

/* RFM_CONFIG_PACKETVARIABLE: set max payload received size
 */
#if defined(RFM_CONFIG_PACKETVARIABLE) && !defined(RFM_CONFIG_PACKETSIZE)

// Max receive size is 255, regardless of addressing.
  #ifndef RFM_FEATURE_ENCRYPTION
    #define RFM_CONFIG_PACKETSIZE 255
  #endif

// Max receive size is 65 bytes without addressing: length byte + message (1 + 64)
// Max receive size is 50 bytes with addressing: length byte + address byte + message (1 + 1 + 48)
  #ifdef RFM_FEATURE_ENCRYPTION
    #ifndef RFM_FEATURE_ADDRESSING
      #define RFM_CONFIG_PACKETSIZE 65
    #else
      #define RFM_CONFIG_PACKETSIZE 50
    #endif
  #endif // #ifdef RFM_FEATURE_ENCRYPTION
#endif

// ===== Sanity Checks =========================================================
// =============================================================================

/* RFM_FEATURE_ADDRESSING: An address is specified.
 */
#if defined(RFM_FEATURE_ADDRESSING) && !defined(RFM_CONFIG_NODEADDRESS)
  #error To use addressing: specify an address with RFM_CONFIG_NODEADDRESS.
#endif

/* RFM_FEATURE_ENCRYPTION: An encryption key must be supplied when encryption is enabled.
 */
#if defined(RFM_FEATURE_ENCRYPTION) && !defined(RFM_CONFIG_ENCRYPTIONKEY)
  #error To use packet encryption: specify an encryption key up to 16 bytes in length with RFM_CONFIG_ENCRYPTIONKEY.
#endif

/* RFM_CONFIG_PACKETFIXED:
 * - RFM_CONFIG_PACKETVARIABLE is not also defined (more generally covers both being defined).
 * - RFM_CONFIG_PACKETSIZE is defined.
 */
#ifdef RFM_CONFIG_PACKETFIXED
  #ifdef RFM_CONFIG_PACKETVARIABLE
    #error Both fixed- and variable-size packets cannot be used at the same time.
  #endif

  #ifndef RFM_CONFIG_PACKETSIZE
    #error One of either fixed or variable-length packets must be specified with RFM_CONFIG_PACKETFIXED or RFM_CONFIG_PACKETVARIABLE.
  #endif
#endif

/* RFM_CONFIG_PACKETVARIABLE: check the maximum received payload size (max 255 bytes).
 */
#ifdef RFM_CONFIG_PACKETVARIABLE
  #if !defined(RFM_FEATURE_ENCRYPTION) && RFM_CONFIG_PACKETSIZE > 255
    #error Packet size cannot be greater than 255 bytes.
  #endif

// Max receive size is 50 bytes with addressing: length byte + address byte + message (1 + 1 + 48)
  #if defined(RFM_FEATURE_ENCRYPTION) && defined(RFM_FEATURE_ADDRESSING) && RFM_CONFIG_PACKETSIZE > 50
    #error Packet size cannot be greater than 50 bytes.
  #endif

// Max receive size is 65 bytes without addressing: length byte + message (1 + 64)
  #if defined(RFM_FEATURE_ENCRYPTION) && !defined (RFM_FEATURE_ADDRESSING) && RFM_CONFIG_PACKETSIZE > 65
     #error Packet size cannot be greater than 65 bytes.
  #endif
#endif // encryption

/* RFM_CONFIG_PACKETFIXED: packet size must be specified
 */
#if defined(RFM_CONFIG_PACKETFIXED) && !defined(RFM_CONFIG_PACKETSIZE)
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
#if defined(RFM_FEATURE_RESET) && !defined(RFM_CONFIG_PINRESET)
  #error To use the reset pin: a pin number must be specified with RFM_CONFIG_PINRESET.
#endif
