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
 * This driver class is provided as an example of how to create custom driver
 * classes. It provides an interface to blink an LED at a configurable rate.
 * Implementations could include simple GPIO-controlled LEDs, addressable LEDs,
 * etc.
 */

/**
 * @defgroup drivers_blink_ops Blink driver operations
 * @{
 *
 * @brief Operations of the blink driver class.
 *
 * Each driver class tipically provides a set of operations that need to be
 * implemented by each driver. These are used to implement the public API. If
 * support for system calls is needed, the operations structure must be tagged
 * with `__subsystem` and follow the `${class}_driver_api` naming scheme.
 */

/** @brief Blink driver class operations */
__subsystem struct blink_driver_api {
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
    int (*set_period_ms)(const struct device *dev, unsigned int period_ms);
};
#endif // APP_DRIVERS_FAR_NFC_H_
