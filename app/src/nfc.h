#ifndef NCSFARDEVICE_NFC_H
#define NCSFARDEVICE_NFC_H

#include <nfc_t4t_lib.h>

/**
 * @brief Function for initializing and starting read-only NFC services
 *
 * @return 0 when module has been set up properly,
 * error code otherwise.
 */
int start_nfc_ro(nfc_t4t_callback_t callback);

/**
 * @brief Function for initializing and starting read/write NFC services
 *
 * @return 0 when module has been set up properly,
 * error code otherwise.
 */
int start_nfc_rw(nfc_t4t_callback_t callback);

#endif //NCSFARDEVICE_NFC_H
