
#ifndef CC1101_REGS_H
#define CC1101_REGS_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************
 *      INCLUDES
 *********************/
#include "cc1101_enums.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
typedef union __attribute__((__packed__, aligned(1))) {
    uint8_t reg;
    struct __attribute__((__packed__, aligned(1)))
    {
        gdox_cfg_t gdox_cfg : 6;
        uint8_t gdox_inv : 1;
        uint8_t _reserved_7 : 1;
    };
} cc1101_iocfgx_t;



typedef union __attribute__((__packed__, aligned(1))) {
    uint8_t reg;
    struct __attribute__((__packed__, aligned(1)))
    {
        uint8_t fifo_thr : 4;
        close_in_rx_t close_in_rx : 2;
        uint8_t adc_retention : 1;
        uint8_t _reserved_7 : 1;
    };
} fifothr_t;

typedef union __attribute__((__packed__, aligned(1))) {
    uint16_t sync;
    struct __attribute__((__packed__, aligned(1)))
    {
        uint8_t sync0;
        uint8_t sync1;
    };
} sync_word_t;

typedef struct __attribute__((__packed__, aligned(1)))
{
    uint8_t sync_15_8;
} sync1_t;

typedef struct __attribute__((__packed__, aligned(1)))
{
    uint8_t sync_7_0;
} sync0_t;

typedef struct
{
    uint8_t pktlen_length;
} pktlen_t;

typedef union __attribute__((__packed__, aligned(1))) {
    uint8_t reg;
    struct __attribute__((__packed__, aligned(1)))
    {
        adr_chk_t adr_chk : 2;
        uint8_t append_status : 1;
        uint8_t crc_autoflush : 1;
        uint8_t _reserved_4 : 1;
        uint8_t pqt : 3;
    };
} pktctrl1_t;

typedef union __attribute__((__packed__, aligned(1))) {
    uint8_t reg;
    struct __attribute__((__packed__, aligned(1)))
    {
        pck_length_config_t pck_length_config : 2;
        uint8_t crc_en : 1;
        uint8_t _reserved_3 : 1;
        pkt_format_t pkt_format : 2;
        uint8_t white_data : 1;
        uint8_t _reserved_7 : 1;
    };
} pktctrl0_t;

// Device Address
typedef struct __attribute__((__packed__, aligned(1)))
{
    uint8_t device_addr;
} addr_t;

// Channel Number
typedef struct __attribute__((__packed__, aligned(1)))
{
    uint8_t chan;
} channr_t;

// Frequency Synthesizer Control
typedef struct __attribute__((__packed__, aligned(1)))
{
    uint8_t freq_if : 5;
} fsctrl1_t;

// Frequency Synthesizer Control
typedef struct __attribute__((__packed__, aligned(1)))
{
    uint8_t freqoff;
} fsctrl0_t;

// Frequency Control Word, High Byte
typedef union __attribute__((__packed__, aligned(1))) {
    uint8_t reg;
    struct __attribute__((__packed__, aligned(1)))
    {
        uint8_t freq_16_21 : 6;
        uint8_t freq_22_23 : 2;
    };
} freq2_t;

// Frequency Control Word, Mid Byte
typedef struct __attribute__((__packed__, aligned(1)))
{
    uint8_t freq_8_15;
} freq1_t;

// Frequency Control Word, Low Byte
typedef struct __attribute__((__packed__, aligned(1)))
{
    uint8_t freq_0_7;
} freq0_t;

// Modem Configuration
typedef union __attribute__((__packed__, aligned(1))) {
    uint8_t reg;
    struct __attribute__((__packed__, aligned(1)))
    {
        uint8_t drate_e : 4;
        uint8_t chanbw_m : 2;
        uint8_t chanbw_e : 2;
    };
} mdmcfg4_t;

// Modem Configuration
typedef struct
{
    uint8_t drate_m;
} mdmcfg3_t;

// Modem Configuration
typedef union __attribute__((__packed__, aligned(1))) {
    uint8_t reg;
    struct __attribute__((__packed__, aligned(1)))
    {
        sync_mode_t sync_mode : 3;
        uint8_t manchester_en : 1;
        cc1101_mod_format_t mod_format : 3;
        uint8_t dem_dcfilt_off : 1;
    };
} mdmcfg2_t;

// Modem Configuration
typedef union __attribute__((__packed__, aligned(1))) {
    uint8_t reg;
    struct __attribute__((__packed__, aligned(1)))
    {
        uint8_t chanspc_e : 2;
        uint8_t _reserved_2_3 : 2;
        num_preamble_t num_preamble : 3;
        uint8_t fec_en : 1;
    };
} mdmcfg1_t;

// Modem Configuration
typedef struct __attribute__((__packed__, aligned(1)))
{
    uint8_t chanspc_m;
} cc1101_mdmcfg0_t;

// Modem Deviation Setting
typedef union __attribute__((__packed__, aligned(1))) {
    uint8_t reg;
    struct __attribute__((__packed__, aligned(1)))
    {
        uint8_t deviation_m : 3;
        uint8_t _reserved_3 : 1;
        uint8_t deviation_e : 3;
        uint8_t _reserved_7 : 1;
    };
} deviatn_t;

// Main Radio Control State Machine Configuration
typedef struct __attribute__((__packed__, aligned(1)))
{
    uint8_t mcsm2;
} mcsm2_t;

// Main Radio Control State Machine Configuration
typedef union __attribute__((__packed__, aligned(1))) {
    uint8_t reg;
    struct __attribute__((__packed__, aligned(1)))
    {
        txoff_mode_t txoff_mode : 2;
        rxoff_mode_t rxoff_mode : 2;
        cca_mode_t cca_mode : 2;
        uint8_t _reserved_6_7 : 2;
    };
} mcsm1_t;

// Main Radio Control State Machine Configuration
typedef union __attribute__((__packed__, aligned(1))) {
    uint8_t reg;
    struct __attribute__((__packed__, aligned(1)))
    {
        uint8_t xosc_force_on : 1;
        uint8_t pin_ctrl_en : 1;
        po_timeout_t po_timeout : 2;
        fs_autocal_t fs_autocal : 2;
        uint8_t _reserved_6_7 : 2;
    };
} mcsm0_t;

// Frequency Offset Compensation Configuration
typedef struct __attribute__((__packed__, aligned(1)))
{
    uint8_t foccfg;

} foccfg_t;

// Bit Synchronization Configuration
typedef struct __attribute__((__packed__, aligned(1)))
{
    uint8_t bscfg;
} bscfg_t;

// AGC Control
typedef union __attribute__((__packed__, aligned(1))) {
    uint8_t reg;
    struct __attribute__((__packed__, aligned(1)))
    {
        magn_target_t magn_target : 3;
        uint8_t max_lna_gain : 3;
        uint8_t max_dvga_gain : 2;
    };
} agcctrl2_t;

// AGC Control
typedef union __attribute__((__packed__, aligned(1))) {
    uint8_t reg;
    struct __attribute__((__packed__, aligned(1)))
    {
        int8_t carrier_sense_abs_thr : 4;
        carrier_sense_rel_thr_t carrier_sense_rel_thr : 2;
        uint8_t agc_lna_priority : 1;
        uint8_t _reserved_7 : 1;
    };
} agcctrl1_t;

// AGC Control
typedef union __attribute__((__packed__, aligned(1))) {
    uint8_t reg;
    struct __attribute__((__packed__, aligned(1)))
    {
        filter_length_t filter_length : 2;
        uint8_t agc_freeze : 2;
        uint8_t wait_time : 2;
        hysteresis_level_t hyst_level : 2;
    };
} agcctrl0_t;

// Low Byte Event0 Timeout
typedef struct __attribute__((__packed__, aligned(1)))
{
    uint8_t event0;
} worevt0_t;

// Wake On Radio Control
typedef union __attribute__((__packed__, aligned(1))) {
    uint8_t reg;
    struct __attribute__((__packed__, aligned(1)))
    {
        uint8_t wor_res : 2;
        uint8_t _reserved_2 : 1;
        uint8_t rc_cal : 1;
        uint8_t event1 : 3;
        uint8_t rc_pd : 1;
    };
} worctrl_t;

// Front End RX Configuration
typedef union __attribute__((__packed__, aligned(1))) {
    uint8_t reg;
    struct __attribute__((__packed__, aligned(1)))
    {
        uint8_t mix_current : 2;
        uint8_t lodiv_buf_current_rx : 2;
        uint8_t lna2mix_current : 2;
        uint8_t lna_current : 2;
    };
} frend1_t;

// Front End TX Configuration
typedef union __attribute__((__packed__, aligned(1))) {
    uint8_t reg;
    struct __attribute__((__packed__, aligned(1)))
    {
        uint8_t pa_power : 3;
        uint8_t _reserved_3 : 1;
        uint8_t lodiv_buf_current_tx : 2;
        uint8_t _reserved_6_7 : 2;
    };
} frend0_t;

// Frequency Synthesizer Calibration
typedef struct __attribute__((__packed__, aligned(1)))
{
    uint8_t fscal3;
} fscal3_t;

// Frequency Synthesizer Calibration
typedef struct __attribute__((__packed__, aligned(1)))
{
    uint8_t fscal2;
} fscal2_t;

// Frequency Synthesizer Calibration
typedef struct __attribute__((__packed__, aligned(1)))
{
    uint8_t fscal1;
} fscal1_t;

// Frequency Synthesizer Calibration
typedef struct __attribute__((__packed__, aligned(1)))
{
    uint8_t fscal0;
} fscal0_t;

// RC Oscillator Configuration
typedef struct __attribute__((__packed__, aligned(1)))
{
    uint8_t rcctrl1;
} rcctrl1_t;

// RC Oscillator Configuration
typedef struct __attribute__((__packed__, aligned(1)))
{
    uint8_t rcctrl0;
} rcctrl0_t;

typedef struct __attribute__((__packed__, aligned(1)))
{
	uint8_t reg;
	struct __attribute__((__packed__, aligned(1)))
	{
		uint8_t gdo0 : 1;
		uint8_t not_used : 1;
		uint8_t gdo2 : 1;
		uint8_t spd : 1;
		uint8_t cca : 1;
		uint8_t pqt_reached : 1;
		uint8_t cs : 1;
		uint8_t crc_ok : 1;
	};
}pktstatus_t;

typedef struct __attribute__((__packed__, aligned(1))) cc1101_t
{
	cc1101_iocfgx_t iocfg2;
	cc1101_iocfgx_t iocfg1;
	cc1101_iocfgx_t iocfg0;
    fifothr_t fifothr;
    sync1_t sync1;
    sync0_t sync0;
    pktlen_t pktlen;
    pktctrl1_t pktctrl1;
    pktctrl0_t pktctrl0;
    addr_t addr;
    channr_t channr;
    fsctrl1_t fsctrl1;
    fsctrl0_t fsctrl0;
    freq2_t freq2;
    freq1_t freq1;
    freq0_t freq0;
    mdmcfg4_t mdmcfg4;
    mdmcfg3_t mdmcfg3;
    mdmcfg2_t mdmcfg2;
    mdmcfg1_t mdmcfg1;
    cc1101_mdmcfg0_t mdmcfg0;
    deviatn_t deviatn;
    mcsm2_t mcsm2;
    mcsm1_t mcsm1;
    mcsm0_t mcsm0;
    foccfg_t foccfg;
    bscfg_t bscfg;
    agcctrl2_t agcctrl2;
    agcctrl1_t agcctrl1;
    agcctrl0_t agcctrl0;
} cc1101_t;

extern cc1101_t cc1101;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* CC1101_REGS_H */
