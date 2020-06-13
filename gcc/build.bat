
arm-none-eabi-gcc -DN1202 -DVER="\" RLC ver 6.20b12\""  -o "rlc620.elf" -mthumb -march=armv7-m   -specs=nano.specs  -flto -fwhole-program -mfloat-abi=soft -lgcc -mlittle-endian -std=gnu99 -mno-unaligned-access -Os -fmessage-length=0 --fast-math -fomit-frame-pointer -fsigned-char -ffunction-sections -fdata-sections -fno-builtin -fsingle-precision-constant -Wall  -g -T ./link.ld -DSTM32F10X_LD_VL -I".\inc" -L"..\Libraries\Release" -I"..\Libraries\inc" -nostartfiles -Xlinker --gc-sections -s   main.c n1110.c uart.c flash.c eeprom.c  startup_stm32f10x_ld_vl.S system_stm32f10x.c

arm-none-eabi-objcopy -O ihex "rlc620.elf"  "rlc620b12.hex"
arm-none-eabi-objcopy -O binary "rlc620.elf"  "rlc620b12.bin"
