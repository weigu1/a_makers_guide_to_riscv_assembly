/* Minimal linker script for RISC-V multi-core by weigu.lu */
OUTPUT_ARCH(riscv)
ENTRY(_start)

MEMORY {
    FLASH (rx)  : ORIGIN = 0x10000000, LENGTH = 4M    /* FLASH read-only */
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 512k  /* SRAM read/write */
}

/* Sections */
SECTIONS {
    .text : ALIGN(4) {
        *(.text*)
    } > FLASH
    .rodata : ALIGN(4) {
        *(.rodata*)
    } > FLASH
    .data : ALIGN(4) {
        *(.data*)
    } > RAM AT > FLASH
    .bss : ALIGN(4) {
        *(.bss*)
    } > RAM
}

/* RAM boundaries */
__stack_top = ORIGIN(RAM) + LENGTH(RAM);
