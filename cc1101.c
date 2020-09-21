/*
 * cc1101.c
 *
 *  Created on: 12 maj 2020
 *      Author: kurzawa.p
 */

/*********************
 *      INCLUDES
 *********************/
#include <math.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "esp_log.h"

#include "cc1101_cfg.h"
#include "cc1101_regs.h"
#include "cc1101_hal.h"
#include "cc1101.h"
#include "foreach.h"

/*********************
 *      DEFINES
 *********************/
#define SCALING_FREQEST (unsigned long)((RF_XTAL_FREQ)/16.384)

/**********************
 *  STATIC PROTOTYPES
 **********************/
static uint16_t convert_hex_to_manchester(uint8_t hex);
static int32_t cc1101_calculate_freq_error_est(uint8_t freq_reg_error);

/**********************
 *  STATIC VARIABLES
 **********************/
static const char *TAG = "cc1101";

static uint16_t chanBW_limits[] = {812, 650, 541, 464, 406, 325, 270, 232, 203, 162, 135, 116, 102, 81, 68, 58};
cc1101_t cc1101;

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void cc1101_init_config(void)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	foreach (cc1101_reg_setting_t *cfg, preferredSettings)
	{
		cc1101_write_register(cfg->addr, cfg->data);
	}
	ESP_LOGD(TAG, "Set init settings.");
}

void cc1101_read_config(void)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);
	cc1101_read_burst_registers(0, (uint8_t*) &cc1101, sizeof(cc1101));
}

void cc1101_print_config()
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	ESP_LOGI(TAG, "%s %#04x", "iocfg2  ", cc1101.iocfg2.reg);
	ESP_LOGI(TAG, "%s %#04x", "iocfg1  ", cc1101.iocfg1.reg);
	ESP_LOGI(TAG, "%s %#04x", "iocfg0  ", cc1101.iocfg0.reg);
	ESP_LOGI(TAG, "%s %#04x", "fifothr ", cc1101.fifothr.reg);
	ESP_LOGI(TAG, "%s %#04x", "sync1   ", cc1101.sync1.sync_15_8);
	ESP_LOGI(TAG, "%s %#04x", "sync0   ", cc1101.sync0.sync_7_0);
	ESP_LOGI(TAG, "%s %#04x", "pktlen  ", cc1101.pktlen.pktlen_length);
	ESP_LOGI(TAG, "%s %#04x", "pktctrl1", cc1101.pktctrl1.reg);
	ESP_LOGI(TAG, "%s %#04x", "pktctrl0", cc1101.pktctrl0.reg);
	ESP_LOGI(TAG, "%s %#04x", "addr    ", cc1101.addr.device_addr);
	ESP_LOGI(TAG, "%s %#04x", "channr  ", cc1101.channr.chan);
	ESP_LOGI(TAG, "%s %#04x", "fsctrl1 ", cc1101.fsctrl1.freq_if);
	ESP_LOGI(TAG, "%s %#04x", "fsctrl0 ", cc1101.fsctrl0.freqoff);
	ESP_LOGI(TAG, "%s %#04x", "freq2   ", cc1101.freq2.reg);
	ESP_LOGI(TAG, "%s %#04x", "freq1   ", cc1101.freq1.freq_8_15);
	ESP_LOGI(TAG, "%s %#04x", "freq0   ", cc1101.freq0.freq_0_7);
	ESP_LOGI(TAG, "%s %#04x", "mdmcfg4 ", cc1101.mdmcfg4.reg);
	ESP_LOGI(TAG, "%s %#04x", "mdmcfg3 ", cc1101.mdmcfg3.drate_m);
	ESP_LOGI(TAG, "%s %#04x", "mdmcfg2 ", cc1101.mdmcfg2.reg);
	ESP_LOGI(TAG, "%s %#04x", "mdmcfg1 ", cc1101.mdmcfg1.reg);
	ESP_LOGI(TAG, "%s %#04x", "mdmcfg0 ", cc1101.mdmcfg0.chanspc_m);
	ESP_LOGI(TAG, "%s %#04x", "deviatn ", cc1101.deviatn.reg);
	ESP_LOGI(TAG, "%s %#04x", "mcsm2   ", cc1101.mcsm2.mcsm2);
	ESP_LOGI(TAG, "%s %#04x", "mcsm1   ", cc1101.mcsm1.reg);
	ESP_LOGI(TAG, "%s %#04x", "mcsm0   ", cc1101.mcsm0.reg);
	ESP_LOGI(TAG, "%s %#04x", "foccfg  ", cc1101.foccfg.foccfg);
	ESP_LOGI(TAG, "%s %#04x", "bscfg   ", cc1101.bscfg.bscfg);
	ESP_LOGI(TAG, "%s %#04x", "agcctrl2", cc1101.agcctrl2.reg);
	ESP_LOGI(TAG, "%s %#04x", "agcctrl1", cc1101.agcctrl1.reg);
	ESP_LOGI(TAG, "%s %#04x", "agcctrl0", cc1101.agcctrl0.reg);
}

void cc1101_set_carrier_frequency(double frequency_in_mHz)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	// bounds checking for cc1101 hardware limitations
	if (!(((frequency_in_mHz >= 300) && (frequency_in_mHz <= 348)) || ((frequency_in_mHz >= 387) && (frequency_in_mHz <= 464)) || ((frequency_in_mHz >= 779) && (frequency_in_mHz <= 928))))
	{
		ESP_LOGW(TAG, "Frequency out of bounds! Use 300-348, 387-464, 779-928MHz only!");
		return;
	}

	// trying to avoid any floating point issues
	double_t secondByteOverflow = fmod(frequency_in_mHz, 26);
	uint8_t firstByteValue = (frequency_in_mHz - secondByteOverflow) / 26;

	double_t thirdByteOverflow = fmod((secondByteOverflow * 255), 26);
	uint8_t secondByteValue = ((secondByteOverflow * 255) - thirdByteOverflow) / 26;

	double_t excessOverflow = fmod((thirdByteOverflow * 255), 26);
	uint8_t thirdByteValue = ((thirdByteOverflow * 255) - excessOverflow) / 26;

	cc1101_write_register(CC1101_FREQ2, firstByteValue);
	cc1101_write_register(CC1101_FREQ1, secondByteValue);
	cc1101_write_register(CC1101_FREQ0, thirdByteValue);

	ESP_LOGI(TAG, "Set Carrier Frequency: %f. FREQ2: %02X, FREQ1: %02X, FREQ0: %02X", frequency_in_mHz, firstByteValue, secondByteValue, thirdByteValue);
}

double cc1101_get_carrier_frequency(void)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	uint8_t firstRegisterByte, secondRegisterByte, thirdRegisterByte;
	cc1101_read_register(CC1101_FREQ2, &firstRegisterByte);
	cc1101_read_register(CC1101_FREQ1, &secondRegisterByte);
	cc1101_read_register(CC1101_FREQ0, &thirdRegisterByte);
	double_t firstRegister = firstRegisterByte * 26.0;
	double_t secondRegister = secondRegisterByte / 255.0 * 26.0;
	double_t thirdRegister = thirdRegisterByte / 255.0 / 255.0 * 26.0;

	return firstRegister + +secondRegister + +thirdRegister;
}

void cc1101_set_baud_rate_kHz(double baud_rate_in_khz)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	uint8_t baudRateExponent = 0;
	uint8_t baudRateMantissa = 0;

	for (double_t tempExponent = 0; tempExponent < 16; tempExponent++)
	{
		double_t tempMantissa = (baud_rate_in_khz * 1000.0 * pow(2, 28) / (pow(2, tempExponent) * RF_XTAL_FREQ)) - 256 + .5;
		if (tempMantissa < 256)
		{
			baudRateExponent = tempExponent;
			baudRateMantissa = tempMantissa;
			break;
		}
	}

	mdmcfg4_t mdmcfg4;
	cc1101_read_register(CC1101_MDMCFG4, &mdmcfg4.reg);
	mdmcfg4.drate_e = baudRateExponent;

	cc1101_write_register(CC1101_MDMCFG4, mdmcfg4.reg);
	cc1101_write_register(CC1101_MDMCFG3, baudRateMantissa);
	ESP_LOGD(TAG, "Set Transmission Baud rate: %f. E: %d, M: %d", baud_rate_in_khz, baudRateExponent, baudRateMantissa);
}

double cc1101_get_baud_rate(void)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	mdmcfg4_t mdmcfg4;
	cc1101_read_register(CC1101_MDMCFG4, &mdmcfg4.reg);
	uint8_t baudRateExponent = mdmcfg4.drate_e;
	uint8_t baudRateMantissa;
	cc1101_read_register(CC1101_MDMCFG3, &baudRateMantissa);

	double_t baudRate = 1000.0 * CC1101_CLOCK_FREQUENCY * (256 + baudRateMantissa) * pow(2, baudRateExponent) / pow(2, 28);

	return baudRate;
}

/// <summary>
/// Set Receiver Channel Filter Bandwidth
/// </summary>
/// <param name="bandwidth">812, 650, 541, 464, 406, 325, 270, 232, 203, 162, 135, 116, 102, 81, 68, 58</param>
void cc1101_set_rx_channel_bandwidth(uint16_t bandwidth)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	uint8_t chanbwExponent = 3, chanbwMantissa = 3;

	for (uint8_t e_index = 0; e_index < 4; e_index++)
	{
		for (uint8_t m_index = 0; m_index < 4; m_index++)
		{
			if (bandwidth >= chanBW_limits[4 * e_index + m_index])
			{
				chanbwExponent = e_index;
				chanbwMantissa = m_index;
				e_index = 4;
				m_index = 4;
				break;
			}
		}
	}

	mdmcfg4_t mdmcfg4;
	cc1101_read_register(CC1101_MDMCFG4, &mdmcfg4.reg);
	mdmcfg4.chanbw_e = chanbwExponent;
	mdmcfg4.chanbw_m = chanbwMantissa;

	cc1101_write_register(CC1101_MDMCFG4, mdmcfg4.reg);
	ESP_LOGD(TAG, "Set Rx Band Width: %d. E: %d, M: %d", bandwidth, chanbwExponent, chanbwMantissa);
}

uint16_t cc1101_get_rx_channel_bandwidth_kHz(void)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	mdmcfg4_t mdmcfg4;
	cc1101_read_register(CC1101_MDMCFG4, &mdmcfg4.reg);
	uint8_t bandWidthExponent = mdmcfg4.chanbw_e;
	uint8_t bandWidthMantissa = mdmcfg4.chanbw_m;
	//ESP_LOGD(TAG, "Get Band Width: %d. E: %d, M: %d   dr %d", mdmcfg4.reg, mdmcfg4.chanbw_e, mdmcfg4.chanbw_m, mdmcfg4.drate_e);
	double_t bandWidth = 1000.0 * CC1101_CLOCK_FREQUENCY / (8.0 * (4 + bandWidthMantissa) * pow(2, bandWidthExponent));
	return bandWidth;
}

void cc1101_set_modulation_format(cc1101_mod_format_t modFormat)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	mdmcfg2_t mdmcfg2;
	cc1101_read_register(CC1101_MDMCFG2, &mdmcfg2.reg);
	mdmcfg2.mod_format = modFormat;

	cc1101_write_register(CC1101_MDMCFG2, mdmcfg2.reg);
	ESP_LOGD(TAG, "Set Modulation Format: %d", modFormat);
}

cc1101_mod_format_t cc1101_get_modulation_format(void)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	mdmcfg2_t mdmcfg2;
	cc1101_read_register(CC1101_MDMCFG2, &mdmcfg2.reg);
	return mdmcfg2.mod_format;
}

void cc1101_set_manchester_encoding(uint8_t manchester_en)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	mdmcfg2_t mdmcfg2;
	cc1101_read_register(CC1101_MDMCFG2, &mdmcfg2.reg);
	mdmcfg2.manchester_en = manchester_en;
	cc1101_write_register(CC1101_MDMCFG2, mdmcfg2.reg);
	ESP_LOGD(TAG, "Set Manchester Encoding: %d.", mdmcfg2.manchester_en);
}

uint8_t cc1101_get_manchester_encoding(void)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	mdmcfg2_t mdmcfg2;
	cc1101_read_register(CC1101_MDMCFG2, &mdmcfg2.reg);
	return mdmcfg2.manchester_en;
}

void cc1101_set_sync_mode(sync_mode_t sync_mode)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	mdmcfg2_t mdmcfg2;
	cc1101_read_register(CC1101_MDMCFG2, &mdmcfg2.reg);
	mdmcfg2.sync_mode = sync_mode;
	cc1101_write_register(CC1101_MDMCFG2, mdmcfg2.reg);
	ESP_LOGD(TAG, "Set Sync Mode: %d.", mdmcfg2.sync_mode);
}

sync_mode_t cc1101_get_sync_mode(void)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	mdmcfg2_t mdmcfg2;
	cc1101_read_register(CC1101_MDMCFG2, &mdmcfg2.reg);
	return mdmcfg2.sync_mode;
}

void cc1101_set_packet_length(uint8_t packet_length)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	cc1101_write_register(CC1101_PKTLEN, packet_length);
	ESP_LOGD(TAG, "Set Packet length: %d.", packet_length);
}

uint8_t cc1101_get_packet_length(void)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	pktlen_t pktlen;
	cc1101_read_register(CC1101_PKTLEN, &pktlen.pktlen_length);
	return pktlen.pktlen_length;
}

void cc1101_set_crc_enable(uint8_t crc_en)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	pktctrl0_t pktctrl0;
	cc1101_read_register(CC1101_PKTCTRL0, &pktctrl0.reg);
	pktctrl0.crc_en = crc_en;
	cc1101_write_register(CC1101_PKTCTRL0, pktctrl0.reg);
	ESP_LOGD(TAG, "Set CRC check: %d.", pktctrl0.crc_en);
}

uint8_t cc1101_get_crc_enable(void)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	pktctrl0_t pktctrl0;
	cc1101_read_register(CC1101_PKTCTRL0, &pktctrl0.reg);
	return pktctrl0.crc_en;
}

void cc1101_set_white_data(uint8_t white_data)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	pktctrl0_t pktctrl0;
	cc1101_read_register(CC1101_PKTCTRL0, &pktctrl0.reg);
	pktctrl0.white_data = white_data;
	cc1101_write_register(CC1101_PKTCTRL0, pktctrl0.reg);
	ESP_LOGD(TAG, "Set Whitedata: %d.", pktctrl0.white_data);
}

uint8_t cc1101_get_white_data(void)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	pktctrl0_t pktctrl0;
	cc1101_read_register(CC1101_PKTCTRL0, &pktctrl0.reg);
	return pktctrl0.white_data;
}

void cc1101_set_pkt_format(pkt_format_t pkt_format)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	pktctrl0_t pktctrl0;
	cc1101_read_register(CC1101_PKTCTRL0, &pktctrl0.reg);
	pktctrl0.pkt_format = pkt_format;
	cc1101_write_register(CC1101_PKTCTRL0, pktctrl0.reg);
	ESP_LOGD(TAG, "Set Rx, Tx mode configuration: %d.", pktctrl0.pkt_format);
}

pkt_format_t cc1101_get_pkt_format(void)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	pktctrl0_t pktctrl0;
	cc1101_read_register(CC1101_PKTCTRL0, &pktctrl0.reg);
	return pktctrl0.pkt_format;
}

void cc1101_set_length_config(pck_length_config_t pck_length_config)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	pktctrl0_t pktctrl0;
	cc1101_read_register(CC1101_PKTCTRL0, &pktctrl0.reg);
	pktctrl0.pck_length_config = pck_length_config;
	cc1101_write_register(CC1101_PKTCTRL0, pktctrl0.reg);
	ESP_LOGD(TAG, "Set Packet Length Config: %d.", pktctrl0.pck_length_config);
}

pck_length_config_t cc1101_get_length_config(void)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	pktctrl0_t pktctrl0;
	cc1101_read_register(CC1101_PKTCTRL0, &pktctrl0.reg);
	return pktctrl0.pck_length_config;
}

void cc1101_set_txoff_mode(txoff_mode_t txoff_mode)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	mcsm1_t mcsm1;
	cc1101_read_register(CC1101_MCSM1, &mcsm1.reg);
	mcsm1.txoff_mode = txoff_mode;
	cc1101_write_register(CC1101_MCSM1, mcsm1.reg);
	ESP_LOGD(TAG, "Set TxOff Mode %d.", mcsm1.txoff_mode);
}

txoff_mode_t cc1101_get_txoff_mode(void)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	mcsm1_t mcsm1;
	cc1101_read_register(CC1101_MCSM1, &mcsm1.reg);
	return mcsm1.txoff_mode;
}

void cc1101_set_rxoff_mode(rxoff_mode_t rxoff_mode)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	mcsm1_t mcsm1;
	cc1101_read_register(CC1101_MCSM1, &mcsm1.reg);
	mcsm1.rxoff_mode = rxoff_mode;
	cc1101_write_register(CC1101_MCSM1, mcsm1.reg);
	ESP_LOGD(TAG, "Set RxOff Mode %d.", mcsm1.rxoff_mode);
}

rxoff_mode_t cc1101_get_rxoff_mode(void)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	mcsm1_t mcsm1;
	cc1101_read_register(CC1101_MCSM1, &mcsm1.reg);
	return mcsm1.rxoff_mode;
}

void cc1101_set_cca_mode(cca_mode_t cca_mode)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	mcsm1_t mcsm1;
	cc1101_read_register(CC1101_MCSM1, &mcsm1.reg);
	mcsm1.cca_mode = cca_mode;
	cc1101_write_register(CC1101_MCSM1, mcsm1.reg);
	ESP_LOGD(TAG, "Set CCA Mode %d.", mcsm1.cca_mode);
}

cca_mode_t cc1101_get_cca_mode(void)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	mcsm1_t mcsm1;
	cc1101_read_register(CC1101_MCSM1, &mcsm1.reg);
	return mcsm1.cca_mode;
}

void cc1101_set_sync_word(sync_word_t sync_word)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	cc1101_write_register(CC1101_SYNC0, sync_word.sync0);
	cc1101_write_register(CC1101_SYNC1, sync_word.sync1);
	ESP_LOGD(TAG, "Set Sync Word %d.", sync_word.sync);
}

sync_word_t cc1101_get_sync_word(void)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	sync_word_t sync_word;
	cc1101_read_register(CC1101_SYNC0, &sync_word.sync0);
	cc1101_read_register(CC1101_SYNC1, &sync_word.sync1);
	return sync_word;
}

void cc1101_set_forward_error_correction(uint8_t fec_en)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	mdmcfg1_t mdmcfg1;
	cc1101_read_register(CC1101_MDMCFG1, &mdmcfg1.reg);
	mdmcfg1.fec_en = fec_en;
	cc1101_write_register(CC1101_MDMCFG1, mdmcfg1.reg);
	ESP_LOGD(TAG, "Set Forward Error Correction %d.", mdmcfg1.fec_en);
}

uint8_t cc1101_get_forward_error_correction(void)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	mdmcfg1_t mdmcfg1;
	cc1101_read_register(CC1101_MDMCFG1, &mdmcfg1.reg);
	return mdmcfg1.fec_en;
}

void cc1101_set_num_preamble(num_preamble_t num_preamble)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	mdmcfg1_t mdmcfg1;
	cc1101_read_register(CC1101_MDMCFG1, &mdmcfg1.reg);
	mdmcfg1.num_preamble = num_preamble;
	cc1101_write_register(CC1101_MDMCFG1, mdmcfg1.reg);
	ESP_LOGD(TAG, "Set Preamble bytes %d.", mdmcfg1.num_preamble);
}

num_preamble_t cc1101_get_num_preamble(void)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	mdmcfg1_t mdmcfg1;
	cc1101_read_register(CC1101_MDMCFG1, &mdmcfg1.reg);
	return mdmcfg1.num_preamble;
}

void cc1101_set_deviation_frequency(double frequencyInkHz)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	deviatn_t deviatn = {
		.deviation_e = 0,
		.deviation_m = 0};

	for (int tempExponent = 0; tempExponent < 4; tempExponent++)
	{
		double_t tempMantissa = frequencyInkHz * 1000.0 * pow(2, 17) / (pow(2, tempExponent) * RF_XTAL_FREQ) - 8;
		if (tempMantissa < 8)
		{
			deviatn.deviation_e = tempExponent;
			deviatn.deviation_m = tempMantissa < 0 ? 0 : tempMantissa;
			break;
		}
	}

	cc1101_write_register(CC1101_DEVIATN, deviatn.reg);

	ESP_LOGD(TAG, "Set Deviation Frequency %f. E: %d, M: %d ", frequencyInkHz, deviatn.deviation_e, deviatn.deviation_m);
}

double cc1101_get_deviation_frequency(void)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	deviatn_t deviatn;
	cc1101_read_register(CC1101_DEVIATN, &deviatn.reg);
	double_t deviation = 1000.0 * CC1101_CLOCK_FREQUENCY * (8 + deviatn.deviation_m) * pow(2, deviatn.deviation_e) / pow(2, 17);
	return deviation;
}

void cc1101_set_controls_address_check(adr_chk_t adr_chk)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	pktctrl1_t pktctrl1;
	cc1101_read_register(CC1101_PKTCTRL1, &pktctrl1.reg);
	pktctrl1.adr_chk = adr_chk;
	cc1101_write_register(CC1101_PKTCTRL1, pktctrl1.reg);
	ESP_LOGD(TAG, "Set Address Check: %d.", pktctrl1.adr_chk);
}

adr_chk_t get_controls_address_check(void)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	pktctrl1_t pktctrl1;
	cc1101_read_register(CC1101_PKTCTRL1, &pktctrl1.reg);
	return pktctrl1.adr_chk;
}

void cc1101_set_append_status(uint8_t append_status)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	pktctrl1_t pktctrl1;
	cc1101_read_register(CC1101_PKTCTRL1, &pktctrl1.reg);
	pktctrl1.append_status = append_status;
	cc1101_write_register(CC1101_PKTCTRL1, pktctrl1.reg);
	ESP_LOGD(TAG, "Set Append Status: %d.", pktctrl1.append_status);
}

uint8_t cc1101_get_append_status(void)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	pktctrl1_t pktctrl1;
	cc1101_read_register(CC1101_PKTCTRL1, &pktctrl1.reg);
	return pktctrl1.append_status;
}

void cc1101_set_preamble_quality_threshold(uint8_t pqt)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	pktctrl1_t pktctrl1;
	cc1101_read_register(CC1101_PKTCTRL1, &pktctrl1.reg);
	pktctrl1.pqt = pqt;
	cc1101_write_register(CC1101_PKTCTRL1, pktctrl1.reg);
	ESP_LOGD(TAG, "Set Preamble Quality Threshold: %d.", pktctrl1.pqt);
}

uint8_t cc1101_get_preamble_quality_threshold(void)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	pktctrl1_t pktctrl1;
	cc1101_read_register(CC1101_PKTCTRL1, &pktctrl1.reg);
	return pktctrl1.pqt;
}

void cc1101_set_crc_autoflush(uint8_t crc_autoflush)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	pktctrl1_t pktctrl1;
	cc1101_read_register(CC1101_PKTCTRL1, &pktctrl1.reg);
	pktctrl1.crc_autoflush = crc_autoflush;
	cc1101_write_register(CC1101_PKTCTRL1, pktctrl1.reg);
	ESP_LOGD(TAG, "Set CRC Autoflush: %d.", pktctrl1.crc_autoflush);
}

uint8_t cc1101_get_crc_autoflush(void)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	pktctrl1_t pktctrl1;
	cc1101_read_register(CC1101_PKTCTRL1, &pktctrl1.reg);
	return pktctrl1.crc_autoflush;
}

void cc1101_set_device_address(uint8_t device_addr)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	addr_t addr = {
		.device_addr = device_addr};

	cc1101_write_register(CC1101_ADDR, addr.device_addr);
	ESP_LOGD(TAG, "Set Device Address: %d.", addr.device_addr);
}

uint8_t cc1101_get_device_address(void)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);
	addr_t addr;
	cc1101_read_register(CC1101_ADDR, &addr.device_addr);
	return addr.device_addr;
}

void cc1101_set_channel_number(uint8_t chan)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	channr_t channr = {
		.chan = chan};

	cc1101_write_register(CC1101_CHANNR, channr.chan);
	ESP_LOGD(TAG, "Set Channel Number: %d.", channr.chan);
}

uint8_t cc1101_get_channel_number(void)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	channr_t channr;
	cc1101_read_register(CC1101_CHANNR, &channr.chan);
	return channr.chan;
}

void cc1101_set_rx_attenuation(close_in_rx_t close_in_rx)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	fifothr_t fifothr;
	cc1101_read_register(CC1101_FIFOTHR, &fifothr.reg);
	fifothr.close_in_rx = close_in_rx;
	cc1101_write_register(CC1101_FIFOTHR, fifothr.reg);
	ESP_LOGD(TAG, "Set RX Attenuationr: %d.", fifothr.close_in_rx);
}

uint8_t cc1101_get_rx_attenuation(void)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	fifothr_t fifothr;
	cc1101_read_register(CC1101_FIFOTHR, &fifothr.reg);
	return fifothr.close_in_rx;
}

void cc1101_set_pa_power_setting(uint8_t pa_power)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	frend0_t frend0;
	cc1101_read_register(CC1101_FREND0, &frend0.reg);
	frend0.pa_power = pa_power;
	cc1101_write_register(CC1101_FREND0, frend0.reg);
	ESP_LOGD(TAG, "Set PA power setting: %d.", frend0.pa_power);
}

uint8_t cc1101_get_pa_power_setting(void)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	frend0_t frend0;
	cc1101_read_register(CC1101_FREND0, &frend0.reg);
	return frend0.pa_power;
}

void cc1101_set_filter_length(filter_length_t filter_length)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	agcctrl0_t agcctrl0;
	cc1101_read_register(CC1101_AGCCTRL0, &agcctrl0.reg);
	agcctrl0.filter_length = filter_length;
	cc1101_write_register(CC1101_AGCCTRL0, agcctrl0.reg);
	ESP_LOGD(TAG, "Set Filter Length: %d.", agcctrl0.filter_length);
}

uint8_t cc1101_get_filter_length(void)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	agcctrl0_t agcctrl0;
	cc1101_read_register(CC1101_AGCCTRL0, &agcctrl0.reg);
	return agcctrl0.filter_length;
}

void cc1101_set_hysteresis_level(hyst_level_t hyst_level)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	agcctrl0_t agcctrl0;
	cc1101_read_register(CC1101_AGCCTRL0, &agcctrl0.reg);
	agcctrl0.hyst_level = hyst_level;
	cc1101_write_register(CC1101_AGCCTRL0, agcctrl0.reg);
	ESP_LOGD(TAG, "Set Filter Length: %d.", agcctrl0.hyst_level);
}

hyst_level_t cc1101_get_hysteresis_level(void)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	agcctrl0_t agcctrl0;
	cc1101_read_register(CC1101_AGCCTRL0, &agcctrl0.reg);
	return agcctrl0.hyst_level;
}

void cc1101_set_agc_freeze(uint8_t freeze)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	if( freeze > 3) freeze = 3;
	agcctrl0_t agcctrl0;
	cc1101_read_register(CC1101_AGCCTRL0, &agcctrl0.reg);
	agcctrl0.agc_freeze = freeze;
	cc1101_write_register(CC1101_AGCCTRL0, agcctrl0.reg);
}

void cc1101_set_max_lna_gain(uint8_t lna_gain)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	if (lna_gain > 7) lna_gain = 7;
	agcctrl2_t agcctrl2;
	cc1101_read_register(CC1101_AGCCTRL2, &agcctrl2.reg);
	agcctrl2.max_lna_gain = lna_gain;
	cc1101_write_register(CC1101_AGCCTRL2, agcctrl2.reg);
}

void cc1101_set_max_dvga_gain(uint8_t dvga_gain)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	if (dvga_gain > 3) dvga_gain = 3;
	agcctrl2_t agcctrl2;
	cc1101_read_register(CC1101_AGCCTRL2, &agcctrl2.reg);
	agcctrl2.max_dvga_gain = dvga_gain;
	cc1101_write_register(CC1101_AGCCTRL2, agcctrl2.reg);
}

void cc1101_set_cs_abs_threshold(int8_t cs_th)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	agcctrl1_t agcctrl1;
	cc1101_read_register(CC1101_AGCCTRL1, &agcctrl1.reg);
	agcctrl1.carrier_sense_abs_thr = cs_th;
	cc1101_write_register(CC1101_AGCCTRL1, agcctrl1.reg);
}

void cc1101_set_cs_rel_threshold(carrier_sense_rel_thr_t cs_th)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	agcctrl1_t agcctrl1;
	cc1101_read_register(CC1101_AGCCTRL1, &agcctrl1.reg);
	agcctrl1.carrier_sense_rel_thr = cs_th;
	cc1101_write_register(CC1101_AGCCTRL1, agcctrl1.reg);
}

carrier_sense_rel_thr_t cc1101_get_cs_rel_threshold(void)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	agcctrl1_t agcctrl1;
	cc1101_read_register(CC1101_AGCCTRL1, &agcctrl1.reg);
	return agcctrl1.carrier_sense_rel_thr;
}

void cc1101_set_magn_target(magn_target_t magn_target)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	agcctrl2_t agcctrl2;
	cc1101_read_register(CC1101_AGCCTRL2, &agcctrl2.reg);
	agcctrl2.magn_target = magn_target;
	cc1101_write_register(CC1101_AGCCTRL2, agcctrl2.reg);
	ESP_LOGD(TAG, "Set Filter Length: %d.", agcctrl2.magn_target);
}

magn_target_t cc1101_get_magn_target(void)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	agcctrl2_t agcctrl2;
	cc1101_read_register(CC1101_AGCCTRL2, &agcctrl2.reg);
	return agcctrl2.magn_target;
}

int8_t cc1101_get_rssi(void)
{
	uint8_t rssi;
	cc1101_read_status_register(CC1101_RSSI, &rssi);
	return (int8_t)rssi;
}

int8_t cc1101_get_rssi_dbm(void)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	int8_t rssi = cc1101_get_rssi();
	return (rssi / 2) + RF_RSSI_OFFSET_DEFAULT;
}

uint8_t cc1101_get_tx_fifo_info(void)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);
	uint8_t fifo;
	cc1101_read_status_register(CC1101_TXBYTES, &fifo);
	return fifo & 0b01111111;
}

void cc1101_set_pa_table(void)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);
	cc1101_write_burst_registers(CC1101_PATABLE, pa_table_power, sizeof(pa_table_power));
	ESP_LOGD(TAG, "Set PA power table.");
}

void cc1101_get_pa_table(void)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	uint8_t pa_table[8];
	cc1101_read_burst_registers(CC1101_PATABLE, pa_table, sizeof(pa_table_power));

	for (uint8_t i = 0; i < sizeof(pa_table_power); i++)
	{
		ESP_LOGD(TAG, "%x - %x", i, pa_table[i]);
	}
	ESP_LOGD(TAG, "Get PA power table.");
}

void cc1101_reset(void)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	cc1101_command_strobe(CC1101_SRES);
}

void cc1101_set_idle_state(void)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	cc1101_command_strobe(CC1101_SIDLE);
	ESP_LOGD(TAG, "Set Idle state");
}

void cc1101_set_flush_tx(void)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	cc1101_command_strobe(CC1101_SFTX);
	ESP_LOGD(TAG, "Set flush TX");
}

void cc1101_set_flush_rx(void)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	cc1101_command_strobe(CC1101_SFRX);
	ESP_LOGD(TAG, "Set flush RX");
}

void cc1101_set_tx_state(void)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	cc1101_command_strobe(CC1101_STX);
	ESP_LOGD(TAG, "Set Tx state");
}

void cc1101_set_rx_state(void)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	cc1101_command_strobe(CC1101_SRX);
	ESP_LOGD(TAG, "Set Rx state");
}

void cc1101_set_calibrate_state(void)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	cc1101_command_strobe(CC1101_SCAL);
	ESP_LOGD(TAG, "Set Ralibrate state");
}

void cc1101_set_command_state(cc1101_command_strobe_t command_strobe)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	cc1101_command_strobe(command_strobe);
	ESP_LOGD(TAG, "Set Command: %d", command_strobe);
}

cc1101_marc_state_t cc1101_get_marc_state(void)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);
	cc1101_marc_state_t state;
	cc1101_read_status_register(CC1101_MARCSTATE, (uint8_t *)&state);
	ESP_LOGD(TAG, "Marc State is %d", state & 0x1F);
	return state & 0x1F;
}

uint8_t cc1101_get_pkt_status(void)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);
	pktstatus_t pktstatus_t;
	cc1101_read_status_register(CC1101_PKTSTATUS, &pktstatus_t.reg);
	return pktstatus_t.reg;
}


void send_data(void)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	DRAM_ATTR static uint8_t data1[] = {0x00, 0x55, 0x55, 0xAA, 0xAA};
	DRAM_ATTR static uint8_t data2[] = {0x00, 0xAA, 0xAA, 0x55, 0x55};

	cc1101_write_burst_registers(CC1101_TXFIFO, data1, 5);
	cc1101_set_command_state(CC1101_STX);

	while (cc1101_get_marc_state() != CC1101_STATE_IDLE)
	{
		vTaskDelay(100 / portTICK_PERIOD_MS);
	}
	cc1101_set_flush_tx();

	cc1101_write_burst_registers(CC1101_TXFIFO, data2, 5);
	cc1101_set_command_state(CC1101_STX);

	while (cc1101_get_marc_state() != CC1101_STATE_IDLE)
	{
		vTaskDelay(100 / portTICK_PERIOD_MS);
	}

	cc1101_set_flush_tx();
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/******************************************************************************
 * @fn          calculate_freq_error_est
 *
 * @brief       Convert the frequency error estimate from two complement coded
 *              data to frequency error in Hertz
 *
 * input parameters
 *
 * @param       freq_reg_error -  two complement formatted data from tranceiver
 *
 * output parameters
 *
 * @return      freq_error     - 32 bit signed integer value representing
 frequency error in Hertz
 */
static int32_t cc1101_calculate_freq_error_est(uint8_t freq_reg_error)
{

	long freq_error_est;
	int32_t freq_error_est_int;
	int8_t sign;

	/* the incoming data is 16 bit two complement format, separate "sign" */
	if (freq_reg_error > 128)
	{
		freq_error_est = freq_reg_error - 256;
		sign = -1;
	}
	else
	{
		freq_error_est = freq_reg_error;
		sign = 1;
	}

	freq_error_est = freq_error_est * SCALING_FREQEST;

	/* re-assign the "sign" */
	freq_error_est_int = freq_error_est * sign;

	return freq_error_est_int;
}
