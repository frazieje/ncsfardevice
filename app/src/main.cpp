#include <zephyr/kernel.h>

extern "C" {
#include "nfc.h"
#include "usb.h"
}

int main()
{
    printk("Started C++ application \n");

    start_usb();

    start_nfc();

    return 0;
}