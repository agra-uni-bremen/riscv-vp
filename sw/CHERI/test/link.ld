OUTPUT_ARCH(riscv)
ENTRY(_start)

MEMORY
{
    RAM (rwx) : ORIGIN = 0x00000000, LENGTH = 16M      /* --- starting address is important otherwise the VP crashes, for now we are only using one memors region*/ 
}

SECTIONS
{
    /* ---- code and rodata ---- */
    .text : ALIGN(16) {
        _text_start = .;
        KEEP(*(.init))
        KEEP(*(.fini))
        *(.text*)
        *(.rodata*)
        _text_end = .;
    } > RAM

    /* ---- initialized data ---- */
    .data : ALIGN(16) {
        _data_start = .;
        *(.data*)
        *(.data.*)
        _data_end = .;
    } > RAM

    
    __cap_relocs : ALIGN(16) {
        __start__cap_relocs = .;
        *(__cap_relocs)
        __stop__cap_relocs = .;
    } > RAM

    .captable : ALIGN(16) {
        __captable_start = .;
        *(.captable)
        __captable_end = .;
    } > RAM

    /* ---- small data / gp setup ---- */
    .sdata : ALIGN(8) {
        _gp = . + 0x800;
        *(.sdata*)
        *(.srodata*)
        *(.srodata.*)
    } > RAM

    /* ---- uninitialized data ---- */
    .bss (NOLOAD) : ALIGN(8) {
        __bss_start = .;
        *(.bss*)
        *(.bss.*)
        *(COMMON)
        . = ALIGN(8);
        __bss_end = .;
    } > RAM

    /* ---- stack: known-size section so _stack_bottom/_stack_top have sizes ---- */
    .stack (NOLOAD) : ALIGN(8) {
        _stack_bottom = .;
        . = . + 0x4000;        /* 16 KB stack */
        _stack_top = .;
    } > RAM

    /* ---- end marker ---- */
    _end = .;
}
