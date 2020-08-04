
arm-none-eabi-gcc -DCMS015 -DVER="\" RLC ver 6.20b12\""  -o "rlc620.elf" -mthumb -march=armv7-m   -specs=nano.specs  -flto -fwhole-program -mfloat-abi=soft -lgcc -mlittle-endian -std=gnu99 -mno-unaligned-access -Os -fmessage-length=0 --fast-math -fomit-frame-pointer -fsigned-char -ffunction-sections -fdata-sections -fno-builtin -fsingle-precision-constant -Wall  -g -T ./link.ld -DSTM32F10X_MD_VL  -DUSE_STDPERIPH_DRIVER   -I"."  -I".\inc" -L"..\src"  -nostartfiles -Xlinker --gc-sections -s   main.c cms015.c uart.c flash.c eeprom.c  startup_stm32f10x_md_vl.S system_stm32f10x.c .\Lib\stm32f10x_gpio.c

arm-none-eabi-objcopy -O ihex "rlc620.elf"  "rlc620b12.hex"
arm-none-eabi-objcopy -O binary "rlc620.elf"  "rlc620b12.bin"
