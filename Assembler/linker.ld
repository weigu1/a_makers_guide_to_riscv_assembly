/* Simple linker script for RISC-V multi-core by weigu.lu */
OUTPUT_ARCH(riscv)
ENTRY(_start)

MEMORY {
    FLASH (rx)  : ORIGIN = 0x10000000, LENGTH = 4M    /* FLASH read-only */
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 512k  /* SRAM read/write */
    SCRATCH_X(rwx) : ORIGIN = 0x20080000, LENGTH = 4k /* SRAM r/w used for stack1 */
    SCRATCH_Y(rwx) : ORIGIN = 0x20081000, LENGTH = 4k /* SRAM r/w used for stack0 */
}

/* Sections */
SECTIONS {
    .text : ALIGN(4) {
        __text_start = .;
        *(.text*)
        . = ALIGN(4);
        __text_end = .;
    } > FLASH

    .rodata : ALIGN(4) {
        __rodata_start = .;
        *(.rodata*)
        . = ALIGN(4);
        __rodata_end = .;
    } > FLASH

    .data : ALIGN(4) {
        __data_load_addr = LOADADDR(.data); /* Load Memory Addr. LMA in Flash */
        __data_start = .;                   /* Virtual Memory Addr. VMA in RAM */
        *(.data*)
        . = ALIGN(4);
        __data_end = .;
    } > RAM AT > FLASH  /* .data in FLASH (LMA) loaded into RAM (VMA) at runtime */

    .bss : ALIGN(4) {
        __bss_start = .;
        *(.bss*)
        . = ALIGN(4);
        __bss_end = .;
        __end = .;
    } > RAM
}

/* RAM boundaries */
__ram_start = ORIGIN(RAM);
__ram_end = ORIGIN(RAM) + LENGTH(RAM); /* poorly named, hist. maximum heap ptr */
/* Stack definitions */
__stack_top = ORIGIN(SCRATCH_Y) + LENGTH(SCRATCH_Y);  /* Stack top for Core 0 */
__stack1_top = ORIGIN(SCRATCH_X) + LENGTH(SCRATCH_X); /* Stack top for Core 1 */
__stack_bottom = __stack1_top + __stack_guard_bytes;  /* add a guard space */
__stack1_bottom = __ram_end + __stack_guard_bytes;    /* add a guard space */
__stack_guard_bytes = 256; /* Guard space in bytes */
/* Heap definitions */
__heap_start = __bss_end;                             /* Heap (lowest addresses) */
__heap_end = __stack1_bottom - __stack_guard_bytes;
/* Assert no overlap between heap and stacks */
ASSERT(__stack1_bottom > __heap_start, "Heap and stack1 overlap!");
ASSERT(__stack_bottom > __heap_start, "Heap and stack0 overlap!");
