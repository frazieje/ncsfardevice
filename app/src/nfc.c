#include "nfc.h"

#include <nfc_t4t_lib.h>

#include <nfc/ndef/msg.h>
#include <nfc/t4t/ndef_file.h>

static uint8_t ndef_msg_buf[CONFIG_NDEF_FILE_SIZE]; /**< Buffer for NDEF file. */

int ndef_file_default_message(uint8_t *buff, uint32_t *size)
{
    int err;
    uint32_t ndef_size = nfc_t4t_ndef_file_msg_size_get(*size);

    /* Encode URI message into buffer. */
    err = nfc_ndef_uri_msg_encode(NFC_URI_HTTP_WWW,
                                  m_url,
                                  sizeof(m_url),
                                  nfc_t4t_ndef_file_msg_get(buff),
                                  &ndef_size);
    if (err) {
        return err;
    }

    err = nfc_t4t_ndef_file_encode(buff, &ndef_size);
    if (err) {
        return err;
    }

    *size = ndef_size;

    return 0;
}

int start_nfc_ro(nfc_t4t_callback_t callback) {
    return -1;
}

int start_nfc_rw(nfc_t4t_callback_t callback) {

    int err = ndef_file_default_message(ndef_msg_buf, &size);
    if (err < 0) {
        printk("Cannot create default message!\n");
        return err;
    }

    /* Set up NFC */
    err = nfc_t4t_setup(callback, NULL);
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