#include <zephyr/kernel.h>

extern "C" {
#include "nfc.h"
#include "usb.h"
}

#define NFC_PATH DT_PATH(nfc)
#if DT_NODE_EXISTS(NFC_PATH)
#define NFC_TYPE_RO "nfc-ro"
#define NFC_TYPE_RW "nfc-rw"
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
            break;

        case NFC_T4T_EVENT_FIELD_OFF:
            printk("NFC_T4T_EVENT_FIELD_OFF\n");
            break;

        case NFC_T4T_EVENT_NDEF_READ:
            printk("NFC_T4T_EVENT_NDEF_READ\n");
            break;

        case NFC_T4T_EVENT_NDEF_UPDATED:
            if (data_length > 0) {
                printk("NFC_T4T_EVENT_NDEF_UPDATED\n");
            }
            break;

        default:
            break;
    }
}
#endif

int main()
{
    printk("Started C++ application \n");

    start_usb();


#if DT_NODE_EXISTS(NFC_PATH)
    char nfc_type[] = DT_PROP(NFC_PATH, compatible);
    int result;
    if (strcmp(NFC_TYPE_RO, nfc_type) == 0) {
        printk("NFC type is read only\n");
        result = start_nfc_ro(nfc_callback);
    } else if (strcmp(NFC_TYPE_RW, nfc_type) == 0) {
        printk("NFC type is read write\n");
        result = start_nfc_rw(nfc_callback);
    } else {
        printk("NFC type is unknown, ignoring.\n");
    }
    if (result != 0) {
        printk("Problem starting NFC\n");
        return 0;
    } else {
        printk("NFC tag emulation started!\n");
    }
#endif

    return 0;
}