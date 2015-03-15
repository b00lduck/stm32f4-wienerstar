OUTPUT_FORMAT ("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")

MEMORY {
	flash (rx)		: ORIGIN = 0x08000000, LENGTH = 1024K
	ccmram (rw)		: ORIGIN = 0x10000000, LENGTH = 64K
	sram1 (rw)		: ORIGIN = 0x20000000, LENGTH = 112K
	sram2 (rw)		: ORIGIN = 0x2001C000, LENGTH = 16K
}

SECTIONS {
    .text : {
        KEEP(*(.isr_vector .isr_vector.*))
        *(.text .text.* .gnu.linkonce.t.*)
        *(.glue_7t) *(.glue_7)
        *(.rodata .rodata* .gnu.linkonce.r.*)
    } > flash

    .ARM.extab : {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > flash

    __exidx_start = .;
    .ARM.exidx : {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > flash
    __exidx_end = .;

    . = ALIGN(4);
    _etext = .;
    _sidata = .;
    
    .data : AT (_etext) {
        _sdata = .;
        *(.data .data.*)
        . = ALIGN(4);
        _edata = . ;
    } > ccmram
    
    . = ALIGN(4);
    
    /* 112k used for video buffer */
    .sram1 (NOLOAD) : {
        _ssram1 = . ;
        *(.sram1 .sram1.*)
        . = ALIGN(4);
        _esram1 = . ;
    } > sram1

    /* 16k used for sound buffer */
    .sram2 (NOLOAD) : {
        _ssram2 = . ;
        *(.sram2 .sram2.*)
        . = ALIGN(4);
        _esram2 = . ;
    } > sram2

    /* .bss section which is used for uninitialized data */
    .bss (NOLOAD) :
    {
        _sbss = . ;
        *(.bss .bss.*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = . ;
    } > ccmram

    /* stack section */
    .co_stack (NOLOAD):
    {
        . = ALIGN(8);
        *(.co_stack .co_stack.*)
    } > ccmram

    . = ALIGN(4);

    _end = . ;
}
