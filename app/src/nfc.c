#include <zephyr/kernel.h>
#include "nfc.h"

int start_nfc_ro(void) {
    printk("starting NFC read-only!\n");
    return 0;
}

int start_nfc_rw(void) {
    printk("starting NFC read/write!\n");
    return 0;
}