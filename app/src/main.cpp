#include <zephyr/kernel.h>

extern "C" {
#include "nfc.h"
}

#define NFC_PATH DT_PATH(nfc)
#if DT_NODE_EXISTS(NFC_PATH)
#define NFC_TYPE_RO "nfc-ro"
#define NFC_TYPE_RW "nfc-rw"

#endif


int main()
{
    int result;
    printk("Started C++ application \n");

#if DT_NODE_EXISTS(NFC_PATH)
    char nfc_type[] = DT_PROP(NFC_PATH, compatible);
    if (strcmp(NFC_TYPE_RO, nfc_type) == 0) {
        printk("NFC type is read only\n");
        result = start_nfc_ro();
    } else if (strcmp(NFC_TYPE_RW, nfc_type) == 0) {
        printk("NFC type is read write\n");
        result = start_nfc_rw();
    } else {
        printk("NFC type is unknown, ignoring.\n");
    }

    if (result != 0) {
        printk("Problem starting NFC\n");
        return 0;
    } else {
        printk("NFC started!\n");
    }
#endif

    return 0;
}