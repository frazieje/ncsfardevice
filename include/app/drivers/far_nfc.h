//
// Created by Joel Frazier on 5/22/24.
//

#ifndef APP_DRIVERS_FAR_NFC_H_
#define APP_DRIVERS_FAR_NFC_H_

#include <zephyr/device.h>
#include <zephyr/toolchain.h>

/**
 * @defgroup drivers_far_nfc FAR NFC drivers
 * @ingroup drivers
 * @{
 *
 * @brief A custom driver class to provide NFC behavior
 *
 * This provides an interface to configure and start/stop NFC tag emulation.
 */

/**
 * @defgroup drivers_far_nfc_ops FAR NFC driver operations
 * @{
 *
 * @brief Operations of the FAR NFC driver class.
 */

/** @brief FAR NFC driver class operations */
__subsystem struct far_nfc_api {
    /**
     * @brief Configure the LED blink period.
     *
     * @param dev Blink device instance.
     * @param period_ms Period of the LED blink in milliseconds, 0 to
     * disable blinking.
     *
     * @retval 0 if successful.
     * @retval -EINVAL if @p period_ms can not be set.
     * @retval -errno Other negative errno code on failure.
     */
    int (*start_emulation_ro)(const struct device *dev);

    /**
     * @brief Configure the LED blink period.
     *
     * @param dev Blink device instance.
     * @param period_ms Period of the LED blink in milliseconds, 0 to
     * disable blinking.
     *
     * @retval 0 if successful.
     * @retval -EINVAL if @p period_ms can not be set.
     * @retval -errno Other negative errno code on failure.
     */
    int (*start_emulation_rw)(const struct device *dev);

    /**
     * @brief Configure the LED blink period.
     *
     * @param dev Blink device instance.
     * @param period_ms Period of the LED blink in milliseconds, 0 to
     * disable blinking.
     *
     * @retval 0 if successful.
     * @retval -EINVAL if @p period_ms can not be set.
     * @retval -errno Other negative errno code on failure.
     */
    int (*stop_emulation)(const struct device *dev);

};
#endif // APP_DRIVERS_FAR_NFC_H_
