/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

#include <app/drivers/blink.h>

#include <app_version.h>

#include <nfc_t4t_lib.h>

#include <nfc/ndef/msg.h>
#include <nfc/t4t/ndef_file.h>

#include "ndef_file_m.h"

LOG_MODULE_REGISTER(main, CONFIG_APP_LOG_LEVEL);

#define BLINK_PERIOD_MS_STEP 100U
#define BLINK_PERIOD_MS_MAX  1000U

static uint8_t ndef_msg_buf[CONFIG_NDEF_FILE_SIZE]; /**< Buffer for NDEF file. */

enum {
    FLASH_WRITE_FINISHED,
    FLASH_BUF_PREP_STARTED,
    FLASH_BUF_PREP_FINISHED,
    FLASH_WRITE_STARTED,
};
static atomic_t op_flags;
static uint8_t flash_buf[CONFIG_NDEF_FILE_SIZE]; /**< Buffer for flash update. */
static uint8_t flash_buf_len; /**< Length of the flash buffer. */

static void flash_buffer_prepare(size_t data_length)
{
    if (atomic_cas(&op_flags, FLASH_WRITE_FINISHED,
                   FLASH_BUF_PREP_STARTED)) {
        flash_buf_len = data_length + NFC_NDEF_FILE_NLEN_FIELD_SIZE;
        memcpy(flash_buf, ndef_msg_buf, sizeof(flash_buf));

        atomic_set(&op_flags, FLASH_BUF_PREP_FINISHED);
    } else {
        printk("Flash update pending. Discarding new data...\n");
    }

}

/**
 * @brief Callback function for handling NFC events.
 */
static void nfc_callback(void *context,
                         nfc_t4t_event_t event,
                         const uint8_t *data,
                         size_t data_length,
                         uint32_t flags)
{
    ARG_UNUSED(context);
    ARG_UNUSED(data);
    ARG_UNUSED(flags);

    switch (event) {
        case NFC_T4T_EVENT_FIELD_ON:
            printk("NFC_T4T_EVENT_FIELD_ON\n");
//            dk_set_led_on(NFC_FIELD_LED);
            break;

        case NFC_T4T_EVENT_FIELD_OFF:
            printk("NFC_T4T_EVENT_FIELD_OFF\n");
//            dk_set_leds(DK_NO_LEDS_MSK);
            break;

        case NFC_T4T_EVENT_NDEF_READ:
            printk("NFC_T4T_EVENT_NDEF_READ\n");
//            dk_set_led_on(NFC_READ_LED);
            break;

        case NFC_T4T_EVENT_NDEF_UPDATED:
            if (data_length > 0) {
                printk("NFC_T4T_EVENT_NDEF_UPDATED\n");
//                dk_set_led_on(NFC_WRITE_LED);
                flash_buffer_prepare(data_length);
            }
            break;

        default:
            break;
    }
}


int main(void)
{
	int ret;
	unsigned int period_ms = BLINK_PERIOD_MS_MAX;
	const struct device *sensor, *blink;
	struct sensor_value last_val = { 0 }, val;

	printk("Zephyr Example Application %s\n", APP_VERSION_STRING);

	sensor = DEVICE_DT_GET(DT_NODELABEL(example_sensor));
	if (!device_is_ready(sensor)) {
		LOG_ERR("Sensor not ready");
		return 0;
	}

	blink = DEVICE_DT_GET(DT_NODELABEL(blink_led));
	if (!device_is_ready(blink)) {
		LOG_ERR("Blink LED not ready");
		return 0;
	}

//	ret = blink_off(blink);
//	if (ret < 0) {
//		LOG_ERR("Could not turn off LED (%d)", ret);
//		return 0;
//	}

	printk("Use the sensor to change LED blinking period\n");

    /* Initialize NVS. */
    if (ndef_file_setup() < 0) {
        printk("Cannot setup NDEF file!\n");
        return 0;
    }

    if (ndef_file_load(ndef_msg_buf, sizeof(ndef_msg_buf)) < 0) {
        printk("Cannot load NDEF file!\n");
        return 0;
    }

    /* Set up NFC */
    int err = nfc_t4t_setup(nfc_callback, NULL);
    if (err < 0) {
        printk("Cannot setup t4t library!\n");
        return 0;
    }

    /* Run Read-Write mode for Type 4 Tag platform */
    if (nfc_t4t_ndef_rwpayload_set(ndef_msg_buf,
                                   sizeof(ndef_msg_buf)) < 0) {
        printk("Cannot set payload!\n");
        return 0;
    }

    /* Start sensing NFC field */
    if (nfc_t4t_emulation_start() < 0) {
        printk("Cannot start emulation!\n");
        return 0;
    }

    printk("Starting NFC Writable NDEF Message example\n");
    while (true) {
        if (atomic_cas(&op_flags, FLASH_BUF_PREP_FINISHED,
                       FLASH_WRITE_STARTED)) {
            if (ndef_file_update(flash_buf, flash_buf_len) < 0) {
                printk("Cannot flash NDEF message!\n");
            } else {
                printk("NDEF message successfully flashed.\n");
            }
            atomic_set(&op_flags, FLASH_WRITE_FINISHED);
        }
        k_sleep(K_MSEC(100));
    }
//	while (1) {
//		ret = sensor_sample_fetch(sensor);
//		if (ret < 0) {
//			LOG_ERR("Could not fetch sample (%d)", ret);
//			return 0;
//		}
//
//		ret = sensor_channel_get(sensor, SENSOR_CHAN_PROX, &val);
//		if (ret < 0) {
//			LOG_ERR("Could not get sample (%d)", ret);
//			return 0;
//		}
//
//		if ((last_val.val1 == 0) && (val.val1 == 1)) {
//			if (period_ms == 0U) {
//				period_ms = BLINK_PERIOD_MS_MAX;
//			} else {
//				period_ms -= BLINK_PERIOD_MS_STEP;
//			}
//
//			printk("Proximity detected, setting LED period to %u ms\n",
//			       period_ms);
//			blink_set_period_ms(blink, period_ms);
//		}
//
//		last_val = val;
//
//		k_sleep(K_MSEC(100));
//	}
	return 0;
}

