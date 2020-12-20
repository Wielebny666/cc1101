/*
 * cc1101_cfg.h
 *
 *  Created on: 6 lip 2020
 *      Author: kurza
 */

#ifndef CC1101_CFG_H
#define CC1101_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "cc1101_defs.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  GLOBAL VARIABLES
 **********************/
// Address Config = No address check
// Base Frequency = 434.399872
// CRC Autoflush = false
// CRC Enable = false
// Carrier Frequency = 434.399872
// Channel Number = 0
// Channel Spacing = 199.951172
// Data Format = Asynchronous serial mode
// Data Rate = 249.939
// Deviation = 47.607422 
// Device Address = 0
// Manchester Enable = false
// Modulation Format = ASK/OOK
// PA Ramping = false
// Packet Length = 255
// Packet Length Mode = Infinite packet length mode
// Preamble Count = 4
// RX Filter BW = 541.666667
// Sync Word Qualifier Mode = No preamble/sync
// TX Power = 10
// Whitening = false
static const cc1101_reg_setting_t config_register_ask_async_rx[]=
{
	{CC1101_IOCFG2, 	0x0E}, //CS th indicator
	{CC1101_IOCFG0,		0x0D}, //asyn tx or rx
	{CC1101_PKTCTRL0, 	0x32},
	{CC1101_FSCTRL1, 	0x0C},
	{CC1101_FREQ2, 		0x10},
	{CC1101_FREQ1, 		0xB5},
	{CC1101_FREQ0, 		0x2B},
	{CC1101_MDMCFG4, 	0x2D},
	{CC1101_MDMCFG3, 	0x3B},
	{CC1101_MDMCFG2, 	0x30},
	{CC1101_MCSM1,		0x00},
	{CC1101_MCSM0, 		0x18},
	{CC1101_FOCCFG, 	0x1D},
	{CC1101_BSCFG, 		0x1C},
	{CC1101_AGCCTRL2, 	0xC7},
	{CC1101_AGCCTRL1, 	0x00},
	{CC1101_AGCCTRL0, 	0xB0},
	{CC1101_WORCTRL, 	0xFB},
	{CC1101_FREND1, 	0xB6},
	{CC1101_FREND0, 	0x11},
	{CC1101_FSCAL3, 	0xEA},
	{CC1101_FSCAL2,		0x2A},
	{CC1101_FSCAL1, 	0x00},
	{CC1101_FSCAL0, 	0x1F},
	{CC1101_TEST0, 		0x09},
};

// Address Config = No address check 
// Base Frequency = 434.399872 
// CRC Autoflush = false 
// CRC Enable = false 
// Carrier Frequency = 434.399872 
// Channel Number = 0 
// Channel Spacing = 199.951172 
// Data Format = Asynchronous serial mode 
// Data Rate = 3.335 
// Deviation = 5.157471 
// Device Address = 0 
// Manchester Enable = false 
// Modulation Format = ASK/OOK 
// PA Ramping = false 
// Packet Length = 255 
// Packet Length Mode = Infinite packet length mode 
// Preamble Count = 4 
// RX Filter BW = 58.035714 
// Sync Word Qualifier Mode = No preamble/sync 
// TX Power = 10 
// Whitening = false 

static const cc1101_reg_setting_t config_register_ask_async_tx[]= 
{
	{CC1101_IOCFG2, 	0x0E}, //CS th indicator
	{CC1101_IOCFG0,		0x0D}, //asyn tx or rx
	{CC1101_FIFOTHR,     0x47},
	{CC1101_PKTCTRL0,    0x32},
	{CC1101_FSCTRL1,     0x06},
	{CC1101_FREQ2,       0x10},
	{CC1101_FREQ1,       0xB5},
	{CC1101_FREQ0,       0x2B},
	{CC1101_MDMCFG4,     0xF7},
	{CC1101_MDMCFG3,     0x0D},
	{CC1101_MDMCFG2,     0x30},
	{CC1101_DEVIATN,     0x15},
	{CC1101_MCSM1,		0x00},
	{CC1101_MCSM0,       0x18},
	{CC1101_FOCCFG,      0x14},
	{CC1101_AGCCTRL0,    0x92},
	{CC1101_WORCTRL,     0xFB},
	{CC1101_FREND0,      0x11},
	{CC1101_FSCAL3,      0xE9},
	{CC1101_FSCAL2,      0x2A},
	{CC1101_FSCAL1,      0x00},
	{CC1101_FSCAL0,      0x1F},
	{CC1101_TEST2,       0x81},
	{CC1101_TEST1,       0x35},
	{CC1101_TEST0,       0x09},
};

static cc1101_reg_setting_t config_registe_sniffed_ask[] = {
														  {CC1101_IOCFG2,      0x09},
														   {CC1101_IOCFG0,      0x0D},
														   {CC1101_FIFOTHR,     0x47},
														   {CC1101_PKTCTRL0,    0x32},
														   {CC1101_FSCTRL1,     0x08},
														  {CC1101_FREQ2,       0x11},
														  {CC1101_FREQ1,       0x38},
														  {CC1101_FREQ0,       0x90},
														   {CC1101_MDMCFG4,     0xCA},
														   {CC1101_MDMCFG3,     0x83},
														   {CC1101_MDMCFG2,     0x80},
														   {CC1101_DEVIATN,     0x35},
														   {CC1101_MCSM0,       0x18},
														   {CC1101_FOCCFG,      0x16},
														   {CC1101_AGCCTRL2,    0x43},
														   {CC1101_WORCTRL,     0xFB},
														   {CC1101_FREND0,      0x17},
														   {CC1101_FSCAL3,      0xE9},
														   {CC1101_FSCAL2,      0x2A},
														   {CC1101_FSCAL1,      0x00},
														   {CC1101_FSCAL0,      0x1F},
														   {CC1101_TEST2,       0x81},
														   {CC1101_TEST1,       0x35},
														   {CC1101_TEST0,       0x09},
};

//aktywny
#define PA_TABLE {0x00,0x12,0x0e,0x34,0x60,0xc5,0xc1,0xc0}
static cc1101_reg_setting_t config_registe_sniffed_fsk[] = {
														  {CC1101_IOCFG2,      0x09},
														  {CC1101_IOCFG0,      0x0D},
														  {CC1101_FIFOTHR,	   0x47},
														  {CC1101_PKTCTRL0,    0x32},
														  {CC1101_FREQ2,       0x11},
														  {CC1101_FREQ1,       0x38},
														  {CC1101_FREQ0,       0x90},
														  {CC1101_MDMCFG4,     0x7B},
														  {CC1101_MDMCFG3,     0x93},
														  {CC1101_MDMCFG2,     0x00},
														  {CC1101_DEVIATN,     0x42},
														  {CC1101_MCSM1,       0x00},
														  {CC1101_MCSM0,       0x18},
														  {CC1101_FOCCFG,      0x1D},
														  {CC1101_BSCFG,       0x1C},
														  {CC1101_AGCCTRL2,    0x43},
														  {CC1101_AGCCTRL0,    0x90},

														  {CC1101_FREND0,      0x17},
														  {CC1101_FSCAL3,      0xEA},
														  {CC1101_FSCAL2,      0x2A},
														  {CC1101_FSCAL1,      0x00},
														  {CC1101_FSCAL0,      0x1F},
														  {CC1101_TEST2,       0x81},
														  {CC1101_TEST1,       0x35},
														  {CC1101_TEST0,       0x09},
};
static cc1101_reg_setting_t config_register[] = {
	//0x06 - Asserts when sync word has been sent / received, and de-asserts at the end of the packet. In RX, the pin will also deassert when a packet is discarded due to address or maximum length filtering or when the radio enters RXFIFO_OVERFLOW state. In TX the pin will de-assert if the TX FIFO underflows.
	// {CC1101_IOCFG2, 0x06}, //0x0D - Data output in async rx
	{CC1101_IOCFG0, 0x06}, //Data input if set async tx
	{CC1101_IOCFG2, 0x0D},

	{CC1101_FIFOTHR, 0x47},
	{CC1101_PKTCTRL0, 0x30},
	//433MHz
	{CC1101_FREQ2, 0x10},
	{CC1101_FREQ1, 0xA7},
	{CC1101_FREQ0, 0x62},
	//GFSK
	//{CC1101_MDMCFG2,     0x10},
	//ASK
	{CC1101_MDMCFG2, 0x30},
	//2-FSK
	//{CC1101_MDMCFG2,     0x00},
	{CC1101_MCSM1, 0x00},
	//ASK
	{CC1101_FREND0, 0x11},
	{CC1101_FSCAL3, 0xEA},
	{CC1101_FSCAL2, 0x2A},
	{CC1101_FSCAL1, 0x00},
	{CC1101_FSCAL0, 0x1F},
	{CC1101_TEST2, 0x81},
	{CC1101_TEST1, 0x35},
	{CC1101_TEST0, 0x09}

	// {CC1101_IOCFG2, 0x0D},
	// {CC1101_IOCFG0, 0x0C},
	// {CC1101_FIFOTHR, 0x47},
	// {CC1101_PKTCTRL0, 0x30},
	// {CC1101_FSCTRL1, 0x06},
	// {CC1101_FREQ2, 0x10},
	// {CC1101_FREQ1, 0xA7},
	// {CC1101_FREQ0, 0x62},
	// {CC1101_MDMCFG4, 0xF5},
	// {CC1101_MDMCFG3, 0x83},
	// {CC1101_MDMCFG2, 0x30},
	// {CC1101_DEVIATN, 0x15},
	// {CC1101_MCSM0, 0x18},
	// {CC1101_FOCCFG, 0x14},
	// {CC1101_AGCCTRL0, 0x93},
	// {CC1101_WORCTRL, 0xFB},
	// {CC1101_FREND0, 0x11},
	// {CC1101_FSCAL3, 0xE9},
	// {CC1101_FSCAL2, 0x2A},
	// {CC1101_FSCAL1, 0x00},
	// {CC1101_FSCAL0, 0x1F},
	// {CC1101_TEST2, 0x81},
	// {CC1101_TEST1, 0x35},
	// {CC1101_TEST0, 0x09}
};
//FSK
//static uint8_t pa_table_power[] = {0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
//ASK
static uint8_t pa_table_power[] = {0x00, 0x17, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; //0 dB  {0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; //12 dB

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* CC1101_CFG_H_ */
