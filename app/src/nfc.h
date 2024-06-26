#ifndef NCSFARDEVICE_NFC_H
#define NCSFARDEVICE_NFC_H

/**
 * @brief Function for initializing and starting read-only NFC services
 *
 * @return 0 when module has been set up properly,
 * error code otherwise.
 */
int start_nfc_ro(void);

/**
 * @brief Function for initializing and starting read/write NFC services
 *
 * @return 0 when module has been set up properly,
 * error code otherwise.
 */
int start_nfc_rw(void);

#endif //NCSFARDEVICE_NFC_H
