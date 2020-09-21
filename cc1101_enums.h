/*
 * cc1101_enums.h
 *
 *  Created on: 6 lip 2020
 *      Author: kurzawa.p
 */

#ifndef CC1101_ENUMS_H
#define CC1101_ENUMS_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
typedef enum cc1101_marc_state_e
{
    CC1101_STATE_SLEEP = 0,
    CC1101_STATE_IDLE,
    CC1101_STATE_XOFF,
    CC1101_STATE_VCOON_MC,
    CC1101_STATE_REGON_MC,
    CC1101_STATE_MANCAL,
    CC1101_STATE_VCOON,
    CC1101_STATE_REGON,
    CC1101_STATE_STARTCAL,
    CC1101_STATE_BWBOOST,
    CC1101_STATE_FS_LOCK,
    CC1101_STATE_IFADCON,
    CC1101_STATE_ENDCAL,
    CC1101_STATE_RX,
    CC1101_STATE_RX_END,
    CC1101_STATE_RX_RST,
    CC1101_STATE_TXRX_SWITCH,
    CC1101_STATE_RXFIFO_OVERFLOW,
    CC1101_STATE_FSTXON,
    CC1101_STATE_TX,
    CC1101_STATE_TX_END,
    CC1101_STATE_RXTX_SWITCH,
    CC1101_STATE_TXFIFO_UNDERFLOW
} cc1101_marc_state_t;

typedef enum CC1101_command_strobe_e
{
    CC1101_SRES = 0x30,    // Reset chip.
    CC1101_SFSTXON = 0x31, // Enable/calibrate freq synthesizer
    CC1101_SXOFF = 0x32,   // Turn off crystal oscillator.
    CC1101_SCAL = 0x33,    // Calibrate freq synthesizer & disable
    CC1101_SRX = 0x34,     // Enable RX.
    CC1101_STX = 0x35,     // Enable TX.
    CC1101_SIDLE = 0x36,   // Exit RX / TX
    CC1101_SAFC = 0x37,    // AFC adjustment of freq synthesizer
    CC1101_SWOR = 0x38,    // Start automatic RX polling sequence
    CC1101_SPWD = 0x39,    // Enter pwr down mode when CSn goes hi
    CC1101_SFRX = 0x3A,    // Flush the RX FIFO buffer.
    CC1101_SFTX = 0x3B,    // Flush the TX FIFO buffer.
    CC1101_SWORRST = 0x3C, // Reset real time clock.
    CC1101_SNOP = 0x3D     // No operation.
} cc1101_command_strobe_t;

typedef enum
{
    GDOx_RX_FIFO = 0x00,                  /* assert above threshold, deassert when below         */
    GDOx_RX_FIFO_EOP = 0x01,              /* assert above threshold or EOP                       */
    GDOx_TX_FIFO = 0x02,                  /* assert above threshold, deassert below thr          */
    GDOx_TX_THR_FULL = 0x03,              /* asserts when TX FIFO is full. De-asserts when       */
                                          /* the TX FIFO is drained below TXFIFO_THR.            */
    GDOx_RX_OVER = 0x04,                  /* asserts when RX overflow, deassert when flushed     */
    GDOx_TX_UNDER = 0x05,                 /* asserts when RX underflow, deassert when flushed    */
    GDOx_SYNC_WORD = 0x06,                /* assert SYNC sent/recv, deasserts on EOP             */
                                          /* In RX, de-assert on overflow or bad address         */
                                          /* In TX, de-assert on underflow                       */
    GDOx_RX_OK = 0x07,                    /* assert when RX PKT with CRC ok, de-assert on 1byte  */
                                          /* read from RX Fifo                                   */
    GDOx_PREAMB_OK = 0x08,                /* assert when preamble quality reached : PQI/PQT ok   */
    GDOx_CCA = 0x09,                      /* Clear channel assessment. High when RSSI level is   */
                                          /* below threshold (dependent on the current CCA_MODE) */
    GDOx_PLL = 0x0A,                      /*Lock detector output. The PLL is in lock if the lock detector output has a positive transition or is constantly logic high.*/
                                          /*To check for PLL lock the lock detector output should be used as an interrupt for the MCU.*/
    GDOx_SERIAL_SYNC_CLOCK = 0x11,        /*Serial Clock. Synchronous to the data in synchronous serial mode.*/
                                          /*In RX mode, data is set up on the falling edge by CC1101 when GDOx_INV=0.*/
                                          /*In TX mode, data is sampled by CC1101 on the rising edge of the serial clock when GDOx_INV=0.*/
    GDOx_SERIAL_SYNC_DATA_OUTPUT = 0x0C,  /*Serial Synchronous Data Output. Used for synchronous serial mode.*/
    GDOx_SERIAL_ASYNC_DATA_OUTPUT = 0x0D, /*Serial Data Output. Used for asynchronous serial mode.*/
    GDOx_RSSI_OK = 0x0E,                  /*Carrier sense. High if RSSI level is above threshold. Cleared when entering IDLE mode.*/
    GDOx_CRC_MATCH = 0x0F,                /*CRC_OK. The last CRC comparison matched. Cleared when entering/restarting RX mode.*/
    GDOx_CHIP_RDY = 0x29,                 /* CHIP_RDY     */
    GDOx_XOSC_STABLE = 0x2B,              /* XOSC_STABLE  */
    GDOx_CLK_XOSC_1 = 0x30,               /* CLK_XOSC/1   */
    GDOx_CLK_XOSC_1p5 = 0x31,             /* CLK_XOSC/1.5 */
    GDOx_CLK_XOSC_2 = 0x32,               /* CLK_XOSC/2   */
    GDOx_CLK_XOSC_3 = 0x33,               /* CLK_XOSC/3   */
    GDOx_CLK_XOSC_4 = 0x34,               /* CLK_XOSC/4   */
    GDOx_CLK_XOSC_6 = 0x35,               /* CLK_XOSC/6   */
    GDOx_CLK_XOSC_8 = 0x36,               /* CLK_XOSC/8   */
    GDOx_CLK_XOSC_12 = 0x37,              /* CLK_XOSC/12  */
    GDOx_CLK_XOSC_16 = 0x38,              /* CLK_XOSC/16  */
    GDOx_CLK_XOSC_24 = 0x39,              /* CLK_XOSC/24  */
    GDOx_CLK_XOSC_32 = 0x3A,              /* CLK_XOSC/32  */
    GDOx_CLK_XOSC_48 = 0x3B,              /* CLK_XOSC/48  */
    GDOx_CLK_XOSC_64 = 0x3C,              /* CLK_XOSC/64  */
    GDOx_CLK_XOSC_96 = 0x3D,              /* CLK_XOSC/96  */
    GDOx_CLK_XOSC_128 = 0x3E,             /* CLK_XOSC/128 */
    GDOx_CLK_XOSC_192 = 0x3F              /* CLK_XOSC/192 */
} gdox_cfg_t;

typedef enum
{
    ATTENUATION_0DB,
    ATTENUATION_6DB,
    ATTENUATION_12DB,
    ATTENUATION_18DB
} close_in_rx_t;

/* Available data rates */
typedef enum rate_e
{
    RATE_50,
    RATE_110,
    RATE_300,
    RATE_600,
    RATE_1200,
    RATE_2400,
    RATE_4800,
    RATE_9600,
    RATE_14400,
    RATE_19200,
    RATE_28800,
    RATE_38400,
    RATE_57600,
    RATE_76800,
    RATE_115200,
	RATE_MAX
} rate_t;

typedef enum
{
    HYSTERESIS_NONE = 0,
    HYSTERESIS_LOW,
    HYSTERESIS_MEDIUM,
    HYSTERESIS_LARGE,
} hyst_level_t;

typedef enum
{
    FILTER_4DB = 0,
    FILTER_8DB,
    FILTER_12DB,
    FILTER_16DB,
} filter_length_t;

typedef enum
{
    MAGN_24DB = 0,
    MAGN_27DB,
    MAGN_30DB,
    MAGN_33DB,
    MAGN_36DB,
    MAGN_38DB,
    MAGN_40DB,
    MAGN_42DB
} magn_target_t;

typedef enum
{
    TIMEOUT_1,
    TIMEOUT_16,
    TIMEOUT_64,
    TIMEOUT_256
} po_timeout_t;

typedef enum
{
    AUTOCAL_NEVER,
    AUTOCAL_IDLE_TO_RX_TX,
    AUTOCAL_RX_TX_TO_IDLE,
    AUTOCAL_RX_TX_TO_IDLE_EVERY_4TH
} fs_autocal_t;

/*Clear channel indication*/
typedef enum
{
    CCA_MODE_ALWAYS = 0,
    CCA_MODE_RSSI_BELOW_TH,
    CCA_MODE_PKT_RX,
    CCA_MODE_RSSI__BELOW_TH_OR_PKT_RX,
} cca_mode_t;

typedef enum
{
    TXOFF_MODE_IDLE = 0,
    TXOFF_MODE_FSTXON,
    TXOFF_MODE_STAY_TX,
    TXOFF_MODE_RX
} txoff_mode_t;

typedef enum
{
    RXOFF_MODE_IDLE = 0,
    RXOFF_MODE_FSTXON,
    RXOFF_MODE_TX,
    RXOFF_MODE_STAY_RX
} rxoff_mode_t;

typedef enum
{
    PREAMBLE_2,
    PREAMBLE_3,
    PREAMBLE_4,
    PREAMBLE_6,
    PREAMBLE_8,
    PREAMBLE_12,
    PREAMBLE_16,
    PREAMBLE_24
} num_preamble_t;

/* Different modulation types */
typedef enum
{
    RADIO_MOD_2FSK = 0,
    RADIO_MOD_GFSK = 1,
    RADIO_MOD_OOK = 3,
    RADIO_MOD_4FSK = 4,
    RADIO_MOD_MSK = 7
} cc1101_mod_format_t;


/* Sync word type */
typedef enum
{
    NO_SYNC = 0,             // No preamble/sync
    SYNC_15_OVER_16,         // 15/16 sync word bits detected
    SYNC_16_OVER_16,         // 16/16 sync word bits detected
    SYNC_30_over_32,         // 30/32 sync word bits detected
    SYNC_CARRIER,            // No preamble/sync, carrier-sense above threshold
    SYNC_15_OVER_16_CARRIER, // 15/16 + carrier-sense above threshold
    SYNC_16_OVER_16_CARRIER, // 16/16 + carrier-sense above threshold
    SYNC_30_over_32_CARRIER  // 30/32 + carrier-sense above threshold
} sync_mode_t;

typedef enum
{
    PKT_FORMAT_NORMAL,
    PKT_FORMAT_SYNC_SERIAL,
    PKT_FORMAT_RANDOM,
    PKT_FORMAT_ASYNC_SERIAL
} pkt_format_t;

typedef enum
{
    LENGTH_FIXED,
    LENGTH_VARIABLE,
    LENGTH_INFINITE,
    LENGTH_RESERVERD
} pck_length_config_t;

typedef enum
{
    ADDR_NO_CHECK,
    ADDR_CHECK_NO_BROADCAST,
    ADDR_CHECK_BROADCAST_0,
    ADDR_CHECK_NO_BROADCAST_0_255
} adr_chk_t;

typedef enum
{
	CS_REL_THR_INC_DISABLE,
	CS_REL_THR_INC_6DB,
	CS_REL_THR_INC_10DB,
	CS_REL_THR_INC_14DB,
} carrier_sense_rel_thr_t;
/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* CC1101_ENUMS_H */
