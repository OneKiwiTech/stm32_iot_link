esptool.py --port COM14 write_flash --flash_mode qio --flash_size 4MB 0x0 boot_v1.6.bin  0x1000 user1.bin 0x3FC000 esp_init_data_default.bin  0x3FE000 blank.bin