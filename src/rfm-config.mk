RFM_BITRATE = 0x1A0B
RFM_ENCRYPTIONKEY = { 0x4D, 0x22, 0x28, 0xAE, 0x54, 0x90, 0x80, 0x20, 0x00, 0x02, 0xFE, 0x10, 0x09, 0x00, 0x59, 0x11  }
RFM_NODEADDRESS = 0x34
RFM_PINRESET = 9
RFM_PINSS = 10
RFM_SYNCWORD = { 0xAC, 0xDC, 0xFF, 0x06, 0x05, 0x04, 0x03 }
RFM_SYNCWORDLENGTH = 6
RFM_UCMODULE = spi::Module::A0

RFM_CONFIG = \
	-DRFM_BITRATE="${RFM_BITRATE}" \
	-DRFM_ENCRYPTIONKEY="${RFM_ENCRYPTIONKEY}" \
	-DRFM_NODEADDRESS="${RFM_NODEADDRESS}" \
	-DRFM_PINRESET="${RFM_PINRESET}" \
	-DRFM_PINSS="${RFM_PINSS}" \
	-DRFM_SYNCWORD="${RFM_SYNCWORD}" \
	-DRFM_SYNCWORDLENGTH="${RFM_SYNCWORDLENGTH}" \
	-DRFM_UCMODULE="${RFM_UCMODULE}"