/*
 * cc1101_hal.h
 *
 *  Created on: 6 maj 2020
 *      Author: kurzawa.p
 */

#ifndef CC1101_HAL_H
#define CC1101_HAL_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <driver/spi_master.h>
#include <driver/gpio.h>

/*********************
 *      DEFINES
 *********************/
//#define cc1101_read_status_register(cmd, byte) cc1101_read_burst_registers(cmd, byte, 1)

/**********************
 *      TYPEDEFS
 **********************/
typedef void *cc1101_handle_t;
typedef struct
{
    spi_host_device_t spi_host;
    gpio_num_t mosi;
    gpio_num_t miso;
    gpio_num_t sclk;
    gpio_num_t cs;
    uint32_t spi_clock_speed_hz;
} cc1101_cfg_t;

/**********************
 * GLOBAL PROTOTYPES
 **********************/
cc1101_handle_t cc1101_create(const cc1101_cfg_t *spi_cfg);
esp_err_t cc1101_destroy(cc1101_handle_t, bool);

esp_err_t cc1101_read_register(uint8_t cmd, uint8_t *byte);
esp_err_t cc1101_read_burst_registers(uint8_t cmd, uint8_t *bytes, uint8_t length);
esp_err_t cc1101_write_register(uint8_t cmd, const uint8_t byte);
esp_err_t cc1101_write_burst_registers(uint8_t cmd, const uint8_t *bytes, const uint8_t length);
static inline esp_err_t cc1101_read_status_register(uint8_t cmd, uint8_t *byte)
{
	return cc1101_read_burst_registers(cmd, byte, 1);
}
esp_err_t cc1101_command_strobe(uint8_t strobe);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* CC1101_HAL_H */
