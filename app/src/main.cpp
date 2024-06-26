#include <zephyr/kernel.h>

extern "C" {
#include "nfc.h"
}

int main()
{
    int result;
    printk("Started C++ application \n");

#if DT_NODE_EXISTS(DT_PATH(nfc))
    result = start_nfc();
    if (result != 0) {
        printk("Problem starting NFC\n");
        return 0;
    } else {
        printk("NFC started!\n");
    }
#endif

    return 0;
}