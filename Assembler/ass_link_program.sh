#!/bin/bash
if [ "$#" -ne 1 ]; then
    printf 'ERROR! You must provide one and only one argument!\n' >&2
    exit 1
fi
riscv32-pico-elf-gcc -c -march=rv32ima_zicsr_zifencei_zba_zbb_zbs_zbkb_zca_zcb_zcmp -mabi=ilp32 -std=gnu11 -Wall -O0 -g -o $1/$1.o $1/$1.S
riscv32-pico-elf-ld -g -m elf32lriscv -T linker.ld -o $1/$1.elf $1/$1.o -Map linker_output.map
#riscv32-pico-elf-objcopy -O binary $1/$1.elf $1/$1.bin
#riscv32-pico-elf-objdump -f $1/$1.elf
openocd -f interface/cmsis-dap.cfg -f target/rp2350-riscv.cfg -c "adapter speed 5000" \
-c "program $1/$1.elf verify reset exit"
