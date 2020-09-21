/*
 * cc1101_hal.c
 *
 *  Created on: 6 maj 2020
 *      Author: kurzawa.p
 */

/*********************
 *      INCLUDES
 *********************/
#include <string.h>

#include "esp_log.h"
#include "esp_err.h"

#include "spi_master_ext.h"
#include "cc1101_hal.h"
#include "cc1101_defs.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
typedef struct
{
	spi_device_handle_t spi_handle;
	cc1101_cfg_t spi_config;
} cc1101_dev_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/
static spi_device_handle_t cc1101_spi_handle;
static const char *TAG = "cc1101_hal";

/**********************
 *      MACROS
 **********************/
#define CHECK(a, ret_val, str, ...)                                               \
      if (!(a))                                                                   \
      {                                                                           \
            ESP_LOGE(TAG, "%s(%u): " str, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
            return (ret_val);                                                     \
      }

#define ASSERT(con)                                                                           \
      do                                                                                      \
      {                                                                                       \
            if (!(con))                                                                       \
            {                                                                                 \
                  ESP_LOGE(TAG, "assert errno:%d, errno_str: !(%s)", errno, strerror(errno)); \
                  assert(0 && #con);                                                          \
            }                                                                                 \
      } while (0)

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

cc1101_handle_t cc1101_create(const cc1101_cfg_t *spi_cfg)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	cc1101_dev_t *cc1101_dev = (cc1101_dev_t*) calloc(1, sizeof(cc1101_dev_t));
	CHECK((cc1101_dev != NULL), NULL, "CC1101 ALLOC FAIL");

	spi_bus_config_t buscfg =
		{
			.miso_io_num = spi_cfg->miso,
			.mosi_io_num = spi_cfg->mosi,
			.sclk_io_num = spi_cfg->sclk,
			.quadwp_io_num = -1,
			.quadhd_io_num = -1 };

	spi_device_interface_config_t devcfg =
		{
			.clock_speed_hz = spi_cfg->spi_clock_speed_hz, // SPI_MASTER_FREQ_10M / 2, //Clock out at 10 MHz
			.mode = 3,                          //SPI mode 1
			.spics_io_num = spi_cfg->cs,            //CS pin
			.queue_size = 1, //We want to be able to queue 7 transactions at a time
			.flags = 0,
			.command_bits = 2,
			.address_bits = 6,
			.dummy_bits = 0, };

	esp_err_t error = ESP_OK;

	if (spi_handle[spi_cfg->spi_host] == NULL)
	{
		//Initialize the SPI bus
		ESP_LOGD(TAG, "Initialization SPI%d", spi_cfg->spi_host + 1);
		error = spi_bus_initialize(spi_cfg->spi_host, &buscfg, 0);
		CHECK((error == ESP_OK), NULL, "SPI device %d initialize fail", spi_cfg->spi_host);
	}

	//Attach the Device to the SPI bus
	error = spi_bus_add_device(spi_cfg->spi_host, &devcfg, &cc1101_spi_handle);
	CHECK((error == ESP_OK), NULL, "SPI device %d add fail", spi_cfg->spi_host);

	cc1101_dev->spi_config = *spi_cfg;
	cc1101_dev->spi_handle = cc1101_spi_handle;

	if (spi_handle[spi_cfg->spi_host] == NULL)
	{
		spi_handle[spi_cfg->spi_host] = cc1101_spi_handle;
	}

	return cc1101_dev;
}

esp_err_t cc1101_destroy(cc1101_handle_t handle, bool del_bus)
{
	cc1101_dev_t *dev = (cc1101_dev_t*) handle;
	esp_err_t error = ESP_OK;
	CHECK((handle != NULL), ESP_ERR_INVALID_STATE, "SPI interface uninitialized.");
	error = spi_bus_remove_device(dev->spi_handle);
	CHECK((error == ESP_OK), ESP_ERR_INVALID_STATE, "SPI device %d. remove fail", dev->spi_config.spi_host);
	if (del_bus)
	{
		error = spi_bus_free(dev->spi_config.spi_host);
		CHECK((error == ESP_OK), ESP_ERR_INVALID_STATE, "SPI device %d. free fail", dev->spi_config.spi_host);
	}
	free(handle);
	return error;
}

esp_err_t cc1101_read_register(uint8_t addr, uint8_t *byte)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	CHECK((cc1101_spi_handle != NULL), ESP_ERR_INVALID_STATE, "SPI interface uninitialized.");

	spi_transaction_t tx_trans =
		{
			.cmd = CC1101_READ_SINGLE,
			.addr = addr,
			.length = 8,
			.rx_buffer = byte,

		};

	esp_err_t error = spi_device_polling_transmit(cc1101_spi_handle, &tx_trans);
	return error;
}

esp_err_t cc1101_read_burst_registers(uint8_t addr, uint8_t *bytes, uint8_t length)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	CHECK((cc1101_spi_handle != NULL), ESP_ERR_INVALID_STATE, "SPI interface uninitialized.");

	spi_transaction_t tx_trans =
		{
			.cmd = (CC1101_READ_SINGLE | CC1101_BURST),
			.addr = addr,
			.length = 8 * length,
			.rx_buffer = bytes,
		};

	esp_err_t error = spi_device_polling_transmit(cc1101_spi_handle, &tx_trans);
	return error;
}

esp_err_t cc1101_write_register(uint8_t addr, const uint8_t byte)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	CHECK((cc1101_spi_handle != NULL), ESP_ERR_INVALID_STATE, "SPI interface uninitialized.");

	spi_transaction_t tx_trans =
		{
			.cmd = CC1101_WRITE_SINGLE,
			.addr = addr,
			.length = 8,
			.tx_buffer = &byte,
		};

	esp_err_t error = spi_device_polling_transmit(cc1101_spi_handle, &tx_trans);
	return error;
}

esp_err_t cc1101_write_burst_registers(uint8_t addr, const uint8_t *bytes, const uint8_t length)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	CHECK((cc1101_spi_handle != NULL), ESP_ERR_INVALID_STATE, "SPI interface uninitialized.");

	spi_transaction_t tx_trans =
		{
			.cmd = (CC1101_WRITE_SINGLE | CC1101_BURST),
			.addr = addr,
			.length = 8 * length,
			.tx_buffer = bytes,
		};

	esp_err_t error = spi_device_polling_transmit(cc1101_spi_handle, &tx_trans);
	return error;
}

esp_err_t cc1101_command_strobe(uint8_t strobe)
{
	ESP_LOGD(TAG, "%s", __FUNCTION__);

	CHECK((cc1101_spi_handle != NULL), ESP_ERR_INVALID_STATE, "SPI interface uninitialized.");

	spi_transaction_t tx_trans =
		{
			.cmd = CC1101_WRITE_SINGLE,
			.addr = strobe,
			.length = 0,
		};

	esp_err_t error = spi_device_polling_transmit(cc1101_spi_handle, &tx_trans);
	return error;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
