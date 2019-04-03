#pragma once

// ===== Operating Modes =======================================================
// =============================================================================

/* Sleep
 * No blocks enabled
 */
#define RFM_OPMODE_SLEEP      0

/* Standby
 * Top regulator, crystal oscillator enabled
 */
#define RFM_OPMODE_STANDBY    1

/* Frequency Synthesizer (FS) mode
 * Frequency synthesizer enabled
 */
#define RFM_OPMODE_FS         2

/* Transmit mode
 * Frequency synthesizer and transmitter enabled
 */
#define RFM_OPMODE_TRANSMIT   3

/* Receive mode
 * Frequency synthesizer and receiver enabled
 */
#define RFM_OPMODE_RECEIVE    4
