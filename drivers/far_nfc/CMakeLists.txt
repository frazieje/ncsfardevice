zephyr_library()
zephyr_library_sources_ifdef(CONFIG_FAR_NFC far_nfc.c)
