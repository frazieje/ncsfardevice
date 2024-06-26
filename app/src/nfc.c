#include "nfc.h"

#include <nfc_t4t_lib.h>

#include <nfc/ndef/msg.h>
#include <nfc/t4t/ndef_file.h>

#include "ndef_file_m.h"

static uint8_t ndef_msg_buf[CONFIG_NDEF_FILE_SIZE]; /**< Buffer for NDEF file. */

int start_nfc_ro(nfc_t4t_callback_t callback) {
    return -1;
}

int start_nfc_rw(nfc_t4t_callback_t callback) {
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
    int err = nfc_t4t_setup(callback, NULL);
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
    return 0;
}