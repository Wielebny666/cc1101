#ifndef CC1101_DEFS_H
#define CC1101_DEFS_H


#ifdef __cplusplus
extern "C"
{
#endif

/*********************
 *      INCLUDES
 *********************/

/*********************
 *      DEFINES
 *********************/
#define CC1101_CLOCK_FREQUENCY (uint8_t)26

#define	RF_XTAL_FREQ				26000000UL
#define RF_RSSI_OFFSET_DEFAULT		(-74)

// Configuration Registers
#define CC1101_IOCFG2 		0x00   // GDO2 output pin configuration
#define CC1101_IOCFG1 		0x01   // GDO1 output pin configuration
#define CC1101_IOCFG0 		0x02   // GDO0 output pin configuration
#define CC1101_FIFOTHR 		0x03  // RX FIFO and TX FIFO thresholds
#define CC1101_SYNC1 		0x04    // Sync word, high byte
#define CC1101_SYNC0 		0x05    // Sync word, low byte
#define CC1101_PKTLEN 		0x06   // Packet length
#define CC1101_PKTCTRL1 	0x07 // Packet automation control
#define CC1101_PKTCTRL0 	0x08 // Packet automation control
#define CC1101_ADDR 		0x09     // Device address
#define CC1101_CHANNR 		0x0A   // Channel number
#define CC1101_FSCTRL1 		0x0B  // Frequency synthesizer control
#define CC1101_FSCTRL0 		0x0C  // Frequency synthesizer control
#define CC1101_FREQ2 		0x0D    // Frequency control word, high byte
#define CC1101_FREQ1 		0x0E    // Frequency control word, middle byte
#define CC1101_FREQ0 		0x0F    // Frequency control word, low byte
#define CC1101_MDMCFG4 		0x10  // Modem configuration
#define CC1101_MDMCFG3 		0x11  // Modem configuration
#define CC1101_MDMCFG2 		0x12  // Modem configuration
#define CC1101_MDMCFG1 		0x13  // Modem configuration
#define CC1101_MDMCFG0 		0x14  // Modem configuration
#define CC1101_DEVIATN		0x15  // Modem deviation setting
#define CC1101_MCSM2 		0x16    // Main Radio Cntrl State Machine config
#define CC1101_MCSM1 		0x17    // Main Radio Cntrl State Machine config
#define CC1101_MCSM0 		0x18    // Main Radio Cntrl State Machine config
#define CC1101_FOCCFG 		0x19   // Frequency Offset Compensation config
#define CC1101_BSCFG 		0x1A    // Bit Synchronization configuration
#define CC1101_AGCCTRL2 	0x1B // AGC control
#define CC1101_AGCCTRL1		0x1C // AGC control
#define CC1101_AGCCTRL0 	0x1D // AGC control
#define CC1101_WOREVT1 		0x1E  // High byte Event 0 timeout
#define CC1101_WOREVT0 		0x1F  // Low byte Event 0 timeout
#define CC1101_WORCTRL 		0x20  // Wake On Radio control
#define CC1101_FREND1 		0x21   // Front end RX configuration
#define CC1101_FREND0 		0x22   // Front end TX configuration
#define CC1101_FSCAL3 		0x23   // Frequency synthesizer calibration
#define CC1101_FSCAL2 		0x24   // Frequency synthesizer calibration
#define CC1101_FSCAL1 		0x25   // Frequency synthesizer calibration
#define CC1101_FSCAL0 		0x26   // Frequency synthesizer calibration
#define CC1101_RCCTRL1 		0x27  // RC oscillator configuration
#define CC1101_RCCTRL0 		0x28  // RC oscillator configuration
#define CC1101_FSTEST 		0x29   // Frequency synthesizer cal control
#define CC1101_PTEST 		0x2A    // Production test
#define CC1101_AGCTEST 		0x2B  // AGC test
#define CC1101_TEST2 		0x2C    // Various test settings
#define CC1101_TEST1 		0x2D    // Various test settings
#define CC1101_TEST0 		0x2E    // Various test settings

// Strobe commands
// #define CC1101_SRES 0x30    // Reset chip.
// #define CC1101_SFSTXON 0x31 // Enable/calibrate freq synthesizer
// #define CC1101_SXOFF 0x32   // Turn off crystal oscillator.
// #define CC1101_SCAL 0x33    // Calibrate freq synthesizer & disable
// #define CC1101_SRX 0x34     // Enable RX.
// #define CC1101_STX 0x35     // Enable TX.
// #define CC1101_SIDLE 0x36   // Exit RX / TX
// #define CC1101_SAFC 0x37    // AFC adjustment of freq synthesizer
// #define CC1101_SWOR 0x38    // Start automatic RX polling sequence
// #define CC1101_SPWD 0x39    // Enter pwr down mode when CSn goes hi
// #define CC1101_SFRX 0x3A    // Flush the RX FIFO buffer.
// #define CC1101_SFTX 0x3B    // Flush the TX FIFO buffer.
// #define CC1101_SWORRST 0x3C // Reset real time clock.
// #define CC1101_SNOP 0x3D    // No operation.

// Status registers
#define CC1101_PARTNUM 			0x30        // Part number
#define CC1101_VERSION 			0x31        // Current version number
#define CC1101_FREQEST 			0x32        // Frequency offset estimate
#define CC1101_LQI 				0x33            // Demodulator estimate for link quality
#define CC1101_RSSI 			0x34           // Received signal strength indication
#define CC1101_MARCSTATE 		0x35      // Control state machine state
#define CC1101_WORTIME1 		0x36       // High byte of WOR timer
#define CC1101_WORTIME0 		0x37       // Low byte of WOR timer
#define CC1101_PKTSTATUS 		0x38      // Current GDOx status and packet status
#define CC1101_VCO_VC_DAC 		0x39     // Current setting from PLL cal module
#define CC1101_TXBYTES 			0x3A        // Underflow and # of bytes in TXFIFO
#define CC1101_RXBYTES 			0x3B        // Overflow and # of bytes in RXFIFO
#define CC1101_RCCTRL1_STATUS 	0x3C // Last RC Oscillator Calibration Result
#define CC1101_RCCTRL0_STATUS 	0x3D // Last RC Oscillator Calibration Result
#define CC1101_NUM_RXBYTES 		0x7F    // Mask "# of bytes" field in _RXBYTES

// Other memory locations
#define CC1101_PATABLE 0x3E
#define CC1101_TXFIFO 0x3F
#define CC1101_RXFIFO 0x3F

// Masks for appended status bytes
#define CC1101_LQI_RX 0x01 // Position of LQI byte
#define CC1101_CRC_OK 0x80 // Mask "CRC_OK" bit within LQI byte

// Definitions to support burst/single access:
#define CC1101_WRITE_SINGLE				0x00
#define CC1101_READ_SINGLE 				0x02
#define CC1101_BURST 					0x01

/**********************
 *      TYPEDEFS
 **********************/
typedef struct
{
    uint8_t addr;
    uint8_t data;
} cc1101_reg_setting_t;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* CC1101_DEFS_H */
